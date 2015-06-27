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

#pragma once;
#ifndef __PROJECTITEM_H__
#define __PROJECTITEM_H__
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include "Project/Item.qt.h"
#include "CoreDLL.h"

namespace Core
{
	namespace Project
	{
		/**
		 * \brief project item class.
		 *
		 */
		class CORE_API ProjectItem: public Item
		{
			Q_OBJECT
		private:
			std::vector<ItemPtr> _items;
		public:
			/**
			 * \brief constructor.
			 *
			 *	@param name the item name.
			 */
			ProjectItem(const std::string name = "Project");

			/**
			 * \brief copy constructor.
			 *
			 */
			ProjectItem(const ProjectItem& rhs);

			/**
			 * \brief destructor.
			 *
			 */
			~ProjectItem();

			/**
			 * \brief add item to project.
			 *
			 *	@param item item to add.
			 */
			void addItem(ItemPtr item);

			/**
			 * \brief get project items.
			 *
			 *	@return list of items.
			 */
			std::vector<ItemPtr> getItems();
		};
		typedef boost::shared_ptr<ProjectItem>				ProjectItemPtr;
		typedef boost::shared_ptr<const ProjectItem>		ConstProjectItemPtr;
		typedef boost::weak_ptr<ProjectItem>				ProjectItemWeakPtr;
	}
};

Q_DECLARE_METATYPE(Core::Project::ProjectItem)
Q_DECLARE_METATYPE(Core::Project::ProjectItemPtr)
Q_DECLARE_METATYPE(Core::Project::ConstProjectItemPtr)
Q_DECLARE_METATYPE(Core::Project::ProjectItemWeakPtr)


namespace
{
	void RegisterProjectItem()	
	{
		qRegisterMetaType<Core::Project::ProjectItem>();
		qRegisterMetaType<Core::Project::ProjectItem>("ProjectItem");
	}

	Q_CONSTRUCTOR_FUNCTION(RegisterProjectItem)
};

#endif
