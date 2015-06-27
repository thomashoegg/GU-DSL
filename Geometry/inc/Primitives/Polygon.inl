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

#include "Primitives/Polygon.h"

namespace Geometry
{
namespace Primitives
{
	template<class T> Polygon<T>::Polygon()
		:QObject()
	{
	}

	template<class T> Polygon<T>::Polygon( const Polygon<T> &rhs )
	{
		for(size_t i = 0; i < rhs._points.size(); i++)
			_points.push_back(rhs._points[i]);
	}

	template<class T> Polygon<T>::~Polygon()
	{

	}

	template<class T> Polygon<T>& Polygon<T>::operator=( const Polygon<T> &rhs )
	{
		internalCopyPoints(rhs);
		return *this;
	}

	template<class T> void Polygon<T>::addPoint(Point3<T> point)
	{
		_points.push_back(point);
	}


	template<class T> void Polygon<T>::internalCopyPoints( const Polygon<T> &source )
	{
		_points.clear();
		for(size_t i = 0; i < source._points.size(); i++)
			_points.push_back(source._points[i]);
	}

};//namespace Misc
};//namespace Core

