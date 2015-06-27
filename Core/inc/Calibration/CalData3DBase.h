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


#ifndef CALDATA3DBASE_H
#define CALDATA3DBASE_H

#include "CoreDLL.h"


namespace Core
{

	namespace Calibration
	{


		/**
		* \brief Distance Calibration Data Base
		*
		* Data store for the 3D calibration parameters.
		*/
		class CORE_API CalData3DBase
		{
		public:

			//---------------------------------------------------------------------------
			//! \name                      Data Processing
			//! \{
			//---------------------------------------------------------------------------

			//! Check if valid.
			bool isValid(void) const { return _bValid; };

			//! Correct distance.
			virtual float correctDistance(unsigned short x, unsigned short y, 
				float distance, bool bPostAdjustment, float intensity) const =0;

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

			//---------------------------------------------------------------------------
			//! \name                      Instantiation
			//! \{
			//---------------------------------------------------------------------------

			//! Constructor
			CalData3DBase(void);

			//! Copy Constructor
			CalData3DBase(const CalData3DBase &source);

			//! Destructor
			virtual ~CalData3DBase(void);

			//!\}------------------------------------------------------------------------

			bool _bValid;   //!< indicator whether the calibration object is valid or not


		private:

			void operator=(const CalData3DBase &source);
		};
	}

}

#endif // CALDATA3DBASE_H
