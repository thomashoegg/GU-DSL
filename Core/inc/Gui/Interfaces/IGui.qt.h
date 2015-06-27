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
#ifndef __IGUI_H__
#define __IGUI_H__
#include <boost/shared_ptr.hpp>
#include "Plugin/Interfaces/IPluginObject.qt.h"
#include "CoreDLL.h"

namespace Core
{
	namespace Gui
	{
		namespace Interfaces
		{
			class IGui;

			typedef boost::shared_ptr<IGui>	IGuiPtr;
			typedef boost::shared_ptr<const IGui> ConstIGuiPtr;

			class CORE_API IGui : public Core::Plugin::Interfaces::IPluginObject
			{
			public:
				IGui();
				virtual ~IGui() = 0;
				virtual void show() = 0;
				virtual void close() = 0;
				virtual void setActiveWindow() = 0;
				virtual bool isUseStyles() const = 0;
			};
		};	//namespace Interfaces
	};	//namespace Gui
};	//namespace Core
#endif // __IGUI_H__
