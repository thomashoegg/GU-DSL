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
#ifndef __NEWCOMMUNICATIONDETECTEDEVENT_H__
#define __NEWCOMMUNICATIONDETECTEDEVENT_H__
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
			 * \brief connect camera event
			 *
			 */
			class CORE_API NewCommunicationDetectedEvent : public Core::Mediation::Event
			{
				DECLARE_EVENT
			public:
				/**
				 * \brief constructor
				 *
				 * \param Core::Communication::CommunicationDataPtr communicationData. The communication data.
				 */
				NewCommunicationDetectedEvent(Core::Communication::CommunicationDataPtr communicationData);

				/**
				 * \brief destructor.
				 *
				 */
				virtual ~NewCommunicationDetectedEvent();

				/**
				 * \brief get data
				 *
				 * \returns Core::Communication::CommunicationDataPtr. The communication data. 
				 */
				Core::Communication::CommunicationDataPtr getData();
			protected:
				
				Core::Communication::CommunicationDataPtr	_communicationData;		//!< The camera data.
			};
		}; //namespace Events
	}; //namespace Mediation
}; //namespace DataProcessing

#pragma warning(pop)

#endif // __NEWCOMMUNICATIONDETECTEDEVENT_H__
