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
#ifndef __GEOMETRY_POLYGON__H__
#define __GEOMETRY_POLYGON__H__

#include <QObject>
#include "Misc/GeometryDLL.h"

#include "Primitives/Point3.h"

namespace Geometry
{
	namespace Primitives
	{
		template <class T> class Polygon;

		typedef Polygon<float>		Polygonf;
		typedef Polygon<double>		Polygond;
		typedef Polygon<int>		Polygoni;

		/**
		 * \brief Polygon class.
		 *
		 */
		template <class T> class Polygon : public QObject
		{
		public:
			/**
			 * \brief Constructor.
			 *
			 */
			Polygon();

			/**
			 * \brief copy constructor
			 *
			 */
			Polygon(const Polygon &rhs);

			/**
			 * \brief destructor
			 *
			 */
			~Polygon();

			/**
			 * \brief assignment operator
			 *
			 */
			Polygon& operator= (const Polygon &rhs);

			/**
			 * \brief Adds a new point to the internal list
			 *
			 *	@param Point<T> point. The new point
			 */
			void addPoint(Point3<T> point);

			/**
			 * \brief get all polygon points.
			 *
			 *	@return all polygon points.
			 */
			const std::vector<Point3<T>>& points() const { return _points; }

		protected:

			/**
			 * \brief Copies all points
			 *
			 *	@param Polygon<T> &source. Copies all points from the source to this instance
			 */
			void internalCopyPoints( const Polygon<T> &source );

		protected:
			std::vector<Point3<T>>	_points;									//!< The point list.
		};

	}; //namespace Primitives
}; //namespace Geometry

#include "Primitives/Polygon.inl"

Q_DECLARE_METATYPE(Geometry::Primitives::Polygonf)
Q_DECLARE_METATYPE(Geometry::Primitives::Polygond)
Q_DECLARE_METATYPE(Geometry::Primitives::Polygoni)

namespace
{
	void RegisterPolygonData()	
	{
		qRegisterMetaType<Geometry::Primitives::Polygonf>();
		qRegisterMetaType<Geometry::Primitives::Polygonf>("Polygonf");

		qRegisterMetaType<Geometry::Primitives::Polygond>();
		qRegisterMetaType<Geometry::Primitives::Polygond>("Polygond");

		qRegisterMetaType<Geometry::Primitives::Polygoni>();
		qRegisterMetaType<Geometry::Primitives::Polygoni>("Polygoni");
	}

	Q_CONSTRUCTOR_FUNCTION(RegisterPolygonData)
};

#endif /*__GEOMETRY_POLYGON__H__ */
