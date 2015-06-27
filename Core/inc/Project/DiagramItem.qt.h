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
#ifndef __DIAGRAMITEM_H__
#define __DIAGRAMITEM_H__
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
		 * \brief diagram item class.
		 *
		 */
		class CORE_API DiagramItem: public Item
		{
			Q_OBJECT
		private:
			std::vector<ItemPtr> _items;
		public:
			/**
			 * \brief constructor.
			 *
			 *	@param name the item name.
			 *	@param object the corresponding processing object.
			 */
			DiagramItem(const std::string name = "Diagram", const Core::Processing::Interfaces::IObjectPtr object = Core::Processing::Interfaces::IObjectPtr());

			/**
			 * \brief copy constructor.
			 *
			 */
			DiagramItem(const DiagramItem& rhs);

			/**
			 * \brief destructor.
			 *
			 */
			~DiagramItem();

			/**
			 * \brief add item to diagram.
			 *
			 *	@param item item to add.
			 */
			void addItem(ItemPtr item);

			/**
			 * \brief get diagram items.
			 *
			 *	@return list of items.
			 */
			std::vector<ItemPtr> getItems();
		};
		typedef boost::shared_ptr<DiagramItem>				DiagramItemPtr;
		typedef boost::shared_ptr<const DiagramItem>		ConstDiagramItemPtr;
		typedef boost::weak_ptr<DiagramItem>				DiagramItemWeakPtr;
	}
};

Q_DECLARE_METATYPE(Core::Project::DiagramItem)
Q_DECLARE_METATYPE(Core::Project::DiagramItemPtr)
Q_DECLARE_METATYPE(Core::Project::ConstDiagramItemPtr)
Q_DECLARE_METATYPE(Core::Project::DiagramItemWeakPtr)

namespace
{
	void RegisterDiagramItem()	
	{
		qRegisterMetaType<Core::Project::DiagramItem>();
		qRegisterMetaType<Core::Project::DiagramItem>("DiagramItem");
	}

	Q_CONSTRUCTOR_FUNCTION(RegisterDiagramItem)
};

#endif
