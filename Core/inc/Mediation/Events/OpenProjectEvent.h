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
#ifndef __OPENPROJECTEVENT_H__
#define __OPENPROJECTEVENT_H__
#include "Mediation/Event.h"
#include "Processing/PtrDefinitions.h"
#include "CoreDLL.h"
#include <string>

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
			class CORE_API OpenProjectEvent : public Core::Mediation::Event
			{
				DECLARE_EVENT
			public:
				/**
				 * \brief constructor
				 *
				 *	\param std::string projectName. The project name
				 */
				OpenProjectEvent(const std::string& projectName = "");

				/**
				 * \brief destructor
				 *
				 */
				virtual ~OpenProjectEvent();

				/**
				 * \brief get project name
				 *
				 *	\return std::string. the project name.
				 */
				std::string getProject();
			protected:
				std::string		_projectName;		//!< The project name.
			};
		}; //namespace Events
	}; //namespace Mediation
}; //namespace Core

#pragma warning(pop)

#endif // __OPENPROJECTEVENT_H__
