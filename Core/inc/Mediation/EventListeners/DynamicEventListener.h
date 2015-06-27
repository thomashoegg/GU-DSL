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


#pragma once
#ifndef __DYNAMICEVENTLISTENER_H__
#define __DYNAMICEVENTLISTENER_H__
#include "Mediation/EventListener.h"
#include <unordered_map>
#include "CoreDLL.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace Core
{
	namespace Mediation
	{
		namespace EventListeners
		{
			/**
			 * \brief dynamic event listener class.
			 *		This class can add handle multiple event handlers.
			 *		The class won't update the event bus dynamic so the dynamic event listener
			 *		needs to be registered to the event bus after all events are added.
			 *
			 */
			class CORE_API DynamicEventListener : public Core::Mediation::EventListener
			{
				DECLARE_EVENTLISTENER
			public:
				/**
				 * \brief constructor
				 *
				 */
				DynamicEventListener();
				
				/**
				 * \brief destructor
				 *
				 */
				virtual ~DynamicEventListener();

				/**
				 * \brief default handle event method.
				 *
				 */
				virtual void handleEvent( const Core::Mediation::Interfaces::IEvent& event );

				/**
				 * \brief get handle event function to get the event handles.
				 *
				 */
				virtual boost::function<void(const Core::Mediation::Interfaces::IEvent& event)> getHandleEventFunction( std::type_index eventType );

				/**
				 * \brief add event handle to the dynamic event listener
				 *
				 *	\param std::type_index eventType. The event to call the handle.
				 *	\param boost::function<void(const Core::Mediation::Interfaces::IEvent& event)> handle. The event handle to call.
				 */
				virtual void addEventHandle( std::type_index eventType, boost::function<void(const Core::Mediation::Interfaces::IEvent& event)> handle);

				/**
				 * \brief remove event handle from the dynamic event listener
				 *
				 *	\param std::type_index eventType. the event to remove.
				 */
				virtual void removeEventHandle( std::type_index eventType);
			protected:
				std::unordered_map<std::type_index, boost::function<void(const Core::Mediation::Interfaces::IEvent& event)>>	_handleMap;		//!< The event handle map.
				static std::vector<std::type_index>																				_eventTypes;	//!< The type index map.
			};
		}; // namespace EventListeners
	}; // namespace Mediation
}; // namespace Core

#pragma warning(pop)
#endif // __DYNAMICEVENTLISTENER_H__
