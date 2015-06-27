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

#pragma once;
#ifndef __TCPCOMMUNICATION_H__
#define __TCPCOMMUNICATION_H__
#include "CoreDLL.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <vector>
#include "Communication/Message.h"

#pragma warning( push )
#pragma warning( disable: 4251 )

namespace Core
{
	namespace Communication
	{
		/**
		 * \brief Tcp communication class.
		 *
		 */
		class CORE_API TcpCommunication
			: public boost::enable_shared_from_this< TcpCommunication >
		{
		private:
			typedef boost::asio::ip::tcp tcp;

		public:
			/**
			 * \brief constructor
			 *
			 *	@param listenPort standard port to listen to receive messages.
			 *	@param sendPort standard port to send the messages to.
			 */
			TcpCommunication( unsigned short listenPort, unsigned short sendPort );
			
			/**
			 * \brief destructor
			 *
			 */
			~TcpCommunication();

			/**
			 * \brief start tcp communication.
			 *
			 */
			void start();

			/**
			 * \brief stop tcp communication.
			 *
			 */
			void stop();

			/**
			 * \brief send a message to the ip.
			 *
			 *	@param message The message to send.
			 *	@param ip The ip address to send the message to.
			 *	@param port The port to send the message to. If port is 0 the default port is used.
			 */
			void send(const Message& message, std::string ip, unsigned short port = 0);
		private:

			/**
			 * \brief 
			 *
			 */
			void handleAccept( const boost::system::error_code& error );

			/**
			 * \brief
			 *
			 */
			void readHeader();

			/**
			 * \brief
			 *
			 */
			void
				handleReadHeader( const boost::system::error_code& error,
				std::size_t bytes_transferred );

			/**
			 * \brief
			 *
			 */
			void handleReadData( const boost::system::error_code& error,
				std::size_t bytes_transferred );

		private:
			unsigned short _listenPort;							//<! The port to listen to the messages.
			unsigned short _sendPort;							//<! The port to send to the messages.
			boost::asio::io_service _service;			//<! The io service to handle async send and receive.
			boost::asio::io_service::strand _strand;	//<! Provides serialized handler execution.
			tcp::acceptor _acceptor;					//<! The tcp acceptor.
			tcp::socket _socket;						//<! The tcp socket.
			std::vector<Message> _readMessages;			//<! The receive messages buffer.
			Message _readMessage;						//<! The current message to receive.
			boost::thread* _thread;						//<! The thread to execute the boost io service.
		};
	};
};

#pragma warning( pop )
#endif
