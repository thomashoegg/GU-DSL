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
#ifndef __IEVENTBUS_H__
#define __IEVENTBUS_H__
#include <boost/signals2.hpp>
#include "CoreDLL.h"
#include "Mediation/Interfaces/IEvent.h"
#include "Mediation/Interfaces/IEventListener.h"

namespace Core
{
	namespace Mediation
	{
		namespace Interfaces
		{
			/**
			 * \brief the handle event signal type.
			 *
			 */
			typedef boost::signals2::signal<void (const Core::Mediation::Interfaces::IEvent&)> handleEventSignal;

			/**
			 * \brief event bus interface.
			 *
			 */
			class CORE_API IEventBus
			{
			public:
				IEventBus();
				virtual ~IEventBus() = 0;

				/**
				 * \brief register listener
				 *
				 *	/param const Core::Mediation::Interfaces::IEventListener* listener. The listener to register
				 */
				virtual void registerListener(const Core::Mediation::Interfaces::IEventListener* listener) = 0;

				/**
				 * \brief unregister listener
				 *
				 *	/param const Core::Mediation::Interfaces::IEventListener* listener. The listener to unregister
				 */
				virtual void unregisterListener(const Core::Mediation::Interfaces::IEventListener* listener) = 0;

				/**
				 * \brief post event into the bus
				 *
				 *	\param const Core::Mediation::Interfaces::IEvent& event. The event to send into the bus.
				 */
				virtual void postEvent(const Core::Mediation::Interfaces::IEvent& event) = 0;
			};
		}; // namespace Interfaces
	}; // namespace Mediation
}; // namespace Core
#endif	// __IEVENTBUS_H__
