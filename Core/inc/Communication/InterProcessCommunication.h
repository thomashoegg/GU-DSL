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
#ifndef __INTERPROCESSCOMMUNICATION_H__
#define __INTERPROCESSCOMMUNICATION_H__
#include "CoreDLL.h"
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/signals2/signal.hpp>
#include <boost/thread.hpp>
#include "Communication/Message.h"

#pragma warning( push )
#pragma warning( disable: 4251 )

namespace Core
{
	namespace Communication
	{
		#define READ_INTERVALL 5

		/**
		 * \brief Interprocess communication class
		 *
		 *	The maximum Message length is 1020 bytes.
		 */
		class CORE_API InterprocessCommunication
		{
		public:
			typedef boost::signals2::signal<void (Message)> signal_t;

		private:
			signal_t onMessageReceived;					//!< the message received event.
			boost::interprocess::message_queue _queue;	//!< The inter process communication queue.
			Message _receiveMessage;					//!< The current message to receive.
			boost::thread _thread;						//!< The thread to poll for new messages.
			boost::mutex _mutex;						//!< The mutex for thread synchronization.
		protected:
			/**
			 * \brief read message from queue.
			 *
			 */
			void read();
		public:
			/**
			 * \brief Constructor.
			 *
			 *	@param queueName Message queue name.
			 *	@param length Message queue length.
			 */
			InterprocessCommunication(const std::string queueName, int length = 10);

			/**
			 * \brief Destructor.
			 *
			 */
			~InterprocessCommunication();

			/**
			 * \brief send a message
			 *
			 *	@param message the message to send.
			 */
			void send(const Core::Communication::Message& message);

			/**
			 * \brief operator +=
			 *
			 */
			InterprocessCommunication& operator+= (const signal_t::slot_type& rhs);

			/**
			 * \brief connect on message received event.
			 *
			 */
			boost::signals2::connection connect(const signal_t::slot_type& rhs);
		};
	};
};

#pragma warning( pop )
#endif
