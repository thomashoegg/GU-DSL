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
#ifndef __SELECTINTEGRATIONTIMEEVENT_H__
#define __SELECTINTEGRATIONTIMEEVENT_H__

#include "Mediation/Event.h"
#include "CoreDLL.h"
#include "Camera/CameraInformation.qt.h"

namespace Core
{
	namespace Mediation
	{
		namespace Events
		{
			/**
			 * \brief An event to set a new camera integration time
			 *
			 */
			class CORE_API SelectIntegrationTimeEvent : public Core::Mediation::Event
			{
				DECLARE_EVENT
			public:
				/**
				 * \brief constructor
				 *
				 * \param int integrationTime. The new integration time.
				 */
				SelectIntegrationTimeEvent(int integrationTime);

				/**
				 * \brief destructor.
				 *
				 */
				virtual ~SelectIntegrationTimeEvent();

				/**
				 * \brief get the new integration time
				 *
				 * \returns int. The new integration time. 
				 */
				int getIntegrationTime();
			protected:
				
				int _integrationTime;		//!< The integration time
			};
		}; //namespace Events
	}; //namespace Mediation
}; //namespace Core

#endif // __SELECTINTEGRATIONTIMEEVENT_H__
