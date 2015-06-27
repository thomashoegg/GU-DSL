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


#ifndef CALDATAPOLYNOM_H
#define CALDATAPOLYNOM_H

#include <QtCore/QString.h>

#include "CoreDLL.h"
#include "Calibration/CalData3DBase.h"

#include "boost/shared_ptr.hpp"

#define NB_POLY_PARAMS 6

namespace Core
{

	namespace Calibration
	{


		/**
		* \brief Distance Calibration Data
		*
		* Data store for 3D B-Spline calibration parameters.
		*/
		class CORE_API CalDataPolynom : public CalData3DBase
		{
		public:

			//---------------------------------------------------------------------------
			//! \name                      Instantiation
			//! \{
			//---------------------------------------------------------------------------

			//! Constructor
			CalDataPolynom(void);

			//! Copy Constructor
			CalDataPolynom(const CalDataPolynom &source);

			//! Destructor
			virtual ~CalDataPolynom(void);

			//!\}------------------------------------------------------------------------

			//---------------------------------------------------------------------------
			//! \name                      Data Processing
			//! \{
			//---------------------------------------------------------------------------

			//! Correct distance.
			float correctDistance(unsigned short x, unsigned short y, 
				float distance, bool bPostAdjustment=true, float intensity=0) const;

			//!\}------------------------------------------------------------------------

			//---------------------------------------------------------------------------
			//! \name                      Attributes Handler
			//! \{
			//---------------------------------------------------------------------------


			//! Get minimal supported input distance, which is mapped to u = 0.0.
			inline double minDistance(void) const { return _minDistance; };

			//! Get maximal supported input distance, which is mapped to u = 1.0.
			inline double maxDistance(void) const { return _maxDistance; };

			inline double* polynomParameters(void) const { return _polyParams; };

			inline double  getPolyParamInd(unsigned int ind) const
			{
				if(ind < NB_POLY_PARAMS)
					return _polyParams[ind];
				else return 0;
			}


			//!\}------------------------------------------------------------------------


			//---------------------------------------------------------------------------
			//! \name                       File I/O
			//! \{
			//---------------------------------------------------------------------------

			//! Save to file.
			virtual bool save(const char* filename) const;

			//! Load from file.
			virtual bool load(const char* filename); 

			//!\}------------------------------------------------------------------------ 



		protected:




			double			_minDistance;   //!< min distance 
			double			_maxDistance;   //!< min distance 

			double*			_polyParams;
		};


		typedef boost::shared_ptr<CalDataPolynom>		CalDataPolynomPtr;
		typedef boost::shared_ptr<const CalDataPolynom>	ConstCalDataPolynomPtr;
	}

}

#endif // CalDataPolynom_H
