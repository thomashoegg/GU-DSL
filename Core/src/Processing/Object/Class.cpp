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

#include "Processing/Object/Class.qt.h"
#include "Processing/Interfaces/IPort.qt.h"
#include "Processing/Interfaces/IProcessor.qt.h"
#include "Processing/Interfaces/IProvider.qt.h"
#include "Processing/Object/Port.qt.h"
#include "Processing/PtrDefinitions.h"
#include "DebugOutput/DebugStream.h"
#include <exception>
#include "Utilities/StopWatch.h"
#include <QThread>

namespace Core
{
namespace Processing
{
namespace Object
{

Class::Class(QString uid)
	: IObject(uid),
	_stopWatch(nullptr),
	_debugLevel(Core::DebugOutput::DebugLevel::ModuleDebugLevel::Off),
	_enableViewer(true),
	_enabled(true),
	_isEventBased(true)
{		
}

Class::Class( const Class& rhs )
	: IObject(rhs),
	_stopWatch(nullptr),
	_debugLevel(Core::DebugOutput::DebugLevel::ModuleDebugLevel::Off),
	_enableViewer(rhs._enableViewer),
	_enabled(rhs._enabled),
	_isEventBased(rhs._isEventBased)
{
	for(PortMap::const_iterator it = rhs._ports.begin(); it != rhs._ports.end(); it++ )
	{
		std::string portname(it->first);
		Core::Processing::Interfaces::IPortPtr port = boost::dynamic_pointer_cast<Core::Processing::Interfaces::IPort>(it->second->create());
		if(port)
		{
			addPort(port, portname.c_str());
		}
		else
		{
			E_OUT("Could not copy port")
		}
	}
}

Class::Class( const Class& rhs, QString uid )
	: IObject(rhs, uid),
	_stopWatch(nullptr),
	_debugLevel(Core::DebugOutput::DebugLevel::ModuleDebugLevel::Off),
	_enableViewer(rhs._enableViewer),
	_enabled(rhs._enabled),
	_isEventBased(rhs._isEventBased)
{
	for(PortMap::const_iterator it = rhs._ports.begin(); it != rhs._ports.end(); it++ )
	{
		std::string portname(it->first);
		Core::Processing::Interfaces::IPortPtr port = boost::dynamic_pointer_cast<Core::Processing::Interfaces::IPort>(it->second->create());
		if(port)
		{
			addPort(port, portname.c_str());
		}
		else
		{
			E_OUT("Could not copy port")
		}
	}
}

Class::~Class()
{

}

bool Class::addPort( Core::Processing::Interfaces::IPortPtr port, QString portName )
{
	if(!port || portName.isEmpty())
		return false;

	// Check if there is already a port with this name
	if(_ports.find(portName.toStdString()) != _ports.end())
		return false;

	port->setName(portName);
	_ports.insert(PortMapPair(portName.toStdString(), port));

	Core::Processing::Interfaces::IProcessorPtr processorPtr = boost::dynamic_pointer_cast<Core::Processing::Interfaces::IProcessor>(port);

	if(processorPtr)
		registerProcessorPortListeners(port);

	return true;
}

bool Class::addInterface(QString portName, Core::Processing::Interfaces::IInterfacePtr interfacePtr )
{
	PortMap::iterator portIt = _ports.find(portName.toStdString());
	if(portIt != _ports.end())
		return (*portIt).second->addInterface(interfacePtr);

	return false;
}

bool Class::addAsyncInterface( QString portName, Core::Processing::Interfaces::IInterfacePtr interfacePtr )
{
	return false;
}

const Core::Processing::Object::Class::PortMap Class::getPortMap() const
{
	return _ports;
}

void Class::registerProcessorPortListeners()
{
	for(PortMap::const_iterator it = _ports.begin(); it != _ports.end(); it++ )
	{
		if(it->second)
		{
			registerProcessorPortListeners(it->second);
		}
		else
		{
			E_OUT("Could not copy port")
		}
	}
}

Class::QtWrapperModuleDebugLevelFlag Class::getDebugLevel()
{
	return QtWrapperModuleDebugLevelFlag((int)_debugLevel);
}

void Class::setDebugLevel(Class::QtWrapperModuleDebugLevelFlag value )
{
	_debugLevel = Core::DebugOutput::DebugLevel::ModuleDebugLevelFlag((int)value);
}

bool Class::isModuleDebugLevel( Core::DebugOutput::DebugLevel::ModuleDebugLevel level )
{
	return _debugLevel.testFlag(level);
}

bool Class::getEnableViewer()
{
	return _enableViewer;
}

void Class::setEnableViewer( bool value )
{
	_enableViewer = value;
}

bool Class::getEnabled()
{
	return _enabled;
}

void Class::setEnabled( bool enabled )
{
	_enabled = enabled;
}

void Class::handleCloseEvent( const Core::Mediation::Events::CloseEvent& event )
{
	Core::Mediation::Events::CloseEvent closeEvent = const_cast<Core::Mediation::Events::CloseEvent&>(event);
	if(this->thread() == QThread::currentThread())
	{
		doHandleCloseEvent(closeEvent.getExitCode());
	}
	else
	{
		QMetaObject::invokeMethod(this, "doHandleCloseEvent", Qt::AutoConnection, Q_ARG(int, closeEvent.getExitCode()));
	}
}

void Class::doHandleCloseEvent(int exitCode)
{
	stop();
	setEnabled(false);
}

};	// namespace Object
};	// namespace Processing
}; //namespace Core
