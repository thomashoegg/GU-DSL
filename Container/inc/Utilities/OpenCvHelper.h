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


#pragma once;
#ifndef OPENCVHELPER_H
#define OPENCVHELPER_H

#include "ContainerDLL.h"

namespace cv
{
	class Mat;
};

namespace Container
{
	namespace Images
	{
		class Image;
	};

	namespace Utilities
	{
		/**
		 * \brief open cv helper class.
		 *
		 */
		class CONTAINER_API OpenCvHelper
		{
		public:
			/**
			 * \brief create new open cv matrix.
			 *
			 *	\param image to create matrix from.
			 *	\param outMatrix. open cv matrix.
			 *	\param frameIndex.
			 */
			static void createCvMat(const Container::Images::Image* image, cv::Mat& outMatrix, unsigned int frameIndex);
		
		private:
			/**
			 * \brief get cv data type
			 *
			 *	\return open cv data type
			 */
			static int getCvDatatype(const Container::Images::Image* image, unsigned int frameIndex);

			/**
			 * \brief get image channels
			 *
			 *	\return number of image channels
			 */
			static int getImageChannels(const Container::Images::Image* image, unsigned int frameIndex);
		};

	};
};

#endif	// OPENCVHELPER_H
