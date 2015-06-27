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
#ifndef __COMMANDHELPER_H__
#define __COMMANDHELPER_H__
#include "CoreDLL.h"
#include <string>
#include <vector>
#include <boost/bimap/bimap.hpp>
#include <QVariant>

namespace Core
{
	namespace Utilities
	{
		class CORE_API CommandHelper 
		{
		public:
			typedef boost::bimaps::bimap<std::string, unsigned short> commandMap;
			typedef commandMap::value_type position;

			enum Command: unsigned short
			{
				UNDEFINED				= 0x0000,
				SETMINCLAMPDISTANCE		= 0x0001,
				GETMINCLAMPDISTANCE		= 0x0002,
				SETMAXCLAMPDISTANCE		= 0x0003,
				GETMAXCLAMPDISTANCE		= 0x0004,
				SETCLAMPRANGE			= 0x0005,
				GETCLAMPRANGE			= 0x0006,
				SETFILEWRITERENABLED	= 0x0007,
				GETFILEWRITERENABLED	= 0x0008,
				SETFILENAME				= 0x0009,
				GETFILENAME				= 0x000A,
				SETENABLECLAMP			= 0x000B,
				GETENABLECLAMP			= 0x000C,
				USER					= 0x0400
			};
		private:
			void init();
		public:
			CommandHelper();
			~CommandHelper();
			void convertValues(const std::string& value, std::vector<QVariant>& returnValue);
			unsigned short getCommandId(const std::string& command);
			const std::string getCommand(const unsigned short commandId) const;
			bool registerCommand( const std::string& command, unsigned short commandId );

			static const std::string Delimiter;
		private:
			commandMap	_commandLookupTable;	//!< The lookup table for the command ids.
		};
	};
};

#endif	// __COMMANDHELPER_H__
