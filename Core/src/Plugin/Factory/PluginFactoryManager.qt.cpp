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

#include "Plugin/Factory/PluginFactoryManager.qt.h"
#include "DebugOutput/DebugStream.h"
#include "Processing/Interfaces/IObjectPlugin.qt.h"

namespace Core
{
namespace Plugin
{
namespace Factory
{
	PluginFactoryManager::PluginFactoryManager()
	{
		if(_pluginDirs.size() <= 0)
			_pluginDirs.push_back("");

		_pluginDirs[0] = QDir::currentPath();

		QDir pluginDir(_pluginDirs[0]);
		Misc::PluginFinder::instance().setPluginDir(_pluginDirs[0]);
	}

	PluginFactoryManager::~PluginFactoryManager()
	{
		cleanup();
	}

	void PluginFactoryManager::loadPlugins()
	{
		QList<QObject*> plugins = Misc::PluginFinder::instance().loadPlugins(Core::Plugin::Interfaces::IPlugin::staticMetaObject.className());
		for (QList<QObject*>::iterator it = plugins.begin(); it != plugins.end();it++)
		{
			Core::Plugin::Interfaces::IPlugin* pluginLoader = dynamic_cast<Core::Plugin::Interfaces::IPlugin*>(*it);
			if(pluginLoader != nullptr)
			{
				for(std::string name : pluginLoader->getFactory().getPluginNames())
				{
					if(_plugins.find(name) == _plugins.end())
					{
						_plugins.insert(std::pair<std::string, Core::Plugin::Interfaces::IPluginFactory*>(name, &pluginLoader->getFactory()));
						_pluginNames.push_back(name);
					}
				}

				for(std::type_index pluginType :pluginLoader->getFactory().getPluginTypes())
				{
					std::unordered_map<std::type_index, std::vector<Core::Plugin::Interfaces::IPluginFactory*>>::iterator pluginTypeIt = _pluginsTypesMap.find(pluginType);
					if(pluginTypeIt != _pluginsTypesMap.end())
					{
						if(std::find(pluginTypeIt->second.begin(), pluginTypeIt->second.end(), &pluginLoader->getFactory()) == pluginTypeIt->second.end())
						{
							pluginTypeIt->second.push_back(&pluginLoader->getFactory());
						}
					}
					else
					{
						std::vector<Core::Plugin::Interfaces::IPluginFactory*> tmp;
						tmp.push_back(&pluginLoader->getFactory());
						_pluginsTypesMap.insert(std::pair<std::type_index, std::vector<Core::Plugin::Interfaces::IPluginFactory*>>(pluginType, tmp) );
						_pluginTypes.push_back(pluginType);
					}
				}

				for(Core::Plugin::Interfaces::IPluginFactory::PluginData pluginData :pluginLoader->getFactory().getPluginDatas())
				{
					if(std::find(_pluginDatas.begin(), _pluginDatas.end(), pluginData) == _pluginDatas.end())
					{
						_pluginDatas.push_back(pluginData);
					}
				}
			}
			else if(pluginLoader == nullptr && dynamic_cast<Core::Processing::Interfaces::IObjectPlugin*>(*it) == nullptr)
			{
				std::string errorMessage = "Could not load plugin: ";
				errorMessage.append((*it)->metaObject()->className());
				E_OUT(errorMessage)
			}
		}
	}

	Core::Plugin::Interfaces::IPluginObjectPtr PluginFactoryManager::create( std::string name)
	{
		Core::Plugin::Interfaces::IPluginObjectPtr object;
		std::unordered_map<std::string, Core::Plugin::Interfaces::IPluginFactory*>::iterator it = _plugins.find(name);
		if(it != _plugins.end())
		{
			if(Core::Plugin::Interfaces::IPluginFactory* pluginFactory = it->second)
			{
				object = pluginFactory->create(name);
			}
		}

		return object;
	}

	std::vector<Core::Plugin::Interfaces::IPluginObjectPtr> PluginFactoryManager::create( std::type_index pluginType )
	{
		std::vector<Core::Plugin::Interfaces::IPluginObjectPtr> returnVector;
		std::unordered_map<std::type_index, std::vector<Core::Plugin::Interfaces::IPluginFactory*>>::iterator pluginTypeIt = _pluginsTypesMap.find(pluginType);
		if(pluginTypeIt != _pluginsTypesMap.end())
		{
			std::vector<Core::Plugin::Interfaces::IPluginFactory*> pluginVector = pluginTypeIt->second;
			for(std::vector<Core::Plugin::Interfaces::IPluginFactory*>::iterator it = pluginVector.begin(); it != pluginVector.end(); ++it)
			{
				if(*it)
				{
					std::vector<Core::Plugin::Interfaces::IPluginObjectPtr> tmp = (*it)->create(pluginType);
					for(Core::Plugin::Interfaces::IPluginObjectPtr pluginObject : tmp)
					{
						returnVector.push_back(pluginObject);
					}
				}
			}
		}
		return returnVector;
	}

	void PluginFactoryManager::cleanup()
	{
		_plugins.clear();
		_pluginsTypesMap.clear();
		_pluginNames.clear();
		_pluginTypes.clear();
		_pluginDatas.clear();
	}

	PluginFactoryManager& PluginFactoryManager::instance()
	{
		static PluginFactoryManager theInstance;
		return theInstance;
	}

	QString PluginFactoryManager::getPluginDirectory() const
	{
		if(_pluginDirs.size() > 0)
			return _pluginDirs[0];

		return "";
	}

	void PluginFactoryManager::setPluginDirectory( QString directory )
	{
		if(_pluginDirs.size() <= 0)
			_pluginDirs.push_back("");

		_pluginDirs[0] = directory;
		Misc::PluginFinder::instance().setPluginDir(_pluginDirs[0]);
	}

	std::vector<std::string> PluginFactoryManager::getPluginNames()
	{
		return _pluginNames;
	}

	std::vector<std::string> PluginFactoryManager::getPluginNames( std::type_index type )
	{
		std::vector<std::string> retValue;
		for(::size_t i = 0; i < _pluginDatas.size(); i++)
		{
			if(_pluginDatas[i].type == type)
			{
				retValue.push_back(_pluginDatas[i].name);
			}
		}
		return retValue;
	}

	std::vector<std::type_index> PluginFactoryManager::getPluginTypes()
	{
		return _pluginTypes;
	}

	std::type_index PluginFactoryManager::getPluginTypes( std::string name )
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

	std::vector<Core::Plugin::Interfaces::IPluginFactory::PluginData> PluginFactoryManager::getPluginDatas()
	{
		return _pluginDatas;
	}

	void PluginFactoryManager::addPluginDirectory( QString directory )
	{
		QDir pluginDir(directory);
		if(pluginDir.exists())
			Misc::PluginFinder::instance().addPluginDir(pluginDir);
	}

};	//namespace Factory
};	//namespace Plugin
};	//namespace Core
