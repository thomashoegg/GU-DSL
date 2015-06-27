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
#ifndef __SOURCEITEM_H__
#define __SOURCEITEM_H__
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "Project/Item.qt.h"
#include "CoreDLL.h"

namespace Core
{
	namespace Project
	{
		/**
		 * \brief source item class.
		 *
		 */
		class CORE_API SourceItem: public Item
		{
			Q_OBJECT
		public:
			/**
			 * \brief constructor.
			 *
			 *	@param name the item name.
			 *	@param object the corresponding processing object.
			 */
			SourceItem(const std::string name = "Source", const Core::Processing::Interfaces::IObjectPtr object = Core::Processing::Interfaces::IObjectPtr());

			/**
			 * \brief copy constructor.
			 *
			 */
			SourceItem(const SourceItem& rhs);
			
			/**
			 * \brief destructor
			 *
			 */
			~SourceItem();
		};
		typedef boost::shared_ptr<SourceItem>				SourceItemPtr;
		typedef boost::shared_ptr<const SourceItem>			ConstSourceItemPtr;
		typedef boost::weak_ptr<SourceItem>					SourceItemWeakPtr;
	}
};

Q_DECLARE_METATYPE(Core::Project::SourceItem)
Q_DECLARE_METATYPE(Core::Project::SourceItemPtr)
Q_DECLARE_METATYPE(Core::Project::ConstSourceItemPtr)
Q_DECLARE_METATYPE(Core::Project::SourceItemWeakPtr)

namespace
{
	void RegisterSourceItem()	
	{
		qRegisterMetaType<Core::Project::SourceItem>();
		qRegisterMetaType<Core::Project::SourceItem>("SourceItem");
	}

	Q_CONSTRUCTOR_FUNCTION(RegisterSourceItem)
};

#endif
