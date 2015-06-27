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
#ifndef __IPLUGINFACTORY_H__
#define __IPLUGINFACTORY_H__
#include <boost/shared_ptr.hpp>
#include <QObject>
#include <QtPlugin>
#include <vector>
#include <string>
#include <typeindex>
#include "Plugin/Interfaces/IPluginObject.qt.h"
#include "CoreDLL.h"

namespace Core
{
	namespace Plugin
	{
		namespace Interfaces
		{
			class CORE_API IPluginFactory: public QObject
			{
				Q_OBJECT
			public:
				/**
				 * \brief plugin data helper class
				 *
				 */
				struct CORE_API PluginData
				{
					/**
					 * \brief constructor
					 *
					 */
					PluginData(std::string pname, std::type_index ptype);

					/**
					 * \brief destructor
					 *
					 */
					~PluginData();

					bool operator==(const PluginData& rhs); 

					std::string name;		//!< plugin name
					std::type_index type;	//!< plugin type
				};

			public:
				/**
				 * \brief constructor
				 *
				 */
				IPluginFactory();

				/**
				 * \brief destructor
				 *
				 */
				virtual ~IPluginFactory() = 0;

				/**
				 * \brief get plugin names
				 *
				 */
				virtual std::vector<std::string> getPluginNames() = 0;

				/**
				 * \brief get plugin names of given plugin type
				 *
				 */
				virtual std::vector<std::string> getPluginNames(std::type_index type) = 0;

				/**
				 * \brief create plugin with given name.
				 *
				 */
				virtual IPluginObjectPtr create(std::string name) = 0;

				/**
				 * \brief get pugin types
				 *
				 */
				virtual std::vector<std::type_index> getPluginTypes() = 0;

				/**
				 * \brief get pugin type of the given plugin.
				 *
				 */
				virtual std::type_index getPluginTypes(std::string name) = 0;

				/**
				 * \brief create all plugins of the given plugin type.
				 *
				 */
				virtual std::vector<IPluginObjectPtr> create(std::type_index type) = 0;

				/**
				 * \brief get plugin datas
				 *
				 */
				virtual std::vector<PluginData> getPluginDatas() = 0;
			};
		};	//namespace Interfaces
	};	//namespace Plugin
};	//namespace Core

//Q_DECLARE_INTERFACE(Core::Plugin::Interfaces::IPluginFactory, "de.ce.IPluginFactory/1.0")

#endif // __IPLUGINFACTORY_H__
