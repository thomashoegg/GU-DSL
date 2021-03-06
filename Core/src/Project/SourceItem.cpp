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

#include "Project/SourceItem.qt.h"

namespace Core
{
	namespace Project
	{
		SourceItem::SourceItem(const std::string name /*= "Source"*/, const Core::Processing::Interfaces::IObjectPtr object/* = Core::Processing::Interfaces::IObjectPtr()*/)
			:Item(name,object)
		{

		}

		SourceItem::SourceItem( const SourceItem& rhs )
			:Item(rhs)
		{

		}

		SourceItem::~SourceItem()
		{

		}

	};
};
