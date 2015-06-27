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

#include "Processing/Object/Port.qt.h"
#include "Processing/Interfaces/IProcessor.qt.h"
#include "Processing/Interfaces/IProvider.qt.h"

#include <unordered_map>
#include <exception>

#include "Processing/PtrDefinitions.h"

namespace Core
{
namespace Processing
{
namespace Object
{

Port::Port(QString uid)
	: IPort(uid)
{
}

Port::Port( const Port& rhs )
	: IPort(rhs)
{

}

Port::Port( const Port& rhs, QString uid )
	: IPort(rhs, uid)
{

}

bool Port::addInterface(Core::Processing::Interfaces::IInterfacePtr interface)
{
	// First check if the interface is a provider
	Core::Processing::Interfaces::IProviderPtr lPtr = boost::dynamic_pointer_cast<Core::Processing::Interfaces::IProvider>(interface);
	if(lPtr != nullptr)
	{
		// Check if it is already added
		if(_interfaces.contains(interface))
		{
#if DEBUG
			QString excepString("%1: %2");
			excepString.arg(__FUNCTION__).arg(": interface already added!");

			throw std::exception(excepString.toLatin1());
#endif
			return false;
		}

		// Else append the new interface
		_interfaces.append(interface);
		_providers.append(interface);

		return true;
	}

	// Second check if the interface is a processor
	Core::Processing::Interfaces::IProcessorPtr sPtr = boost::dynamic_pointer_cast<Core::Processing::Interfaces::IProcessor>(interface);
	if(sPtr != nullptr)
	{
		// Check if it is already added
		if(_interfaces.contains(interface))
		{
#if DEBUG
			QString excepString("%1: %2");
			excepString.arg(__FUNCTION__).arg(": interface already added!");

			throw std::exception(excepString.toLatin1());
#endif
			return false;
		}

		// Else append the new interface
		_interfaces.append(interface);
		_processors.append(interface);

		return true;
	}

	return false; 
}

bool Port::connectInterfaces(Core::Processing::Interfaces::IProviderPtr source, Core::Processing::Interfaces::IProcessorPtr target)
{
	if(!source)
	{
#if DEBUG
		QString excepString("%1: %2");
		excepString.arg(__FUNCTION__).arg(": source parameter null!");
		
		throw std::exception(excepString.toLatin1());
#endif
		return false;
	}

	if(!target)
	{
#if DEBUG
		QString excepString("%1: %2");
		excepString.arg(__FUNCTION__).arg(": target parameter null!");

		throw std::exception(excepString.toLatin1());
#endif
		return false;
	}

	ProviderProcessorListMap::iterator itFound = _sourceTargetMap.find(source.get());
	if(itFound == _sourceTargetMap.end())
	{
		// If we don't have an entry, then insert a new one
		QList<Core::Processing::Interfaces::IProcessorPtr> processors;
		_sourceTargetMap.insert(ProviderProcessorListPair(source.get(), processors));
		itFound = _sourceTargetMap.find(source.get());
	}

	// Check if we have already a connection
	if(itFound->second.contains(target))
	{
#if DEBUG
		QString excepString("%1: %2");
		excepString.arg(__FUNCTION__).arg(": interfaces already connected!");

		throw std::exception(excepString.toLatin1());
#endif
		return false;
	}
	else	// if there is no connection, add the new target
	{
		if(connectInterfaceMembers(source, target))
			itFound->second.append(target);
	}

	return true;
}


bool Port::disconnectInterfaces(Core::Processing::Interfaces::IProviderPtr source, Core::Processing::Interfaces::IProcessorPtr target)
{
	return true;
}

const Core::Processing::Object::Port::ProviderProcessorListMap Port::getProviderProcessorListMap() const
{
	return _sourceTargetMap;
}

};	// namespace Object
};	// namespace Processing
}; //namespace Core
