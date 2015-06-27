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
#ifndef __IPLUGININITIALIZER_H__
#define __IPLUGININITIALIZER_H__
#include <boost/shared_ptr.hpp>
#include "CoreDLL.h"
#include "Plugin/Interfaces/IPluginObject.qt.h"
#include <vector>
#include <typeindex>

#pragma warning(push)
#pragma warning(disable: 4251)

namespace Core
{
	namespace Plugin
	{
		namespace Interfaces
		{
			class IPluginInitializer;

			typedef boost::shared_ptr<IPluginInitializer>			IPluginInitializerPtr;
			typedef boost::shared_ptr<const IPluginInitializer>		ConstIPluginInitializerPtr;

			class CORE_API IPluginInitializer : public IPluginObject
			{
			public:
				struct CORE_API Argument
				{
					Argument( void* arg, std::type_index argType);
					~Argument();
					void* argument;
					std::type_index	type;
				};
			public:
				IPluginInitializer();
				virtual ~IPluginInitializer() = 0;
				virtual void initializePlugin(std::vector<Argument> arguments) = 0;
				virtual void cleanupPlugin() = 0;
			};
		};	//namespace Interfaces
	};	//namespace Plugin
};	//namespace Core

#pragma warning(pop)

#endif // __IPLUGINOBJECT_H__
