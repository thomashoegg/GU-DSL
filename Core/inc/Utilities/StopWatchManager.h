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
#ifndef __STOPWATCHMANAGER_H__
#define __STOPWATCHMANAGER_H__
#include <string>
#include <unordered_map>
#include <boost/shared_ptr.hpp>
#include "Utilities/StopWatch.h"
#include "CoreDLL.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace Core
{
	namespace Utilities
	{

		class CORE_API StopWatchManager
		{
			typedef std::unordered_map<std::string, StopWatchPtr> StopWatchStore;
		public:
			static StopWatchManager& instance();

			/**
			 * \brief get stopwatch instance with the given name. If stopwatch does not exists create a new one.
			 *
			 *	\param std::string name. The stop watch name.
			 *	\return StopWatchPtr. The stop watch pointer.
			 */
			StopWatchPtr getStopWatch(std::string name = "");

		private:
			/**
			* \brief The Constructor
			*/
			StopWatchManager();

			StopWatchManager(const StopWatchManager&);  // Don't Implement
			void operator=(const StopWatchManager&);	// Don't implement
		private:
			StopWatchStore		_stopWatchStore;	//!< The stop watch store.
		};
	}; //namespace Utilities
}; //namespace Dore

#pragma warning(pop)

#endif			// __STOPWATCHMANAGER_H__
