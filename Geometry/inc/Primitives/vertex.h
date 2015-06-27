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

#ifndef GEOMETRY_VERTEXFORMAT_H
#define GEOMETRY_VERTEXFORMAT_H

#if WIN32
#pragma once;
#endif

#include "Misc/GeometryDLL.h"
#include <pshpack8.h>

namespace Geometry
{
	namespace Primitives
	{
		struct GEOMETRY_API vec2
		{
			vec2()
			{
				d0 = d1 = 0;
			}

			vec2(float x, float y)
			{
				this->d0 = x;
				this->d1 = y;
			}

			union
			{
				float x;
				float r;
				float d0;
			};

			union
			{
				float y;
				float g;
				float d1;
			};

		};

		struct GEOMETRY_API vec3
		{
			vec3()
			{
				d0 = d1 = d2 = 0;
			}

			vec3(float x, float y, float z)
			{
				this->d0 = x;
				this->d1 = y;
				this->d2 = z;
			}

			union
			{
				float x;
				float r;
				float d0;
			};

			union
			{
				float y;
				float g;
				float d1;
			};

			union
			{
				float z;
				float b;
				float d2;
			};
		};


		struct GEOMETRY_API vec4
		{
			vec4()
			{
				d0 = d1 = d2 = d3 = 0;
			}

			vec4(float x, float y, float z, float w)
			{
				this->d0 = x;
				this->d1 = y;
				this->d2 = z;
				this->d3 = w;
			}

			union
			{
				float x;
				float r;
				float d0;
			};

			union
			{
				float y;
				float g;
				float d1;
			};

			union
			{
				float z;
				float b;
				float d2;
			};


			union
			{
				float w;
				float a;
				float d3;
			};
		};

		struct GEOMETRY_API vec6
		{
			vec6()
			{
				d0 = d1 = d2 = d3 = d4 = d5 = 0;
			}

			vec6(float d0, float d1, float d2, float d3, float d4, float d5)
			{
				this->d0 = d0;
				this->d1 = d1;
				this->d2 = d2;
				this->d3 = d3;
				this->d4 = d4;
				this->d5 = d5;
			}

			union
			{
				float x;
				float r;
				float d0;
			};

			union
			{
				float y;
				float g;
				float d1;
			};

			union
			{
				float z;
				float b;
				float d2;
			};


			union
			{
				float w;
				float a;
				float d3;
			};

			union
			{
				float d4;
			};

			union
			{
				float d5;
			};

		};

		struct GEOMETRY_API vec8
		{
			vec8()
			{
				d0 = d1 = d2 = d3 = d4 = d5 = d6 = d7 = 0;
			}

			vec8(float d0, float d1, float d2, float d3, float d4, float d5, float d6, float d7)
			{
				this->d0 = d0;
				this->d1 = d1;
				this->d2 = d2;
				this->d3 = d3;
				this->d4 = d4;
				this->d5 = d5;
				this->d6 = d6;
				this->d7 = d7;
			}

			union
			{
				float x;
				float r;
				float d0;
			};

			union
			{
				float y;
				float g;
				float d1;
			};

			union
			{
				float z;
				float b;
				float d2;
			};


			union
			{
				float w;
				float a;
				float d3;
			};

			union
			{
				float d4;
			};

			union
			{
				float d5;
			};

			union
			{
				float d6;
			};

			union
			{
				float d7;
			};
		};

		/**
		 * \brief Vertex structure for the format V2F
		 */
		struct GEOMETRY_API vertex_v2
		{
			vertex_v2() {}

			vertex_v2(vec2 const & Position) :
				Position(Position)
			{}

			vec2 Position;
		};

		/**
		 * \brief Vertex structure for the format V3F
		 */
		struct GEOMETRY_API vertex_v3
		{
			vertex_v3() {}

			vertex_v3(vec3 const & Position) :
				Position(Position)
			{}

			vec3 Position;
		};
		
		/**
		 * \brief Vertex structure for the format V4F
		 */
		struct GEOMETRY_API vertex_v4
		{
			vertex_v4() {}

			vertex_v4(vec4 const & Position) :
				Position(Position)
			{}

			vec4 Position;
		};

		/**
		 * \brief Vertex structure for the format V2F_C4
		 */
		struct GEOMETRY_API vertex_v2c4
		{
			vertex_v2c4() {}

			vertex_v2c4(vec2 const & Position, vec4 const & Color) :
				Position(Position),
				Color(Color)
			{}

			vec2 Position;
			vec4 Color;
		};

		/**
		 * \brief Vertex structure for the format V3F_C4
		 */
		struct GEOMETRY_API vertex_v3c4
		{
			vertex_v3c4() {}

			vertex_v3c4(vec3 const & Position, vec4 const & Color) :
				Position(Position),
				Color(Color)
			{}

			vec3 Position;
			vec4 Color;
		};

		/**
		 * \brief Vertex structure for the format V2F_T2
		 */
		struct GEOMETRY_API vertex_v2t2
		{
			vertex_v2t2() {}

			vertex_v2t2(vec2 const & Position, vec2 const & Texcoord) :
				Position(Position),
				Texcoord(Texcoord)
			{}

			vec2 Position;
			vec2 Texcoord;
		};

		/**
		 * \brief Vertex structure for the format V3F_T2
		 */
		struct GEOMETRY_API vertex_v3t2
		{
			vertex_v3t2() {}

