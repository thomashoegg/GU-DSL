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

#include "Mediation/Events/OpenProjectEvent.h"
#include "Processing/Interfaces/IObject.qt.h"

namespace Core
{
	namespace Mediation
	{
		namespace Events
		{
			IMPLEMENT_EVENT(OpenProjectEvent, Core::Mediation::Event)

			OpenProjectEvent::OpenProjectEvent( const std::string& projectName /*= ""*/ )
			{
				_projectName = projectName;
			}

			OpenProjectEvent::~OpenProjectEvent()
			{

			}

			std::string OpenProjectEvent::getProject()
			{
				return _projectName;
			}
		}; //namespace Events
	}; //namespace Mediation
}; //namespace Core