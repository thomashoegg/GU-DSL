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
#ifndef __CAMERAINFORMATION_H__
#define __CAMERAINFORMATION_H__
#include "CoreDLL.h"
#include <string>
#include <QObject>
#include "Images/CameraType.h"

namespace Core
{
	namespace Camera
	{
		struct CORE_API CameraInformation : public QObject
		{
			Q_OBJECT
		public:
			/**
			 * \brief constructor
			 *
			 */
			CameraInformation(std::string ip = "", std::string mac = "", std::string name = "", unsigned short port = 0,
				std::string uid = "", std::string vendor = "", Container::Images::CameraType type = Container::Images::CameraType::UNKNOWN, std::string cameraId = "", std::string description = "");

			/**
			 * \brief copy constructor
			 *
			 */
			CameraInformation(const CameraInformation& rhs);

			/**
			 * \brief destructor
			 *
			 */
			~CameraInformation();

			/**
			* \brief comparison operator
			*
			*/
			bool operator==(const CameraInformation& rhs);

			/**
			* \brief assignment operator
			*
			*/
			CameraInformation& operator=(const CameraInformation& rhs);

			/**
			 * \brief check if camera information contains data.
			 *
			 */
			bool isEmpty();

			std::string IP;
			std::string MAC;
			std::string Name;
			unsigned short Port;
			std::string UID;
			std::string Vendor;
			Container::Images::CameraType Type;
			std::string CameraId;
			std::string Description;
		};
	};
};

Q_DECLARE_METATYPE(Core::Camera::CameraInformation)

namespace
{
	void RegisterCameraInformation()	
	{
		qRegisterMetaType<Core::Camera::CameraInformation>();
		qRegisterMetaType<Core::Camera::CameraInformation>("CameraInformation");
	}

	Q_CONSTRUCTOR_FUNCTION(RegisterCameraInformation)
}

#endif // __CAMERAINFORMATION_H__
