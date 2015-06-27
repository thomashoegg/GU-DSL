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
#ifndef __IWIDGET_H__
#define __IWIDGET_H__
#include <boost/shared_ptr.hpp>
#include "Plugin/Interfaces/IPluginObject.qt.h"
#include "CoreDLL.h"
#include <qglobal.h>

QT_BEGIN_NAMESPACE
	class QWidget;
QT_END_NAMESPACE

namespace Core
{
	namespace Gui
	{
		namespace Interfaces
		{
			class IWidget;

			typedef boost::shared_ptr<IWidget>	IWidgetPtr;
			typedef boost::shared_ptr<const IWidget> ConstIWidgetPtr;

			class CORE_API IWidget : public Core::Plugin::Interfaces::IPluginObject
			{
			public:
				enum Position
				{
					Center	= 0,
					Left	= 0x1,
					Right	= 0x2,
					Top		= 0x4,
					Bottom	= 0x8,
					Mask	= 0xf,
					All		= Mask
				};
			public:
				IWidget();
				virtual ~IWidget() = 0;
				virtual QWidget* getWidget() = 0;
				virtual std::string getName() = 0;
				virtual Position getPosition() = 0;
			};
		};	//namespace Interfaces
	};	//namespace Gui
};	//namespace Core
#endif // __IWIDGET_H__
