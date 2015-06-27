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
#ifndef __PRIMITIVESARRAYPROVIDERPORT_H__
#define __PRIMITIVESARRAYPROVIDERPORT_H__

#include "CoreDLL.h"

#include "QObject"
#include "Processing/Object/Port.qt.h"
#include "Processing/Interfaces/IPrimitivesArrayProvider.qt.h"
#include "boost/shared_ptr.hpp"

namespace Core
{
	namespace Processing
	{
		namespace Ports
		{
			class PrimitivesArrayProviderPort;

			typedef boost::shared_ptr<PrimitivesArrayProviderPort>				PrimitivesArrayProviderPortPtr;
			typedef boost::shared_ptr<const PrimitivesArrayProviderPort>		ConstPrimitivesArrayProviderPortPtr;

			/**
			 * \brief single shot provider port.
			 *
			 */
			class CORE_API PrimitivesArrayProviderPort : public Core::Processing::Object::Port, public Core::Processing::Interfaces::IPrimitivesArrayProvider
			{
				Q_OBJECT
				DECLARE_OBJECT_FULL(PrimitivesArrayProviderPort)
			public:
				/**
				 * \brief constructor
				 *
				 */
				PrimitivesArrayProviderPort(QString uid = "");

				/**
				 * \brief copy constructor
				 *
				 */
				PrimitivesArrayProviderPort(const PrimitivesArrayProviderPort& rhs);

				/**
				 * \brief copy constructor
				 *
				 */
				PrimitivesArrayProviderPort(const PrimitivesArrayProviderPort& rhs, QString uid);

				MEMBER_SIGNAL_3(void, provideBool, Core::Processing::Interfaces::ConstIObjectPtr, sender, const bool*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, provideChar, Core::Processing::Interfaces::ConstIObjectPtr, sender, const char*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, provideWChar, Core::Processing::Interfaces::ConstIObjectPtr, sender, const wchar_t*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, provideSChar, Core::Processing::Interfaces::ConstIObjectPtr, sender, const signed char*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, provideShort, Core::Processing::Interfaces::ConstIObjectPtr, sender, const short*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, provideInt, Core::Processing::Interfaces::ConstIObjectPtr, sender, const int*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, provideLong, Core::Processing::Interfaces::ConstIObjectPtr, sender, const long*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, provideLongLong, Core::Processing::Interfaces::ConstIObjectPtr, sender, const long long*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, provideUChar, Core::Processing::Interfaces::ConstIObjectPtr, sender, const unsigned char*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, provideUShort, Core::Processing::Interfaces::ConstIObjectPtr, sender, const unsigned short*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, provideUInt, Core::Processing::Interfaces::ConstIObjectPtr, sender, const unsigned int*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, provideULong, Core::Processing::Interfaces::ConstIObjectPtr, sender, const unsigned long*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, provideULongLong, Core::Processing::Interfaces::ConstIObjectPtr, sender, const unsigned long long*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, provideFloat, Core::Processing::Interfaces::ConstIObjectPtr, sender, const float*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, provideDouble, Core::Processing::Interfaces::ConstIObjectPtr, sender, const double*, value, size_t, arrayLength);
			protected:
				virtual bool connectInterfaceMembers(Core::Processing::Interfaces::IProviderPtr source, Core::Processing::Interfaces::IProcessorPtr target);
			};
		};	// namespace Ports
	};	// namespace Processing
}; //namespace Core
Q_DECLARE_METATYPE(Core::Processing::Ports::PrimitivesArrayProviderPort)

#endif	// __PRIMITIVESARRAYPROVIDERPORT_H__
