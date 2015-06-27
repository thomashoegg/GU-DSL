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

#include "Utilities/WaitCondition.qt.h"
#include <QReadWriteLock>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QMutex>

namespace Core
{
namespace Utilities
{
	WaitCondition::WaitCondition()
		: _condition(new QWaitCondition()),
		_skipWait(false),
		_skipWaitMutex(new QMutex())
	{
	}

	WaitCondition::~WaitCondition()
	{
		delete _condition;
		_condition = nullptr;

		delete _skipWaitMutex;
		_skipWaitMutex = nullptr;
	}

	bool WaitCondition::wait( QMutex * lockedMutex, unsigned long time /*= ULONG_MAX*/ )
	{
		QMutexLocker locker(_skipWaitMutex);
		bool returnValue = _skipWait;
		if(_skipWait)
		{
			_skipWait = false;
		}
		locker.unlock();

		if(!returnValue)
		{
			returnValue = _condition->wait(lockedMutex, time);
		}

		locker.relock();
		if(returnValue && _skipWait)
		{
			_skipWait = false;
		}
		locker.unlock();

		return returnValue;
	}

	bool WaitCondition::wait( QReadWriteLock * lockedReadWriteLock, unsigned long time /*= ULONG_MAX*/ )
	{
		QMutexLocker locker(_skipWaitMutex);
		bool returnValue = _skipWait;
		if(_skipWait)
		{
			_skipWait = false;
		}
		locker.unlock();

		if(!returnValue)
		{
			returnValue = _condition->wait(lockedReadWriteLock, time);
		}

		locker.relock();
		if(returnValue && _skipWait)
		{
			_skipWait = false;
		}
		locker.unlock();

		return returnValue;
	}

	void WaitCondition::wake()
	{
		QMutexLocker locker(_skipWaitMutex);
		_skipWait = true;
		locker.unlock();
		_condition->wakeAll();
	}
}; //namespace Utilities
}; //namespace Core
