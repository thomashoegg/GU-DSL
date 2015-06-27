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

#include "Communication/Message.h"

namespace Core
{
	namespace Communication
	{
		Message::Message(bool enableMessageHeader /*= true*/)
		{
			_enableMessageHeader = enableMessageHeader;
			if(_enableMessageHeader)
			{
				_headerLength = headerLength;
			}
			else
			{
				_headerLength = 0;
			}
			init(maxUdpMessageLength - _headerLength);
		}

		Message::Message( const std::string message, bool enableMessageHeader /*= true*/ )
		{
			_enableMessageHeader = enableMessageHeader;
			if(_enableMessageHeader)
			{
				_headerLength = headerLength;
			}
			else
			{
				_headerLength = 0;
			}
			init(message.length(), message.c_str());
		}

		Message::Message( const char* buffer, const size_t length, bool enableMessageHeader /*= true*/ )
		{
			_enableMessageHeader = enableMessageHeader;
			if(_enableMessageHeader)
			{
				_headerLength = headerLength;
			}
			else
			{
				_headerLength = 0;
			}
			init(length, buffer);
		}

		Message::Message( const Message& rhs)
			:_enableMessageHeader(rhs._enableMessageHeader),
			_headerLength(rhs._headerLength)
		{
			init(rhs._bodyLength, rhs.body(), rhs._sender);
		}

		void Message::init( const size_t length, const char* buffer /*= nullptr*/, std::string sender /*= ""*/ )
		{
			_bodyLength = length;
			_sender = sender;
			_buffer = new char[_headerLength + _bodyLength];
			if(buffer)
			{
				memcpy(body(), buffer, _bodyLength);
			}
			encodeHeader();
		}

		void Message::reinit()
		{
			init(maxUdpMessageLength - _headerLength);
		}

		Message::~Message()
		{
			_bodyLength = 0;
			delete[] _buffer;
			_buffer = nullptr;
		}

		const char* Message::data() const
		{
			return _buffer;
		}

		char* Message::data()
		{
			return _buffer;
		}

		size_t Message::length() const
		{
			return _headerLength + _bodyLength;
		}

		const char* Message::body() const
		{
			return _buffer + _headerLength;
		}

		char* Message::body()
		{
			return _buffer + _headerLength;
		}

		size_t Message::bodyLength() const
		{
			return _bodyLength;
		}

		void Message::bodyLength( size_t newLength )
		{
			_bodyLength = newLength;
		}

		bool Message::decodeHeader()
		{
			if(_enableMessageHeader)
			{
				char header[headerLength + 1] = "";
				std::strncat(header, _buffer, headerLength);
				size_t oldBodyLength = _bodyLength;
				_bodyLength = std::atoi(header);

				//resize if buffer if necessary
				if(oldBodyLength < _bodyLength)
				{
					char* tmp = new char[headerLength + _bodyLength];
					std::memcpy(tmp, _buffer, headerLength + oldBodyLength);
					delete[] _buffer;
					_buffer = tmp;
				}
			}
			return true;
		}

		void Message::encodeHeader()
		{
			if(_enableMessageHeader)
			{
				char header[headerLength + 1] = "";
				std::sprintf(header, "%4d", _bodyLength);
				std::memcpy(_buffer, header, headerLength);
			}
		}

		std::string Message::sender()
		{
			return _sender;
		}

		void Message::sender( std::string sender )
		{
			_sender = sender;
		}

	};
};
