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

#include "Primitives/Triangle3.h"

namespace Geometry
{
namespace Primitives
{
	template<class T> Triangle3<T>::Triangle3()
	{
		_points.push_back(Point3<T>(0, 0, 0));
		_points.push_back(Point3<T>(0, 0, 0));
		_points.push_back(Point3<T>(0, 0, 0));
		_points.push_back(Point3<T>(0, 0, 0));
	}

	template<class T> Triangle3<T>::Triangle3( const Triangle3<T> &rhs )
	{
		internalCopyPoints(rhs);
	}

	template<class T> Triangle3<T>::Triangle3(const double x1, const double y1, const double z1, const double x2, const double y2, const double z2, const double x3, const double y3, const double z3)
	{
		_points.push_back(Point3<T>(x1, y1, z1));
		_points.push_back(Point3<T>(x2, y2, z2));
		_points.push_back(Point3<T>(x3, y3, z3));
	}

	template<class T> Triangle3<T>::~Triangle3()
	{
	}

	template<class T> Triangle3<T>& Triangle3<T>::operator=( const Triangle3<T> &rhs )
	{
		internalCopyPoints(rhs);
		return *this;
	}


	// First Point
	template<class T> double Triangle3<T>::getX1() const
	{
		return _points[0].x;
	}

	template<class T> void Triangle3<T>::setX1( double value )
	{
		_points[0].x = value;
	}

	template<class T> double Triangle3<T>::getY1() const
	{
		return _points[0].y;
	}

	template<class T> void Triangle3<T>::setY1( double value )
	{
		_points[0].y = value;
	}

	template<class T> double Triangle3<T>::getZ1() const
	{
		return _points[0].z;
	}

	template<class T> void Triangle3<T>::setZ1( double value )
	{
		_points[0].z = value;
	}

	// Second Point
	template<class T> double Triangle3<T>::getX2() const
	{
		return _points[1].x;
	}

	template<class T> void Triangle3<T>::setX2( double value )
	{
		_points[1].x = value;
	}

	template<class T> double Triangle3<T>::getY2() const
	{
		return _points[1].y;
	}

	template<class T> void Triangle3<T>::setY2( double value )
	{
		_points[1].y = value;
	}

	template<class T> double Triangle3<T>::getZ2() const
	{
		return _points[1].z;
	}

	template<class T> void Triangle3<T>::setZ2( double value )
	{
		_points[1].z = value;
	}

	// Third Point
	template<class T> double Triangle3<T>::getX3() const
	{
		return _points[2].x;
	}

	template<class T> void Triangle3<T>::setX3( double value )
	{
		_points[2].x = value;
	}

	template<class T> double Triangle3<T>::getY3() const
	{
		return _points[2].y;
	}

	template<class T> void Triangle3<T>::setY3( double value )
	{
		_points[2].y = value;
	}

	template<class T> double Triangle3<T>::getZ3() const
	{
		return _points[2].z;
	}

	template<class T> void Triangle3<T>::setZ3( double value )
	{
		_points[2].z = value;
	}

};//namespace Misc
};//namespace Core
