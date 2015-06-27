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

#include "Communication/UdpCommunication.h"
#include "DebugOutput/DebugStream.h"

namespace Core
{
	namespace Communication
	{
		UdpCommunication::UdpCommunication(unsigned short listenPortStart, unsigned short sendPortStart,
											boost::asio::ip::address_v4 broadcastIp /*= boost::asio::ip::address_v4::from_string("192.168.25.255")*/,
											bool disableBuffer /*= false*/, unsigned short listenPortRange /*= 1*/, unsigned short sendPortRange /*= 1*/)
			:_socket(_service),
			_readMessages(10),
			_broadcastIp(broadcastIp),
			_broadcastOptionSet(_broadcastIp == boost::asio::ip::address_v4::broadcast())
		{ 
			_sendPortStart = sendPortStart;
			_sendPortRange = sendPortRange;
			_thread = nullptr;
			_disableBuffer = disableBuffer;
			_enableMessageHeader = true;

			try
			{
				boost::asio::ip::tcp::resolver resolver(_service);
				boost::asio::ip::tcp::resolver::query query(boost::asio::ip::host_name(), "");
				boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
				boost::asio::ip::tcp::resolver::iterator end; // End marker.

				while (iter != end)
				{
					boost::asio::ip::tcp::endpoint ep = *iter++;
					if(ep.address().is_v4())
					{
						_localIPs.push_back(ep.address().to_string());
					}
				}

				for(unsigned short port = listenPortStart; port < listenPortStart+listenPortRange; port++)
				{
					try
					{
						_socket.open(boost::asio::ip::udp::v4());
						_socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
						_socket.bind(boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port));
						_listenPort = port;
						break;
					}
					catch (...)
					{
						//do nothing
					}
				}
			}
			catch(const std::exception& ex)
			{
				std::stringstream message;
				message << "Could not create udp communication." << std::endl << ex.what() << std::endl;
				E_OUT(message.str());
			}
			catch(...)
			{
				E_OUT("Could not create udp communication.");
			}
		}

		UdpCommunication::~UdpCommunication()
		{
			stop();
		}

		void UdpCommunication::broadcast(const Message& message, unsigned short port /*= 0*/)
		{
			try
			{
				if(_broadcastOptionSet)
				{
					_socket.set_option(boost::asio::ip::udp::socket::broadcast(true));
				}
				if(port <= 0 && _sendPortStart <= 0)
				{
					//error
					return;
				}
				else if(port > 0)
				{
					boost::asio::ip::udp::endpoint senderEndpoint(_broadcastIp, port);
					_socket.send_to(boost::asio::buffer(message.data(), message.length()), senderEndpoint);	
				}
				else
				{
					for(port = _sendPortStart; port < _sendPortStart+_sendPortRange; port++)
					{

						boost::asio::ip::udp::endpoint senderEndpoint(_broadcastIp, port);
						_socket.send_to(boost::asio::buffer(message.data(), message.length()), senderEndpoint);	
					}
				}
				if(_broadcastOptionSet)
				{
					_socket.set_option(boost::asio::ip::udp::socket::broadcast(false));
				}
			}
			catch(const std::exception& ex)
			{
				std::stringstream message;
				message << "Could not send udp broadcast message." << std::endl << ex.what() << std::endl;
				E_OUT(message.str());
			}
			catch(...)
			{
				E_OUT("Could not send udp broadcast message.");
			}
		}

		void UdpCommunication::send(const Message& message, std::string ip, unsigned short port /*= 0*/ )
		{
			try
			{
				if(port <= 0 && _sendPortStart <= 0)
				{
					//error
					return;
				}
				else if(port > 0)
				{
					boost::asio::ip::udp::endpoint senderEndpoint(boost::asio::ip::address_v4::from_string(ip), port);
					_socket.send_to(boost::asio::buffer(message.data(), message.length()),senderEndpoint);
				}
				else
				{
					boost::asio::ip::address_v4 sendToIp = boost::asio::ip::address_v4::from_string(ip);
					for(port = _sendPortStart; port < _sendPortStart+_sendPortRange; port++)
					{
						boost::asio::ip::udp::endpoint senderEndpoint(sendToIp, port);
						_socket.send_to(boost::asio::buffer(message.data(), message.length()),senderEndpoint);
					}
				}
			}
			catch(const std::exception& ex)
			{
				std::stringstream message;
				message << "Could not send udp message." << std::endl << ex.what() << std::endl;
				E_OUT(message.str());
			}
			catch(...)
			{
				E_OUT("Could not send udp message.");
			}
		}

		void UdpCommunication::start()
		{
			try
			{
				doRead();
				_thread = new boost::thread(boost::bind(&boost::asio::io_service::run, &_service));
			}
			catch(const std::exception& ex)
			{
				std::stringstream message;
				message << "Could not start udp communication." << std::endl << ex.what() << std::endl;
				E_OUT(message.str());
			}
			catch(...)
			{
				E_OUT("Could not start udp communication.");
			}
		}

		void UdpCommunication::stop()
		{
			try
			{
				if(_socket.is_open())
				{
					_socket.close();
				}
				_service.stop();
				if(_thread)
				{
					_thread->detach();
					delete _thread;
					_thread = nullptr;
				}
			}
			catch(const std::exception& ex)
			{
				std::stringstream message;
				message << "Could not stop udp communication." << std::endl << ex.what() << std::endl;
				E_OUT(message.str());
			}
			catch(...)
			{
				E_OUT("Could not stop udp communication.");
			}
		}

		void UdpCommunication::doRead()
		{
			try
			{
				Message* readMessage = new Message(_enableMessageHeader);
				_socket.async_receive_from(boost::asio::buffer(readMessage->data(), Message::maxUdpMessageLength), _receiveEndpoint,
					[this, readMessage](const boost::system::error_code& ec, std::size_t length)
				{
					if (!ec)
					{
						readMessage->bodyLength(length);
						readMessage->decodeHeader();
						readMessage->sender(_receiveEndpoint.address().to_string());
						onMessageReceived(*readMessage);
						if(!_disableBuffer)
						{
							_readMessages.push_back(Message(*readMessage));
						}
					}
					delete readMessage;
					//start new receive
					doRead();

				});
			}
			catch(const std::exception& ex)
			{
				std::stringstream message;
				message << "Could not read udp message." << std::endl << ex.what() << std::endl;
				E_OUT(message.str());
			}
			catch(...)
			{
				E_OUT("Could not read udp message.");
			}
		}

		UdpCommunication& UdpCommunication::operator+=( const signal_t::slot_type &rhs )
		{
			onMessageReceived.connect(rhs);
			return *this;
		}

		boost::signals2::connection UdpCommunication::connect( const signal_t::slot_type &rhs )
		{
			return onMessageReceived.connect(rhs);
		}

		void UdpCommunication::clearBuffer()
		{
			_readMessages.clear();
		}

		std::vector<Message> UdpCommunication::getMessages()
		{
			return _readMessages;
		}

		size_t UdpCommunication::getMessageCount()
		{
			return _readMessages.size();
		}

		void UdpCommunication::disableReceiveBuffer( bool value /*= true*/ )
		{
			_disableBuffer = value;
		}

		void UdpCommunication::setEnableMessageHeader( bool value )
		{
			_enableMessageHeader = value;
		}

		bool UdpCommunication::getEnableMessageHeader()
		{
			return _enableMessageHeader;
		}

	};
};
