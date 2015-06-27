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

#ifndef PROCESSING_IOBJECT_PLUGIN_H
#define PROCESSING_IOBJECT_PLUGIN_H

#ifdef WIN32
#pragma once
#endif

#include "CoreDLL.h"

#include <QObject>
#include <QList>

#include "boost/shared_ptr.hpp"

#include "Processing/Interfaces/IObject.qt.h"

namespace Core
{
	namespace Processing
	{
		namespace Interfaces
		{
			class IObjectParameters;

			class CORE_API IObjectPlugin : public QObject
			{
				Q_OBJECT

			public:
				virtual QList<ConstIObjectPtr> getObjects() = 0;

				virtual QList<ConstIObjectPtr> getComponentDiagrams() = 0;
			};

		}	// namespace Object
	}	// namespace Processing
}; //namespace Core

#ifdef DECLARE_OBJECT_PLUGIN
#undef DECLARE_OBJECT_PLUGIN
#endif

#define DECLARE_OBJECT_PLUGIN(Plugin)																													\
		protected:		QList<::Core::Processing::Interfaces::ConstIObjectPtr> _Object;																	\
						QList<::Core::Processing::Interfaces::ConstIObjectPtr> _ComponentDiagrams;														\
						static Plugin* _instance;																										\
		public:			void registerObject(::Core::Processing::Interfaces::ConstIObjectPtr Object) { _Object.append(Object); }							\
						void registerComponentDiagram(::Core::Processing::Interfaces::ConstIObjectPtr Object) { _ComponentDiagrams.append(Object); }	\
						static Plugin* instance() { if(_instance == nullptr) _instance = new Plugin(); return _instance; }								\
						QList<::Core::Processing::Interfaces::ConstIObjectPtr> getObjects();															\
						QList<::Core::Processing::Interfaces::ConstIObjectPtr> getComponentDiagrams();													\


#ifdef DEFINE_OBJECT_PLUGIN
#undef DEFINE_OBJECT_PLUGIN
#endif
	
#define DEFINE_OBJECT_PLUGIN(Plugin)	Plugin* Plugin::_instance = nullptr;													\
										QList<::Core::Processing::Interfaces::ConstIObjectPtr> Plugin::getObjects()				\
										{																						\
											return instance()->_Object;															\
										}																						\
																																\
										QList<::Core::Processing::Interfaces::ConstIObjectPtr> Plugin::getComponentDiagrams()	\
										{																						\
											return instance()->_ComponentDiagrams;												\
										}

#define GENERIC_REGISTER_OBJECT(PluginType, ObjectType, RegisterFunction)				\
																						\
namespace																				\
{																						\
																						\
void RegisterFunction()																	\
{																						\
	Core::Processing::Interfaces::ConstIObjectPtr sObject = ObjectType::createObject();	\
	PluginType::instance()->RegisterFunction(sObject);									\
	qRegisterMetaType<ObjectType>();													\
}																						\
Q_CONSTRUCTOR_FUNCTION(RegisterFunction)												\
}


#ifdef REGISTER_OBJECT
#undef REGISTER_OBJECT
#endif

#ifdef REGISTER_COMPONENT_DIAGRAM
#undef REGISTER_COMPONENT_DIAGRAM
#endif


#define REGISTER_OBJECT(PluginType, ObjectType)					GENERIC_REGISTER_OBJECT(PluginType, ObjectType, registerObject)		
#define REGISTER_COMPONENT_DIAGRAM(PluginType, ObjectType)		GENERIC_REGISTER_OBJECT(PluginType, ObjectType, registerComponentDiagram)		


										

Q_DECLARE_INTERFACE(Core::Processing::Interfaces::IObjectPlugin, "de.ce.IObjectPlugin/0.1")

#endif	// PROCESSING_IOBJECT_PLUGIN_H
