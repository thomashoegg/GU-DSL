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
#ifndef __FRAME_H__
#define __FRAME_H__
#include "ContainerDLL.h"
#include <vector>
#include "Images/FrameData.h"
#include "Images/FrameHeader.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace Container
{
	namespace Images
	{
		class CONTAINER_API Frame
		{
		public:
			std::vector<FrameHeader>	header;
			std::vector<FrameData>		data;
		public:
			Frame();

			virtual ~Frame() throw();
				
			bool operator == (const Frame & rhs) const;
			bool operator != (const Frame &rhs) const;

			bool operator < (const Frame & ) const;
		};
	}; //namespace Images
}; //namespace Container

#pragma warning(pop)
#endif // __FRAME_H__
