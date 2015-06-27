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

#include "Communication/CommunicationClient.qt.h"

#include "DebugOutput/DebugStream.h"
#include "Utilities/WaitCondition.qt.h"
#include <QMutex>
#include <QThread>

#include "Images/Image.h"

#include <assert.h>

namespace Core
{
namespace Communication
{

CommunicationClient::CommunicationClient()
	: _thread( new QThread()), 
	_waitMutex(new QMutex()),
	_waitForDataCondition(new Core::Utilities::WaitCondition())
{
	_isRunning = false;
	QObject::connect(_thread, SIGNAL(finished()), this, SLOT(finished()));
	moveToThread(_thread);
}

CommunicationClient::~CommunicationClient()
{
	_isRunning = false;

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

	if(_thread)
	{
		if(!_thread->isFinished())
		{
			_thread->quit();
		}

		delete _thread;
		_thread = nullptr;
	}
}

void CommunicationClient::init()
{
	if(!_isRunning)
	{
		_isRunning = true;
		_thread->start();
		QMetaObject::invokeMethod(this, "doWork", Qt::QueuedConnection);
	}
}

void CommunicationClient::doWork()
{
	run();

	//_isRunning = true; // moved to init

	while(_isRunning)
	{
		QMutexLocker locker(_waitMutex); 
		//	_waitForDataCondition->wait(_waitMutex);

		try
		{
			if(!onImageReceived.empty())
			{
				Container::Images::Image image;
				getImage(image, true);

				if(image.frameIndexSize() > 0 && image.width() != 0 && image.height() != 0)
				{	
					onImageReceived(image);
				}
			}
		}
		catch(const std::exception& ex)
		{
			D_OUT2(__FUNCTION__, ex.what());
			_isRunning = false;
		}
		catch (...)
		{}

		QThread::msleep(10);
	}
}

void CommunicationClient::finished()
{
	_isRunning = false;
}

ConnectionPtr CommunicationClient::operator+=( const signal_t::slot_type &rhs )
{
	return ConnectionPtr(new boost::signals2::connection(onImageReceived.connect(rhs)));
}

ConnectionPtr CommunicationClient::bind( const signal_t::slot_type &rhs )
{
	return ConnectionPtr(new boost::signals2::connection(onImageReceived.connect(rhs)));
}

}; //namespace Communication
}; //namespace Core
