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
#ifndef __LINE2_H_
#define __LINE2_H_

#include "Misc/GeometryDLL.h"
#include "Primitives/Polygon.h"

namespace Geometry
{
	namespace Primitives
	{
		template <class T> class Line2;

		typedef Line2<float>	Line2f;
		typedef Line2<double>	Line2d;
		typedef Line2<int>		Line2i;

		/**
		 * \brief Line2 class.
		 *
		 */
		template <class T> class Line2 : public Polygon<T>
		{
		public:
			/**
			 * \brief Constructor.
			 *
			 */
			Line2();

			/**
			 * \brief constructor.
			 *
			 */
			Line2(const double x1, const double y1, const double x2, const double y2);

			/**
			 * \brief copy constructor
			 *
			 */
			Line2(const Line2 &rhs);

			/**
			 * \brief destructor
			 *
			 */
			~Line2();

			/**
			 * \brief assignment operator
			 *
			 */
			Line2& operator= (const Line2 &rhs);

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
			 * \brief set y2 value.
			 *
			 *	@param double value. y2 value.
			 */
			void setY2(double value);

		private:
			Polygon<T>& operator=( const Polygon<T> &rhs );
		};


	}; //namespace Primitives
}; //namespace Geometry

#include "Primitives/Line2.inl"

Q_DECLARE_METATYPE(Geometry::Primitives::Line2f)
Q_DECLARE_METATYPE(Geometry::Primitives::Line2d)
Q_DECLARE_METATYPE(Geometry::Primitives::Line2i)

namespace
{
	void RegisterLine2Data()	
	{
		qRegisterMetaType<Geometry::Primitives::Line2f>();
		qRegisterMetaType<Geometry::Primitives::Line2f>("Line2f");

		qRegisterMetaType<Geometry::Primitives::Line2d>();
		qRegisterMetaType<Geometry::Primitives::Line2d>("Line2d");

		qRegisterMetaType<Geometry::Primitives::Line2i>();
		qRegisterMetaType<Geometry::Primitives::Line2i>("Line2i");
	}

	Q_CONSTRUCTOR_FUNCTION(RegisterLine2Data)
};

#endif /*__LINE2_H_*/
