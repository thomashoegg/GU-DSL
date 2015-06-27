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
#ifndef __DEFAULTPLUGIN_FACTORY_H__
#define __DEFAULTPLUGIN_FACTORY_H__
#include <unordered_map>
#include <typeindex>
#include "Plugin/Interfaces/IPluginFactory.qt.h"
#include "Plugin/Interfaces/IPluginObject.qt.h"
#include "CoreDll.h"

namespace Core
{
	namespace Plugin
	{
		namespace Factory
		{
			/**
			 * \brief
			 *
			 */
			class CORE_API DefaultPluginFactory: public Core::Plugin::Interfaces::IPluginFactory
			{
				Q_OBJECT
			private:
				/**
				 * \brief constructor
				 *
				 */
				DefaultPluginFactory();

				/**
				 * \brief private copy construtor.
				 *
				 */
				DefaultPluginFactory(const DefaultPluginFactory&);	//Don't implement

				/**
				 * \brief assignment operator
				 *
				 */
				void operator=(const DefaultPluginFactory&);	//Don't implement
			public:

				/**
				 * \brief destructor
				 *
				 */
				~DefaultPluginFactory();

				/**
				 * \brief get the static factory instance
				 *
				 */
				static DefaultPluginFactory& instance();

				/**
				 * \brief add plugin to the factory
				 *
				 */
				void addPlugin(std::string name, std::type_index pluginType, Core::Plugin::Interfaces::IPluginObject* (*createFunction)(void));

				/**
				 * \brief get plugin names
				 *
				 */
				virtual std::vector<std::string> getPluginNames();

				/**
				 * \brief get plugin names of the given plugin types
				 *
				 */
				virtual std::vector<std::string> getPluginNames( std::type_index type );

				/**
				 * \brief create plugin object.
				 *
				 */
				virtual Core::Plugin::Interfaces::IPluginObjectPtr create( std::string name );

				/**
				 * \brief get plugin types
				 *
				 */
				virtual std::vector<std::type_index> getPluginTypes();

				/**
				 * \brief get plugin type of the given plugin
				 *
				 */
				virtual std::type_index getPluginTypes( std::string name );

				/**
				 * \brief create plugin objects.
				 *
				 */
				virtual std::vector<Core::Plugin::Interfaces::IPluginObjectPtr> create( std::type_index pluginType );
				 
				/**
				  * \brief get plugin datas
				  *
				  */
				virtual std::vector<PluginData> getPluginDatas();


			protected:
				std::unordered_map<std::string, Core::Plugin::Interfaces::IPluginObject* (*)(void)>							_plugins;			//<! The plugin factory method map.
				std::vector<std::string>																					_pluginNames;		//<! The plugin names.
				std::unordered_map<std::type_index, std::vector<Core::Plugin::Interfaces::IPluginObject* (*)(void)>>		_pluginTypeMap;		//!< The plugin type to factory methods map.
				std::vector<std::type_index>																				_pluginTypes;		//<! The plugin names.
				std::vector<PluginData>																						_pluginDatas;		//!< The plugin datas.
			};
		}; //namespace Factory
	}; //namespace Plugin
}; //namespace Core

#endif // __DEFAULTPLUGIN_FACTORY_H__
