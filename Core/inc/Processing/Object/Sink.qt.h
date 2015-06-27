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

#ifndef PROCESSING_SINK_H
#define PROCESSING_SINK_H

#ifdef WIN32
#pragma once
#endif

#include "CoreDLL.h"

#include "QObject"
#include "Processing/Object/Class.qt.h"

#include "boost/shared_ptr.hpp"

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
			class CORE_API Sink : public Class
			{
				Q_OBJECT
				DECLARE_OBJECT(Sink)
			public:
				Sink(QString uid);
				Sink(const Sink& rhs, QString uid);
				Sink(const Sink& rhs);
				virtual ~Sink();

				virtual void start();

				virtual void stop();

			protected:

				virtual void init();

				virtual bool process( Core::Processing::Interfaces::ConstIObjectParametersPtr parameters );

				Q_INVOKABLE virtual void startSubModules();

				virtual void cleanup();

			protected Q_SLOTS:
				void doWork();

				void finished();

			protected:
				bool				_isRunning;
				bool				_isProcessing;
				QThread*			_thread;
				Core::Utilities::WaitCondition*		_waitForDataCondition;
				QMutex*				_waitMutex;
			};
		};	// namespace Object
	};	// namespace Processing
}; //namespace Core
#endif	// PROCESSING_SINK_H
