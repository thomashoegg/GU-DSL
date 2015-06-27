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
#ifndef __LOGGER_H__
#define __LOGGER_H__

#define ELPP_AS_DLL // Tells Easylogging++ that it's used for DLL
#define ELPP_EXPORT_SYMBOLS // Tells Easylogging++ to export symbols
#define ELPP_THREAD_SAFE
//#define ELPP_WINSOCK2
#define ELPP_BOOST_LOGGING
#define ELPP_QT_LOGGING

#include "CoreDLL.h"
#include <easylogging++.h>

namespace Core
{
	namespace DebugOutput
	{
		class CORE_API Logger
		{
		public:
			Logger();
			~Logger();
			static el::base::type::StoragePointer getEasyLoggingStorage();
		private:
			static int runceOnceHelper;
			static int runOnce();
		};
	};	//namespace DebugOutput
};	//namespace Core

#endif //__LOGGER_H__
