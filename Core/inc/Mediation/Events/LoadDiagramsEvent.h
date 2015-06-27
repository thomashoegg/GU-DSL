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
#ifndef __LOADDIAGRAMSEVENT_H__
#define __LOADDIAGRAMSEVENT_H__
#include "Mediation/Event.h"
#include "Processing/PtrDefinitions.h"
#include "CoreDLL.h"
#include <QStringList>

#pragma warning(push)
#pragma warning(disable: 4251)

namespace Core
{
	namespace Mediation
	{
		namespace Events
		{
			/**
			 * \brief select item event.
			 *
			 */
			class CORE_API LoadDiagramsEvent : public Core::Mediation::Event
			{
				DECLARE_EVENT
			public:
				/**
				 * \brief constructor
				 *
				 *	\param std::vector<Core::Processing::Component::ComponentDiagramPtr> diagrams. The diagram to load.
				 */
				LoadDiagramsEvent(std::vector<Core::Processing::Component::ComponentDiagramPtr> diagrams);

				/**
				 * \brief destructor
				 *
				 */
				virtual ~LoadDiagramsEvent();

				/**
				 * \brief get diagrams to load
				 *
				 *	\return std::vector<Core::Processing::Component::ComponentDiagramPtr>. The diagram to open.
				 */
				std::vector<Core::Processing::Component::ComponentDiagramPtr> getDiagrams();
			protected:
				std::vector<Core::Processing::Component::ComponentDiagramPtr>		_diagrams;		//!< The diagrams to load.
			};
		}; //namespace Events
	}; //namespace Mediation
}; //namespace Core

#pragma warning(pop)

#endif // __LOADDIAGRAMSEVENT_H__
