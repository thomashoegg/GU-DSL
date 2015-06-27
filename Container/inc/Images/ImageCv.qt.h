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
#ifndef __IMAGECV_H__
#define __IMAGECV_H__

#include "ContainerDLL.h"

#include <QObject>
#include "boost/shared_ptr.hpp"
#include "Images/Image.h"
#include "opencv2/opencv.hpp"


namespace Container
{
	namespace Images
	{
		class CONTAINER_API ImageCv : public Container::Images::Image
		{
		public:
			/**
			* \brief Constructor for ImageCv Class
			*/
			ImageCv();

			/**
			* \brief Constructor for ImageCv Class
			*
			* An ImageCv has an internal Thrift Frame, to help you get more information about the frame
			*
			* \param  Container::Images::FrameData frame The thrift frame data
			* \param  Container::Images::FrameHeader header The thrift frame header
			*/
			ImageCv(FrameData* data, FrameHeader* header);

			/**
			* \brief Constructor for ImageCv Class
			*
			* An ImageCv has an internal Thrift Frame, to help you get more information about the frame
			*
			* \param  Container::Images::Frame frame The thrift frame 
			*/
			ImageCv(Frame* frame, bool copy = true);

			/**
			* \brief Constructor for ImageCv Class
			*
			* An ImageCv has an internal Thrift Frame, to help you get more information about the frame
			*
			* \param  frameSize frameSize The size of frames to create.
			*/
			ImageCv(unsigned int frameSize);

			/**
				* \brief copy constructor from Image class
				*
				*/
			ImageCv(const Container::Images::Image& rhs);

			/**
				* \brief copy constructor
				*
				*/
			ImageCv(const ImageCv& rhs);

			/**
			* \brief Destructor for ImageCv Class

			*/
			virtual ~ImageCv();

			/**
			 * \brief get the open cv image.
			 *
			 *	\param unsigned int frameIndex.
			 *	\return open cv image.
			 */
			cv::Mat& getImageCv(unsigned int frameIndex);

			/**
			 * \brief get a open cv image copy.
			 *
			 *	\param unsigned int frameIndex.
			 *	\return open cv image copy.
			 */
			cv::Mat	copyImageCv(unsigned int frameIndex);

		protected:
			/**
			 * \brief fill the container with open cv images
			 *
			 */
			void fillCvFrames();

			/**
			 * \brief update cv image if memory location is not equal
			 *
			 *	\param unsigned int frameindex.
			 */
			void checkAndUpdate( unsigned int frameIndex );
		protected:
			std::vector<cv::Mat>			_cvFrames;
		};

	};	// namespace Images
}; //namespace Container
#endif	// __IMAGECV_H__
