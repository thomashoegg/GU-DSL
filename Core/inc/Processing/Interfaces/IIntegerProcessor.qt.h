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
#ifndef __IINTEGERPROCESSOR_H__
#define __IINTEGERPROCESSOR_H__

#include "CoreDLL.h"
#include "QObject"
#include "Processing/Interfaces/IProcessor.qt.h"
#include "Processing/PtrDefinitions.h"
#include "boost/shared_ptr.hpp"

namespace Container
{
	namespace Images
	{
		class Image;
	}
}


namespace Core
{
	namespace Processing
	{
		namespace Interfaces
		{

			class CORE_API IIntegerProcessor : public Core::Processing::Interfaces::IProcessor
			{
				Q_OBJECT

			public:

				IIntegerProcessor() {}

				virtual void processInteger(ConstIObjectPtr sender, const int& value) = 0;
			};
		}	// namespace Interfaces
	}	// namespace Processing
}; //namespace 

#endif	// __IINTEGERPROCESSOR_H__
