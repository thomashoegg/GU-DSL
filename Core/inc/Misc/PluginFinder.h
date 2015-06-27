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


#ifndef PLUGINFINDER_H_
#define PLUGINFINDER_H_

#if WIN32
#pragma once
#endif

#include "CoreDLL.h"

#include <QtCore/QDir>
#include <QtCore/qpluginloader.h>

#include "boost/shared_ptr.hpp"

namespace Core
{
	namespace Misc
	{

		class CORE_API PluginFinder
		{
		public:
			typedef boost::shared_ptr<PluginFinder> Ptr;
			typedef boost::shared_ptr<const PluginFinder> ConstPtr;
		public:

			// Constructor and Destructor
			PluginFinder();
			~PluginFinder();

			PluginFinder(QDir pluginDir);

			// Creation
			static PluginFinder::Ptr create() { return PluginFinder::Ptr (new PluginFinder());};
			static PluginFinder::Ptr create(QDir pluginDir) { return PluginFinder::Ptr (new PluginFinder(pluginDir));};

			// Static instance
			static PluginFinder& instance() { static PluginFinder inst; return inst; }
	
			// Loading
			bool findPlugins();
			QList<QObject*> loadPlugins(QString baseInterfaceName);

			QList<std::string> getPluginNames();

			// Getter and Setter
			QDir getPluginDir() const;
			void setPluginDir(QDir val);

			void addPluginDir(QDir dir);

		protected:
			// Getter and Setter
			QList<QPluginLoader*> getPlugins() const { return _plugins; }
			void setPlugins(QList<QPluginLoader*> val) { _plugins = val; }

		private:
			QList<QPluginLoader*> _plugins;
			QList<QObject*> _instances;
			std::vector<QDir> _pluginDirs;
	
		};

	}; //namespace Misc
}; //namespace Core
#endif /*PLUGINFINDER_H_*/
