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

#include "Primitives/Rect2.h"

namespace Geometry
{
namespace Primitives
{
	template<class T> Rect2<T>::Rect2()
	{
		_points.push_back(Point3<T>(0, 0, 0));
		_points.push_back(Point3<T>(0, 0, 0));
		_points.push_back(Point3<T>(0, 0, 0));
		_points.push_back(Point3<T>(0, 0, 0));
	}

	template<class T> Rect2<T>::Rect2( const Rect2<T> &rhs )
	{
		internalCopyPoints(rhs);
	}

	template<class T> Rect2<T>::Rect2( const double x, const double y, const double width, const double height )
	{
		_points.push_back(Point3<T>(x,			y,			0));
		_points.push_back(Point3<T>(x + width,	y,			0));
		_points.push_back(Point3<T>(x + width,	y + height, 0));
		_points.push_back(Point3<T>(x,			y + height, 0));
	}

	template<class T> Rect2<T>::~Rect2()
	{

	}

	template<class T> Rect2<T>& Rect2<T>::operator=( const Rect2<T> &rhs )
	{
		internalCopyPoints(rhs);
		return *this;
	}

	template<class T> Rect2<T>& Rect2<T>::operator=( const QRect &rhs )
	{
		_points.clear();
		_points.push_back(Point3<T>(rhs.x(),				rhs.y(),					0));
		_points.push_back(Point3<T>(rhs.x() + rhs.width(),	rhs.y(),					0));
		_points.push_back(Point3<T>(rhs.x() + rhs.width(),	rhs.y() + rhs.height(),		0));
		_points.push_back(Point3<T>(rhs.x(),				rhs.y() + rhs.height(),		0));

		return *this;
	}

	template<class T> QRect Rect2<T>::toQRect() const
	{
		return QRect(getX(), getY(), getWidth(), getHeight());
	}

	template<class T> double Rect2<T>::getX() const
	{
		return _points[0].x;
	}

	template<class T> void Rect2<T>::setX( double value )
	{
		_points[0].x = value;
	}

	template<class T> double Rect2<T>::getY() const
	{
		return _points[0].y;
	}

	template<class T> void Rect2<T>::setY( double value )
	{
		_points[0].y = value;
	}


	// First Point
	template<class T> double Rect2<T>::getX1() const
	{
		return _points[0].x;
	}

	template<class T> void Rect2<T>::setX1( double value )
	{
		_points[0].x = value;
	}

	template<class T> double Rect2<T>::getY1() const
	{
		return _points[0].y;
	}

	template<class T> void Rect2<T>::setY1( double value )
	{
		_points[0].y = value;
	}

	// Second Point
	template<class T> double Rect2<T>::getX2() const
	{
		return _points[1].x;
	}

	template<class T> void Rect2<T>::setX2( double value )
	{
		_points[1].x = value;
	}

	template<class T> double Rect2<T>::getY2() const
	{
		return _points[1].y;
	}

	template<class T> void Rect2<T>::setY2( double value )
	{
		_points[1].y = value;
	}

	// Third Point
	template<class T> double Rect2<T>::getX3() const
	{
		return _points[2].x;
	}

	template<class T> void Rect2<T>::setX3( double value )
	{
		_points[2].x = value;
	}

	template<class T> double Rect2<T>::getY3() const
	{
		return _points[2].y;
	}

	template<class T> void Rect2<T>::setY3( double value )
	{
		_points[2].y = value;
	}

	// Fourth Point
	template<class T> double Rect2<T>::getX4() const
	{
		return _points[3].x;
	}

	template<class T> void Rect2<T>::setX4( double value )
	{
		_points[3].x = value;
	}

	template<class T> double Rect2<T>::getY4() const
	{
		return _points[3].y;
	}

	template<class T> void Rect2<T>::setY4( double value )
	{
		_points[3].y = value;
	}


	template<class T> double Rect2<T>::getWidth() const
	{
		return fabs(_points[0].x - _points[1].x);
	}

	template<class T> void Rect2<T>::setWidth( double value )
	{
		_points[1].x = _points[0].x + value;
		_points[2].x = _points[0].x + value;
	}

	template<class T> double Rect2<T>::getHeight() const
	{
		return fabs(_points[0].y - _points[3].y);
	}

	template<class T> void Rect2<T>::setHeight( double value )
	{
		_points[2].x = _points[0].y + value;
		_points[3].x = _points[0].y + value;
	}


};//namespace Misc
};//namespace Core
