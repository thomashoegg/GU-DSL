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

#ifndef __CALDATAPERSPECTIVE_H__
#define __CALDATAPERSPECTIVE_H__

#include "CoreDLL.h"

#include "boost/shared_ptr.hpp"
#include "DebugOutput/DebugStream.h"
#include "Primitives/Rect2.h"
#include <fstream>
#include <opencv2/calib3d/calib3d.hpp>
#include <string>
#include <pshpack1.h>

namespace Core
{
	namespace Calibration
	{

		class CalibrationFileWriter;

		/**
		 * \brief region of interest data struct.
		 *
		 */
		struct CORE_API RegionsOfInterest
		{
			int x;				//!< The x value.
			int y;				//!< The y value.
			int width;			//!< The width value.
			int height;			//!< The height value.

			/**
			 * \brief default constructor
			 *
			 */
			RegionsOfInterest()
			{
				x = 0;
				y = 0;
				width = 0;
				height = 0;
			}

			/**
			 * \brief constructor
			 *
			 */
			RegionsOfInterest(int x, int y, int width, int height)
			{
				this->x = x;
				this->y = y;
				this->width = width;
				this->height = height;
			}

			/**
			 * \brief copy constructor.
			 *
			 */
			RegionsOfInterest(Geometry::Primitives::Rect2f& rhs)
			{
				x = rhs.getX();
				y = rhs.getY();
				width = rhs.getWidth();
				height = rhs.getHeight();
			}

			/**
			 * \brief destructor
			 *
			 */
			~RegionsOfInterest()
			{

			}
		};

		/**
		 * \brief perspective calibration data struct.
		 *
		 */
		struct CORE_API PerspectiveCalibrationData
		{
			unsigned int		cameraIdLength;			//!< The length of the camera id string.
			char*				cameraId;				//!< The camera id string.
			int					rows;					//!< The number of rows in the transformation matrix.
			int					columns;				//!< The number of columns in the transformation matrix.
			int					matrixType;				//!< The type of the transformation matrix.
			unsigned long long	dataSize;				//!< The number of unsigned character elements in the transformation matrix.
			uchar*				data;					//!< The transformation matrix.
			RegionsOfInterest	calibrationRoI;			//!< The calibration region of interest.
			RegionsOfInterest	imageRoI;				//!< The image region of interest.

			/**
			 * \brief default constructor
			 *
			 */
			PerspectiveCalibrationData()
			{
				cameraIdLength = 0;
				cameraId = nullptr;
				rows = 3;
				columns = 3;
				matrixType = CV_32F;
				data = 0;
				data = nullptr;
			}

			/**
			 * \brief copy constructor
			 *
			 */
			PerspectiveCalibrationData(const PerspectiveCalibrationData& rhs)
			{
				cameraIdLength = rhs.cameraIdLength;
				cameraId = new char[cameraIdLength];
				strncpy(cameraId, rhs.cameraId, cameraIdLength);
				rows = rhs.rows;
				columns = rhs.columns;
				matrixType = rhs.matrixType;
				dataSize = rhs.dataSize;
				data = new uchar[dataSize];
				memcpy(data, rhs.data, dataSize);
				calibrationRoI = rhs.calibrationRoI;
				imageRoI = rhs.imageRoI;
			}

			/**
			 * \brief constructor
			 *
			 * \param cv::Mat matrix. The perspective transformation matrix.
			 * \param std::string cameraId. The camera id.
			 */
			PerspectiveCalibrationData(cv::Mat matrix, std::string cameraId, RegionsOfInterest calibrationRoi, RegionsOfInterest imageRoi)
			{
				cameraIdLength = (unsigned int)cameraId.length() + 1;
				this->cameraId = new char[cameraIdLength];
				strncpy(this->cameraId, cameraId.c_str(), cameraIdLength);
				rows = matrix.rows;
				columns = matrix.cols;
				matrixType = matrix.type();
				dataSize = matrix.total() * matrix.elemSize();
				data = new uchar[dataSize];
				memcpy(data, matrix.data, dataSize);
				this->calibrationRoI = calibrationRoi;
				this->imageRoI = imageRoi;
			}

			/**
			 * \brief constructor to deserialize class frome file.
			 *
			 *	\param const char* data. data buffer from file input.
			 */
			PerspectiveCalibrationData(const char* data)
			{
				char* pos = (char*)data;
				
				cameraIdLength = *((unsigned int*)pos);
				pos += sizeof(unsigned int);
				
				cameraId = new char[cameraIdLength];
				strncpy(cameraId, pos, cameraIdLength);
				pos += cameraIdLength;

				rows = *((int*)pos);
				pos += sizeof(int);

				if(rows < 0)
				{
					D_OUT2(__FUNCTION__, "Unable to read calibration file. Calibration data are damaged.");
					assert(false && "Unable to read calibration file. Calibration data are damaged.");
				}

				columns = *((int*)pos);
				pos += sizeof(int);

				if(columns < 0)
				{
					D_OUT2(__FUNCTION__, "Unable to read calibration file. Calibration data are damaged.");
					assert(false && "Unable to read calibration file. Calibration data are damaged.");
				}

				matrixType = *((int*)pos);
				pos += sizeof(int);

				if(matrixType < 0)
				{
					D_OUT2(__FUNCTION__, "Unable to read calibration file. Calibration data are damaged.");
					assert(false && "Unable to read calibration file. Calibration data are damaged.");
				}

				dataSize = *((unsigned long long*)pos);
				pos += sizeof(unsigned long long);

				if(dataSize <= 0)
				{
					D_OUT2(__FUNCTION__, "Unable to read calibration file. Calibration data are damaged.");
					assert(false && "Unable to read calibration file. Calibration data are damaged.");
				}

				this->data = new uchar[dataSize];
				memcpy(this->data, pos, dataSize);

				pos += dataSize;
				memcpy(&calibrationRoI, pos, sizeof(RegionsOfInterest));
				
				pos += sizeof(RegionsOfInterest);
				memcpy(&imageRoI, pos, sizeof(RegionsOfInterest));
			}

