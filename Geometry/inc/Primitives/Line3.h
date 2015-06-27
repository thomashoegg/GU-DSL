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
#ifndef __LINE3_H__
#define __LINE3_H__

#include <QObject>
#include <QRect>
#include "Misc/GeometryDLL.h"

namespace Geometry
{
	namespace Primitives
	{
		template <class T> class Line3;

		typedef Line3<float>	Line3f;
		typedef Line3<double>	Line3d;
		typedef Line3<int>		Line3i;

		/**
		 * \brief Line3 class.
		 *
		 */
		template <class T> class Line3 : public Polygon<T>
		{
		public:
			/**
			 * \brief Constructor.
			 *
			 */
			Line3();

			/**
			 * \brief constructor.
			 *
			 */
			Line3(const double x1, const double y1, const double z1, const double x2, const double y2, const double z2);

			/**
			 * \brief copy constructor
			 *
			 */
			Line3(const Line3 &rhs);

			/**
			 * \brief destructor
			 *
			 */
			~Line3();

			/**
			 * \brief assignment operator
			 *
			 */
			Line3& operator= (const Line3 &rhs);

			/**
			 * \brief assignment operator from QRect
			 *
			 */
			Line3& operator= (const QRect &rhs);

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

		};
	}; //namespace Primitives
}; //namespace Geometry

#include "Primitives/Line3.inl"

Q_DECLARE_METATYPE(Geometry::Primitives::Line3f)
Q_DECLARE_METATYPE(Geometry::Primitives::Line3d)
Q_DECLARE_METATYPE(Geometry::Primitives::Line3i)

namespace
{
	void RegisterLine3Data()	
	{
		qRegisterMetaType<Geometry::Primitives::Line3f>();
		qRegisterMetaType<Geometry::Primitives::Line3f>("Line3f");

		qRegisterMetaType<Geometry::Primitives::Line3d>();
		qRegisterMetaType<Geometry::Primitives::Line3d>("Line3d");

		qRegisterMetaType<Geometry::Primitives::Line3i>();
		qRegisterMetaType<Geometry::Primitives::Line3i>("Line3i");
	}

	Q_CONSTRUCTOR_FUNCTION(RegisterLine3Data)
};

#endif /*__LINE3_H__*/
