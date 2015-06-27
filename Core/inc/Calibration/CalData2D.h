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

#ifndef __CALDATA2D_H__
#define __CALDATA2D_H__

#include "CoreDLL.h"

#include "boost/shared_ptr.hpp"

namespace Core
{
	namespace Calibration
	{

		/**
		* \brief Lateral Calibration Data
		*
		* Data store for 2D calibration parameters.
		*/
		class CORE_API CalData2D
		{

		public:


			//---------------------------------------------------------------------------
			//! \name                        Instantiation
			//! \{
			//---------------------------------------------------------------------------

			//! Constructor
			CalData2D(void);

			//! Copy Constructor
			CalData2D(const CalData2D &source);

			//! Destructor
			virtual ~CalData2D(void) 
			{
			};

			//!\}------------------------------------------------------------------------


			//---------------------------------------------------------------------------
			//! \name                    Intrinsic parameter
			//! \{
			//---------------------------------------------------------------------------

			//! Get image size in px.
			unsigned short width(void) const { return m_imgsize[0]; };

			//! Get image size in px.
			unsigned short height(void) const { return m_imgsize[1]; };


			//! Set image size in px.
			void setWidth(unsigned short value) { m_imgsize[0] = value; };

			//! Set image size in px.
			void setHeight(unsigned short value) { m_imgsize[1] = value; };


			//! Get focallength in px.
			double fx(void) const { return m_focallength[0]; };

			//! Get focal length in px.
			double fy(void) const { return m_focallength[1]; };

			//! Get coordinate of the principle point in px.
			double cx(void) const { return m_center[0]; };

			//! Get coordinate of the principle point in px.
			double cy(void) const { return m_center[1]; };

			//! Get focal length in px.
			const double* focallength(void) const { return m_focallength; };

			//! Get coordinate of the principle point in px.
			const double* center(void) const { return m_center; };


			//! Set focal length in px.
			void fx(double value) { m_focallength[0] = value; };

			//! Set focal length in px.
			void fy(double value) { m_focallength[1] = value; };

			//! Set coordinate of the principle point in px.
			void cx(double value) { m_center[0] = value; };

			//! Set coordinate of the principle point in px.
			void cy(double value) { m_center[1] = value; };


			//! Get pixel size in mm.
			double sx(void) const { return m_pxlsize[0]; };

			//! Get pixel size in mm.
			double sy(void) const { return m_pxlsize[1]; };


			//! Set pixel size in mm.
			void sx(double value) { m_pxlsize[0] = value; };

			//! Set pixel size in mm.
			void sy(double value) { m_pxlsize[1] = value; };

			//! Calculate world coordinate for the given pixel and cart distane.
			void cartesian(unsigned short x, unsigned short y, 
				float cartDist, float *p) const;

			//! Calculate viewing frustum
			void frustum(float maxDist, float (&frustum)[8][3]) const;

			//!\}------------------------------------------------------------------------



			//---------------------------------------------------------------------------
			//! \name                     Radial distortion
			//! \{
			//---------------------------------------------------------------------------

			// Get first radial distortion coefficients.
			inline double r1(void) const { return m_radial[0]; };

			// Get second radial distortion coefficients.
			inline double r2(void) const { return m_radial[1]; };

			// Get first tangential distortion coefficients.
			inline double t1(void) const { return m_tangential[0]; };

			// Get second tangential distortion coefficients.
			inline double t2(void) const { return m_tangential[1]; };

			// Get radial distortion coefficients.
			inline const double* radial(void) const { return m_radial; };

			// Get tangential distortion coefficients.
			inline const double* tangential(void) const { return m_tangential; };

			// Get polar angles.
			inline const double* polarAngles(unsigned int angle) const 
			{ 
				if(angle < 2)
					return m_polarAngles[angle]; 
				else return nullptr;
			};

			// Get cosines of polar angles.
			inline const double* cosAngles(unsigned int angle) const
			{ 
				if(angle < 2)
					return m_cosAngles[angle]; 
				else return nullptr;
			};
			// Get sinus of polar angles.
			inline const double* sinAngles(unsigned int angle) const 
			{ 
				if(angle < 2)
					return m_cosAngles[angle]; 
				else return nullptr;
			};

			// Set first radial distortion coefficients.
			void r1(double value) { m_radial[0] = value; };

			// Set second radial distortion coefficients.
			void r2(double value) { m_radial[1] = value; };

			// Set first tangential distortion coefficients.
			void t1(double value) { m_tangential[0] = value; };

			// Set second tangential distortion coefficients.
			void t2(double value) { m_tangential[1] = value; };

			//!\}------------------------------------------------------------------------

			CalData2D& operator=(const CalData2D& source);


			/**
			* \brief Save the calibration
			*
			* \return bool true if successful, else false
			* \param  const char * filename The filename
			*/
			virtual bool save(const char* filename) const;
			
			/**
			* \brief Loads the calibration
			*
			* \return bool true if successful, else false
			* \param  const char * filename The filename
			*/
			virtual bool load(const char* filename);

		private:


			unsigned short  m_imgsize[2];			//!< image size in px

			double  m_pxlsize[2];					//!< pixel size in mm
			double  m_focallength[2];				//!< focal length in px
			double  m_center[2];					//!< coordinate of the principle point

			double  m_radial[2];					//!< radial distortion coefficient
			double  m_tangential[2];				//!< tangential distortion coefficient

			double* m_polarAngles[2];				//!< inclination (polar angles) and azimuth angles (RADIAN)
			double* m_cosAngles[2];					//!< cosines of the polar angles
			double* m_sinAngles[2];					//!< sinus of the polar angles

		};

		typedef boost::shared_ptr<CalData2D>		CalData2DPtr;
		typedef boost::shared_ptr<const CalData2D>	ConstCalData2DPtr;

	}

} // TDV

#endif // CALDATA2D_H
