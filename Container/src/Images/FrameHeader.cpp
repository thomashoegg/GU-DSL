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

#include "Images/FrameHeader.h"

namespace Container
{
namespace Images
{
	FrameHeader::FrameHeader()
		: width(0),
		height(0),
		layerCount(1),
		rangeMin(0),
		rangeMax(65535),
		pixelData(PixelData::UNDEFINED),
		dataType(DataType::FLOAT),
		coordinateType(CoordinateType::UNDEFINED),
		imageType(ImageType::UNDEFINED),
		cameraId(),
		versionMajor(currentFrameVersionMajor),
		versionMinor(currentFrameVersionMinor)
	{

	}

	FrameHeader::~FrameHeader()
	{

	}

	bool FrameHeader::operator!=( const FrameHeader &rhs ) const
	{
		return !(*this == rhs);
	}

	bool FrameHeader::operator==( const FrameHeader & rhs ) const
	{
		if (!(width == rhs.width))
			return false;
		if (!(height == rhs.height))
			return false;
		if (!(layerCount == rhs.layerCount))
			return false;
		if (!(rangeMin == rhs.rangeMin))
			return false;
		if (!(rangeMax == rhs.rangeMax))
			return false;
		if (!(pixelData == rhs.pixelData))
			return false;
		if (!(cameraParameters == rhs.cameraParameters))
			return false;
		if (!(dataType == rhs.dataType))
			return false;
		if (!(coordinateType == rhs.coordinateType))
			return false;
		if (!(imageType == rhs.imageType))
			return false;
		if (!(cameraId == rhs.cameraId))
			return false;
		if( !(versionMajor == rhs.versionMajor))
			return false;
		if( !(versionMinor == rhs.versionMinor))
			return false;
		return true;
	}

}; //namespace Images
}; //namespace Container
