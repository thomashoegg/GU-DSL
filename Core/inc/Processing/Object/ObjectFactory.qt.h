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

#ifndef PROCESSING_OBJECT_FACTORY_H
#define PROCESSING_OBJECT_FACTORY_H

#ifdef WIN32
#pragma once
#endif

#include "CoreDLL.h"

#include "QObject"
#include "QString"
#include "QList"
#include "QMap"


#include "boost/shared_ptr.hpp"
#include "Misc/PluginFinder.h"

#include "Processing/Interfaces//IObject.qt.h"
#include "Processing/PtrDefinitions.h"

namespace Core
{
	namespace Processing
	{
		namespace Object
		{
			class ObjectFactory;

			typedef boost::shared_ptr<ObjectFactory>	ObjectFactoryPtr;

			class CORE_API ObjectFactory : public QObject
			{
				Q_OBJECT

			private:

				ObjectFactory();

			public:

				bool installObjectPlugin(QString ObjectPluginFileName);

				void loadObjects();

				QList<Core::Processing::Interfaces::ConstIObjectPtr> getComponentDiagrams();

				static std::list<QPluginLoader*> findPlugins(QString pluginDir);

				QString getPluginDirectory() const { return _pluginDir; }
				void setPluginDirectory(QString directory);

				Core::Processing::Interfaces::IObjectPtr createObject(QString ObjectName, QString uid = "", bool disableEmitObjectCreated = false);

				void cleanup();

				static ObjectFactoryPtr instance();

				void emitObjectCreated(Core::Processing::Interfaces::IObjectPtr objPtr);

	Q_SIGNALS:
				void objectCreated(Core::Processing::Interfaces::IObjectPtr objPtr);


			protected:
				QString _pluginDir;

				QList<Core::Processing::Interfaces::ConstIObjectPtr> _ComponentDiagrams;

				typedef QMap<QString, Core::Processing::Interfaces::ConstIObjectPtr> ObjectMap;

				ObjectMap _ObjectMap;

				static ObjectFactoryPtr _instance;

				bool _disableEmitObjectCreated;
			};

		};	// namespace Object
	};	// namespace Processing
}; //namespace Core
#endif	// PROCESSING_OBJECT_FACTORY_H
