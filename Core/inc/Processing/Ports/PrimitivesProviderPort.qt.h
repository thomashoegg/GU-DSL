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
#ifndef __PRIMITIVESPROVIDERPORT_H__
#define __PRIMITIVESPROVIDERPORT_H__

#include "CoreDLL.h"

#include "QObject"
#include "Processing/Object/Port.qt.h"
#include "Processing/Interfaces/IPrimitivesProvider.qt.h"
#include "boost/shared_ptr.hpp"

namespace Core
{
	namespace Processing
	{
		namespace Ports
		{
			class PrimitivesProviderPort;

			typedef boost::shared_ptr<PrimitivesProviderPort>			PrimitivesProviderPortPtr;
			typedef boost::shared_ptr<const PrimitivesProviderPort>		ConstPrimitivesProviderPortPtr;

			/**
			 * \brief single shot provider port.
			 *
			 */
			class CORE_API PrimitivesProviderPort : public ::Core::Processing::Object::Port, public ::Core::Processing::Interfaces::IPrimitivesProvider
			{
				Q_OBJECT
				DECLARE_OBJECT_FULL(PrimitivesProviderPort)

			public:
				/**
				 * \brief constructor
				 *
				 */
				PrimitivesProviderPort(QString uid = "");

				/**
				 * \brief copy constructor
				 *
				 */
				PrimitivesProviderPort(const PrimitivesProviderPort& rhs);

				/**
				 * \brief copy constructor
				 *
				 */
				PrimitivesProviderPort(const PrimitivesProviderPort& rhs, QString uid);

				MEMBER_SIGNAL_2(void, provideBool, Core::Processing::Interfaces::ConstIObjectPtr, sender, const bool&, value);
				MEMBER_SIGNAL_2(void, provideChar, Core::Processing::Interfaces::ConstIObjectPtr, sender, const char&, value);
				MEMBER_SIGNAL_2(void, provideWChar, Core::Processing::Interfaces::ConstIObjectPtr, sender, const wchar_t&, value);
				MEMBER_SIGNAL_2(void, provideSChar, Core::Processing::Interfaces::ConstIObjectPtr, sender, const signed char&, value);
				MEMBER_SIGNAL_2(void, provideShort, Core::Processing::Interfaces::ConstIObjectPtr, sender, const short&, value);
				MEMBER_SIGNAL_2(void, provideInt, Core::Processing::Interfaces::ConstIObjectPtr, sender, const int&, value);
				MEMBER_SIGNAL_2(void, provideLong, Core::Processing::Interfaces::ConstIObjectPtr, sender, const long&, value);
				MEMBER_SIGNAL_2(void, provideLongLong, Core::Processing::Interfaces::ConstIObjectPtr, sender, const long long&, value);
				MEMBER_SIGNAL_2(void, provideUChar, Core::Processing::Interfaces::ConstIObjectPtr, sender, const unsigned char&, value);
				MEMBER_SIGNAL_2(void, provideUShort, Core::Processing::Interfaces::ConstIObjectPtr, sender, const unsigned short&, value);
				MEMBER_SIGNAL_2(void, provideUInt, Core::Processing::Interfaces::ConstIObjectPtr, sender, const unsigned int&, value);
				MEMBER_SIGNAL_2(void, provideULong, Core::Processing::Interfaces::ConstIObjectPtr, sender, const unsigned long&, value);
				MEMBER_SIGNAL_2(void, provideULongLong, Core::Processing::Interfaces::ConstIObjectPtr, sender, const unsigned long long&, value);
				MEMBER_SIGNAL_2(void, provideFloat, Core::Processing::Interfaces::ConstIObjectPtr, sender, const float&, value);
				MEMBER_SIGNAL_2(void, provideDouble, Core::Processing::Interfaces::ConstIObjectPtr, sender, const double&, value);
			protected:
				virtual bool connectInterfaceMembers(Core::Processing::Interfaces::IProviderPtr source, Core::Processing::Interfaces::IProcessorPtr target);
			};
		};	// namespace Ports
	};	// namespace Processing
}; //namespace Core
Q_DECLARE_METATYPE(Core::Processing::Ports::PrimitivesProviderPort)

#endif	// __PRIMITIVESPROVIDERPORT_H__
