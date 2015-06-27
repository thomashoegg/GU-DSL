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
#ifndef __RECT_H__
#define __RECT_H__

#include "Misc/GeometryDLL.h"
#include "Primitives/Polygon.h"

#include <QRect>


namespace Geometry
{
	namespace Primitives
	{
		template <class T> class Rect3;

		typedef Rect3<float>	Rect3f;
		typedef Rect3<double>	Rect3d;
		typedef Rect3<int>		Rect3i;

		/**
		 * \brief Rect3 class.
		 *
		 */
		template <class T> class Rect3 : public Polygon<T>
		{
		public:
			/**
			 * \brief Constructor.
			 *
			 */
			Rect3();

			/**
			 * \brief constructor.
			 *
			 */
			Rect3(const double x1, const double y1, const double z1, const double x2, const double y2, const double z2, const double x3, const double y3, const double z3);

			/**
			 * \brief copy constructor
			 *
			 */
			Rect3(const Rect3 &rhs);

			/**
			 * \brief destructor
			 *
			 */
			~Rect3();

			/**
			 * \brief assignment operator
			 *
			 */
			Rect3& operator= (const Rect3 &rhs);

			/**
			 * \brief assignment operator from QRect
			 *
			 */
			Rect3& operator= (const QRect &rhs);

					/**
			 * \brief get x value.
			 *
			 *	@return double x value.
			 */
			double getX() const;

			/**
			 * \brief set x value.
			 *
			 *	@param double value. x value.
			 */
			void setX(double value);

			/**
			 * \brief get y value.
			 *
			 *	@return double y value.
			 */
			double getY() const;

			/**
			 * \brief set y value.
			 *
			 *	@param double value. y value.
			 */
			void setY(double value);
			
			/**
			 * \brief get z value.
			 *
			 *	@return double z value.
			 */
			double getZ() const;

			/**
			 * \brief set z value.
			 *
			 *	@param double value. z value.
			 */
			void setZ(double value);

			/**
			 * \brief get the left bottom x value.
			 *
			 *	@return double the left bottom x value.
			 */
			double getX1() const;

			/**
			 * \brief set the left bottom x value.
			 *
			 *	@param double value. the left bottom x value.
			 */
			void setX1(double value);

			/**
			 * \brief get the left bottom y value.
			 *
			 *	@return double the left bottom y value.
			 */
			double getY1() const;

			/**
			 * \brief set the left bottom y value.
			 *
			 *	@param double value. the left bottom y value.
			 */
			void setY1(double value);

			/**
			 * \brief get the left bottom z value.
			 *
			 *	@return double the left bottom z value.
			 */
			double getZ1() const;

			/**
			 * \brief set the left bottom z value.
			 *
			 *	@param double value. the left bottom z value.
			 */
			void setZ1(double value);

			/**
			 * \brief get the right bottom x value.
			 *
			 *	@return double the right bottom x value.
			 */
			double getX2() const;
		
			/**
			 * \brief set the right bottom x value.
			 *
			 *	@param double value. the right bottom x value.
			 */
			void setX2(double value);

			/**
			 * \brief get the left bottom y value.
			 *
			 *	@return double the left bottom y value.
			 */
			double getY2() const;

			/**
			 * \brief set the right bottom y value.
			 *
			 *	@param double value. the right bottom y value.
			 */
			void setY2(double value);
			
			/**
			 * \brief get the left bottom z value.
			 *
			 *	@return double the left bottom z value.
			 */
			double getZ2() const;

			/**
			 * \brief set the right bottom z value.
			 *
			 *	@param double value. the right bottom z value.
			 */
			void setZ2(double value);

			/**
			 * \brief get the right top x value.
			 *
			 *	@return double the right top x value.
			 */
			double getX3() const;

			/**
			 * \brief set the right top x value.
			 *
			 *	@param double value. the right top x value.
			 */
			void setX3(double value);

			/**
			 * \brief get the right top y value.
			 *
			 *	@return double the right top y value.
			 */
			double getY3() const;

			/**
			 * \brief set the right top y value.
			 *
			 *	@param double value. the right top y value.
			 */
			void setY3(double value);
			
			/**
			 * \brief get the right top z value.
			 *
			 *	@return double the right top z value.
			 */
			double getZ3() const;

			/**
			 * \brief set the right top z value.
			 *
			 *	@param double value. the right top z value.
			 */
			void setZ3(double value);

			/**
			 * \brief get the left top x value.
			 *
			 *	@return double the left top x value.
			 */
			double getX4() const;

			/**
			 * \brief set the left top x value.
			 *
			 *	@param double value. the left top x value.
			 */
			void setX4(double value);

			/**
			 * \brief get the left top x value.
			 *
			 *	@return double the left top x value.
			 */
			double getY4() const;

			/**
			 * \brief set the left top y value.
			 *
			 *	@param double value. the left top y value.
			 */
			void setY4(double value);
			
			/**
			 * \brief get the left top z value.
			 *
			 *	@return double the left top z value.
			 */
			double getZ4() const;

			/**
			 * \brief set the left top z value.
			 *
			 *	@param double value. the left top z value.
			 */
			void setZ4(double value);

			/**
			 * \brief get x value.
			 *
			 *	@return double x value.
			 */
			double getWidth() const;

			/**
			 * \brief set width value.
			 *
			 *	@param double value. width value.
			 */
			void setWidth(double value);

			/**
			 * \brief get height value.
			 *
			 *	@return double height value.
			 */
			double getHeight() const;

			/**
			 * \brief set height value.
			 *
			 *	@param double value. height value.
			 */
			void setHeight(double value);

			/**
			 * \brief set the size of the rectangle.
			 *
			 *	@param double width. The new width.
			 *	@param double height. The new height.
			 */
			void setSize(double width, double height);

			/**
			 * \brief copy Rect3 into a QRect
			 *
			 * \return QRect
			 */
			QRect toQRect() const;

		private:
			Polygon<T>& operator=( const Polygon<T> &rhs );
		};
}; //namespace Primitives
}; //namespace Geometry

#include "Primitives/Rect3.inl"

Q_DECLARE_METATYPE(Geometry::Primitives::Rect3f)
Q_DECLARE_METATYPE(Geometry::Primitives::Rect3d)
Q_DECLARE_METATYPE(Geometry::Primitives::Rect3i)

namespace
{
	void RegisterRect3Data()	
	{
		qRegisterMetaType<Geometry::Primitives::Rect3f>();
		qRegisterMetaType<Geometry::Primitives::Rect3f>("Rect3f");

		qRegisterMetaType<Geometry::Primitives::Rect3d>();
		qRegisterMetaType<Geometry::Primitives::Rect3d>("Rect3d");

		qRegisterMetaType<Geometry::Primitives::Rect3i>();
		qRegisterMetaType<Geometry::Primitives::Rect3i>("Rect3i");
	}

	Q_CONSTRUCTOR_FUNCTION(RegisterRect3Data)
};

#endif /*__RECT_H__*/
