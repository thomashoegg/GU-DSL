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
#ifndef __LASERSCANNERMESSAGE_H__
#define __LASERSCANNERMESSAGE_H__
#include <string>
#include <vector>
#include "CoreDLL.h"

namespace Core
{
	namespace Communication
	{
		/**
		 * \brief LaserscannerMessage class
		 *
		 */
		class CORE_API LaserscannerMessage
		{
		public:
			/**
			 * \brief
			 *
			 */
			enum
			{ 
				headerLength = 0,
				maxTcpMessageLength = 1500 
			};
		private:
			char* _buffer;				//!< internal buffer
			size_t _bodyLength;			//!< body length
			std::string _sender;	//!< message sender
			std::vector<std::string> _splittedBuffer;
			bool						_splitted;
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
			 */
			LaserscannerMessage();

			/**
			 * \brief Constructor.
			 *
			 *	Message from string.
			 *
			 *	@param message Message from string.
			 */
			LaserscannerMessage(const std::string message);

			/**
			 * \brief Constructor.
			 *
			 *	Message from buffer.
			 *
			 *	@param buffer Message buffer.
			 *	@param length Buffer length.
			 */
			LaserscannerMessage(const char* buffer, const size_t length);

			/**
			 * \brief Copy constructor.
			 *
			 *	@param Message to copy.
			 */
			LaserscannerMessage(const LaserscannerMessage&);

			/**
			 * \brief Destructor
			 *
			 */
			~LaserscannerMessage();

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

			std::string getCommandType();
			std::string getCommand();
			std::string getVersionNumber();
			std::string getDeviceNumber();
			int getSerialNumber();
			std::string getStatus();
			int getTelegramCounter();
			int getScanCounter();
			int getTimeSinceStartup();
			int getTimeOfTransmission();
			int getScanFrequency();
			int getMeasurementFrequency();
			int getStartingAngle();
			int getAngularStepWith();
			int getAmountOfData();
			std::string getContent();
			std::vector<int> getMeasurementData(); 



		};
	};
};

#endif
