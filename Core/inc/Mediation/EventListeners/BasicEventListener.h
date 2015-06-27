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
#ifndef __BASICEVENTLISTENER_H__
#define __BASICEVENTLISTENER_H__
#include "Mediation/EventListener.h"
#include "Mediation/Events/SelectItemEvent.h"
#include "Mediation/Events/CloseEvent.h"
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
			 * \brief select item event listener class.
			 *
			 */
			class CORE_API BasicEventListener : public Core::Mediation::EventListener
			{
				DECLARE_EVENTLISTENER
			public:
				/**
				 * \brief constructor
				 *
				 */
				BasicEventListener();
				
				/**
				 * \brief destructor
				 *
				 */
				virtual ~BasicEventListener();

				/**
				 * \brief default handle event method.
				 *
				 */
				virtual void handleEvent( const Core::Mediation::Interfaces::IEvent& event );

				/**
				 * \brief add select item event handle to the event listener
				 *
				 *	\param boost::function<void(const Core::Mediation::Events::SelectItemEvent& event)> handle. The select item event handle to call.
				 */
				virtual void setSelectItemEventHandle( boost::function<void(const Core::Mediation::Events::SelectItemEvent& event)> handle);

				/**
				 * \brief remove select item event handle from the event listener
				 *
				 */
				virtual void removeSelectItemEventHandle();

				/**
				 * \brief add close event handle to the event listener
				 *
				 *	\param boost::function<void(const Core::Mediation::Events::CloseEvent& event)> handle. The close event handle to call.
				 */
				virtual void setCloseEventHandle( boost::function<void(const Core::Mediation::Events::CloseEvent& event)> handle);

				/**
				 * \brief remove close event handle from the event listener
				 *
				 */
				virtual void removeCloseEventHandle();
			protected:
				boost::function<void(const Core::Mediation::Events::SelectItemEvent& event)>			_selectItemHandle;			//!< The select item event handle.
				boost::function<void(const Core::Mediation::Events::CloseEvent& event)>					_closeHandle;			//!< The select item event handle.
			};
		}; // namespace EventListeners
	}; // namespace Mediation
}; // namespace Core

#pragma warning(pop)
#endif // __BASICEVENTLISTENER_H__
