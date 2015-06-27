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
#ifndef __IPRIMITIVESARRAYPROCESSOR_H__
#define __IPRIMITIVESARRAYPROCESSOR_H__

#include "CoreDLL.h"
#include "QObject"
#include "Processing/Interfaces/IProcessor.qt.h"
#include "Processing/PtrDefinitions.h"
#include "boost/shared_ptr.hpp"

namespace Core
{
	namespace Processing
	{
		namespace Interfaces
		{

			class CORE_API IPrimitivesArrayProcessor : public Core::Processing::Interfaces::IProcessor
			{
				Q_OBJECT

			public:
				IPrimitivesArrayProcessor() {}

				virtual void processBool(ConstIObjectPtr sender, const bool* value, size_t arrayLength) = 0;
				virtual void processChar(ConstIObjectPtr sender, const char* value, size_t arrayLength) = 0;
				virtual void processWChar(ConstIObjectPtr sender, const wchar_t* value, size_t arrayLength) = 0;
				virtual void processSChar(ConstIObjectPtr sender, const signed char* value, size_t arrayLength) = 0;
				virtual void processShort(ConstIObjectPtr sender, const short* value, size_t arrayLength) = 0;
				virtual void processInt(ConstIObjectPtr sender, const int* value, size_t arrayLength) = 0;
				virtual void processLong(ConstIObjectPtr sender, const long* value, size_t arrayLength) = 0;
				virtual void processLongLong(ConstIObjectPtr sender, const long long* value, size_t arrayLength) = 0;
				virtual void processUChar(ConstIObjectPtr sender, const unsigned char* value, size_t arrayLength) = 0;
				virtual void processUShort(ConstIObjectPtr sender, const unsigned short* value, size_t arrayLength) = 0;
				virtual void processUInt(ConstIObjectPtr sender, const unsigned int* value, size_t arrayLength) = 0;
				virtual void processULong(ConstIObjectPtr sender, const unsigned long* value, size_t arrayLength) = 0;
				virtual void processULongLong(ConstIObjectPtr sender, const unsigned long long* value, size_t arrayLength) = 0;
				virtual void processFloat(ConstIObjectPtr sender, const float* value, size_t arrayLength) = 0;
				virtual void processDouble(ConstIObjectPtr sender, const double* value, size_t arrayLength) = 0;
			};
		}	// namespace Interfaces
	}	// namespace Processing
}; //namespace 

#endif	// __IPRIMITIVESARRAYPROCESSOR_H__
