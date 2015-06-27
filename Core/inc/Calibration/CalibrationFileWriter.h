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

#ifndef __CALIBRATIONFILEWRITER_H__
#define __CALIBRATIONFILEWRITER_H__

#include "CoreDLL.h"
#include <string>
#include <pshpack1.h>

#pragma warning(push)
#pragma warning(disable: 4251)

namespace Core
{
	namespace Calibration
	{
		#define CCF_FILEHEADER_SIGNATURE	0x40636366	//!< The calibration file signature "@ccf"
		#define CCF_CURRENT_FILE_VERSION	1			//!< The calibration file version
		#define CCF_FILE_EXTENSION			".ccf"		//!< The calibration file extension.

		/**
		 * \brief the calibration type enum
		 *
		 */
		enum calibrationType: unsigned __int16
		{
			UNDEFINED = 0,		//!< undefined calibration file
			PERSPECTIVE = 1		//!< perspective calibration
		};

		/**
		 * \brief calibration file version header
		 *
		 */
		struct VersionHeader
		{
			unsigned __int32	signature;	//!< The calibration file signature
			unsigned __int16	version;	//!< The calibration file version

			/**
			 * \brief constructor
			 *
			 */
			VersionHeader()
			{
				signature = CCF_FILEHEADER_SIGNATURE;
				version = CCF_CURRENT_FILE_VERSION;
			}
		};

		/**
		 * \brief The calibration file header
		 *
		 */
		struct FileHeader
		{
			VersionHeader		header;		//!< The version header.
			calibrationType		type;		//!< The calibration type.
			unsigned __int64	dataSize;	//!< The size of the calibration data.

			/**
			 * \brief constructor
			 *
			 */
			FileHeader()
			{
				type = UNDEFINED;
				dataSize = 0;
			}

			/**
			 * \brief constructor
			 *
			 * \param calibrationType calType The calibration type.
			 * \param unsigned __int64 dataSize The size of the calibration data.
			 */
			FileHeader(calibrationType calType, unsigned __int64 dataSize)
			{
				type = calType;
				this->dataSize = dataSize;
			}
		};

		/**
		* \brief Perspective Calibration Data
		*
		* Data store for perspective calibration parameters.
		*/
		class CORE_API CalibrationFileWriter
		{

		public:
			/**
			 * \brief constructor
			 *
			 */
			CalibrationFileWriter(std::string calibrationFilePath);

			/**
			 * \brief destructor
			 *
			 */
			virtual ~CalibrationFileWriter();
		
			/**
			* \brief Save the calibration
			*
			* \return bool true if successful, else false
			* \param  const char * filename The filename
			*/
			virtual bool save(const std::string& filename, char* data, std::size_t size, calibrationType type) const;
			
			/**
			* \brief Loads the calibration
			*
			* \return char* calibration data.
			* \param  const char * filename The filename
			* \param  const calibrationType The expected calibration type.
			*/
			virtual char* load(const std::string& filename, const calibrationType type);

			/**
			 * \brief Check if file exists
			 *
			 *	\param const std::string& filename. The file name to check.
			 *	\return bool. If true the file exists.
			 */
			virtual bool fileExists(const std::string& filename);

		protected:

			/**
			 * \brief get calibration file path.
			 *
			 *	\return std::string calibration file path.
			 */
			std::string calibrationFilePath();

			/**
			 * \brief set calibration file path.
			 *
			 *	\param const std::string calibrationFilePath. The calibration file path to be set.
			 */
			void setCalibrationFilePath(const std::string calibrationFilePath);
		private:
			std::string					_calibrationFilePath;		//!< The calibration file path.
		};
	}; //namespace Calibration
}; //namespace Core

#pragma warning(pop)

#include <poppack.h>
#endif // CALDATA2D_H
