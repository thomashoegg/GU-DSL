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

#include "Plane/PlaneCoefficients.h"


Geometry::Plane::PlaneCoefficients::PlaneCoefficients()
{
	for(int i = 0; i < 4; i++)
		values[i] = 0;
}

Geometry::Plane::PlaneCoefficients::PlaneCoefficients( float a, float b, float c, float d )
{
	values[0] = a;
	values[1] = b;
	values[2] = c;
	values[3] = d;
}
