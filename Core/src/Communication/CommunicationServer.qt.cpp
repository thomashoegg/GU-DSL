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

#include "Communication/CommunicationServer.qt.h"
#include <QThread>
#include <QMutex>
#include "Utilities/WaitCondition.qt.h"
#include <assert.h>

namespace Core
{
	namespace Communication
	{

CommunicationServer::CommunicationServer()
	: _isRunning(false),
	_thread( new QThread()), 
	_waitMutex(new QMutex()),
	_waitForDataCondition(new Core::Utilities::WaitCondition())
{
	QObject::connect(_thread, SIGNAL(finished()), this, SLOT(finished()));
	moveToThread(_thread);
}

CommunicationServer::~CommunicationServer()
{
}

void CommunicationServer::init()
{
	if(!_isRunning)
	{
		_isRunning = true;
		_thread->start();
		QMetaObject::invokeMethod(this, "doWork", Qt::QueuedConnection);
	}
}

void CommunicationServer::doWork()
{
	run();
}

bool CommunicationServer::isRunning() const
{
	return _isRunning;
}

void CommunicationServer::finished()
{
	_isRunning = false;
}

Core::DebugOutput::DebugLevel::ModuleDebugLevelFlag CommunicationServer::getDebugLevel()
{
	return Core::DebugOutput::DebugLevel::ModuleDebugLevelFlag((int)_debugLevel);
}

void CommunicationServer::setDebugLevel( Core::DebugOutput::DebugLevel::ModuleDebugLevelFlag value )
{
	_debugLevel = Core::DebugOutput::DebugLevel::ModuleDebugLevelFlag((int)value);
}

bool CommunicationServer::isModuleDebugLevel( Core::DebugOutput::DebugLevel::ModuleDebugLevel level )
{
	return _debugLevel.testFlag(level);
}

}; //namespace Communication
}; //namespace Core
