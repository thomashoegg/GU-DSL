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



#ifndef BOUNDINGRECT2D_H
#define BOUNDINGRECT2D_H

namespace Geometry
{
	namespace BoundingBoxes
	{


		/*!
		 * \class BoundingRect2D
		 *
		 * \brief
		 * A class that represents the 2D bounding box of a point cloud
		 *
		 */ 
		template<class T>
		class BoundingRect2D
		{
		public:

			//!< Default constructor
			BoundingRect2D();

			//!< Constructor
			BoundingRect2D(T left, T top, T right, T bottom);

	
			//!< Sets the width of the bounding rectangle
			void setWidth(T width);

			//!< Returns the width of the bounding rectangle
			T width() const { return abs(_right - _left); }

			//!< Sets the height
			void setHeight(T height);

			//!< Returns the height of the bounding rectangle
			T height() const { return abs(_top - _bottom); }

			//!< Sets the left side of the bounding rectangle
			void setLeft(T left);

			//!< Returns the left side of the bounding rectangle
			T left() const { return _left; }

			//!< Sets the right side of the bounding rectangle
			void setRight(T right);

			//!< Returns the left side of the bounding rectangle
			T right() const { return _right; }

			//!< Sets the top side of the bounding rectangle
			void setTop(T top);

			//!< Returns the top side of the bounding rectangle
			T top() const { return _top; }

			//!< Sets the bottom side of the bounding rectangle
			void setBottom(T bottom);

			//!< Returns the bottom side of the bounding rectangle
			T bottom() const { return _bottom; }

			//!< Adds a new point to the bounding rectangle and expands it if necessary
			void addPoint(T x, T y);

		private:

			T _left;							//!< The left side
			T _right;							//!< The right side
			T _top;								//!< The top side
			T _bottom;							//!< The bottom side
		};

		#include "BoundingRect2D.inl"

		typedef BoundingRect2D<float> BoundingRect2Df;
		typedef BoundingRect2D<double> BoundingRect2Dd;
		typedef BoundingRect2D<int> BoundingRect2Di;

	}
}

#endif // BOUNDINGRECT2D_H
