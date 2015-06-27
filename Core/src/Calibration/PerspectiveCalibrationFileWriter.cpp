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

#include <QtXml>
#include "Calibration/PerspectiveCalibrationFileWriter.h"
#include "DebugOutput/DebugStream.h"
#include "Calibration/CalibrationFileWriter.h"
#include <QApplication>

namespace Core
{
namespace Calibration
{

	PerspectiveCalibrationFileWriter::PerspectiveCalibrationFileWriter( std::string calibrationFilePath /* = ""*/ )
	{
		if(calibrationFilePath.length() <= 0)
		{
			calibrationFilePath = qApp->applicationDirPath().toStdString() + "/Calibration/";
		}
		_calibrationFileWriter = new CalibrationFileWriter(calibrationFilePath);
	}

	PerspectiveCalibrationFileWriter::~PerspectiveCalibrationFileWriter( void )
	{
		delete _calibrationFileWriter;
		_calibrationFileWriter = nullptr;
	}

	bool PerspectiveCalibrationFileWriter::saveCalibration( const PerspectiveCalibrationData& data)
	{
		bool returnValue = false;
		if(_calibrationFileWriter)
		{
			char* bytes = data.toByte();
			returnValue = _calibrationFileWriter->save(data.cameraId, bytes, data.size(), PERSPECTIVE);
			delete[] bytes;
		}
		return returnValue;
	}

	bool PerspectiveCalibrationFileWriter::loadCalibration( PerspectiveCalibrationData& data, std::string cameraId )
	{
		bool returnValue = false;
		if(_calibrationFileWriter)
		{
			char* buffer = _calibrationFileWriter->load(cameraId, PERSPECTIVE);
			if(buffer)
			{
				data = PerspectiveCalibrationData(buffer);
				delete buffer;
				buffer = nullptr;
				returnValue = true;
			}
		}
		return returnValue;
	}

	bool PerspectiveCalibrationFileWriter::calibrationFileExists( std::string cameraId )
	{
		return _calibrationFileWriter->fileExists(cameraId);
	}

}
}
