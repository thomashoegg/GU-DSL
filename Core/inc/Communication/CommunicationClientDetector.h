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
#ifndef __COMMUNICATIONCLIENTDETECTOR_H__
#define __COMMUNICATIONCLIENTDETECTOR_H__
#include "CoreDLL.h"
#include <string>

namespace Core
{
	namespace Communication
	{
		class Message;
		class UdpCommunication;

		/**
		 * \brief processing detector.
		 *		detect all processing modules to send Communication data to.
		 */
		class CORE_API CommunicationClientDetector
		{
		private:
			Core::Communication::UdpCommunication* _udpCommunication;	//!< udp communication module.
			Core::Communication::Message* _hello;						//!< the broadcast hello message.
			Core::Communication::Message* _bye;						//!< the broadcast bye message.
		protected:
			/**
			 * \brief handle received messages.
			 *
			 *	@param message received message.
			 */
			void handleMessage(Core::Communication::Message message);
		public:
			/**
			 * \brief constructor
			 *
			 */
			CommunicationClientDetector();
		
			/**
			 * \brief destructor 
			 *
			 */
			~CommunicationClientDetector();

			/**
			 * \brief start process detector module.
			 *
			 *	@param std::string messageData message data to send to the processing modules.
			 */
			void start(std::string messageData = "");

			/**
			 * \brief stop process detector module.
			 *
			 */
			void stop();

			/**
			 * \brief set Communication manager message data.
			 *
			 *	@param std::string messageData message data to send to the processing modules.
			 */
			void setCommunicationManagerMessageData(std::string messageData);

			/**
			 * \brief send hello message to all known processing modules.
			 *
			 */
			void sendHello();

			/**
			 * \brief send bye message to all known processing modules.
			 *
			 */
			void sendBye();
		};
	};
};

#endif // __COMMUNICATIONCLIENTDETECTOR_H__
