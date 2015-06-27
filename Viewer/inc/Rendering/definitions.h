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

#ifndef COMMONBASICS_DEFINITION_H
#define COMMONBASICS_DEFINITION_H

#if WIN32
#pragma once;
#endif

namespace Viewer
{
	namespace Rendering
	{


		namespace buffer
		{
			enum type
			{
				VERTEX,
				VERTEX_EXTERNAL,
				ELEMENT,
				TRANSFORM_MODEL,
				TRANSFORM_VIEW,
				TRANSFORM_PROJECTION,
				MAX
			};
		}//namespace buffer

		namespace semantic
		{
			namespace uniform
			{
				enum type
				{
					MATERIAL  = 0,
					TRANSFORM_MODEL = 1,
					TRANSFORM_VIEW = 2,
					TRANSFORM_PROJECTION = 3,
				};
			};

			namespace image
			{
				enum type
				{
					DIFFUSE = 0
				};
			}//namesapce image

			namespace attr
			{
				enum type
				{
					POSITION = 0,
					NORMAL	 = 3,
					COLOR	 = 6,
					TEXCOORD = 7
				};
			}//namespace attr

			namespace vert
			{
				enum type
				{
					POSITION = 0,
					NORMAL	 = 3,
					COLOR	 = 6,
					TEXCOORD = 7,
					INSTANCE = 10
				};
			}//namespace vert

			namespace frag
			{
				enum type
				{
					COLOR	= 0,
					RED		= 0,
					GREEN	= 1,
					BLUE	= 2,
					ALPHA	= 0
				};
			}//namespace frag

			namespace renderbuffer
			{
				enum type
				{
					DEPTH,
					COLOR0
				};
			}//namespace renderbuffer

		} //namespace semantic
	} // namespace Rendering
} // namespace Viewer

#endif	// COMMONBASICS_DEFINITION_H
