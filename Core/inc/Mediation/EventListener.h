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
#ifndef __EVENTLISTENER_H__
#define __EVENTLISTENER_H__
#include "Mediation/Interfaces/IEventListener.h"
#include "CoreDLL.h"

namespace Core
{
	namespace Mediation
	{
		/**
		 * \brief Abstract event listener class.
		 *
		 */
		class CORE_API EventListener : public Core::Mediation::Interfaces::IEventListener
		{
			DECLARE_EVENTLISTENER
		public:
			/**
			 * \brief constructor
			 *
			 */
			EventListener();

			/**
			 * \brief destructor
			 *
			 */
			virtual ~EventListener() = 0;

			/**
			* \brief default handle event method.
			*
			*	\param const Core::Mediation::Interfaces::IEvent& event. The event to handle.
			*/
			virtual void handleEvent( const Core::Mediation::Interfaces::IEvent& event );

			/**
			* \brief get event handle functions for the given event type.
			*
			*	\param std::type_index eventType. The event type to look for.
			*	\returns boost::function<void(const Core::Mediation::Interfaces::IEvent& event)> The function to call for the event type.
			*/
			virtual boost::function<void(const Core::Mediation::Interfaces::IEvent& event)> getHandleEventFunction( std::type_index eventType );
		};
	}; // namespace Mediation
}; // namespace Core

#endif // __EVENTLISTENER_H__
