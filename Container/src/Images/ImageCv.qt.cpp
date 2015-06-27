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

#include "Images/ImageCv.qt.h"
#include "Utilities/OpenCvHelper.h"

namespace Container
{
namespace Images
{
	ImageCv::ImageCv()
		: Image()
	{
		fillCvFrames();
	}

	ImageCv::ImageCv( FrameData* data, FrameHeader* header )
		: Image(data, header)
	{
		fillCvFrames();
	}

	ImageCv::ImageCv( Frame* frame, bool copy /*= true*/ )
		: Image(frame, copy)
	{
		fillCvFrames();
	}

	ImageCv::ImageCv( unsigned int frameSize )
		: Image(frameSize)
	{
		fillCvFrames();
	}

	ImageCv::ImageCv( const Container::Images::Image& rhs )
		:Image(rhs)
	{
		fillCvFrames();
	}

	ImageCv::ImageCv( const ImageCv& rhs )
		:Image(rhs)
	{
		fillCvFrames();
	}

	ImageCv::~ImageCv()
	{

	}

	cv::Mat& ImageCv::getImageCv( unsigned int frameIndex )
	{
		checkAndUpdate(frameIndex);
		return _cvFrames[frameIndex];
	}

	cv::Mat ImageCv::copyImageCv( unsigned int frameIndex )
	{
		checkAndUpdate(frameIndex);
		return _cvFrames[frameIndex].clone();
	}

	void ImageCv::fillCvFrames()
	{
		_cvFrames.clear();
		for(unsigned int i = 0; i < frameIndexSize(); i++)
		{
			cv::Mat frame;
			Container::Utilities::OpenCvHelper::createCvMat(this, frame, i);
			_cvFrames.push_back(frame);
		}
	}

	void ImageCv::checkAndUpdate( unsigned int frameIndex )
	{
		if(frameIndex < frameIndexSize())
		{
			if((uchar*)buffer(frameIndex) != _cvFrames[frameIndex].data)
			{
				cv::Mat frame;
				Container::Utilities::OpenCvHelper::createCvMat(this, frame, frameIndex);
				_cvFrames[frameIndex] = frame;
			}
			else if(memcmp((uchar*)buffer(frameIndex), _cvFrames[frameIndex].data, size(frameIndex)) != 0)
			{
				fillCvFrames();
			}
		}
		else
		{
			fillCvFrames();
		}
	}
};
};
