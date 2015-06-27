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

#include "Primitives/Rect3.h"

namespace Geometry
{
namespace Primitives
{
	template<class T> Rect3<T>::Rect3()
	{
		_points.push_back(Point3<T>(0, 0, 0));
		_points.push_back(Point3<T>(0, 0, 0));
		_points.push_back(Point3<T>(0, 0, 0));
		_points.push_back(Point3<T>(0, 0, 0));
	}

	template<class T> Rect3<T>::Rect3( const Rect3<T> &rhs )
	{
		internalCopyPoints(rhs);
	}

	template<class T> Rect3<T>::Rect3(const double x1, const double y1, const double z1, const double x2, const double y2, const double z2, const double x3, const double y3, const double z3)
	{
		double vX1 = x2-x1;
		double vY1 = y2-y1;
		double vZ1 = z2-z1;

		double vX2 = x3-x1;
		double vY2 = y3-y1;
		double vZ2 = z3-z1;

		double cosAZ = (vX1*vX2 + vY1*vY2 + vZ1*vZ2);
		double cosAN = sqrt(vX1*vX1 + vY1*vY1 + vZ1*vZ1) * sqrt(vX2*vX2 + vY2*vY2 + vZ2*vZ2);
		double cosA = -1;

		if(cosAN > 0)
			cosA = fabs(cosAZ / cosAN);

		// We have an angle round about 90 degree
		if((cosA-0.00001) <= 0)
		{
			_points.push_back(Point3<T>(x1,				y1,				z1));
			_points.push_back(Point3<T>(x1 + vX1,		y1 + vY1,		z1 + vZ1));
			_points.push_back(Point3<T>(x1 + vX1 + vX2,	y1 + vY1 + vY2, z1 + vZ1 + vZ2));
			_points.push_back(Point3<T>(x1 + vX2,		y1 + vY2,		z1 + vZ2));
		}
		else
		{
			_points.push_back(Point3<T>(0, 0, 0));
			_points.push_back(Point3<T>(0, 0, 0));
			_points.push_back(Point3<T>(0, 0, 0));
			_points.push_back(Point3<T>(0, 0, 0));
		}
	}

	template<class T> Rect3<T>::~Rect3()
	{

	}


	template<class T> Rect3<T>& Rect3<T>::operator=( const Rect3<T> &rhs )
	{
		internalCopyPoints(rhs);
		return *this;
	}

	template<class T> Rect3<T>& Rect3<T>::operator=( const QRect &rhs )
	{
		_points.push_back(Point3<T>(rhs.x(),				rhs.y(),					0));
		_points.push_back(Point3<T>(rhs.x() + rhs.width(),	rhs.y(),					0));
		_points.push_back(Point3<T>(rhs.x() + rhs.width(),	rhs.y() + rhs.height(),		0));
		_points.push_back(Point3<T>(rhs.x(),				rhs.y() + rhs.height(),		0));

		return *this;
	}

	template<class T> QRect Rect3<T>::toQRect() const
	{
		return QRect(getX(), getY(), getWidth(), getHeight());
	}

	template<class T> double Rect3<T>::getX() const
	{
		return _points[0].x;
	}

	template<class T> void Rect3<T>::setX( double value )
	{
		_points[0].x = value;
	}

	template<class T> double Rect3<T>::getY() const
	{
		return _points[0].y;
	}

	template<class T> void Rect3<T>::setY( double value )
	{
		_points[0].y = value;
	}


	template<class T> double Rect3<T>::getZ() const
	{
		return _points[0].z;
	}

	template<class T> void Rect3<T>::setZ( double value )
	{
		_points[0].z = value;
	}

	// First Point
	template<class T> double Rect3<T>::getX1() const
	{
		return _points[0].x;
	}

	template<class T> void Rect3<T>::setX1( double value )
	{
		_points[0].x = value;
	}

	template<class T> double Rect3<T>::getY1() const
	{
		return _points[0].y;
	}

	template<class T> void Rect3<T>::setY1( double value )
	{
		_points[0].y = value;
	}

	template<class T> double Rect3<T>::getZ1() const
	{
		return _points[0].z;
	}

	template<class T> void Rect3<T>::setZ1( double value )
	{
		_points[0].z = value;
	}

