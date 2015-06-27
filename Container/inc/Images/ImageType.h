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
#ifndef __IMAGETYPE_H__
#define __IMAGETYPE_H__
#include "ContainerDLL.h"

namespace Container
{
	namespace Images
	{
		enum class ImageType
		{
			UNDEFINED	= 0x0000,
			RAWDATA 	= 0x0001,
			RAWIMAGE 	= 0x0002,
			IMAGE 		= 0x0003,
			RGB			= 0x0004,
			RGBA		= 0x0005,
			ARGB		= 0x0006,
			USER 		= 0x1000
		};
	}; //namespace Images
}; //namespace Container

#endif // __IMAGETYPE_H__
