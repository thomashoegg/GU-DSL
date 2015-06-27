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

#include "Utilities/StopWatch.h"
#include <iostream>
#include <windows.h>
#include "DebugOutput/DebugStream.h"

namespace Core
{
namespace Utilities
{

StopWatch::StopWatch() :
	_CounterStart(0),
	_PCFreq(0.0),
	_isStarted(false)
{

}

void StopWatch::start()
{
	reset();

	LARGE_INTEGER li;
	if(!QueryPerformanceFrequency(&li))
	{
		std::stringstream message;
		message << "QueryPerformanceFrequency failed! System error code: " << GetLastError();
		E_OUT(message.str());
	}

	_PCFreq = double(li.QuadPart)/1000.0;

	QueryPerformanceCounter(&li);
	_CounterStart = li.QuadPart;

	_isStarted = true;
}

void StopWatch::reset()
{
	_PCFreq = 0.0;
	_CounterStart = 0;
	_isStarted = false;
}

double StopWatch::stop()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);

	_isStarted = false;

	return double (li.QuadPart-_CounterStart)/_PCFreq;
}

double StopWatch::getTime()
{
	if(_isStarted)
	{
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		return double (li.QuadPart-_CounterStart)/_PCFreq;
	}
	else
	{
		return 0;
	}
}

bool StopWatch::isStarted() const
{
	return _isStarted;
}

}; //namespace Utilities
}; //namespace Core
