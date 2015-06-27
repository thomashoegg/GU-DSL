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
#ifndef __MESSAGE_H__
#define __MESSAGE_H__
#include <string>
#include "CoreDLL.h"

#pragma warning( push )
#pragma warning( disable: 4251 )

namespace Core
{
	namespace Communication
	{
		/**
		 * \brief Message class
		 *
		 */
		class CORE_API Message
		{
		public:
			/**
			 * \brief
			 *
			 */
			enum
			{ 
				headerLength = 4,
				maxUdpMessageLength = 2048 
				//maxUdpMessageLength = 1024
			};
		private:
			char* _buffer;				//!< internal buffer
			size_t _bodyLength;			//!< body length
			std::string _sender;		//!< message sender
			bool _enableMessageHeader;	//!< enable message header.
			size_t _headerLength;		//!< header length;
		protected:
			/**
			 * \brief init the Message.
			 *
			 *	@param length Message body length.
			 *	@param buffer Message as buffer to copy.
			 *	@param sender Message sender as string.
			 */
			void init( const size_t length, const char* buffer = nullptr, std::string sender = "");
		public:
			/**
			 * \brief Default Constructor
			 *
			 *	@param enableMessageHeader enable message header.
			 */
			Message(bool enableMessageHeader = true);

			/**
			 * \brief Constructor.
			 *
			 *	Message from string.
			 *
			 *	@param message Message from string.
			 *  @param enableMessageHeader enable message header.
			 */
			Message(const std::string message, bool enableMessageHeader = true);

			/**
			 * \brief Constructor.
			 *
			 *	Message from buffer.
			 *
			 *	@param buffer Message buffer.
			 *	@param length Buffer length.
			 *	@param enableMessageHeader enable message header.
			 */
			Message(const char* buffer, const size_t length, bool enableMessageHeader = true);

			/**
			 * \brief Copy constructor.
			 *
			 *	@param Message to copy.
			 */
			Message(const Message&);

			/**
			 * \brief Destructor
			 *
			 */
			~Message();

			/**
			 * \brief get data buffer.
			 *
			 *	@return message data buffer.
			 */
			const char* data() const;

			/**
			 * \brief get data buffer.
			 *
			 *	@return message data buffer.
			 */
			char* data();

			/**
			 * \brief get message length.
			 *
			 * @return message length.
			 */
			size_t length() const;

			/**
			 * \brief get message body.
			 *
			 *	@return message body buffer.
			 */
			const char* body() const;

			/**
			 * \brief get message body.
			 *
			 *	@return message body buffer.
			 */
			char* body();

			/**
			 * \brief get message body length.
			 *
			 *	@return message body length.
			 */
			size_t bodyLength() const;

			/**
			 * \brief set new message body length.
			 *
			 *	@param newLength new message body length.
			 */
			void bodyLength(size_t newLength);

			/**
			 * \brief Decode header.
			 *
			 *	@return returns if successful.
			 */
			bool decodeHeader();

			/**
			 * \brief Encode header.
			 *
			 */
			void encodeHeader();

			/**
			 * \brief reinit the Message.
			 *
			 */
			void reinit();

			/**
			 * \brief get message sender.
			 *
			 *	@return message sender as string.
			 */
			std::string sender();

			/**
			 * \brief set message sender.
			 *
			 *	@param sender message sender.
			 */
			void sender(std::string sender);

		};
	};
};

#pragma warning( pop )
#endif
