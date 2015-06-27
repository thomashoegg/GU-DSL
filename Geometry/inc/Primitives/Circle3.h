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
#ifndef __CIRCLE3_H__
#define __CIRCLE3_H__


#include "Misc/GeometryDLL.h"
#include "Primitives/Polygon.h"

namespace Geometry
{
	namespace Primitives
	{
		template <class T> class Circle3;

		typedef Circle3<float>	Circle3f;
		typedef Circle3<double>	Circle3d;
		typedef Circle3<int>	Circle3i;

		/**
		 * \brief Circle3 class.
		 *
		 */
		template <class T> class Circle3 : public Polygon<T>
		{
		public:
			/**
			 * \brief Constructor.
			 *
			 */
			Circle3();

			/**
			 * \brief constructor.
			 *
			 */
			Circle3(const double centerX, const double centerY, const double centerZ, const double normalX, const double normalY, const double normalZ, const double radius);

			/**
			 * \brief copy constructor
			 *
			 */
			Circle3(const Circle3 &rhs);

			/**
			 * \brief destructor
			 *
			 */
			~Circle3();

			/**
			 * \brief assignment operator
			 *
			 */
			Circle3& operator= (const Circle3 &rhs);

			/**
			 * \brief Constructs a new circle
			 *
			 *	@param double x. x center value.
			 *	@param double y. y center value.
			 *	@param double z. z center value.
			 *	@param double NormalX. The x component of the plane normal.
			 *	@param double NormalX. The y component of the plane normal.
			 *	@param double NormalZ. The z component of the plane normal.
			 *	@param double NormalZ. The radius.
			 */
			void constructCircle(const double centerX, const double centerY, const double centerZ, const double normalX, const double normalY, const double normalZ, const double radius);

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
			 * \brief get z center value.
			 *
			 *	@return double z center value.
			 */
			double getCenterZ() const;

			/**
			 * \brief set z center value.
			 *
			 *	@param double value. z center value.
			 */
			void setCenterZ(double value);

			/**
			 * \brief get the x component of the normal.
			 *
			 *	@return double the x component of the normal.
			 */
			double getNormalX() const;

			/**
			 * \brief set the x component of the normal.
			 *
			 *	@param double value. the x component of the normal.
			 */
			void setNormalX(double value);

			
			/**
			 * \brief get the y component of the normal.
			 *
			 *	@return double the y component of the normal.
			 */
			double getNormalY() const;

			/**
			 * \brief set the normal.
			 *
			 *	@param double value. the y component of the normal.
			 */
			void setNormalY(double value);

			/**
			 * \brief get the z component of the normal.
			 *
			 *	@return double the z component of the normal.
			 */
			double getNormalZ() const;

			/**
			 * \brief set the normal.
			 *
			 *	@param double value. the normal.
			 */
			void setNormalZ(double value);
		
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
			T					_z;			//!< The y value.
			T					_normalX;	//!< The x component of the radius.
			T					_normalY;	//!< The y component of the radius.
			T					_normalZ;	//!< The z component of the radius.
			T					_radius;	//!< The circle radius.

		};
	}; //namespace Primitives
}; //namespace Geometry

Q_DECLARE_METATYPE(Geometry::Primitives::Circle3f)
Q_DECLARE_METATYPE(Geometry::Primitives::Circle3d)
Q_DECLARE_METATYPE(Geometry::Primitives::Circle3i)

#include "Primitives/Circle3.inl"

namespace
{
	void RegisterCircle3Data()	
	{
		qRegisterMetaType<Geometry::Primitives::Circle3f>();
		qRegisterMetaType<Geometry::Primitives::Circle3f>("Circle3f");

		qRegisterMetaType<Geometry::Primitives::Circle3d>();
		qRegisterMetaType<Geometry::Primitives::Circle3d>("Circle3d");

		qRegisterMetaType<Geometry::Primitives::Circle3i>();
		qRegisterMetaType<Geometry::Primitives::Circle3i>("Circle3i");
	}

	Q_CONSTRUCTOR_FUNCTION(RegisterCircle3Data)
};

#endif /*__Circle3_H__*/
