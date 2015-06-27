/***********************************************************************************
 * Copyright (c) 2013-2015 "Thomas Hoegg et al."
 * [http://gu-dsl.org]; information about commercial use mail at info@gu-dsl.org
 *
 * This file is part of GU-DSL.
 *
 * GU-DSLis free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************************/

#include "Communication/TcpCommunication.h"
#include <sstream>
#include "DebugOutput/DebugStream.h"

namespace Core
{
	namespace Communication
	{
		TcpCommunication::TcpCommunication(unsigned short listenPort, unsigned short sendPort )
			: _strand( _service ),
			_acceptor( _service, tcp::endpoint( tcp::v4(), listenPort ) ),
			_socket( _service ),
			_readMessages(10)
		{
			try
			{
				_thread = nullptr;
				_listenPort = listenPort;
				_sendPort = sendPort;
				_socket.open(tcp::v4());
				_socket.set_option(tcp::socket::reuse_address(true));
			}
			catch(const std::exception& ex)
			{
				std::stringstream message;
				message << "Could not create tcp communication." << std::endl << ex.what() << std::endl;
				E_OUT(message.str());
			}
			catch(...)
			{
				E_OUT("Could not create tcp communication.");
			}
		}

		TcpCommunication::~TcpCommunication()
		{
			stop();
		}

		void TcpCommunication::start()
		{
			try
			{
				_acceptor.async_accept( 
					_socket,
					boost::bind( &TcpCommunication::handleAccept, this,
					boost::asio::placeholders::error ) );
				_thread = new boost::thread(boost::bind(&boost::asio::io_service::run, &_service));
			}
			catch(const std::exception& ex)
			{
				std::stringstream message;
				message << "Could not start tcp communication." << std::endl << ex.what() << std::endl;
				E_OUT(message.str());
			}
			catch(...)
			{
				E_OUT("Could not start tcp communication.");
			}
		}

		void TcpCommunication::stop()
		{
			try
			{
				_service.stop();
				_socket.close();
				if(_thread)
				{
					_thread->detach();
					delete _thread;
					_thread = nullptr;
				}
			}
			catch(...)
			{
			}
		}

		void TcpCommunication::handleAccept( const boost::system::error_code& error )
		{
			if ( error )
			{  
				std::cout << error.message() << std::endl;
				return;
			}

			readHeader();
		}

		void TcpCommunication::readHeader()
		{
			try
			{
				boost::asio::async_read(
					_socket,
					boost::asio::buffer(_readMessage.data(), Message::headerLength),
					boost::bind( &TcpCommunication::handleReadHeader, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred ) );
			}
			catch(const std::exception& ex)
			{
				std::stringstream message;
				message << "Could not read tcp message header." << std::endl << ex.what() << std::endl;
				E_OUT(message.str());
			}
			catch(...)
			{
				E_OUT("Could not read tcp message header.");
			}
		}

		void TcpCommunication::handleReadHeader( const boost::system::error_code& error, std::size_t bytes_transferred )
		{
			try
			{
				if ( error )
				{  
					std::cout << error.message() << std::endl;
					return;
				}

				// Read data.
				boost::asio::async_read(
					_socket,
					boost::asio::buffer(_readMessage.body(), _readMessage.bodyLength()),
					boost::bind( &TcpCommunication::handleReadData, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred ) );
			}
			catch(const std::exception& ex)
			{
				std::stringstream message;
				message << "Could not read received tcp message." << std::endl << ex.what() << std::endl;
				E_OUT(message.str());
			}
			catch(...)
			{
				E_OUT("Could not read received tcp message.");
			}
		}

		void TcpCommunication::handleReadData( const boost::system::error_code& error, std::size_t bytes_transferred )
		{
			if ( error )
			{  
				std::cout << error.message() << std::endl;
				return;
			}

			// Start reading header again.
			readHeader();
		}

		void TcpCommunication::send( const Message& message, std::string ip, unsigned short port /*= 0*/ )
		{
			try
			{
				if(port <= 0 && _sendPort <= 0)
				{
					//error
					return;
				}
				else if(port <= 0 && _sendPort > 0)
				{
					port = _sendPort;
				}

				tcp::endpoint senderEndpoint(boost::asio::ip::address_v4::from_string(ip), port);
				_socket.connect(senderEndpoint);
				_socket.send(boost::asio::buffer(message.data(), message.length()));
			}
			catch(const std::exception& ex)
			{
				std::stringstream message;
				message << "Could not send tcp message." << std::endl << ex.what() << std::endl;
				E_OUT(message.str());
			}
			catch(...)
			{
				E_OUT("Could not send tcp message.");
			}
		}

	};
};
