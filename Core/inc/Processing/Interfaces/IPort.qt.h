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

#ifndef PROCESSING_IPORT_H
#define PROCESSING_IPORT_H

#ifdef WIN32
#pragma once
#endif

#include "CoreDLL.h"

#include "boost/shared_ptr.hpp"

#include "Processing/PtrDefinitions.h"

#include "QObject"
#include "Processing/Interfaces/IObject.qt.h"
#include "Processing/Interfaces/IInterface.qt.h"

namespace Core
{

	namespace Processing
	{
		namespace Interfaces
		{
			class CORE_API IPort : public IObject
			{
				Q_OBJECT

			public:

				virtual QString	getName() const = 0;

				virtual void setName(const QString& name) = 0;

				IPort(QString uid): IObject(uid) {}

				IPort(const IPort& rhs): IObject(rhs) {}

				IPort(const IPort& rhs, QString uid): IObject(rhs, uid) {}

				virtual bool addInterface(Core::Processing::Interfaces::IInterfacePtr iPtr) = 0;

				virtual bool connectInterfaces(IProviderPtr source, IProcessorPtr target) = 0;

				virtual bool disconnectInterfaces(IProviderPtr source, IProcessorPtr target) = 0;

				virtual void requestData() = 0;
			};

		};	// namespace Object
	};	// namespace Processing
}; //namespace Core
#endif	// PROCESSING_IPORT_H
