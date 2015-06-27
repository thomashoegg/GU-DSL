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

#ifndef PROCESSING_PLUGIN_H
#define PROCESSING_PLUGIN_H

#ifdef WIN32
#pragma once
#endif

#include "CoreDLL.h"
#include "Processing/Interfaces/IObject.qt.h"
#include "Processing/Interfaces/IObjectPlugin.qt.h"
#include "Processing/PtrDefinitions.h"

namespace Core
{
	namespace Processing
	{
		namespace Plugin
		{
			class CORE_API ProcessingPlugin : public Core::Processing::Interfaces::IObjectPlugin
			{
				Q_OBJECT
				Q_INTERFACES(Core::Processing::Interfaces::IObjectPlugin) 
				Q_PLUGIN_METADATA(IID "de.ce.IObjectPlugin/0.1")

				DECLARE_OBJECT_PLUGIN(ProcessingPlugin)

			public:
				ProcessingPlugin();

				~ProcessingPlugin();



			};
		}; // namespace Plugin

	};	// namespace Processing
}; //namespace Core


#define PROCESSING_REGISTER_OBJECT(Filter)		REGISTER_OBJECT(::Core::Processing::Plugin::ProcessingPlugin, Filter)

#endif	// PROCESSING_PLUGIN_H
