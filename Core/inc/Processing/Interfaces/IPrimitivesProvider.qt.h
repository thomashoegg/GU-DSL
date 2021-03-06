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
#ifndef __IPRIMITIVESPROVIDER_H__
#define __IPRIMITIVESPROVIDER_H__

#include "CoreDLL.h"

#include "QObject"
#include "Processing/Interfaces/IProvider.qt.h"
#include "Processing/PtrDefinitions.h"

#include "boost/shared_ptr.hpp"

namespace Core
{
	namespace Processing
	{
		namespace Interfaces
		{

			class CORE_API IPrimitivesProvider : public IProvider
			{
				Q_OBJECT

			public:

				IPrimitivesProvider() {}

				virtual void provideBool(ConstIObjectPtr sender, const bool& value) = 0;
				virtual void provideChar(ConstIObjectPtr sender, const char& value) = 0;
				virtual void provideWChar(ConstIObjectPtr sender, const wchar_t& value) = 0;
				virtual void provideSChar(ConstIObjectPtr sender, const signed char& value) = 0;
				virtual void provideShort(ConstIObjectPtr sender, const short& value) = 0;
				virtual void provideInt(ConstIObjectPtr sender, const int& value) = 0;
				virtual void provideLong(ConstIObjectPtr sender, const long& value) = 0;
				virtual void provideLongLong(ConstIObjectPtr sender, const long long& value) = 0;
				virtual void provideUChar(ConstIObjectPtr sender, const unsigned char& value) = 0;
				virtual void provideUShort(ConstIObjectPtr sender, const unsigned short& value) = 0;
				virtual void provideUInt(ConstIObjectPtr sender, const unsigned int& value) = 0;
				virtual void provideULong(ConstIObjectPtr sender, const unsigned long& value) = 0;
				virtual void provideULongLong(ConstIObjectPtr sender, const unsigned long long& value) = 0;
				virtual void provideFloat(ConstIObjectPtr sender, const float& value) = 0;
				virtual void provideDouble(ConstIObjectPtr sender, const double& value) = 0;
			};

		};	// namespace Interfaces
	};	// namespace Processing
}; //namespace Core
#endif	// __IPRIMITIVESPROVIDER_H__
