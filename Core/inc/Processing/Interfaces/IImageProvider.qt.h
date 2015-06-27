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

#ifndef PROCESSING_IIMAGEPROVIDER_H
#define PROCESSING_IIMAGEPROVIDER_H

#ifdef WIN32
#pragma once
#endif

#include "CoreDLL.h"

#include "QObject"
#include "Processing/Interfaces/IProvider.qt.h"
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

			class CORE_API IImageProvider : public IProvider
			{
				Q_OBJECT

			public:

				IImageProvider() {}

				virtual void provideImage(ConstIObjectPtr sender, const Container::Images::Image* image) = 0;
				virtual void provideShowImage(ConstIObjectPtr sender, const Container::Images::Image* image, int index) = 0;
			};

		};	// namespace Interfaces
	};	// namespace Processing
}; //namespace Core
#endif	// PROCESSING_IIMAGEPROVIDER_H
