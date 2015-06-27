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
#ifndef __DATAPROVIDERITEM_H__
#define __DATAPROVIDERITEM_H__
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "Project/Item.qt.h"
#include "Project/SourceItem.qt.h"
#include "CoreDLL.h"

namespace Core
{
	namespace Project
	{
		/**
		 * \brief data provider item class.
		 *
		 */
		class CORE_API DataProviderItem: public SourceItem
		{
			Q_OBJECT
		public:
			/**
			 * \brief constructor.
			 *
			 *	@param name the item name.
			 *	@param object the corresponding processing object.
			 */
			DataProviderItem(const std::string name = "DataProvider", const Core::Processing::Interfaces::IObjectPtr object = Core::Processing::Interfaces::IObjectPtr());

			/**
			 * \brief copy constructor.
			 *
			 */
			DataProviderItem(const DataProviderItem& rhs);

			/**
			 * \brief destructor.
			 *
			 */
			~DataProviderItem();
		};
		typedef boost::shared_ptr<DataProviderItem>				DataProviderItemPtr;
		typedef boost::shared_ptr<const DataProviderItem>		ConstDataProviderItemPtr;
		typedef boost::weak_ptr<DataProviderItem>				DataProviderItemWeakPtr;
	}
};

Q_DECLARE_METATYPE(Core::Project::DataProviderItem)
Q_DECLARE_METATYPE(Core::Project::DataProviderItemPtr)
Q_DECLARE_METATYPE(Core::Project::ConstDataProviderItemPtr)
Q_DECLARE_METATYPE(Core::Project::DataProviderItemWeakPtr)


namespace
{
	void RegisterDataProviderItem()	
	{
		qRegisterMetaType<Core::Project::DataProviderItem>();
		qRegisterMetaType<Core::Project::DataProviderItem>("DataProviderItem");
	}

	Q_CONSTRUCTOR_FUNCTION(RegisterDataProviderItem)
};

#endif
