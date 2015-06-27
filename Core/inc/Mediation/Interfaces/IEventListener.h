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
#ifndef __IEVENTLISTENER_H__
#define __IEVENTLISTENER_H__
#include "CoreDLL.h"
#include <typeindex>
#include <vector>
#include "Mediation/Interfaces/IEvent.h"
#include <boost/function.hpp>
#include "Utilities/MacroHeaven.h"

namespace Core
{
	namespace Mediation
	{
		namespace Interfaces
		{
			/**
			 * \brief event listener interface
			 *
			 */
			class CORE_API IEventListener
			{
			public:
				IEventListener();
				virtual ~IEventListener() = 0;

				/**
				 * \brief get event types
				 *
				 *	\returns std::vector<std::type_index> The event types listen to.
				 */
				virtual std::vector<std::type_index> getEventTypes() const = 0;

				/**
				 * \brief default handle event method.
				 *
				 *	\param const Core::Mediation::Interfaces::IEvent& event. The event to handle.
				 */
				virtual void handleEvent(const Core::Mediation::Interfaces::IEvent& event) = 0;

				/**
				 * \brief get event handle functions for the given event type.
				 *
				 *	\param std::type_index eventType. The event type to look for.
				 *	\returns boost::function<void(const Core::Mediation::Interfaces::IEvent& event)> The function to call for the event type.
				 */
				virtual boost::function<void(const Core::Mediation::Interfaces::IEvent& event)> getHandleEventFunction( std::type_index eventType ) = 0;
			};
		}; // namespace Interfaces
	}; // namespace Mediation
}; // namespace Core


#ifdef DECLARE_EVENTLISTENER
#undef DECLARE_EVENTLISTENER
#endif
#define DECLARE_EVENTLISTENER														\
	public: static std::vector<std::type_index> getTypes();							\
	public: virtual std::vector<std::type_index> getEventTypes() const;

#define CALL_IMPLEMENT_EVENTLISTENER(Event1) tmp.push_back(typeid(Event1));

#ifdef IMPLEMENT_EVENTLISTENER
#undef IMPLEMENT_EVENTLISTENER
#endif
#define IMPLEMENT_EVENTLISTENER(EventListener, ...)									\
	IMPLEMENT_EVENTLISTENER_BEGIN(EventListener)									\
	EXPAND(VARARG_FOREACH(CALL_IMPLEMENT_EVENTLISTENER, __VA_ARGS__))				\
	IMPLEMENT_EVENTLISTENER_END(EventListener)
	

#define IMPLEMENT_EVENTLISTENER_BEGIN(EventListener)								\
	std::vector<std::type_index> EventListener::getTypes()							\
	{																				\
		std::vector<std::type_index> tmp;										
		
#define IMPLEMENT_EVENTLISTENER_END(EventListener)									\
		return tmp;																	\
	}																				\
																					\
	std::vector<std::type_index> EventListener::getEventTypes() const				\
	{																				\
		return EventListener::getTypes();											\
	}

#endif // __IEVENTLISTENER_H__
