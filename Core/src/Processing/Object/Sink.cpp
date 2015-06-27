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

#include "Processing/Object/Sink.qt.h"

#include "Utilities/WaitCondition.qt.h"
#include <QMutex>
#include <QThread>
#include <QCoreApplication>

#include "Processing/Object/Port.qt.h"

namespace Core
{
namespace Processing
{
namespace Object
{

Sink::Sink(QString uid)
	: Class(uid),
	_isRunning(true),
	_thread(nullptr),
	_waitForDataCondition(nullptr),
	_waitMutex(nullptr),
	_isProcessing(false)
{		
}

Sink::Sink( const Sink& rhs, QString uid )
	: Class(rhs, uid),
	_isRunning(true),
	_thread(nullptr),
	_waitForDataCondition(nullptr),
	_waitMutex(nullptr),
	_isProcessing(false)
{

}

Sink::Sink( const Sink& rhs )
	: Class(rhs),
	_isRunning(true),
	_thread(nullptr),
	_waitForDataCondition(nullptr),
	_waitMutex(nullptr),
	_isProcessing(false)
{

}

Sink::~Sink()
{
	cleanup();
}


void Sink::init()
{
	// A thread can be started once only
	if(_thread != nullptr)
		return;

	_waitMutex = new QMutex();
	_waitForDataCondition = new Core::Utilities::WaitCondition();

	_thread = new QThread();

	connect(_thread, SIGNAL(finished()), this, SLOT(finished()));

	moveToThread(_thread);
	_thread->start();
	start();
	QMetaObject::invokeMethod(this, "doWork", Qt::QueuedConnection);
}

void Sink::cleanup()
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
	}

	if(_thread)
	{
		delete _thread;
		_thread = nullptr;
	}

	if(_waitMutex)
	{
		delete _waitMutex;
		_waitMutex = nullptr;
	}

	if(_waitForDataCondition)
	{
		delete _waitForDataCondition;
		_waitForDataCondition = nullptr;
	}
}

bool Sink::process( Core::Processing::Interfaces::ConstIObjectParametersPtr parameters )
{
	return false;
}



void Sink::doWork()
{
	startSubModules();

	_isRunning = true;

	while(_isRunning)
	{
		if(_isProcessing)
		{
			_waitMutex->lock();
			if(!_isEventBased || _waitForDataCondition->wait(_waitMutex, 5))
			{
				if(_enabled)
				{
					process(nullptr);
				}
				QThread::usleep(10);
			}
			_waitMutex->unlock();
		}
		else
		{
			QThread::msleep(100);
		}
		QCoreApplication::processEvents();

		REQUEST_PORT_DATA;
	}

	_isProcessing = false;
	_isRunning = false;
}

void Sink::finished()
{
	_isProcessing = false;
	_isRunning = false;
}

void Sink::start()
{
	QMetaObject::invokeMethod(this, "startSubModules", Qt::BlockingQueuedConnection);
	_isProcessing = true;
}

void Sink::stop()
{
	_isProcessing = false;
}

void Sink::startSubModules()
{

}

};	// namespace Object
};	// namespace Processing
}; //namespace Core
