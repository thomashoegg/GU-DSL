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
#ifndef __PLUGINFACTORY_H__
#define __PLUGINFACTORY_H__
#include "Plugin/Interfaces/IPluginFactory.qt.h"
#include <boost/shared_ptr.hpp>
#include <QObject>
#include <QPluginLoader>
#include "Plugin/Interfaces/IPluginObject.qt.h"
#include "Plugin/Interfaces/IPluginFactory.qt.h"
#include "Plugin/Interfaces/IPlugin.qt.h"
#include "Misc/PluginFinder.h"
#include "CoreDLL.h"
#include <unordered_map>
#include <typeindex>

namespace Core
{
	namespace Plugin
	{
		namespace Factory
		{
			class CORE_API PluginFactoryManager : public Core::Plugin::Interfaces::IPluginFactory
			{
				Q_OBJECT

			private:
				/**
				 * \brief constructor
				 *
				 */
				PluginFactoryManager();

				/**
				 * \brief private copy construtor.
				 *
				 */
				PluginFactoryManager(const PluginFactoryManager&);	//Don't implement

				/**
				 * \brief assignment operator
				 *
				 */
				void operator=(const PluginFactoryManager&);	//Don't implement

			public:
				/**
				 * \brief deconstructor
				 *
				 */
				~PluginFactoryManager();

				/**
				 * \brief load plugins
				 *
				 */
				void loadPlugins();

				/**
				 * \brief get plugin directory
				 *
				 *	\return QString the plugin directory
				 */
				QString getPluginDirectory() const;

				/**
				 * \brief set plugin directory.
				 *
				 *	\param QString directory. The plugin directory to be set.
				 */
				void setPluginDirectory(QString directory);

				
				/**
				 * \brief adds an additional plugin directory.
				 *
				 *	\param QString directory. The plugin directory to be added.
				 */
				void addPluginDirectory(QString directory);

				/**
				 * \brief create plugin opbject with the given name.
				 *
				 *	\param std::string name. The object name to create.
				 *	\return The created plugin object
				 */
				virtual Core::Plugin::Interfaces::IPluginObjectPtr create(std::string name );

				/**
				 * \brief create all plugins of the given type.
				 *
				 *	\param std::type_index pluginType. The type of the plugins to create.
				 *	\return the list of created plugin objects.
				 */
				virtual std::vector<Core::Plugin::Interfaces::IPluginObjectPtr> create( std::type_index pluginType );

				/**
				 * \brief cleanup.
				 *
				 */
				void cleanup();

				/**
				 * \brief get plugin factory instance.
				 *
				 */
				static PluginFactoryManager& instance();

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
				  * \brief get plugin datas
				  *
				  */
				virtual std::vector<PluginData> getPluginDatas();

			protected:
//				QString																							_pluginDir;				//!< The plugin directory.

				std::vector<QString>																			_pluginDirs;			//!< All available plugin directories
				std::unordered_map<std::string, Core::Plugin::Interfaces::IPluginFactory*>						_plugins;				//!< The loaded plugins.
				std::unordered_map<std::type_index, std::vector<Core::Plugin::Interfaces::IPluginFactory*>>		_pluginsTypesMap;		//!< The containing pugin types.
				std::vector<std::string>																		_pluginNames;			//<! The plugin names.
				std::vector<std::type_index>																	_pluginTypes;			//<! The plugin names.
				std::vector<PluginData>																			_pluginDatas;			//!< The plugin datas.
			};
		};	//namespace Factory
	};	//namespace Plugin
};	//namespace Core
#endif //__PLUGINFACTORY_H__
