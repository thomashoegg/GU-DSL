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

#ifndef COMMONBASICS_VERTEXFORMAT_H
#define COMMONBASICS_VERTEXFORMAT_H

#if WIN32
#pragma once;
#endif

#include "ViewerDLL.h"
#include <pshpack8.h>


namespace Viewer
{
	namespace Rendering
	{
		namespace VERTEX_FORMAT
		{
			enum Format
			{
				V1F,
				V1I,
				V1UI,
				V2F,
				V3F,
				V4F,
				V2F_C4,
				V3F_C4,
				V2F_T2,
				V3F_T2,
				V2F_T2_C4,
				V3F_T2_C4,
				V2F_N2F,
				V3F_N3F,
				V4F_N4F,
				V2F_N2F_C4,
				V3F_N3F_C4,
				V2F_N2F_T2,
				V3F_N3F_T2,
				V2F_N2F_T2_C4,
				V3F_N3F_T2_C4
			};
		}

	} // namespace Rendering
} // namespace Basics

#endif	// COMMONBASICS_VERTEXFORMAT_H
