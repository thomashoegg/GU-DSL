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

#include "Primitives/Circle3.h"

namespace Geometry
{
namespace Primitives
{
	template<class T> Circle3<T>::Circle3()
	{
		_x = 0;
		_y = 0;
		_normalX = -1;
		_normalY = -1;
		_normalZ = -1;
		_radius = 0;

		_points.push_back(Point3<T>(0, 0, 0));
	}

	template<class T> Circle3<T>::Circle3( const Circle3<T> &rhs )
	{
		_x = rhs._x;
		_y = rhs._y;
		_normalX = rhs._normalX;
		_normalY = rhs._normalY;
		_normalZ = rhs._normalZ;
		_radius = rhs._radius;

		internalCopyPoints(rhs);
	}

	template<class T> Circle3<T>::Circle3(const double centerX, const double centerY, const double centerZ, const double normalX, const double normalY, const double normalZ, const double radius)
	{
		_x = centerX;
		_y = centerY;
		_z = centerZ;
		_normalX = normalX;
		_normalY = normalY;
		_normalZ = normalZ;
		_radius = radius;

		constructCircle(_x, _y, _z, _normalX, _normalY, _normalZ, _radius);
	}

	template<class T> Circle3<T>::~Circle3()
	{

	}


	template<class T> Circle3<T>& Circle3<T>::operator=( const Circle3<T> &rhs )
	{
		_x = rhs._x;
		_y = rhs._y;
		_normalX = rhs._normalX;
		_normalY = rhs._normalY;
		_normalZ = rhs._normalZ;
		_radius = rhs._radius;

		constructCircle(_x, _y, _z, _normalX, _normalY, _normalZ, _radius);

		return *this;
	}

	template<class T> void Circle3<T>::constructCircle(const double centerX, const double centerY, const double centerZ, const double normalX, const double normalY, const double normalZ, const double radius)
	{
		_points.clear();

		QMatrix4x4 m;

		double vx = 0;
		double vy = 0;
		double vz = 0;

		QVector3D normal(normalX, normalY, normalZ);
		normal.normalize();

		if(normal.x() != 0)
		{
			vx = (-normal.y()-normal.z())/(normal.x());
			vy = vz = 1;
		}
		else if(normal.y() != 0)
		{
			vy = (-normal.x()-normal.z())/(normal.y());
			vx = vz = 1;
		}
		else if(normal.z() != 0)
		{
			vz = (-normal.x()-normal.y())/(normal.z());
			vx = vy = 1;
		}

		QVector3D circlePos(vx, vy, vz);
		circlePos.normalize();
		circlePos *= radius;

		m.rotate(1, normal.x(), normal.y(), normal.z());

		_points.push_back(Point3f(centerX + circlePos.x(), centerY + circlePos.y(), centerZ + circlePos.z()));
		for(int i = 0; i < 359; i++)
		{
			circlePos = m * circlePos;
			_points.push_back(Point3f(centerX + circlePos.x(), centerY + circlePos.y(), centerZ + circlePos.z()));
		}
	}


	template<class T> double Circle3<T>::getCenterX() const
	{
		return _x;
	}
	template<class T> void Circle3<T>::setCenterX( double value )
	{
		_x = value;
	}

	template<class T> double Circle3<T>::getCenterY() const
	{
		return _y;
	}

	template<class T> void Circle3<T>::setCenterY( double value )
	{
		_y = value;
	}

	template<class T> double Circle3<T>::getCenterZ() const
	{
		return _z;
	}

	template<class T> void Circle3<T>::setCenterZ( double value )
	{
		_z = value;
	}

	template<class T> double Circle3<T>::getNormalX() const
	{
		return _normalX;
	}

	template<class T> void Circle3<T>::setNormalX( double value )
	{
		_normalX = value;
	}


	template<class T> double Circle3<T>::getNormalY() const
	{
		return _normalY;
	}

	template<class T> void Circle3<T>::setNormalY( double value )
	{
		_normalY = value;
	}

	template<class T> double Circle3<T>::getNormalZ() const
	{
		return _normalZ;
	}

	template<class T> void Circle3<T>::setNormalZ( double value )
	{
		_normalZ = value;
	}


	template<class T> double Circle3<T>::getRadius() const
	{
		return _radius;
	}

	template<class T> void Circle3<T>::setRadius( double value )
	{
		_radius = value;
	}
};//namespace Primitives
};//namespace Geometry
