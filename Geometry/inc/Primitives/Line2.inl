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

#include "Primitives/Line2.h"

namespace Geometry
{
namespace Primitives
{
	template<class T> Line2<T>::Line2()
	{
		_points.push_back(Point3<T>(0, 0, 0));
		_points.push_back(Point3<T>(0, 0, 0));
	}

	template <class T> Line2<T>::Line2( const double x1, const double y1, const double x2, const double y2 )
	{
		_points.push_back(Point3<T>(x1, y1, 0));
		_points.push_back(Point3<T>(x2, y2, 0));
	}

	template<class T> Line2<T>::Line2( const Line2<T> &rhs )
	{
		internalCopyPoints(rhs);
	}

	template<class T> Line2<T>::~Line2()
	{
	}

	template <class T> Line2<T>& Line2<T>::operator=( const Line2<T> &rhs )
	{
		internalCopyPoints(rhs);
		return *this;
	}


	template<class T> double Line2<T>::getX1() const
	{
		return _points[0].x;
	}
	template<class T> void Line2<T>::setX1( double value )
	{
		_points[0].x = value;
	}

	template<class T> double Line2<T>::getY1() const
	{
		return _points[0].y;
	}

	template<class T> void Line2<T>::setY1( double value )
	{
		_points[0].y = value;
	}

	template<class T> double Line2<T>::getX2() const
	{
		return _points[1].x;
	}

	template<class T> void Line2<T>::setX2( double value )
	{
		_points[1].x = value;
	}

	template<class T> double Line2<T>::getY2() const
	{
		return _points[1].y;
	}

	template<class T> void Line2<T>::setY2( double value )
	{
		_points[1].y = value;
	}

};//namespace Misc
};//namespace Core
