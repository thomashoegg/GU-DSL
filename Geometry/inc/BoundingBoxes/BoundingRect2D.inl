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

#include <math.h>

/*!
 * \brief
 * The default constructor
 *
 * \return
 */
template<class T> BoundingRect2D<T>::BoundingRect2D() :
	_left(0), _right(0), _top(0), _bottom(0)
{
}

/*!
 * \brief
 * Constructor to initialize the bounding rectange with the given values
 *
 * \param left The left side
 * \param top The top side
 * \param right The right side
 * \param bottom The bottom side
 * \return
 */
template<class T> 
BoundingRect2D<T>::BoundingRect2D(T left, T top, T right, T bottom) :
	_left(left), _right(right), _top(top), _bottom(bottom)
{
}

/*!
 * \brief
 * Sets the new width of the bounding rectangle
 *
 * \param width The new width
 * \return
 */
template<class T> void BoundingRect2D<T>::setWidth(T width)
{
	T oldWidth = width();

	_right = _left + abs(width);

	return oldWidth;
}

/*!
 * \brief
 * Sets the new height of the bounding rectangle
 *
 * \param height The new height
 * \return
 */
template<class T> void BoundingRect2D<T>::setHeight(T height)
{
	_top = _bottom + abs(height);
}

/*!
 * \brief
 * Sets the new left side of the bounding rectangle
 *
 * \param left The new left value
 * \return
 */
template<class T> void BoundingRect2D<T>::setLeft(T left)
{
	if(left <= _right)
		_left = left;
}

/*!
 * \brief
 * Sets the new right side of the bounding rectangle
 *
 * \param right The new right value
 * \return
 */
template<class T> void BoundingRect2D<T>::setRight(T right)
{
	if(right >= _left)
		_right = right;
}

/*!
 * \brief
 * Sets the new top side of the bounding rectangle
 *
 * \param top The new top value
 * \return
 */
template<class T> void BoundingRect2D<T>::setTop(T top)
{
	if(top >= _bottom)
		_top = top;
}

/*!
 * \brief
 * Sets the new bottom side of the bounding rectangle
 *
 * \param bottom The new bottom value
 * \return
 */
template<class T> void BoundingRect2D<T>::setBottom(T bottom)
{
	if(bottom <= _top)
		_bottom = bottom;
}

/*!
 * \brief
 * Adds a new point to the bounding rectangle and expands it, if necessary
 *
 * \param x The x coordinate of the new point
 * \param y The y coordinate of the new point
 * \return
 */
template<class T> void BoundingRect2D<T>::addPoint(T x, T y)
{
	_left = std::min(x, _left);
	_right = std::max(x, _right);
	_bottom = std::min(y, _bottom);
	_top = std::max(y, _top);
}
