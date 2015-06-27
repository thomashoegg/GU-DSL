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
#ifndef __SINKITEM_H__
#define __SINKITEM_H__
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "Project/Item.qt.h"
#include "CoreDLL.h"

namespace Core
{
	namespace Project
	{
		/**
		 * \brief sink item class.
		 *
		 */
		class CORE_API SinkItem: public Item
		{
			Q_OBJECT
		public:
			/**
			 * \brief constructor.
			 *
			 *	@param name the item name.
			 *	@param object the corresponding processing object.
			 */
			SinkItem(const std::string name = "Sink", const Core::Processing::Interfaces::IObjectPtr object = Core::Processing::Interfaces::IObjectPtr());

			/**
			 * \brief copy constructor.
			 *
			 */
			SinkItem(const SinkItem& rhs);

			/**
			 * \brief destructor.
			 *
			 */
			~SinkItem();
		};
		typedef boost::shared_ptr<SinkItem>				SinkItemPtr;
		typedef boost::shared_ptr<const SinkItem>		ConstSinkItemPtr;
		typedef boost::weak_ptr<SinkItem>				SinkItemWeakPtr;
	}
};

Q_DECLARE_METATYPE(Core::Project::SinkItem)
Q_DECLARE_METATYPE(Core::Project::SinkItemPtr)
Q_DECLARE_METATYPE(Core::Project::ConstSinkItemPtr)
Q_DECLARE_METATYPE(Core::Project::SinkItemWeakPtr)

namespace
{
	void RegisterSinkItem()	
	{
		qRegisterMetaType<Core::Project::SinkItem>();
		qRegisterMetaType<Core::Project::SinkItem>("SinkItem");
	}

	Q_CONSTRUCTOR_FUNCTION(RegisterSinkItem)
};

#endif
