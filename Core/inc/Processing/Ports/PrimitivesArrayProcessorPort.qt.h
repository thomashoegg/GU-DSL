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
#ifndef __PRIMITIVESARRAYPROCESSORPORT_H__
#define __PRIMITIVESARRAYPROCESSORPORT_H__

#include "CoreDLL.h"
#include "QObject"
#include "Processing/Object/Port.qt.h"
#include "Processing/Interfaces/IPrimitivesArrayProcessor.qt.h"
#include "boost/shared_ptr.hpp"

namespace Core
{
	namespace Processing
	{
		namespace Ports
		{
			class PrimitivesArrayProcessorPort;

			typedef boost::shared_ptr<PrimitivesArrayProcessorPort>				PrimitivesArrayProcessorPortPtr;
			typedef boost::shared_ptr<const PrimitivesArrayProcessorPort>		ConstPrimitivesArrayProcessorPortPtr;

			/**
			 * \brief single shot processor port class.
			 *
			 */
			class CORE_API PrimitivesArrayProcessorPort : public Core::Processing::Object::Port, public Core::Processing::Interfaces::IPrimitivesArrayProcessor
			{
				Q_OBJECT
				DECLARE_OBJECT_FULL(PrimitivesArrayProcessorPort)

			public:
				/**
				 * \brief constructor.
				 *
				 */
				PrimitivesArrayProcessorPort(QString uid = "");

				/**
				 * \brief copy constructor.
				 *
				 */
				PrimitivesArrayProcessorPort(const PrimitivesArrayProcessorPort& rhs);

				/**
				 * \brief copy constructor.
				 *
				 */
				PrimitivesArrayProcessorPort(const PrimitivesArrayProcessorPort& rhs, QString uid);

				MEMBER_SIGNAL_3(void, processBool, Core::Processing::Interfaces::ConstIObjectPtr, sender, const bool*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, processChar, Core::Processing::Interfaces::ConstIObjectPtr, sender, const char*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, processWChar, Core::Processing::Interfaces::ConstIObjectPtr, sender, const wchar_t*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, processSChar, Core::Processing::Interfaces::ConstIObjectPtr, sender, const signed char*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, processShort, Core::Processing::Interfaces::ConstIObjectPtr, sender, const short*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, processInt, Core::Processing::Interfaces::ConstIObjectPtr, sender, const int*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, processLong, Core::Processing::Interfaces::ConstIObjectPtr, sender, const long*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, processLongLong, Core::Processing::Interfaces::ConstIObjectPtr, sender, const long long*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, processUChar, Core::Processing::Interfaces::ConstIObjectPtr, sender, const unsigned char*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, processUShort, Core::Processing::Interfaces::ConstIObjectPtr, sender, const unsigned short*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, processUInt, Core::Processing::Interfaces::ConstIObjectPtr, sender, const unsigned int*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, processULong, Core::Processing::Interfaces::ConstIObjectPtr, sender, const unsigned long*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, processULongLong, Core::Processing::Interfaces::ConstIObjectPtr, sender, const unsigned long long*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, processFloat, Core::Processing::Interfaces::ConstIObjectPtr, sender, const float*, value, size_t, arrayLength);
				MEMBER_SIGNAL_3(void, processDouble, Core::Processing::Interfaces::ConstIObjectPtr, sender, const double*, value, size_t, arrayLength);
			};

		};	// namespace Ports
	};	// namespace Processing
}; //namespace Core
Q_DECLARE_METATYPE(Core::Processing::Ports::PrimitivesArrayProcessorPort)

#endif	// __PRIMITIVESARRAYPROCESSORPORT_H__
