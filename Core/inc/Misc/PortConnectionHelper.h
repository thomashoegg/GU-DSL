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
#ifndef __PORTCONNECTIONHELPER_H__
#define __PORTCONNECTIONHELPER_H__

#include "CoreDLL.h"
#include <QString>
#include "Processing/Interfaces/IPort.qt.h"

namespace Core
{
	namespace Misc
	{
		/**
		 * \brief PortConnectionHelper struct.
		 *
		 */
		struct CORE_API PortConnectionHelper
		{
			Core::Processing::Interfaces::IPortPtr	SourcePort;
			QString									TargetUid;

			PortConnectionHelper(Core::Processing::Interfaces::IPortPtr source, QString target)
			{
				SourcePort = source;
				TargetUid = target;
			}
		};
	};
};
#endif //__PORTCONNECTIONHELPER_H__
