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
#ifndef __IPLUGINOBJECT_H__
#define __IPLUGINOBJECT_H__
#include <boost/shared_ptr.hpp>
#include "CoreDLL.h"
#include "Utilities/MacroHeaven.h"

namespace Core
{
	namespace Plugin
	{
		namespace Interfaces
		{
			class IPluginObject;

			typedef boost::shared_ptr<IPluginObject>			IPluginObjectPtr;
			typedef boost::shared_ptr<const IPluginObject>		ConstIPluginObjectPtr;

			class CORE_API IPluginObject
			{
			public:
				IPluginObject();
				virtual ~IPluginObject() = 0;
				virtual void init();
			};
		};	//namespace Interfaces
	};	//namespace Plugin
};	//namespace Core

#ifdef DELCARE_PLUGIN_OBJECT
#undef DELCARE_PLUGIN_OBJECT
#endif
#define DELCARE_PLUGIN_OBJECT(PluginObjectType)																									\
	public: static Core::Plugin::Interfaces::IPluginObject* create(void)																		\
	{																																			\
		return new PluginObjectType();																											\
	}

#ifdef REGISTER_PLUGIN_OBJECT
#undef REGISTER_PLUGIN_OBJECT
#endif
#define REGISTER_PLUGIN_OBJECT(PluginObject, PluginType, ...)																					\
	VARARG(REGISTER_PLUGIN_OBJECT_, PluginObject, PluginType, __VA_ARGS__)

#ifdef REGISTER_PLUGIN_OBJECT_2
#undef REGISTER_PLUGIN_OBJECT_2
#endif
#define REGISTER_PLUGIN_OBJECT_2(PluginObject, PluginType)																						\
	namespace																																	\
	{																																			\
		void Register##PluginObject()																											\
		{																																		\
			Core::Plugin::Factory::DefaultPluginFactory::instance().addPlugin(#PluginObject, typeid(PluginType), &PluginObject::create);		\
		}																																		\
		Q_CONSTRUCTOR_FUNCTION(Register##PluginObject)																							\
	}

#ifdef REGISTER_PLUGIN_OBJECT_3
#undef REGISTER_PLUGIN_OBJECT_3
#endif
#define REGISTER_PLUGIN_OBJECT_3(PluginObject, PluginType, FactoryName)																			\
	namespace																																	\
	{																																			\
		void Register##PluginObject()																											\
		{																																		\
			FactoryName::instance().addPlugin(#PluginObject, typeid(#PluginType), &PluginObject::create);										\
		}																																		\
		Q_CONSTRUCTOR_FUNCTION(Register##PluginObject)																							\
	}
	

#endif // __IPLUGINOBJECT_H__
