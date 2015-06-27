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
#ifndef __RUNTIMEEVENTBUS_H__
#define __RUNTIMEEVENTBUS_H__
#include "Mediation/Interfaces/IEventBus.h"
#include "Mediation/Interfaces/IEvent.h"
#include "Mediation/Interfaces/IEventListener.h"
#include "Mediation/EventBus.h"
#include "CoreDLL.h"

namespace Core
{
	namespace Mediation
	{
		/**
		 * \brief the static runtime bus.
		 *
		 */
		class CORE_API RuntimeEventBus : public Core::Mediation::EventBus
		{
		public:
			/**
			 * \brief get instance
			 *
			 *	\returns RuntimeEventBus&. The instance.
			 */
			static RuntimeEventBus& getInstance();
			
			/**
			 * \brief destructor
			 *
			 */
			virtual ~RuntimeEventBus();
		private:
			/**
			 * \brief constructor
			 *
			 */
			RuntimeEventBus();
			
			/**
			 * \brief copy constructor
			 *		not implemented
			 */
			RuntimeEventBus(RuntimeEventBus const&);    // Don't Implement
			
			/**
			 * \brief assignment operator
			 *		not implemented
			 */
			void operator=(RuntimeEventBus const&);		// Don't implement
		};
	};
};

#endif	// __RUNTIMEEVENTBUS_H__