	// Second Point
	template<class T> double Rect3<T>::getX2() const
	{
		return _points[1].x;
	}

	template<class T> void Rect3<T>::setX2( double value )
	{
		_points[1].x = value;
	}

	template<class T> double Rect3<T>::getY2() const
	{
		return _points[1].y;
	}

	template<class T> void Rect3<T>::setY2( double value )
	{
		_points[1].y = value;
	}

	template<class T> double Rect3<T>::getZ2() const
	{
		return _points[1].z;
	}

	template<class T> void Rect3<T>::setZ2( double value )
	{
		_points[1].z = value;
	}


	// Third Point
	template<class T> double Rect3<T>::getX3() const
	{
		return _points[2].x;
	}

	template<class T> void Rect3<T>::setX3( double value )
	{
		_points[2].x = value;
	}

	template<class T> double Rect3<T>::getY3() const
	{
		return _points[2].y;
	}

	template<class T> void Rect3<T>::setY3( double value )
	{
		_points[2].y = value;
	}

	template<class T> double Rect3<T>::getZ3() const
	{
		return _points[2].z;
	}

	template<class T> void Rect3<T>::setZ3( double value )
	{
		_points[2].z = value;
	}

	// Fourth Point
	template<class T> double Rect3<T>::getX4() const
	{
		return _points[3].x;
	}

	template<class T> void Rect3<T>::setX4( double value )
	{
		_points[3].x = value;
	}

	template<class T> double Rect3<T>::getY4() const
	{
		return _points[3].y;
	}

	template<class T> void Rect3<T>::setY4( double value )
	{
		_points[3].y = value;
	}

	template<class T> double Rect3<T>::getZ4() const
	{
		return _points[3].z;
	}

	template<class T> void Rect3<T>::setZ4( double value )
	{
		_points[3].z = value;
	}

	template<class T> double Rect3<T>::getWidth() const
	{
		double vX = _points[1].x-_points[0].x;
		double vY = _points[1].y-_points[0].y;
		double vZ = _points[1].z-_points[0].z;

		return sqrt(vX*vX + vY*vY + vZ*vZ);
	}

	template<class T> void Rect3<T>::setWidth( double value )
	{
		setSize(value, getHeight());
	}

	template<class T> double Rect3<T>::getHeight() const
	{
		double vX = _points[3].x-_points[0].x;
		double vY = _points[3].y-_points[0].y;
		double vZ = _points[3].z-_points[0].z;

		return sqrt(vX*vX + vY*vY + vZ*vZ);
	}

	template<class T> void Rect3<T>::setHeight( double value )
	{
		setSize(getWidth(), value);
	}


	template <class T> void Rect3<T>::setSize( double width, double height )
	{
		if(width < 0 || height < 0)
			return;

		double vX1 = _points[1].x-_points[0].x;
		double vY1 = _points[1].y-_points[0].y;
		double vZ1 = _points[1].z-_points[0].z;
		double lenV1 = sqrt(vX1*vX1 + vY1*vY1 + vZ1*vZ1);
		vX1 = (vX1 / lenV1)*width;
		vY1 = (vY1 / lenV1)*width;
		vZ1 = (vZ1 / lenV1)*width;

		double vX2 = _points[3].x-_points[0].x;
		double vY2 = _points[3].y-_points[0].y;
		double vZ2 = _points[3].z-_points[0].z;
		double lenV2 = sqrt(vX2*vX2 + vY2*vY2 + vZ2*vZ2);
		vX2 = (vX2 / lenV2)*height;
		vY2 = (vY2 / lenV2)*height;
		vZ2 = (vZ2 / lenV2)*height;

		_points[0] = Point3<T>(_points[0].x,				_points[0].y,				_points[0].z);
		_points[1] = Point3<T>(_points[0].x + vX1,			_points[0].y + vY1,			_points[0].z + vZ1);
		_points[2] = Point3<T>(_points[0].x + vX1 + vX2,	_points[0].y + vY1 + vY2,	_points[0].z + vZ1 + vZ2);
		_points[3] = Point3<T>(_points[0].x + vX2,			_points[0].y + vY2,			_points[0].z + vZ2);
	}



};//namespace Misc
};//namespace Core
