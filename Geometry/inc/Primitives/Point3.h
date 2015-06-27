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
#ifndef __POINT3_H__
#define __POINT3_H__

#include <QObject>

namespace Geometry
{
	namespace Primitives
	{
		template <class T> class Point3;

		typedef Point3<float>	Point3f;
		typedef Point3<double>	Point3d;
		typedef Point3<int>		Point3i;

		/**
		 * \brief Rect class.
		 *
		 */
		template <class T> class Point3 : public QObject
		{
		public:
			/**
			 * \brief Constructor.
			 *
			 */
			Point3();

			/**
			 * \brief constructor.
			 *
			 */
			Point3(const T& x, const T& y, const T& z);

			/**
			 * \brief copy constructor
			 *
			 */
			Point3(const Point3<T> &rhs);

			/**
			 * \brief destructor
			 *
			 */
			~Point3();

			/**
			 * \brief assignment operator
			 *
			 */
			Point3& operator= (const Point3<T> &rhs);

			/**
			 * \brief Relational operators
			 *
			 */
			bool operator< ( const Point3<T>& rhs);
			bool operator> ( const Point3<T>& rhs);
			bool operator<=( const Point3<T>& rhs);
			bool operator>=( const Point3<T>& rhs);

			public:
			T						x;			//!< The x value.
			T						y;			//!< The y value.
			T						z;			//!< The z value.
		};
	}; //namespace Primitives
}; //namespace Geometry

#include "Primitives//Point3.inl"

Q_DECLARE_METATYPE(Geometry::Primitives::Point3f)
Q_DECLARE_METATYPE(Geometry::Primitives::Point3d)
Q_DECLARE_METATYPE(Geometry::Primitives::Point3i)

#endif /*__POINT3_H__*/
