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
#ifndef __UDPCOMMUNICATION_H__
#define __UDPCOMMUNICATION_H__
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/signals2/signal.hpp>
#include <string>
#include <vector>
#include "CoreDLL.h"
#include "Communication/Message.h"

#pragma warning( push )
#pragma warning( disable: 4251 )

namespace Core
{
	namespace Communication
	{
		/**
		 * \brief Udp communication class.
		 *
		 *	The maximum Message length is 1020 bytes.
		 */
		class CORE_API UdpCommunication
		{
		public:
			typedef boost::signals2::signal<void (Message)> signal_t;
		private:
			signal_t onMessageReceived;							//!< The message received event.
			unsigned short _listenPort;							//<! The port to listen to the messages.
			unsigned short _sendPortStart;						//<! The port to send to the messages.
			unsigned short _sendPortRange;						//<! The port range to send the messages to.
			boost::asio::io_service _service;					//<! The io service to handle async send and receive.
			boost::asio::ip::udp::socket _socket;				//<! The socket to receive messages.
			boost::asio::ip::udp::endpoint _receiveEndpoint;	//!< The receiver endpoint.
			boost::system::error_code _error;					//!< The last error code.
			boost::thread* _thread;								//!< The thread to execute the boost io service.
			boost::asio::ip::address_v4 _broadcastIp;			//!< The broadcast ip (e.g. 192.168.25.255).
			std::vector<Message> _readMessages;					//!< The received messages buffer.
			Message _readMessage;								//!< The current message to receive.
			bool _disableBuffer;								//!< Indicator to disable the receive buffer.
			bool _broadcastOptionSet;							//!< Indicator to enable broadcast socket option.
			std::vector<std::string> _localIPs;					//!< A list of all local ip addresses
			bool _enableMessageHeader;							//!< Indicator if message header is enabled.
		public:
			/**
			 * \brief constructor
			 *
			 *	@param listenPortStart start port of the port range to listen to receive messages.
			 *	@param sendPortStart start port of the port range to send the messages to.
			 *	@param broadcastIp ip to broadcast to.
			 *	@param disableBuffer if true disable the receive buffer. default is false.
			 *	@param listenPortRange listen port range to listen to receive messages. default port range = 1.
			 *	@param sendPortRange send port range to send messages to. default port range = 1.
			 */
			UdpCommunication(unsigned short listenPortStart, unsigned short sendPortStart,
				boost::asio::ip::address_v4 broadcastIp = boost::asio::ip::address_v4::broadcast(),
				bool disableBuffer = false, unsigned short listenPortRange = 1, unsigned short sendPortRange = 1);

			/**
			 * \brief destructor
			 *
			 */
			~UdpCommunication();

			/**
			 * \brief start udp communication.
			 *
			 *	needs to be called to receive and send messages.
			 */
			void start();

			/**
			 * \brief stop udp communication.
			 *
			 */
			void stop();

			/**
			 * \brief broadcast a message.
			 *
			 *	@param message The message to broadcast.
			 *	@param port The port do send the broadcast to. If port is 0 the default port is used.
			 */
			void broadcast(const Message& message, unsigned short port = 0);

			/**
			 * \brief send a message to the ip.
			 *
			 *	@param message The message to send.
			 *	@param ip The ip address to send the message to.
			 *	@param port The port to send the message to. If port is 0 the default port is used.
			 */
			void send(const Message& message, std::string ip, unsigned short port = 0);

			/**
			 * \brief operator +=
			 *
			 */
			UdpCommunication& operator+= (const signal_t::slot_type &rhs);

			/**
			 * \brief connect on message received event.
			 *
			 */
			boost::signals2::connection connect(const signal_t::slot_type &rhs);

			/**
			 * \brief clear receive buffer.
			 *
			 */
			void clearBuffer();

			/**
			 * \brief get received messages.
			 *
			 */
			std::vector<Message> getMessages();

			/**
			 * \brief get number of messages in the receive buffer.
			 *
			 */
			size_t getMessageCount();

			/**
			 * \brief disable or enable receive buffer.
			 *
			 *	@param value if true disables the receive buffer.
			 */
			void disableReceiveBuffer(bool value = true);

			/**
			* \brief Returns a list of all local ip addresses
			*
			* \return std::vector<std::string> A list of all local ip addresses
			*/
			std::vector<std::string> getLocalIpAddresses() const { return _localIPs; };

			/**
			 * \brief enable/disable message header.
			 *
			 *	\param bool value. if value is true message header is enabled.
			 */
			void setEnableMessageHeader(bool value);

			/**
			 * \brief get indicator if message header is enabled.
			 *
			 *	\return bool true if message header is enabled.
			 */
			bool getEnableMessageHeader();

		private:
			/**
			 * \brief read incoming messages.
			 *
			 */
			void doRead();
		};
	};
};

#pragma warning ( pop )
#endif