			vertex_v3t2(vec3 const & Position, vec2 const & Texcoord) :
				Position(Position),
				Texcoord(Texcoord)
			{}

			vec3 Position;
			vec2 Texcoord;
		};

		/**
		 * \brief Vertex structure for the format V2F_T2_C4
		 */
		struct GEOMETRY_API vertex_v2t2c4
		{
			vertex_v2t2c4() {}

			vertex_v2t2c4(vec2 const & Position, vec2 const & Texcoord, vec4 const & Color) :
				Position(Position),
				Texcoord(Texcoord),
				Color(Color)
			{}

			vec2 Position;
			vec2 Texcoord;
			vec4 Color;
		};

		/**
		 * \brief Vertex structure for the format V3F_T2_C4
		 */
		struct GEOMETRY_API vertex_v3t2c4
		{
			vertex_v3t2c4() {}

			vertex_v3t2c4(vec3 const & Position, vec2 const & Texcoord, vec4 const & Color) :
				Position(Position),
				Texcoord(Texcoord),
				Color(Color)
			{}

			vec3 Position;
			vec2 Texcoord;
			vec4 Color;
		};

		/**
		 * \brief Vertex structure for the format V2F_N2F
		 */
		struct GEOMETRY_API vertex_v2n2
		{
			vertex_v2n2() {}

			vertex_v2n2(vec2 const & Position, vec2 const & Normal) :
				Position(Position),
				Normal(Normal)
			{}

			vec2 Position;
			vec2 Normal;
		};
		
		/**
		 * \brief Vertex structure for the format V3F_N3F
		 */
		struct GEOMETRY_API vertex_v3n3
		{
			vertex_v3n3() {}

			vertex_v3n3(vec3 const & Position, vec3 const & Normal) :
				Position(Position),
				Normal(Normal)
			{}

			vec3 Position;
			vec3 Normal;
		};

		/**
		 * \brief Vertex structure for the format V4F_N4F
		 */
		struct GEOMETRY_API vertex_v4n4
		{
			vertex_v4n4() {}

			vertex_v4n4(vec4 const & Position, vec4 const & Normal) :
				Position(Position),
				Normal(Normal)
			{}

			vec4 Position;
			vec4 Normal;
		};

		/**
		 * \brief Vertex structure for the format V2F_N2F_C4
		 */
		struct GEOMETRY_API vertex_v2n2c4
		{
			vertex_v2n2c4() {}

			vertex_v2n2c4(vec2 const & Position, vec2 const & Normal, vec4 const & Color) :
				Position(Position),
				Normal(Normal),
				Color(Color)
			{}

			vec2 Position;
			vec2 Normal;
			vec4 Color;
		};

		/**
		 * \brief Vertex structure for the format V3F_N3F_C4
		 */
		struct GEOMETRY_API vertex_v3n3c4
		{
			vertex_v3n3c4() {}

			vertex_v3n3c4(vec3 const & Position, vec3 const & Normal, vec4 const & Color) :
				Position(Position),
				Normal(Normal),
				Color(Color)
			{}

			vec3 Position;
			vec3 Normal;
			vec4 Color;
		};

		/**
		 * \brief Vertex structure for the format V2F_N2F_T2
		 */
		struct GEOMETRY_API vertex_v2n2t2
		{
			vertex_v2n2t2() {}

			vertex_v2n2t2(vec2 const & Position, vec2 const & Normal, vec2 const & Texcoord) :
				Position(Position),
				Normal(Normal),
				Texcoord(Texcoord)
			{}

			vec2 Position;
			vec2 Normal;
			vec2 Texcoord;
		};

		/**
		 * \brief Vertex structure for the format V3F_N3F_T2
		 */
		struct GEOMETRY_API vertex_v3n3t2
		{
			vertex_v3n3t2() {}

			vertex_v3n3t2(vec3 const & Position, vec3 const & Normal, vec2 const & Texcoord) :
				Position(Position),
				Normal(Normal),
				Texcoord(Texcoord)
			{}

			vec3 Position;
			vec3 Normal;
			vec2 Texcoord;
		};

		/**
		 * \brief Vertex structure for the format V2F_N2F_T2_C4
		 */
		struct GEOMETRY_API vertex_v2n2t2c4
		{
			vertex_v2n2t2c4() {}

			vertex_v2n2t2c4(vec2 const & Position, vec2 const & Normal, vec2 const & Texcoord) :
				Position(Position),
				Normal(Normal),
				Texcoord(Texcoord),
				Color(Color)
			{}

			vec2 Position;
			vec2 Normal;
			vec2 Texcoord;
			vec4 Color;
		};

		/**
		 * \brief Vertex structure for the format V3F_N3F_T2_C4
		 */
		struct GEOMETRY_API vertex_v3n3t2c4
		{
			vertex_v3n3t2c4() {}

			vertex_v3n3t2c4(vec3 const & Position, vec3 const & Normal, vec2 const & Texcoord, vec4 const & Color) :
				Position(Position),
				Normal(Normal),
				Texcoord(Texcoord),
				Color(Color)
			{}

			vec3 Position;
			vec3 Normal;
			vec2 Texcoord;
			vec4 Color;
		};
	

	} // namespace Primitives
} // namespace Geometry

#endif	// GEOMETRY_VERTEXFORMAT_H
