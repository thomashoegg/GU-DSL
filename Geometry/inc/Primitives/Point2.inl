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

namespace Geometry
{
namespace Primitives
{
	template<class T> Point2<T>::Point2()
		:QObject()
	{
		x = 0;
		y = 0;
	}

	template<class T> Point2<T>::Point2( const Point2<T> &rhs )
	{
		x = rhs.x;
		y = rhs.y;
	}

	template<class T> Point2<T>::Point2( const T& x, const T& y )
	{
		this->x = x;
		this->y = y;
	}

	template<class T> Point2<T>::~Point2()
	{

	}

	template<class T> Point2<T>& Point2<T>::operator=( const Point2<T> &rhs )
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}

	template <class T>
	bool Point2<T>::operator<( const Point2<T>& rhs )
	{
		return x < rhs.x || (x == rhs.x && y < rhs.y);
	}

	template <class T>
	bool Point2<T>::operator>( const Point2<T>& rhs )
	{
		return x > rhs.x || (x == rhs.x && y > rhs.y);
	}

	template <class T>
	bool Point2<T>::operator<=(const Point2<T>& rhs)
	{
		return !(*this > rhs);
	}

	template <class T>
	bool Point2<T>::operator>=(const Point2<T>& rhs)
	{
		return !(*this < rhs);
	}
};//namespace Misc
};//namespace Core
