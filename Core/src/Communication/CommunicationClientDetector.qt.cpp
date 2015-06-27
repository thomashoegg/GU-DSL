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
#include "Communication/Message.h"
#include "Communication/CommunicationClientDetector.h"
#include "Communication/MessageType.hpp"
#include <iomanip>
#include "DebugOutput/DebugStream.h"

namespace Core
{
	namespace Communication
	{
		CommunicationClientDetector::CommunicationClientDetector()
			:_udpCommunication(new Core::Communication::UdpCommunication(9655,9650, boost::asio::ip::address_v4::broadcast(), true))
		{
			_udpCommunication->connect(boost::bind(&CommunicationClientDetector::handleMessage, this, _1));
			_hello = nullptr;
			_bye = nullptr;
		}

		CommunicationClientDetector::~CommunicationClientDetector()
		{
			delete _udpCommunication;

			delete _hello;
			_hello = nullptr;
			delete _bye;
			_bye = nullptr;
		}

		void CommunicationClientDetector::handleMessage( Core::Communication::Message message )
		{
			D_OUT2(__FUNCTION__, "HandleMessage:");
			unsigned int messagetype =0;
			memcpy(&messagetype, message.body(), 4);
			std::cout << std::hex << messagetype << std::dec << std::endl;
			if(messagetype == (unsigned int)Core::Communication::MessageType::Hello)
			{
				sendHello();
			}
		}

		void CommunicationClientDetector::start(std::string messageData /*= ""*/)
		{
			_udpCommunication->start();
			setCommunicationManagerMessageData(messageData);
			sendHello();
		}

		void CommunicationClientDetector::stop()
		{
			sendBye();
			_udpCommunication->stop();
		}

		void CommunicationClientDetector::setCommunicationManagerMessageData( std::string messageData )
		{
			if(messageData.length() > 0)
			{
				unsigned int hello = (unsigned int)Core::Communication::MessageType::Hello;
				unsigned int bye = (unsigned int)Core::Communication::MessageType::Bye;
				char* buffer = new char[messageData.length()];
				memcpy(buffer,&hello,4);
				memcpy(buffer+4, messageData.data(), messageData.length());
				_hello = new Core::Communication::Message(buffer, 4 + messageData.length());
				memcpy(buffer,&bye,4);
				memcpy(buffer+4, messageData.data(), messageData.length());
				_bye = new Core::Communication::Message(buffer, 4 + messageData.length());
			}
		}

		void CommunicationClientDetector::sendHello()
		{
			if(_hello)
			{
				_udpCommunication->broadcast(*_hello);
			}
		}

		void CommunicationClientDetector::sendBye()
		{
			if(_bye)
			{
				_udpCommunication->broadcast(*_bye);
			}
		}
	};
};
