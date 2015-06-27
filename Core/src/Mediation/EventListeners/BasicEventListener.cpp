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

#include "Mediation/EventListeners/BasicEventListener.h"
#include "Mediation/Event.h"
#include <boost/bind.hpp>

IMPLEMENT_EVENTLISTENER(Core::Mediation::EventListeners::BasicEventListener, Core::Mediation::Events::SelectItemEvent, Core::Mediation::Events::CloseEvent)

namespace Core
{
namespace Mediation
{
namespace EventListeners
{
	

	BasicEventListener::BasicEventListener()
	{
	}

	BasicEventListener::~BasicEventListener()
	{
		_selectItemHandle.clear();
	}

	void BasicEventListener::handleEvent( const Core::Mediation::Interfaces::IEvent& event )
	{
		if(event.getEventType() == typeid(const Core::Mediation::Events::SelectItemEvent))
		{
			if(!_selectItemHandle.empty())
			{
				Core::Mediation::Events::SelectItemEvent selectItemEvent = dynamic_cast<const Core::Mediation::Events::SelectItemEvent&>(event);
				_selectItemHandle(selectItemEvent);
			}
		}
		if(event.getEventType() == typeid(const Core::Mediation::Events::CloseEvent))
		{
			if(!_closeHandle.empty())
			{
				Core::Mediation::Events::CloseEvent closeEvent = dynamic_cast<const Core::Mediation::Events::CloseEvent&>(event);
				_closeHandle(closeEvent);
			}
		}
	}

	void BasicEventListener::setSelectItemEventHandle( boost::function<void(const Core::Mediation::Events::SelectItemEvent& event)> handle)
	{
		_selectItemHandle = handle;
	}

	void BasicEventListener::removeSelectItemEventHandle()
	{
		_selectItemHandle.clear();
	}

	void BasicEventListener::setCloseEventHandle( boost::function<void(const Core::Mediation::Events::CloseEvent& event)> handle )
	{
		_closeHandle = handle;
	}

	void BasicEventListener::removeCloseEventHandle()
	{
		_closeHandle.clear();
	}


}; //namespace EventListeners
}; //namespace Mediation
}; //namespace Core
