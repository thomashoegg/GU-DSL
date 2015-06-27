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

#include "Processing/Object/ObjectFactory.qt.h"
#include "QDirIterator"
#include "QPluginLoader"
#include <QApplication>
#include "Processing/Interfaces/IObjectPlugin.qt.h"
#include "Processing/Interfaces/IObject.qt.h"
#include <assert.h>
#include "DebugOutput/DebugStream.h"
#include "Plugin/Interfaces/IPlugin.qt.h"


namespace Core
{
namespace Processing
{
namespace Object
{

ObjectFactoryPtr ObjectFactory::_instance(new ObjectFactory());

ObjectFactory::ObjectFactory()
{
	QString	pluginDirPath = QDir::currentPath();

	QDir pluginDir(pluginDirPath);

	Misc::PluginFinder::instance().setPluginDir(pluginDir);
	_disableEmitObjectCreated = false;
}

/**
* \brief Returns the single instance
*
* Returns the instance of the singleton Object Factory
*
* \return  ObjectFactory&		The Instance
*/
ObjectFactoryPtr ObjectFactory::instance()
{
	return _instance;
}


bool ObjectFactory::installObjectPlugin( QString ObjectPluginFileName )
{
	return false;
}

void ObjectFactory::loadObjects()
{
	QList<Core::Processing::Interfaces::ConstIObjectPtr> objects;

	QList<QObject*> plugins = Misc::PluginFinder::instance().loadPlugins(Core::Processing::Interfaces::IObjectPlugin::staticMetaObject.className());
	for (QList<QObject*>::iterator it = plugins.begin();
		it != plugins.end();
		it++)
	{
		Core::Processing::Interfaces::IObjectPlugin* pluginLoader = dynamic_cast<Core::Processing::Interfaces::IObjectPlugin*>(*it);
		if(pluginLoader != nullptr)
		{
			objects.append(pluginLoader->getObjects());
			_ComponentDiagrams.append(pluginLoader->getComponentDiagrams());
		}
		else if ( pluginLoader == nullptr && dynamic_cast<Core::Plugin::Interfaces::IPlugin*>(*it) == nullptr)
		{
			std::string errorMessage = "Could not load plugin: ";
			errorMessage.append((*it)->metaObject()->className());
			D_OUT2(__FUNCTION__,  errorMessage)
		}
	}

	for(QList<Core::Processing::Interfaces::ConstIObjectPtr>::iterator it = objects.begin(); it != objects.end(); it++)
	{
		const QMetaObject* meta = (*it)->metaObject();
		QString name = meta->className();

		if(!name.isEmpty())
		{
			_ObjectMap.insert(name, *it);
		}
		else
		{
			D_OUT2(__FUNCTION__, "Plugin name is emtpy")
		}
	}
}

Core::Processing::Interfaces::IObjectPtr ObjectFactory::createObject(QString ObjectName, QString uid /*= ""*/, bool disableEmitObjectCreated /*= false*/)
{
	ObjectMap::iterator it = _ObjectMap.find(ObjectName);
	if(it != _ObjectMap.end())
	{
		_disableEmitObjectCreated = disableEmitObjectCreated;
		Core::Processing::Interfaces::IObjectPtr ptr = it.value()->create(uid);
		_disableEmitObjectCreated = false;
		return ptr;
	}
	std::string errorMessage = "Could not create object with the name ";
	errorMessage.append(ObjectName.toStdString());
	D_OUT2(__FUNCTION__, errorMessage)
	assert(it != _ObjectMap.end() && "Could not create object");
	return Core::Processing::Interfaces::IObjectPtr();
}

QList<Core::Processing::Interfaces::ConstIObjectPtr> ObjectFactory::getComponentDiagrams()
{
	return _ComponentDiagrams;
}

void ObjectFactory::cleanup()
{
	_ObjectMap.clear();
	_ComponentDiagrams.clear();
}

void ObjectFactory::emitObjectCreated( Core::Processing::Interfaces::IObjectPtr objPtr )
{
	if(!_disableEmitObjectCreated)
	{
		objPtr->init();
		emit objectCreated(objPtr);
		qApp->processEvents();
	}
}

void ObjectFactory::setPluginDirectory( QString directory )
{
	_pluginDir = directory;
	Misc::PluginFinder::instance().setPluginDir(_pluginDir);
}

};	// namespace Object
};	// namespace Processing
}; //namespace Core
