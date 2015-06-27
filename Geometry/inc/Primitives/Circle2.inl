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

#include "Primitives/Circle2.h"

namespace Geometry
{
namespace Primitives
{
	template<class T> Circle2<T>::Circle2()
	{
		_points.push_back(Point3<T>(0, 0, 0));
	}

	template<class T> Circle2<T>::Circle2( const Circle2<T> &rhs )
	{
		internalCopyPoints(rhs);
	}

	template<class T> Circle2<T>::Circle2( const double x, const double y, const double radius)
	{
		_x = x;
		_y = y;
		_radius = radius;
		constructCircle(x, y, radius);
	}

	template<class T> Circle2<T>::~Circle2()
	{

	}

	template <class T> void Circle2<T>::constructCircle( const double x, const double y, const double radius )
	{
		_points.clear();

		QMatrix4x4 m;

		QVector3D circlePos(radius, 0, 0);
		m.rotate(1, 0, 0, 1);

		_points.push_back(Point3f(x + circlePos.x(), y + circlePos.y(), 0));
		for(int i = 0; i < 359; i++)
		{
			circlePos = m * circlePos;
			_points.push_back(Point3f(x + circlePos.x(), y + circlePos.y(), 0));
		}
	}



	template<class T> double Circle2<T>::getCenterX() const
	{
		return _x;
	}

	template<class T> void Circle2<T>::setCenterX( double value )
	{
		_x = value;
	}

	template<class T> double Circle2<T>::getCenterY() const
	{
		return _y;
	}

	template<class T> void Circle2<T>::setCenterY( double value )
	{
		_y = value;
	}

	template<class T> double Circle2<T>::getRadius() const
	{
		return _radius;
	}

	template<class T> void Circle2<T>::setRadius( double value )
	{
		_radius = value;
		constructCircle(_x, _y, _radius);
	}


	template<class T> Circle2<T>& Circle2<T>::operator=( const Circle2<T> &rhs )
	{
		internalCopyPoints(rhs);
		return *this;
	}

};//namespace Primitives
};//namespace Geometry
