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

#include "Camera/ICamera.qt.h"
#include <QtCore/QObject>
#include "Processing/Interfaces/IObject.qt.h"

namespace Core
{
namespace Camera
{
ICamera::ICamera()
{
	_debugLevel = Core::DebugOutput::DebugLevel::Off;
}

ICamera::~ICamera()
{

}

void ICamera::openDatastream()
{

}

void ICamera::addDynamicProperties( QObject* object )
{
}

bool ICamera::propertyChanged( const std::string& propertyName, const std::string& value, const bool& dynamicProperty, Core::Processing::Interfaces::IObject* const cameraInterface )
{
	return false;
}

ConnectionPtr ICamera::operator+=( const signal_t::slot_type &rhs )
{
	return ConnectionPtr(new boost::signals2::connection(onImageReceived.connect(rhs)));
}

ConnectionPtr ICamera::bind( const signal_t::slot_type &rhs )
{
	return ConnectionPtr(new boost::signals2::connection(onImageReceived.connect(rhs)));
}

ConnectionPtr ICamera::operator+=( const signal_t_status::slot_type &rhs )
{
	return ConnectionPtr(new boost::signals2::connection(onStatus.connect(rhs)));
}

ConnectionPtr ICamera::bindStatus( const signal_t_status::slot_type &rhs )
{
	return ConnectionPtr(new boost::signals2::connection(onStatus.connect(rhs)));
}

ICamera::QtWrapperModuleDebugLevelFlag ICamera::getDebugLevel()
{
	return QtWrapperModuleDebugLevelFlag((int)_debugLevel);
}

void ICamera::setDebugLevel( QtWrapperModuleDebugLevelFlag value )
{
	_debugLevel = Core::DebugOutput::DebugLevel::ModuleDebugLevelFlag((int)value);
}

bool ICamera::isModuleDebugLevel( Core::DebugOutput::DebugLevel::ModuleDebugLevel level )
{
	return _debugLevel.testFlag(level);
}

void ICamera::setCameraSource(Core::Processing::Interfaces::IObjectWeakPtr cameraSource)
{
	_cameraSource = cameraSource;
}

bool ICamera::trigger()
{
	return true;
}

}; //namespace Camera
}; //namespace Core
