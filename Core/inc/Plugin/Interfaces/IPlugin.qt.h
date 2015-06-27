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
#ifndef __IPLUGIN_H__
#define __IPLUGIN_H__
#include <QObject>
#include <QtPlugin>
#include "Plugin/Interfaces/IPluginFactory.qt.h"
#include "CoreDLL.h"

namespace Core
{
	namespace Plugin
	{
		namespace Interfaces
		{
			class CORE_API IPlugin: public QObject
			{
				Q_OBJECT
			public:
				IPlugin();
				virtual ~IPlugin() = 0;
				virtual IPluginFactory& getFactory();
			};
		};	//namespace Interfaces
	};	//namespace Plugin
};	//namespace Core

//Q_DECLARE_INTERFACE(Core::Plugin::Interfaces::IPlugin, "de.ce.IPlugin/1.0")

#endif // __IPLUGINFACTORY_H__
