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

#ifndef PROCESSING_IMAGEPROCESSOR_H
#define PROCESSING_IMAGEPROCESSOR_H

#ifdef WIN32
#pragma once
#endif

#include "CoreDLL.h"

#include "QObject"
#include "Processing/Object/Port.qt.h"
#include "Processing/Interfaces/IImageProcessor.qt.h"

#include "boost/shared_ptr.hpp"

namespace Container
{
	namespace Images
	{
		class Image;
	};
};

namespace Core
{
	namespace Processing
	{
		namespace Ports
		{
			class CORE_API ImageProcessorPort : public Core::Processing::Object::Port, public Core::Processing::Interfaces::IImageProcessor
			{
				Q_OBJECT
				DECLARE_OBJECT_FULL(ImageProcessorPort)

			public:

				ImageProcessorPort(QString uid = "");
				ImageProcessorPort(const ImageProcessorPort& rhs);
				ImageProcessorPort(const ImageProcessorPort& rhs, QString uid);

				MEMBER_SIGNAL_2(void, processImage, Core::Processing::Interfaces::ConstIObjectPtr, sender, const Container::Images::Image*, image);
				MEMBER_SIGNAL_3(void, processShowImage, Core::Processing::Interfaces::ConstIObjectPtr, sender, const Container::Images::Image*, image, int, index);
			};

		};	// namespace Ports
	};	// namespace Processing
}; //namespace Core
Q_DECLARE_METATYPE(Core::Processing::Ports::ImageProcessorPort)

#endif	// PROCESSING_IMAGEPROCESSOR_H
