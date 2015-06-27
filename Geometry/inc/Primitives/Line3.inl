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

#include "Primitives/Line3.h"

namespace Geometry
{
namespace Primitives
{
	template<class T> Line3<T>::Line3()
	{
		_points.push_back(Point3<T>(0, 0, 0));
		_points.push_back(Point3<T>(0, 0, 0));
	}

	template<class T> Line3<T>::Line3( const Line3<T> &rhs )
		: Polygon(rhs)
	{
	}

	template<class T> Line3<T>::Line3(const double x1, const double y1, const double z1, const double x2, const double y2, const double z2)
	{
		_points.push_back(Point3<T>(x1, y1, z1));
		_points.push_back(Point3<T>(x2, y2, z2));
	}

	template<class T> Line3<T>::~Line3()
	{

	}

	template <class T> Line3<T>& Line3<T>::operator=( const Line3<T> &rhs )
	{
		internalCopyPoints(rhs);
		return *this;
	}

	template<class T> double Line3<T>::getX1() const
	{
		return _points[0].x;
	}

	template<class T> void Line3<T>::setX1( double value )
	{
		_points[0].x = value;
	}

	template<class T> double Line3<T>::getY1() const
	{
		return _points[0].y;
	}

	template<class T> void Line3<T>::setY1( double value )
	{
		_points[0].y = value;
	}

	template<class T> double Line3<T>::getZ1() const
	{
		return _points[0].z;
	}

	template<class T> void Line3<T>::setZ1( double value )
	{
		_points[0].z = value;
	}

	template<class T> double Line3<T>::getX2() const
	{
		return _points[1].x;
	}

	template<class T> void Line3<T>::setX2( double value )
	{
		_points[1].x = value;
	}

	template<class T> double Line3<T>::getY2() const
	{
		return _points[1].y;
	}

	template<class T> void Line3<T>::setY2( double value )
	{
		_points[1].y = value;
	}


	template<class T> double Line3<T>::getZ2() const
	{
		return _points[1].z;
	}

	template<class T> void Line3<T>::setZ2( double value )
	{
		_points[1].z = value;
	}


};//namespace Misc
};//namespace Core
