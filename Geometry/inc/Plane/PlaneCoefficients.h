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

#ifndef PLANECOEFFICIENTS_H
#define PLANECOEFFICIENTS_H

#include "Misc/GeometryDLL.h"
#include "boost/shared_ptr.hpp"

#include <vector>

namespace Geometry
{
	namespace Plane
	{
		/**
		 * \brief A structure to define a plane in the form ax + by + cz + d = 0; Plane normal: (a, b, c), distance from the origin: d
		 */
		struct GEOMETRY_API PlaneCoefficients
		{
			/**
			* \brief The default constructor
			*/
			PlaneCoefficients ();
			

			/**
			* \brief A constructor to directly pass the plane parameters
			*
			* \param  float a The a component of the normal
			* \param  float b The b component of the normal
			* \param  float c The c component of the normal
			* \param  float d Distance to the origin
			*/
			PlaneCoefficients (float a, float b, float c, float d);
		

			float values[4];							//!< The coefficients storage variable
		};

		typedef boost::shared_ptr<PlaneCoefficients> PlaneCoefficientsPtr;
		typedef boost::shared_ptr<const PlaneCoefficients> ConstPlaneCoefficientsPtr;
	}
}



#endif	// PLANECOEFFICIENTS_H
