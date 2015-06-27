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
#ifndef __PRIMITIVESPROCESSORPORT_H__
#define __PRIMITIVESPROCESSORPORT_H__

#include "CoreDLL.h"
#include "QObject"
#include "Processing/Object/Port.qt.h"
#include "Processing/Interfaces/IPrimitivesProcessor.qt.h"
#include "boost/shared_ptr.hpp"

namespace Core
{
	namespace Processing
	{
		namespace Ports
		{
			class PrimitivesProcessorPort;

			typedef boost::shared_ptr<PrimitivesProcessorPort>				PrimitivesProcessorPortPtr;
			typedef boost::shared_ptr<const PrimitivesProcessorPort>		ConstPrimitivesProcessorPortPtr;

			/**
			 * \brief single shot processor port class.
			 *
			 */
			class CORE_API PrimitivesProcessorPort : public Core::Processing::Object::Port, public Core::Processing::Interfaces::IPrimitivesProcessor
			{
				Q_OBJECT
				DECLARE_OBJECT_FULL(PrimitivesProcessorPort)

			public:
				/**
				 * \brief constructor.
				 *
				 */
				PrimitivesProcessorPort(QString uid = "");

				/**
				 * \brief copy constructor.
				 *
				 */
				PrimitivesProcessorPort(const PrimitivesProcessorPort& rhs);

				/**
				 * \brief copy constructor.
				 *
				 */
				PrimitivesProcessorPort(const PrimitivesProcessorPort& rhs, QString uid);

				MEMBER_SIGNAL_2(void, processBool, Core::Processing::Interfaces::ConstIObjectPtr, sender, const bool&, value);
				MEMBER_SIGNAL_2(void, processChar, Core::Processing::Interfaces::ConstIObjectPtr, sender, const char&, value);
				MEMBER_SIGNAL_2(void, processWChar, Core::Processing::Interfaces::ConstIObjectPtr, sender, const wchar_t&, value);
				MEMBER_SIGNAL_2(void, processSChar, Core::Processing::Interfaces::ConstIObjectPtr, sender, const signed char&, value);
				MEMBER_SIGNAL_2(void, processShort, Core::Processing::Interfaces::ConstIObjectPtr, sender, const short&, value);
				MEMBER_SIGNAL_2(void, processInt, Core::Processing::Interfaces::ConstIObjectPtr, sender, const int&, value);
				MEMBER_SIGNAL_2(void, processLong, Core::Processing::Interfaces::ConstIObjectPtr, sender, const long&, value);
				MEMBER_SIGNAL_2(void, processLongLong, Core::Processing::Interfaces::ConstIObjectPtr, sender, const long long&, value);
				MEMBER_SIGNAL_2(void, processUChar, Core::Processing::Interfaces::ConstIObjectPtr, sender, const unsigned char&, value);
				MEMBER_SIGNAL_2(void, processUShort, Core::Processing::Interfaces::ConstIObjectPtr, sender, const unsigned short&, value);
				MEMBER_SIGNAL_2(void, processUInt, Core::Processing::Interfaces::ConstIObjectPtr, sender, const unsigned int&, value);
				MEMBER_SIGNAL_2(void, processULong, Core::Processing::Interfaces::ConstIObjectPtr, sender, const unsigned long&, value);
				MEMBER_SIGNAL_2(void, processULongLong, Core::Processing::Interfaces::ConstIObjectPtr, sender, const unsigned long long&, value);
				MEMBER_SIGNAL_2(void, processFloat, Core::Processing::Interfaces::ConstIObjectPtr, sender, const float&, value);
				MEMBER_SIGNAL_2(void, processDouble, Core::Processing::Interfaces::ConstIObjectPtr, sender, const double&, value);
			};

		};	// namespace Ports
	};	// namespace Processing
}; //namespace Core
Q_DECLARE_METATYPE(Core::Processing::Ports::PrimitivesProcessorPort)

#endif	// __PRIMITIVESPROCESSORPORT_H__
