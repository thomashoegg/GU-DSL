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

#include "Utilities/CommandHelper.h"
#include <boost/algorithm/string.hpp>
#include "DebugOutput/DebugStream.h"
#include <sstream>

namespace Core
{
	namespace Utilities
	{
		const std::string CommandHelper::Delimiter = ";";

		CommandHelper::CommandHelper()
		{
			init();
		}

		CommandHelper::~CommandHelper()
		{

		}

		void CommandHelper::convertValues( const std::string& value, std::vector<QVariant>& returnValue )
		{
			returnValue.clear();
			std::vector<std::string> strings;
			boost::split(strings, value, boost::is_any_of(Delimiter));
			for(std::string str : strings)
			{
				returnValue.push_back(QVariant(str.c_str()));
			}
		}

		unsigned short CommandHelper::getCommandId( const std::string& command )
		{
			unsigned short commandId = UNDEFINED;
			try
			{
				commandId = _commandLookupTable.left.at(command);
			}
			catch (const std::out_of_range&)
			{
				//no command mapping found
				//do nothing
			}
			return commandId;
		}

		void CommandHelper::init()
		{
			_commandLookupTable.insert(position("SetMinClampDistance", SETMINCLAMPDISTANCE));
			_commandLookupTable.insert(position("GetMinClampDistance", GETMINCLAMPDISTANCE));
			_commandLookupTable.insert(position("SetMaxClampDistance", SETMAXCLAMPDISTANCE));
			_commandLookupTable.insert(position("GetMaxClampDistance", GETMAXCLAMPDISTANCE));
			_commandLookupTable.insert(position("SetClampDistance", SETCLAMPRANGE));
			_commandLookupTable.insert(position("GetClampDistance", GETCLAMPRANGE));
			_commandLookupTable.insert(position("SetFilewriterEnabled", SETFILEWRITERENABLED));
			_commandLookupTable.insert(position("GetFilewriterEnabled", GETFILEWRITERENABLED));
			_commandLookupTable.insert(position("SetFilename", SETFILENAME));
			_commandLookupTable.insert(position("GetFilename", GETFILENAME));
			_commandLookupTable.insert(position("SetEnableClamp", SETENABLECLAMP));
			_commandLookupTable.insert(position("GetEnableClamp", GETENABLECLAMP));
		}

		bool CommandHelper::registerCommand( const std::string& command, unsigned short commandId )
		{
			//check if command already in lookup table
			commandMap::left_const_iterator it = _commandLookupTable.left.find("command");
			if(it == _commandLookupTable.left.end())
			{
				//check if command id already in lookup table
				commandMap::right_const_iterator it2 = _commandLookupTable.right.find(commandId);
				if(it2 == _commandLookupTable.right.end())
				{
					//insert command
					_commandLookupTable.insert(position(command, commandId));
					return true;
				}
				else
				{
					std::string error = "command ID " + std::to_string(commandId) + " already registered";
					D_OUT2(__FUNCTION__, error)
						return false;
				}
			}
			else
			{
				std::string error = "command " + command + " already registered";
				D_OUT2(__FUNCTION__, error)
					return false;
			}
		}

		const std::string CommandHelper::getCommand( const unsigned short commandId ) const
		{

			std::string command = "";
			try
			{
				command = _commandLookupTable.right.at(commandId);
			}
			catch (const std::out_of_range&)
			{
				//no command mapping found
				//do nothing
			}
			return command;
		}
	}; //namespace Utilities
}; //namespace Core
