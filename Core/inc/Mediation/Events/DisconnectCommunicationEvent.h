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
#ifndef __DISCONNECTCOMMUNICATIONEVENT_H__
#define __DISCONNECTCOMMUNICATIONEVENT_H__
#include "Mediation/Event.h"
#include "CoreDLL.h"
#include "Communication/PtrDefinitions.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace Core
{
	namespace Mediation
	{
		namespace Events
		{
			/**
			 * \brief disconnect camera event
			 *
			 */
			class CORE_API DisconnectCommunicationEvent : public Core::Mediation::Event
			{
				DECLARE_EVENT
			public:
				/**
				 * \brief constructor
				 *
				 * \param Core::Communication::CommunicationDataPtr communicationData. The camera data.
				 */
				DisconnectCommunicationEvent(Core::Communication::CommunicationDataPtr communicationData);

				/**
				 * \brief destructor
				 *
				 */
				virtual ~DisconnectCommunicationEvent();

				/**
				 * \brief get data
				 *
				 * \returns Core::Communication::Interfaces::ICommunicationInformation* . The communication data. 
				 */
				Core::Communication::CommunicationDataPtr getData();
			protected:
				Core::Communication::CommunicationDataPtr _communicationData;	//!< The communication data.
			};
		}; //namespace Events
	}; //namespace Mediation
}; //namespace DataProcessing

#pragma warning(pop)

#endif // __DISCONNECTCOMMUNICATIONEVENT_H__
