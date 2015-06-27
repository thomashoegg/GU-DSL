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
#ifndef __TRIANGLE2_H__
#define __TRIANGLE2_H__

#include "Misc/GeometryDLL.h"
#include "Primitives/Polygon.h"

namespace Geometry
{
	namespace Primitives
	{
		template <class T> class Triangle2;

		typedef Triangle2<float>	Triangle2f;
		typedef Triangle2<double>	Triangle2d;
		typedef Triangle2<int>		Triangle2i;

		/**
		 * \brief Triangle2 class.
		 *
		 */
		template <class T> class Triangle2 : public Polygon<T>
		{
		public:
			/**
			 * \brief Constructor.
			 *
			 */
			Triangle2();

			/**
			 * \brief constructor.
			 *
			 */
			Triangle2(const double x1, const double y1, const double x2, const double y2, const double x3, const double y3);

			/**
			 * \brief copy constructor
			 *
			 */
			Triangle2(const Triangle2 &rhs);

			/**
			 * \brief destructor
			 *
			 */
			~Triangle2();

			/**
			 * \brief assignment operator
			 *
			 */
			Triangle2& operator= (const Triangle2 &rhs);

			/**
			 * \brief get x1 value.
			 *
			 *	@return double x1 value.
			 */
			double getX1() const;

			/**
			 * \brief set x1 value.
			 *
			 *	@param double value. x1 value.
			 */
			void setX1(double value);

			/**
			 * \brief get y1 value.
			 *
			 *	@return double y1 value.
			 */
			double getY1() const;

			/**
			 * \brief set y1 value.
			 *
			 *	@param double value. y1 value.
			 */
			void setY1(double value);
			
			/**
			 * \brief get z1 value.
			 *
			 *	@return double z1 value.
			 */
			double getZ1() const;

			/**
			 * \brief set z1 value.
			 *
			 *	@param double value. z1 value.
			 */
			void setZ1(double value);
			
			/**
			 * \brief get x2 value.
			 *
			 *	@return double x2 value.
			 */
			double getX2() const;

			/**
			 * \brief set x2 value.
			 *
			 *	@param double value. x2 value.
			 */
			void setX2(double value);

			/**
			 * \brief get y2 value.
			 *
			 *	@return double y2 value.
			 */
			double getY2() const;

			/**
			 * \brief set y1 value.
			 *
			 *	@param double value. y1 value.
			 */
			void setY2(double value);
			
			/**
			 * \brief get z2 value.
			 *
			 *	@return double z2 value.
			 */
			double getZ2() const;

			/**
			 * \brief set z2 value.
			 *
			 *	@param double value. z2 value.
			 */
			void setZ2(double value);
			
			/**
			 * \brief get x3 value.
			 *
			 *	@return double x3 value.
			 */
			double getX3() const;

			/**
			 * \brief set x3 value.
			 *
			 *	@param double value. x3 value.
			 */
			void setX3(double value);

			/**
			 * \brief get y1 value.
			 *
			 *	@return double y1 value.
			 */
			double getY3() const;

			/**
			 * \brief set y3 value.
			 *
			 *	@param double value. y3 value.
			 */
			void setY3(double value);

			/**
			 * \brief get z3 value.
			 *
			 *	@return double z3 value.
			 */
			double getZ3() const;

			/**
			 * \brief set z3 value.
			 *
			 *	@param double value. z3 value.
			 */
			void setZ3(double value);

		private:
			Polygon<T>& operator=( const Polygon<T> &rhs );

		};
	}; //namespace Primitives
}; //namespace Geometry

#include "Primitives/Triangle2.inl"

Q_DECLARE_METATYPE(Geometry::Primitives::Triangle2f)
Q_DECLARE_METATYPE(Geometry::Primitives::Triangle2d)
Q_DECLARE_METATYPE(Geometry::Primitives::Triangle2i)

namespace
{
	void RegisterTriangle2Data()	
	{
		qRegisterMetaType<Geometry::Primitives::Triangle2f>();
		qRegisterMetaType<Geometry::Primitives::Triangle2f>("Triangle2f");

		qRegisterMetaType<Geometry::Primitives::Triangle2d>();
		qRegisterMetaType<Geometry::Primitives::Triangle2d>("Triangle2d");

		qRegisterMetaType<Geometry::Primitives::Triangle2i>();
		qRegisterMetaType<Geometry::Primitives::Triangle2i>("Triangle2i");
	}

	Q_CONSTRUCTOR_FUNCTION(RegisterTriangle2Data)
};

#endif /*__TRIANGLE2_H__*/
