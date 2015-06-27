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

#include "Mediation/EventListeners/DynamicEventListener.h"
#include "Mediation/Event.h"
#include <boost/bind.hpp>

namespace Core
{
namespace Mediation
{
namespace EventListeners
{
	std::vector<std::type_index> DynamicEventListener::_eventTypes;

	// The getTypes and getEventTypes function is normally implemented by the IMPLEMENT_EVENTLISTENER macro. 
	// Due to the special nature of the dynamic event listener are the methods implemented manually.
	std::vector<std::type_index> DynamicEventListener::getTypes()
	{
		//returns all event types from all dynamic event listeners
		return _eventTypes;
	}
	
	std::vector<std::type_index> DynamicEventListener::getEventTypes() const
	{
		std::vector<std::type_index> tmp;
		for(std::unordered_map<std::type_index, boost::function<void(const Core::Mediation::Interfaces::IEvent& event)>>::const_iterator it = _handleMap.begin(); it != _handleMap.end(); it++)
		{
			tmp.push_back(it->first);
		}
		return tmp;
	}

	DynamicEventListener::DynamicEventListener()
	{

	}

	DynamicEventListener::~DynamicEventListener()
	{
		_handleMap.clear();
	}

	void DynamicEventListener::handleEvent( const Core::Mediation::Interfaces::IEvent& event )
	{
		
	}

	boost::function<void(const Core::Mediation::Interfaces::IEvent& event)> DynamicEventListener::getHandleEventFunction( std::type_index eventType )
	{
		std::unordered_map<std::type_index, boost::function<void(const Core::Mediation::Interfaces::IEvent& event)>>::const_iterator it = _handleMap.find(eventType);
		if(it != _handleMap.end())
		{
			return it->second;
		}
		else
		{
			return boost::bind(&DynamicEventListener::handleEvent, this, _1);
		}
	}

	void DynamicEventListener::addEventHandle( std::type_index eventType, boost::function<void(const Core::Mediation::Interfaces::IEvent& event)> handle)
	{
		if(_handleMap.find(eventType) == _handleMap.end())
		{
			_handleMap.insert(std::pair<std::type_index, boost::function<void(const Core::Mediation::Interfaces::IEvent& event)>>(eventType, handle));
			_eventTypes.push_back(eventType);
		}
	}

	void DynamicEventListener::removeEventHandle( std::type_index eventType )
	{
		if(_handleMap.find(eventType) != _handleMap.end())
		{
			_handleMap.erase(eventType);

			for(std::vector<std::type_index>::iterator it = _eventTypes.begin(); it != _eventTypes.end(); it++)
			{
				if(*it == eventType)
				{
					_eventTypes.erase(it);
					break;
				}
			}
		}
	}


}; //namespace EventListeners
}; //namespace Mediation
}; //namespace Core
