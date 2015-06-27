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

#ifndef __BASIC_STOPWATCH_H__
#define __BASIC_STOPWATCH_H__

#if WIN32
#pragma once;
#endif
#pragma once;

#include <string>
#include <boost/shared_ptr.hpp>
#include "CoreDLL.h"

namespace Core
{
	namespace Utilities
	{
		class CORE_API StopWatch 
		{
		public:
			/**
			* \brief The Constructor
			*/
			StopWatch();

			/**
			* \brief Start the stop watch
			* \return void
			*/
			inline void start();

			/**
			* \brief Reset the stop watch counters
			* \return void
			*/
			inline void reset();

			/**
			* \brief Stop the stopwatch and return the passed time
			* \return double The passed time since start
			*/
			inline double stop();

			/**
			 * \brief Get the stopwatch elapsed time.
			 * \return double The elapsed time.
			 */
			inline double getTime();

			/**
			* \brief Indicates if the timer is started
			*
			* \return bool true if started, else false
			*/
			bool isStarted() const;

		private:
			double			_PCFreq;					//!< The frequency
			__int64			_CounterStart;				//!< Start time
			bool			_isStarted;					//!< Determines if the watch is started
		
		};
		typedef boost::shared_ptr<StopWatch> StopWatchPtr;
	}; //namespace Utilities
}; //namespace Core

#endif			// __BASIC_STOPWATCH_H__
