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

#include "Plugin/Factory/DefaultPluginFactory.qt.h"

namespace Core
{
	namespace Plugin
	{
		namespace Factory
		{
			DefaultPluginFactory::DefaultPluginFactory()
			{

			}

			DefaultPluginFactory::~DefaultPluginFactory()
			{
				_plugins.clear();
				_pluginNames.clear();
				_pluginTypeMap.clear();
				_pluginTypes.clear();
				_pluginDatas.clear();
			}

			DefaultPluginFactory& DefaultPluginFactory::instance()
			{
				static DefaultPluginFactory theInstance;
				return theInstance;
			}

			std::vector<std::string> DefaultPluginFactory::getPluginNames()
			{
				return _pluginNames;
			}

			std::vector<std::string> DefaultPluginFactory::getPluginNames( std::type_index type )
			{
				std::vector<std::string> retVal;
				for(::size_t i = 0; i < _pluginDatas.size(); i++)
				{
					if(_pluginDatas[i].type == type)
					{
						retVal.push_back(_pluginDatas[i].name);
					}
				}
				return retVal;
			}

			Core::Plugin::Interfaces::IPluginObjectPtr DefaultPluginFactory::create( std::string name )
			{
				Core::Plugin::Interfaces::IPluginObjectPtr pluginObject;
				std::unordered_map<std::string, Core::Plugin::Interfaces::IPluginObject* (*)(void)>::iterator it = _plugins.find(name);
				if(it != _plugins.end())
				{
					pluginObject.reset(it->second());
				}
				return pluginObject;
			}


			std::vector<std::type_index> DefaultPluginFactory::getPluginTypes()
			{
				return _pluginTypes;
			}

			std::type_index DefaultPluginFactory::getPluginTypes( std::string name )
			{
				for(::size_t i = 0; i < _pluginDatas.size(); i++)
				{
					if(_pluginDatas[i].name.compare(name) == 0)
					{
						return _pluginDatas[i].type;
					}
				}
				return typeid(Core::Plugin::Interfaces::IPluginObject);
			}

			std::vector<Core::Plugin::Interfaces::IPluginObjectPtr> DefaultPluginFactory::create( std::type_index pluginType )
			{
				std::vector<Core::Plugin::Interfaces::IPluginObjectPtr> tmp;
				std::unordered_map<std::type_index, std::vector<Core::Plugin::Interfaces::IPluginObject* (*)(void)>>::iterator it = _pluginTypeMap.find(pluginType);
				if(it != _pluginTypeMap.end())
				{
					std::vector<Core::Plugin::Interfaces::IPluginObject* (*)(void)> plugins = it->second;
					for(std::vector<Core::Plugin::Interfaces::IPluginObject* (*)(void)>::iterator pluginsIt = plugins.begin(); pluginsIt != plugins.end(); ++pluginsIt)
					{
						Core::Plugin::Interfaces::IPluginObjectPtr pluginptr((*pluginsIt)());
						tmp.push_back(pluginptr);
					}
				}
				return tmp;
			}

			void DefaultPluginFactory::addPlugin( std::string name, std::type_index pluginType, Core::Plugin::Interfaces::IPluginObject* (*createFunction)(void) )
			{
				//if plugin not exists
				if(_plugins.find(name) == _plugins.end())
				{
					_plugins.insert(std::pair<std::string, Core::Plugin::Interfaces::IPluginObject* (*)(void)>(name, createFunction));
					_pluginNames.push_back(name);

					std::unordered_map<std::type_index, std::vector<Core::Plugin::Interfaces::IPluginObject* (*)(void)>>::iterator it = _pluginTypeMap.find(pluginType);
					_pluginTypes.push_back(pluginType);
					if(it == _pluginTypeMap.end())
					{
						std::vector<Core::Plugin::Interfaces::IPluginObject* (*)(void)> newFactoryVector;
						newFactoryVector.push_back(createFunction);
						_pluginTypeMap.insert(std::pair<std::type_index, std::vector<Core::Plugin::Interfaces::IPluginObject* (*)(void)>>(pluginType, newFactoryVector));
					}
					else
					{
						it->second.push_back(createFunction);
					}
					_pluginDatas.push_back(PluginData(name, pluginType));
				}
			}

			std::vector<Core::Plugin::Interfaces::IPluginFactory::PluginData> DefaultPluginFactory::getPluginDatas()
			{
				return _pluginDatas;
			}

		}; //namespace Factory
	}; //namespace Plugin
}; //namespace Core
