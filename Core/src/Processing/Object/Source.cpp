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

#include "Processing/Object/Source.qt.h"

#include "Processing/PtrDefinitions.h"
#include <exception>
#include <QCoreApplication>
#include <QMutex>
#include <QThread>
#include "Utilities/WaitCondition.qt.h"

namespace Core
{
namespace Processing
{
namespace Object
{

	Source::Source(QString uid)
	: Class(uid),
	  _thread(nullptr), 
	  _waitMutex(nullptr),
	  _waitForDataCondition(nullptr),
	  _isProcessing(false),
	  _isRunning(false)
{		
}


Source::Source( const Source& rhs, QString uid )
	: Class(rhs, uid),
	_thread(nullptr), 
	_waitMutex(nullptr),
	_waitForDataCondition(nullptr),
	_isProcessing(false),
	_isRunning(false)
{

}

Source::Source( const Source& rhs )
	: Class(rhs),
	_thread(nullptr), 
	_waitMutex(nullptr),
	_waitForDataCondition(nullptr),
	_isProcessing(false),
	_isRunning(false)
{
}

Source::~Source()
{
	cleanup();
}

void Source::start()
{
	if(this->thread() == QThread::currentThread())
	{
		startSubModules();
	}
	else
	{
		QMetaObject::invokeMethod(this, "startSubModules", Qt::BlockingQueuedConnection);
	}
	_isProcessing = true;
}

void Source::stop()
{
	_isProcessing = false;
}

bool Source::process( Core::Processing::Interfaces::ConstIObjectParametersPtr parameters )
{
	return false;
}

void Source::doWork()
{
	startSubModules();

	_isRunning = true;

	while(_isRunning)
	{
		if(_isProcessing)
		{
			QMutexLocker locker(_waitMutex); 
			if(!_isEventBased || _waitForDataCondition->wait(_waitMutex, 5))
			{
				if(_enabled)
				{
					if(!process(nullptr))
						break;
				}
				QThread::usleep(10);
			}

			updateProcessingState();
		}
		else
		{
			QThread::msleep(100);
		}
		updateState();
		QCoreApplication::processEvents();
	}

	_isProcessing = false;
	_isRunning = false;
}

void Source::finished()
{
	_isProcessing = false;
	_isRunning = false;
}

void Source::init()
{
	// A thread can be started once only
	if(_thread != nullptr)
		return;

	_thread = new QThread();
	_waitMutex = new QMutex();
	_waitForDataCondition = new Core::Utilities::WaitCondition();

	connect(_thread, SIGNAL(finished()), this, SLOT(finished()));

	moveToThread(_thread);
	_thread->start();

	//start();
	QMetaObject::invokeMethod(this, "doWork", Qt::QueuedConnection);
}

void Source::cleanup()
{
	stop();
	_isRunning = false;

	if(_thread)
	{
		if(_waitMutex && _waitForDataCondition)
		{
			_waitMutex->lock();
			_waitForDataCondition->wake();
			_waitMutex->unlock();
			QCoreApplication::processEvents();
			//_thread->wait(200);
		}

		_thread->wait(200);

		if(!_thread->isFinished())
		{
			QMetaObject::invokeMethod(_thread, "terminate");
			_thread->wait(200);
		}

		delete _thread;
		_thread = nullptr;
	}

	delete _waitMutex;
	_waitMutex = nullptr;

	delete _waitForDataCondition;
	_waitForDataCondition = nullptr;
}

void Source::startSubModules()
{

}

void Source::updateState()
{

}

void Source::updateProcessingState()
{

}

};	// namespace Object
};	// namespace Processing
}; //namespace Core
