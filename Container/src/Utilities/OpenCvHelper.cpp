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

#include "Utilities/OpenCvHelper.h"
#include <opencv2/opencv.hpp>
#include <Images/Image.h>

namespace Container
{
	namespace Utilities
	{
		
		void OpenCvHelper::createCvMat( const Container::Images::Image* image, cv::Mat& outMatrix, unsigned int frameIndex )
		{
			int channels = getImageChannels(image, frameIndex);
			int datatype = getCvDatatype(image, frameIndex);
			int cvType = CV_MAKETYPE(datatype, channels);

			outMatrix = cv::Mat(image->height(frameIndex), image->width(frameIndex), cvType, image->buffer(frameIndex));
		}

		int OpenCvHelper::getImageChannels(const Container::Images::Image* image, unsigned int frameIndex)
		{
			 int channels = 1;
			/*
			image, rawdata, rawimage and user image type should have 1 channel

			Container::Images::ImageType::IMAGE
			Container::Images::ImageType::RAWDATA
			Container::Images::ImageType::RAWIMAGE
			Container::Images::ImageType::USER
			*/

			if(image->imageType(frameIndex) == Container::Images::ImageType::RGB)
			{
				channels = 3;
			}
			else if(image->imageType(frameIndex) == Container::Images::ImageType::RGBA)
			{
				channels = 4;
			}
			else if(image->imageType(frameIndex) == Container::Images::ImageType::ARGB)
			{
				channels = 4;
			}
			return channels;
		}

		int OpenCvHelper::getCvDatatype(const Container::Images::Image* image, unsigned int frameIndex)
		{
			int datatype = 0;
			switch (image->dataType(frameIndex))
			{
			case Container::Images::DataType::CHAR:
				datatype = CV_8S;
				break;
			case Container::Images::DataType::UCHAR:
				datatype = CV_8U;
				break;
			case Container::Images::DataType::DOUBLE:
				datatype = CV_64F;
				break;
			case Container::Images::DataType::FLOAT:
				datatype = CV_32F;
				break;
			case Container::Images::DataType::INT:
				datatype = CV_32S;
				break;
			case Container::Images::DataType::UINT:
				datatype = CV_32S;
				break;
			case Container::Images::DataType::SHORT:
				datatype = CV_16S;
				break;
			case Container::Images::DataType::USHORT:
				datatype = CV_16U;
				break;
			default:
				assert(false && "Could not create input matrix. Unknown datatype");
				break;
			}
			return datatype;
		}

	};
};
