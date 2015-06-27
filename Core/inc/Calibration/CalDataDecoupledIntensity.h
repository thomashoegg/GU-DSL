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


#ifndef CALDATADECOUPLEDINTENSITY_H
#define CALDATADECOUPLEDINTENSITY_H

#include "CoreDLL.h"

#include "boost/shared_ptr.hpp"
#include "Calibration/CalData3DBase.h"

namespace Core
{
	namespace Calibration
	{

		/**
		* \brief Distance Calibration Data
		*
		* Data store for 3D B-Spline calibration parameters.
		*/
		class CORE_API CalDataDecoupledIntensity : public CalData3DBase
		{
		public:

			//---------------------------------------------------------------------------
			//! \name                      Instantiation
			//! \{
			//---------------------------------------------------------------------------

			//! Constructor
			CalDataDecoupledIntensity(void);

			//! Copy Constructor
			CalDataDecoupledIntensity(const CalDataDecoupledIntensity &source);

			//! Destructor
			virtual ~CalDataDecoupledIntensity(void);

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

			inline unsigned int getDegree(void) const {return _degree;};

			//! Get minimal supported input distance, which is mapped to u = 0.0.
			inline float minDistance(void) const { return _minDistance; };

			//! Get maximal supported input distance, which is mapped to u = 1.0.
			inline float maxDistance(void) const { return _maxDistance; };

			inline float* polynomParameters(void) const { return _polyDelta; };
			inline float  getPolyDeltaInd(unsigned int ind) const
			{
				if(ind < _degree +1)
					return _polyDelta[ind];
				else return 0;
			}


			//!\}------------------------------------------------------------------------



		protected:

			//---------------------------------------------------------------------------
			//! \name                       File I/O
			//! \{
			//---------------------------------------------------------------------------

			//! Save to file.
			virtual bool save(const char* filename) const;

			//! Load from file.
			virtual bool load(const char* filename); 

			//!\}------------------------------------------------------------------------ 


			//---------------------------------------------------------------------------
			//! \name                          Storage
			//! \{
			//---------------------------------------------------------------------------


			//!\}------------------------------------------------------------------------

			float			_minDistance;   //!< min distance 
			float			_maxDistance;   //!< min distance 
			unsigned int	_degree;

			float*			_polyDelta;
		};
	}

}

#endif // CALDATADECOUPLEDINTENSITY_H
