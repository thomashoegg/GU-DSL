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
#ifndef __CIRCLE2_H__
#define __CIRCLE2_H__

#include "Misc/GeometryDLL.h"
#include "Primitives/Polygon.h"

namespace Geometry
{
	namespace Primitives
	{
		template <class T> class Circle2;

		typedef Circle2<float>	Circle2f;
		typedef Circle2<double>	Circle2d;
		typedef Circle2<int>	Circle2i;

		/**
		 * \brief Circle2 class.
		 *
		 */
		template <class T> class Circle2 : public Polygon<T>
		{
		public:
			/**
			 * \brief Constructor.
			 *
			 */
			Circle2();

			/**
			 * \brief constructor.
			 *
			 */
			Circle2(const double centerX, const double centerY, const double radius);

			/**
			 * \brief copy constructor
			 *
			 */
			Circle2(const Circle2 &rhs);

			/**
			 * \brief destructor
			 *
			 */
			~Circle2();

			/**
			 * \brief assignment operator
			 *
			 */
			Circle2& operator= (const Circle2 &rhs);

			/**
			 * \brief Constructs a new circle
			 *
			 *	@param double x. x center value.
			 *	@param double y. y center value.
			 *	@param double radius. The radius.
			 */
			void constructCircle( const double x, const double y, const double radius );


			/**
			 * \brief get x value.
			 *
			 *	@return double x value.
			 */
			double getCenterX() const;

			/**
			 * \brief set the x center.
			 *
			 *	@param double value. x center value.
			 */
			void setCenterX(double value);

			/**
			 * \brief get y center value.
			 *
			 *	@return double y center value.
			 */
			double getCenterY() const;

			/**
			 * \brief set y center value.
			 *
			 *	@param double value. y center value.
			 */
			void setCenterY(double value);

			/**
			 * \brief get the radius.
			 *
			 *	@return double the radius.
			 */
			double getRadius() const;

			/**
			 * \brief set the radius.
			 *
			 *	@param double value. the radius.
			 */
			void setRadius(double value);

		private:
			Polygon<T>& operator=( const Polygon<T> &rhs );

		private:
			T					_x;			//!< The x value.
			T					_y;			//!< The y value.
			T					_radius;	//!< The radius.
		};
}; //namespace Primitives
}; //namespace Geometry

#include "Primitives/Circle2.inl"

Q_DECLARE_METATYPE(Geometry::Primitives::Circle2f)
Q_DECLARE_METATYPE(Geometry::Primitives::Circle2d)
Q_DECLARE_METATYPE(Geometry::Primitives::Circle2i)

namespace
{
	void RegisterCircle2Data()	
	{
		qRegisterMetaType<Geometry::Primitives::Circle2f>();
		qRegisterMetaType<Geometry::Primitives::Circle2f>("Circle2f");

		qRegisterMetaType<Geometry::Primitives::Circle2d>();
		qRegisterMetaType<Geometry::Primitives::Circle2d>("Circle2d");

		qRegisterMetaType<Geometry::Primitives::Circle2i>();
		qRegisterMetaType<Geometry::Primitives::Circle2i>("Circle2i");
	}

	Q_CONSTRUCTOR_FUNCTION(RegisterCircle2Data)
};

#endif /*__CIRCLE2_H__*/
