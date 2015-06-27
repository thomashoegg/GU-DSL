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
#ifndef __WAITCONDITION_H__
#define __WAITCONDITION_H__
#include "CoreDLL.h"
#include <qglobal.h>

QT_BEGIN_NAMESPACE
	class QMutex;
	class QReadWriteLock;
	class QWaitCondition;
QT_END_NAMESPACE

namespace Core
{
	namespace Utilities
	{
		class CORE_API WaitCondition
		{
		public:
			WaitCondition();
			~WaitCondition();

			bool wait(QMutex * lockedMutex, unsigned long time = ULONG_MAX);
			bool wait(QReadWriteLock * lockedReadWriteLock, unsigned long time = ULONG_MAX);
			void wake();
		protected:
			QWaitCondition*				_condition;
			QMutex*						_skipWaitMutex;
			bool						_skipWait;
		private:
			Q_DISABLE_COPY(WaitCondition)
		};
	}; //namespace Utilities
}; //namespace Core
#endif
