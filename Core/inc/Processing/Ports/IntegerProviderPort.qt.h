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
#ifndef __INTEGERPROVIDERPORT_H__
#define __INTEGERPROVIDERPORT_H__

#include "CoreDLL.h"

#include "QObject"
#include "Processing/Object/Port.qt.h"
#include "Processing/Interfaces/IIntegerProvider.qt.h"
#include "boost/shared_ptr.hpp"

namespace Core
{
	namespace Processing
	{
		namespace Ports
		{
			class IntegerProviderPort;

			typedef boost::shared_ptr<IntegerProviderPort>				IntegerProviderPortPtr;
			typedef boost::shared_ptr<const IntegerProviderPort>		ConstIntegerProviderPortPtr;

			/**
			 * \brief single shot provider port.
			 *
			 */
			class CORE_API IntegerProviderPort : public Core::Processing::Object::Port, public Core::Processing::Interfaces::IIntegerProvider
			{
				Q_OBJECT
				DECLARE_OBJECT_FULL(IntegerProviderPort)

			public:
				/**
				 * \brief constructor
				 *
				 */
				IntegerProviderPort(QString uid = "");

				/**
				 * \brief copy constructor
				 *
				 */
				IntegerProviderPort(const IntegerProviderPort& rhs);

				/**
				 * \brief copy constructor
				 *
				 */
				IntegerProviderPort(const IntegerProviderPort& rhs, QString uid);

				MEMBER_SIGNAL_2(void, provideInteger, Core::Processing::Interfaces::ConstIObjectPtr, sender, const int&, value);
			protected:
				virtual bool connectInterfaceMembers(Core::Processing::Interfaces::IProviderPtr source, Core::Processing::Interfaces::IProcessorPtr target);
			};
		};	// namespace Ports
	};	// namespace Processing
}; //namespace Core
Q_DECLARE_METATYPE(Core::Processing::Ports::IntegerProviderPort)

#endif	// __INTEGERPROVIDERPORT_H__
