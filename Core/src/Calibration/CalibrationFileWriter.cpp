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

#include "Calibration/CalibrationFileWriter.h"
#include "DebugOutput/DebugStream.h"
#include <QDir>
#include <QFileInfo>

namespace Core
{
namespace Calibration
{

	CalibrationFileWriter::CalibrationFileWriter( std::string calibrationFilePath )
	{
		_calibrationFilePath = calibrationFilePath;
	}

	CalibrationFileWriter::~CalibrationFileWriter( void )
	{

	}

	bool CalibrationFileWriter::save( const std::string& filename, char* data, std::size_t size, calibrationType type ) const
	{
		bool returnValue = false;
		if(filename.length() > 0)
		{
			std::string absoluteFileName = _calibrationFilePath + filename + CCF_FILE_EXTENSION;
			QFileInfo fileInfo(absoluteFileName.c_str());
			QDir directory(fileInfo.absoluteDir());
			if(!directory.exists())
			{
				directory.mkpath(directory.absolutePath());
			}

			std::ios_base::open_mode mode = std::ios_base::in | std::ios_base::binary | std::ios_base::out | std::ios_base::trunc;
			std::fstream file(absoluteFileName, mode);

			file.seekg(0, std::ios::beg);
			FileHeader fileHeader(type, size);
			file.write((char*) &fileHeader, sizeof(FileHeader));
			file.write(data, size);
			file.flush();
			file.close();
			returnValue = true;
		}
		else
		{
			D_OUT2(__FUNCTION__, "Could not save file. The filename is empty");
		}
		return returnValue;
	}

	char* CalibrationFileWriter::load( const std::string& filename, const calibrationType type )
	{
		char* returnValue = nullptr;
		if(filename.length() > 0)
		{
			std::string absoluteFileName = _calibrationFilePath + filename + CCF_FILE_EXTENSION;
			std::ios_base::open_mode mode = std::ios_base::in | std::ios_base::binary;
			std::fstream file(absoluteFileName, mode);

			file.tellg();
			VersionHeader versionHeader;
			file.read((char*)&versionHeader, sizeof(VersionHeader));
			if(versionHeader.signature == CCF_FILEHEADER_SIGNATURE)
			{
				file.seekg(0, std::ios::beg);
				switch(versionHeader.version)
				{
				case 1:
					FileHeader fileHeader;
					file.read((char*)&fileHeader, sizeof(FileHeader));
					if(fileHeader.type == type)
					{
						returnValue = new char[fileHeader.dataSize];
						file.read(returnValue, fileHeader.dataSize);
					}
					else
					{
						D_OUT2(__FUNCTION__, "Calibration type does not match!");
					}
					break;
				}
			}
			file.close();
		}
		else
		{
			D_OUT2(__FUNCTION__, "Could not load file. The filename is empty");
		}
		return returnValue;
	}

	bool CalibrationFileWriter::fileExists( const std::string& filename )
	{
		bool returnValue = false;
		if(filename.length() > 0)
		{
			std::string absoluteFileName = _calibrationFilePath + filename + CCF_FILE_EXTENSION;
			QFileInfo fileInfo(absoluteFileName.c_str());

#if QT_VERSION > 0x050100
			returnValue = fileInfo.exists(absoluteFileName.c_str());
#else
			returnValue = fileInfo.exists();
#endif
		}
		return returnValue;
	}

	std::string CalibrationFileWriter::calibrationFilePath()
	{
		return _calibrationFilePath;
	}

	void CalibrationFileWriter::setCalibrationFilePath( const std::string calibrationFilePath )
	{
		_calibrationFilePath = calibrationFilePath;
	}

}
}
