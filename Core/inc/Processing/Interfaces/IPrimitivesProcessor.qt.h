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
#ifndef __IPRIMITIVESPROCESSOR_H__
#define __IPRIMITIVESPROCESSOR_H__

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

			class CORE_API IPrimitivesProcessor : public Core::Processing::Interfaces::IProcessor
			{
				Q_OBJECT

			public:

				IPrimitivesProcessor() {}

				virtual void processBool(ConstIObjectPtr sender, const bool& value) = 0;
				virtual void processChar(ConstIObjectPtr sender, const char& value) = 0;
				virtual void processWChar(ConstIObjectPtr sender, const wchar_t& value) = 0;
				virtual void processSChar(ConstIObjectPtr sender, const signed char& value) = 0;
				virtual void processShort(ConstIObjectPtr sender, const short& value) = 0;
				virtual void processInt(ConstIObjectPtr sender, const int& value) = 0;
				virtual void processLong(ConstIObjectPtr sender, const long& value) = 0;
				virtual void processLongLong(ConstIObjectPtr sender, const long long& value) = 0;
				virtual void processUChar(ConstIObjectPtr sender, const unsigned char& value) = 0;
				virtual void processUShort(ConstIObjectPtr sender, const unsigned short& value) = 0;
				virtual void processUInt(ConstIObjectPtr sender, const unsigned int& value) = 0;
				virtual void processULong(ConstIObjectPtr sender, const unsigned long& value) = 0;
				virtual void processULongLong(ConstIObjectPtr sender, const unsigned long long& value) = 0;
				virtual void processFloat(ConstIObjectPtr sender, const float& value) = 0;
				virtual void processDouble(ConstIObjectPtr sender, const double& value) = 0;
			};
		}	// namespace Interfaces
	}	// namespace Processing
}; //namespace 

#endif	// __IPRIMITIVESPROCESSOR_H__
