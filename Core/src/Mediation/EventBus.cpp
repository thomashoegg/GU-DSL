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

#include "Mediation/EventBus.h"

namespace Core
{
namespace Mediation
{
	EventBus::EventBus()
	{
	}

	EventBus::~EventBus()
	{
		for(std::vector<Core::Mediation::EventBus::EventBusConnection*>::reverse_iterator signalIterator = _listenerConnections.rbegin(); signalIterator != _listenerConnections.rend(); signalIterator++)
		{
			for(std::vector<boost::signals2::connection>::iterator it = (*signalIterator)->signalConnections.begin(); it != (*signalIterator)->signalConnections.end(); it++)
			{
				it->disconnect();
			}
			(*signalIterator)->listener = nullptr;
			delete *signalIterator;
		}
		_listenerConnections.clear();

		for(SignalMap::iterator it = _eventSignals.begin(); it != _eventSignals.end(); it++)
		{
			it->second->disconnect_all_slots();
			delete it->second;
		}
	}

	void EventBus::registerListener( const Core::Mediation::Interfaces::IEventListener* listener )
	{
		if(!listenerExists(listener))
		{
			std::vector<std::type_index> eventTypes = listener->getEventTypes();
			EventBusConnection* tmp = new EventBusConnection();
			tmp->listener = listener;

			for(std::vector<std::type_index>::iterator it = eventTypes.begin(); it != eventTypes.end(); it++)
			{
				SignalMap::iterator signalMapIterator = _eventSignals.find(*it);
				if(signalMapIterator == _eventSignals.end())
				{
					std::pair<SignalMap::iterator,bool> returnValue =_eventSignals.insert(std::pair<std::type_index, Core::Mediation::Interfaces::handleEventSignal*>(*it, new Core::Mediation::Interfaces::handleEventSignal()));
					signalMapIterator = returnValue.first;
				}
				boost::function<void(const Core::Mediation::Interfaces::IEvent& event)> fkn = (const_cast<Core::Mediation::Interfaces::IEventListener*>(listener))->getHandleEventFunction(*it);
				boost::signals2::connection newConnection = signalMapIterator->second->connect(fkn);

				tmp->signalConnections.push_back(newConnection);
			}

			_listenerConnections.push_back(tmp);
		}
	}

	void EventBus::unregisterListener( const Core::Mediation::Interfaces::IEventListener* listener )
	{
		std::vector<Core::Mediation::EventBus::EventBusConnection*>::iterator eventBusConnectionIterator = findEventBusConnection(listener);
		if(eventBusConnectionIterator != _listenerConnections.end())
		{
			 for(std::vector<boost::signals2::connection>::iterator it = (*eventBusConnectionIterator)->signalConnections.begin();
				 it != (*eventBusConnectionIterator)->signalConnections.end(); it++)
			 {
				 it->disconnect();
			 }
			 _listenerConnections.erase(eventBusConnectionIterator);
			 eventBusConnectionIterator = _listenerConnections.end();
		}
	}

	void EventBus::postEvent( const Core::Mediation::Interfaces::IEvent& event )
	{
		std::type_index eventType = event.getEventType();
		SignalMap::iterator signalsIterator = _eventSignals.find(eventType);
		if(signalsIterator != _eventSignals.end())
		{
			if(signalsIterator->second != nullptr)
			{
				//call signal
				(*signalsIterator->second)(event);
			}
		}
		std::vector<std::type_index> eventBaseTypes = event.getEventBaseTypes();
		for(std::vector<std::type_index>::iterator it = eventBaseTypes.begin(); it != eventBaseTypes.end(); it++)
		{
			SignalMap::iterator signalsIteratorBase = _eventSignals.find(*it);
			if(signalsIteratorBase != _eventSignals.end())
			{
				if(signalsIteratorBase->second != nullptr)
				{
					//call event
					(*signalsIteratorBase->second)(event);
				}
			}
		}

	}

	bool EventBus::listenerExists( const Core::Mediation::Interfaces::IEventListener* listener )
	{
		bool retVal = false;

		for(::size_t i = 0; i < _listenerConnections.size(); i++)
		{
			if(_listenerConnections[i]->listener != nullptr)
			{
				if(_listenerConnections[i]->listener == listener)
				{
					retVal = true;
					break;
				}
			}
		}

		return retVal;
	}

	std::vector<Core::Mediation::EventBus::EventBusConnection*>::iterator EventBus::findEventBusConnection( const Core::Mediation::Interfaces::IEventListener* listener )
	{
		std::vector<Core::Mediation::EventBus::EventBusConnection*>::iterator returnValue = _listenerConnections.end();
		for( std::vector<Core::Mediation::EventBus::EventBusConnection*>::iterator it = _listenerConnections.begin(); it != _listenerConnections.end(); ++it )
		{
			if((*it)->listener != nullptr)
			{
				if((*it)->listener == listener)
				{
					returnValue = it;
					break;
				}
			}
		}
		return returnValue;
	}


	EventBus::EventBusConnection::~EventBusConnection()
	{
		// do not delete listener!
		listener = nullptr;
		if(signalConnections.size() > 0)
		{
			for(std::vector<boost::signals2::connection>::iterator it = signalConnections.begin(); it != signalConnections.end(); it++)
			{
				it->disconnect();
			}
			signalConnections.clear();
		}
	}

	EventBus::EventBusConnection::EventBusConnection()
		: listener(nullptr)
	{

	}

}; //namespace Mediation
}; //namespace Core
