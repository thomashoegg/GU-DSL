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

#include "Communication/InterProcessCommunication.h"

namespace Core
{
	namespace Communication
	{
		InterprocessCommunication::InterprocessCommunication(const std::string queueName, int length /*= 10*/)
			:_queue(boost::interprocess::open_or_create, queueName.c_str(), length, sizeof(Message)),
			_thread(boost::bind(&InterprocessCommunication::read, this))
		{
		}

		InterprocessCommunication::~InterprocessCommunication()
		{

		}

		void InterprocessCommunication::send( const Core::Communication::Message& message )
		{
			_queue.send(message.data(),message.length(),0);
		}

		void InterprocessCommunication::read()
		{
			size_t receiveSize =0;
			unsigned int priority = 0;
			_mutex.lock();
			_queue.receive(_receiveMessage.data(), Message::maxUdpMessageLength, receiveSize, priority);
			_receiveMessage.decodeHeader();
			_mutex.unlock();
			onMessageReceived(_receiveMessage);
			std::cout.write(_receiveMessage.body(), _receiveMessage.bodyLength());
			std::cout << "\n";
			boost::this_thread::sleep(boost::posix_time::seconds(READ_INTERVALL));
		}

		InterprocessCommunication& InterprocessCommunication::operator+=( const signal_t::slot_type& rhs )
		{
			onMessageReceived.connect(rhs);
			return *this;
		}

		boost::signals2::connection InterprocessCommunication::connect( const signal_t::slot_type& rhs )
		{
			return onMessageReceived.connect(rhs);
		}
	};
};
