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
#ifndef __EVENTBUS_H__
#define __EVENTBUS_H__
#include <unordered_map>
#include <map>
#include <vector>
#include <typeindex>
#include "Mediation/Interfaces/IEventBus.h"
#include "Mediation/Interfaces/IEvent.h"
#include "Mediation/Interfaces/IEventListener.h"
#include "CoreDLL.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace Core
{
	namespace Mediation
	{
		/**
		 * \brief event bus class.
		 *
		 */
		class CORE_API EventBus : public Core::Mediation::Interfaces::IEventBus
		{
		protected:
			/**
			 * \brief internal event bus connection structure
			 *
			 */
			struct EventBusConnection
			{
				/**
				 * \brief constructor
				 *
				 */
				EventBusConnection();

				/**
				 * \brief destructor
				 *
				 */
				~EventBusConnection();

				const Core::Mediation::Interfaces::IEventListener* listener;	//!< The event listener
				std::vector<boost::signals2::connection> signalConnections;		//!< The signal connection list.
			};

			/**
			 * \brief event to signal map typedef.
			 *
			 */
			typedef std::unordered_map<std::type_index, Core::Mediation::Interfaces::handleEventSignal*> SignalMap;
			
		public:
			/**
			 * \brief constructor
			 *
			 */
			EventBus();

			/**
			 * \brief destructor
			 *
			 */
			virtual ~EventBus();

			/**
			* \brief register listener
			*
			*	/param const Core::Mediation::Interfaces::IEventListener* listener. The listener to register
			*/
			virtual void registerListener( const Core::Mediation::Interfaces::IEventListener* listener );

			/**
			* \brief unregister listener
			*
			*	/param const Core::Mediation::Interfaces::IEventListener* listener. The listener to unregister
			*/
			virtual void unregisterListener( const Core::Mediation::Interfaces::IEventListener* listener );

			/**
			* \brief post event into the bus
			*
			*	\param const Core::Mediation::Interfaces::IEvent& event. The event to send into the bus.
			*/
			virtual void postEvent( const Core::Mediation::Interfaces::IEvent& event );

		private:
			/**
			 * \brief check if listener is already registered
			 *
			 *	\param const Core::Mediation::Interfaces::IEventListener* listener. The listener to check
			 *	\return bool true if listener already registered
			 */
			bool listenerExists(const Core::Mediation::Interfaces::IEventListener* listener);

			/**
			 * \brief find event bus connection
			 *
			 *	\praram const Core::Mediation::Interfaces::IEventListener* listener. The listener to find.
			 *	\return std::vector<Core::Mediation::EventBus::EventBusConnection*>::iterator. The event bus connection iterator != _listenerConnections.end if exists
			 */
			std::vector<Core::Mediation::EventBus::EventBusConnection*>::iterator findEventBusConnection(const Core::Mediation::Interfaces::IEventListener* listener);
		
		protected:
			std::vector<Core::Mediation::EventBus::EventBusConnection*>		_listenerConnections;		//!< The event bus connection list
			SignalMap														_eventSignals;				//!< The event signal map.
		};
	};
};

#pragma warning(pop)
#endif	// __EVENTBUS_H__