			/**
			 * \brief destructor
			 *
			 */
			~PerspectiveCalibrationData()
			{
				delete[] cameraId;
				cameraId = nullptr;

				delete[] data;
				data = nullptr;
			}

			/**
			 * \brief assignment operator
			 *
			 */
			PerspectiveCalibrationData& operator= (const PerspectiveCalibrationData& rhs)
			{
				cameraIdLength = rhs.cameraIdLength;
				cameraId = new char[cameraIdLength];
				strncpy(cameraId, rhs.cameraId, cameraIdLength);
				rows = rhs.rows;
				columns = rhs.columns;
				matrixType = rhs.matrixType;
				dataSize = rhs.dataSize;
				data = new uchar[dataSize];
				memcpy(data, rhs.data, dataSize);
				calibrationRoI = rhs.calibrationRoI;
				imageRoI = rhs.imageRoI;
				return *this;
			}

			/**
			 * \brief get size in bytes.
			 *
			 *	\return std::size_t. size in bytes.
			 */
			std::size_t size() const
			{
				size_t size = sizeof(unsigned int) + sizeof(char) * cameraIdLength;
				size += sizeof(int) * 3 + sizeof(unsigned long long);
				size += sizeof(uchar) * dataSize;
				size += sizeof(RegionsOfInterest) *2;
				return size;
			}

			/**
			 * \brief create new byte array and copy data into it. Needs to be deleted manually.
			 *
			 *	\return char* new byte array.
			 */
			char* toByte() const
			{
				char* bytes = new char[size()];
				char* pos = bytes;

				*((unsigned int*)pos) = cameraIdLength;
				pos += sizeof(unsigned int);

				strncpy(pos, cameraId, cameraIdLength);
				pos += cameraIdLength;

				*((int*)pos) = rows;
				pos += sizeof(int);

				*((int*)pos) = columns;
				pos += sizeof(int);

				*((int*)pos) = matrixType;
				pos += sizeof(int);

				*((unsigned long long*)pos) = dataSize;
				pos += sizeof(unsigned long long);

				memcpy(pos, data, dataSize);

				pos += dataSize;
				memcpy(pos, &calibrationRoI, sizeof(RegionsOfInterest));

				pos += sizeof(RegionsOfInterest);
				memcpy(pos, &imageRoI, sizeof(RegionsOfInterest));

				return bytes;
			}

			/**
			 * \brief get matrix from data.
			 *
			 * \param cv::Mat& matrix. return value.
			 */
			void matrix(cv::Mat& matrix)
			{
				matrix = cv::Mat(rows, columns, matrixType, data).clone();
			}
		};

		/**
		* \brief Perspective Calibration Data
		*
		* Data store for perspective calibration parameters.
		*/
		class CORE_API PerspectiveCalibrationFileWriter
		{

		public:
			/**
			 * \brief constructor
			 *
			 * \param std::string calibrationFilePath. Path to calibration files.
			 */
			PerspectiveCalibrationFileWriter(std::string calibrationFilePath = "");

			/**
			 * \brief destructor
			 *
			 */
			virtual ~PerspectiveCalibrationFileWriter();

			/**
			 * \brief save calibration
			 *
			 *	\param PerspectiveCalibrationData data. The calibration data to save.
			 *  \return bool true if successful
			 */
			bool saveCalibration(const PerspectiveCalibrationData& data);

			/**
			 * \brief load calibration
			 *
			 *	\param PerspectiveCalibrationData data. The calibration data to save.
			 *	\param std::string cameraId. The camera Id to load the calibration data.
			 *  \return bool true if successful
			 */
			bool loadCalibration(PerspectiveCalibrationData& data, std::string cameraId);

			/**
			 * \brief check if calibration file exists for the camera.
			 *
			 *	\param std::string cameraId. The camera id to check.
			 *	\return bool. True if calibration file exists.
			 */
			bool calibrationFileExists(std::string cameraId);
		private:
			CalibrationFileWriter*				_calibrationFileWriter;		//!< The calibration file writer/reader
		};

		typedef boost::shared_ptr<PerspectiveCalibrationFileWriter>			CalDataPerspectivePtr;
		typedef boost::shared_ptr<const PerspectiveCalibrationFileWriter>		ConstCalDataPerspectivePtr;

	}; //namespace Calibration
}; //namespace Core

#include <poppack.h>
#endif // CALDATA2D_H
