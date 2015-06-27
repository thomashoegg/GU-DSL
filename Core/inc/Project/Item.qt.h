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
#ifndef __ITEM_H__
#define __ITEM_H__
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <string>
#include <QObject>
#include "Processing/PtrDefinitions.h"
#include "CoreDLL.h"

namespace Core
{
	namespace Project
	{
		/**
		 * \brief item base class.
		 *
		 */
		class CORE_API Item : public QObject
		{
			Q_OBJECT
		protected:
			Core::Processing::Interfaces::IObjectPtr		_object;	//<! The corresponding processing object.
			std::string										_name;		//<! The item name.
		public:
			/**
			 * \brief constructor.
			 *
			 *	@param name the item name.
			 *	@param object the corresponding processing object.
			 */
			Item(const std::string name = "", const Core::Processing::Interfaces::IObjectPtr object = Core::Processing::Interfaces::IObjectPtr());

			/**
			 * \brief copy constructor
			 *
			 */
			Item(const Item& rhs);

			/**
			 * \brief destructor.
			 *
			 */
			virtual ~Item() = 0;

			/**
			 * \brief get name.
			 *
			 *	@return item name.
			 */
			std::string getName();

			/**
			 * \brief get object.
			 *
			 *	@return object.
			 */
			Core::Processing::Interfaces::IObjectPtr getObject();
		};
		typedef boost::shared_ptr<Item>				ItemPtr;
		typedef boost::shared_ptr<const Item>		ConstItemPtr;
		typedef boost::weak_ptr<Item>				ItemWeakPtr;
	}
};

Q_DECLARE_METATYPE(Core::Project::ItemPtr)
Q_DECLARE_METATYPE(Core::Project::ConstItemPtr)
Q_DECLARE_METATYPE(Core::Project::ItemWeakPtr)
//Q_DECLARE_METATYPE(Core::Project::Item)

//namespace
//{
//	void RegisterItem()	
//	{
//		qRegisterMetaType<Core::Project::Item>();
//		qRegisterMetaType<Core::Project::Item>("Item");
//	}
//
//	Q_CONSTRUCTOR_FUNCTION(RegisterItem)
//};

#endif
