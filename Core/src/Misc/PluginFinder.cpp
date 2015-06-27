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

#include "Misc/PluginFinder.h"

#include "QDirIterator"

#include <algorithm>

namespace Core
{
namespace Misc
{

PluginFinder::PluginFinder()
{
	bool dirSet = false;
	QDirIterator it(QDir::current(), QDirIterator::Subdirectories);
	while (it.hasNext())
	{
		it.next();
		if (it.fileName() == "Plugins")
		{
			_pluginDirs.push_back(QDir(it.filePath()));
			dirSet = true;
			//std::cout << _pluginDir.currentPath().toStdString() << std::endl;
			break;
		}
	}
	if(_pluginDirs.size() < 1) 
		_pluginDirs.push_back(QDir::current());
}


PluginFinder::~PluginFinder()
{
	for(int i = 0; i < _plugins.size(); i++)
		delete _plugins[i];

	_plugins.clear();
	_instances.clear();


}

PluginFinder::PluginFinder(QDir pluginDir)
{
	_pluginDirs.push_back(pluginDir);
}

bool PluginFinder::findPlugins()
{
	QStringList filters;

#ifdef _DEBUG
	filters << "*d.dll";
#else
	filters << "*.dll";
#endif // _DEBUG

	for(QDir dir : _pluginDirs)
	{
		QString dirName = dir.absolutePath();

		QDirIterator it (dirName, filters);								/* Run through dlls */
		while(it.hasNext())
		{
			it.next();
			QString name = it.fileName();
			_plugins.push_back(new QPluginLoader(dir.absoluteFilePath(name)));
		}
	}
	
	return true;

}

bool IsNull (QObject* i) { return i == nullptr; }

QList<QObject*> PluginFinder::loadPlugins(QString baseInterfaceName)
{
	if(_plugins.empty()) findPlugins();
	
	for (QList<QPluginLoader*>::iterator it = _plugins.begin();
		it != _plugins.end();
		it++)
	{
		QObject* pObj = qobject_cast<QObject*>((*it)->instance());
		if(pObj != nullptr && pObj->inherits(baseInterfaceName.toLatin1()))
		{
			_instances.push_back(qobject_cast<QObject*>((*it)->instance()));
		}
	}
	return _instances;
}

QList<std::string> PluginFinder::getPluginNames()
{
	QList<std::string> names;
	QStringList filters;


#ifdef _DEBUG
	filters << "*d.dll";
#else
	filters << "*.dll";
#endif // _DEBUG

	for(QDir dir : _pluginDirs)
	{
		QDirIterator it (dir.dirName(), filters);								/* Run through dlls */
		while(it.hasNext())
		{
			it.next();
			QString name = it.fileName();
			names.push_back(name.toStdString());															
		}
	}
	
	return names;
}

QDir PluginFinder::getPluginDir() const
{
	if(_pluginDirs.size() < 1)
		return QDir::current();

	return _pluginDirs[0];
}

void PluginFinder::setPluginDir( QDir val )
{
	if(_pluginDirs.size() < 1)
		_pluginDirs.push_back(QDir::current());

	_pluginDirs[0] = val;
}

void PluginFinder::addPluginDir( QDir dir )
{
	_pluginDirs.push_back(dir);
}

}; //namespace Misc
}; //namespace Core
