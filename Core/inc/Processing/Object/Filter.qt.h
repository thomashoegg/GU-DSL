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

#ifndef PROCESSING_FILTER_H
#define PROCESSING_FILTER_H

#ifdef WIN32
#pragma once
#endif

#include "CoreDLL.h"
#include <QObject>
#include "boost/shared_ptr.hpp"
#include "Processing/Object/Class.qt.h"
#include "Processing/PtrDefinitions.h"

QT_BEGIN_NAMESPACE
class QThread;
class QMutex;
QT_END_NAMESPACE

namespace Core
{
	namespace Utilities
	{
		class WaitCondition;
	};
};

namespace Core
{
	namespace Processing
	{
		namespace Object
		{
			class CORE_API Filter : public Class
			{
				Q_OBJECT
				DECLARE_OBJECT(Filter)
			public:

				Filter(QString uid);
				Filter(const Filter& rhs, QString uid);
				Filter(const Filter& rhs);
				virtual ~Filter();

				virtual void start();

				virtual void stop();

			protected:

				virtual void init(); /*{ throw std::logic_error("The method or operation is not implemented."); }*/

				virtual bool process( Core::Processing::Interfaces::ConstIObjectParametersPtr parameters );

				Q_INVOKABLE virtual void startSubModules();

				virtual void cleanup();

			protected Q_SLOTS:
				void doWork();

				void finished();

			protected:
				bool									_isRunning;							//!< Indicator if filter is running.
				bool									_isProcessing;						//!< Indicator if filter is processing.
				QThread*								_thread;							//!< The working tread.
				Core::Utilities::WaitCondition*							_waitForDataCondition;				//!< The wait condition to wake the thread.
				QMutex*									_waitMutex;							//!< The wait mutex.
			};
		};	// namespace Object
	};	// namespace Processing
}; //namespace Core
#endif	// PROCESSING_FILTER_H
