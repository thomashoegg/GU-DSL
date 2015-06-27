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


#pragma once
#ifndef __POINT2_H__
#define __POINT2_H__

#include <QObject>

namespace Geometry
{
	namespace Primitives
	{
		template <class T> class Point2;

		typedef Point2<float>	Point2f;
		typedef Point2<double>	Point2d;
		typedef Point2<int>		Point2i;

		/**
		 * \brief Rect class.
		 *
		 */
		template <class T> class Point2: public QObject
		{
		public:
			/**
			 * \brief Constructor.
			 *
			 */
			Point2();

			/**
			 * \brief constructor.
			 *
			 */
			Point2(const T& x, const T& y);

			/**
			 * \brief copy constructor
			 *
			 */
			Point2(const Point2<T> &rhs);

			/**
			 * \brief destructor
			 *
			 */
			~Point2();

			/**
			 * \brief assignment operator
			 *
			 */
			Point2& operator= (const Point2<T> &rhs);

			/**
			 * \brief Relational operators
			 *
			 */
			bool operator< ( const Point2& rhs);
			bool operator> ( const Point2& rhs);
			bool operator<=( const Point2& rhs);
			bool operator>=( const Point2& rhs);

		public:
			T						x;			//!< The x value.
			T						y;			//!< The y value.
		};
	}; //namespace Primitives
}; //namespace Geometry

#include "Primitives/Point2.inl"

Q_DECLARE_METATYPE(Geometry::Primitives::Point2f)
Q_DECLARE_METATYPE(Geometry::Primitives::Point2d)
Q_DECLARE_METATYPE(Geometry::Primitives::Point2i)

#endif /*__POINT2_H__*/
