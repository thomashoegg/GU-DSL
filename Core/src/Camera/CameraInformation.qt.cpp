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

#include "Camera/CameraInformation.qt.h"


namespace Core
{
	namespace Camera
	{
		CameraInformation::CameraInformation( std::string ip /*= ""*/, std::string mac /*= ""*/, std::string name /*= ""*/, unsigned short port /*= 0*/, std::string uid /*= ""*/,
			std::string vendor /*= ""*/, Container::Images::CameraType type /*= Container::Images::CameraType::UNKNOWN*/, std::string cameraId /*= ""*/, std::string description /*= ""*/ )
			: IP(ip),
			MAC(mac),
			Name(name),
			Port(port),
			UID(uid),
			Vendor(vendor),
			Type(type),
			CameraId(cameraId),
			Description(description)
		{
		}

		CameraInformation::CameraInformation( const CameraInformation& rhs )
			: IP(rhs.IP),
			MAC(rhs.MAC),
			Name(rhs.Name),
			Port(rhs.Port),
			UID(rhs.UID),
			Vendor(rhs.Vendor),
			Type(rhs.Type),
			CameraId(rhs.CameraId),
			Description(rhs.Description)
		{

		}

		CameraInformation::~CameraInformation()
		{

		}

		bool CameraInformation::operator==( const CameraInformation& rhs )
		{
			return IP.compare(rhs.IP) == 0 && MAC.compare(rhs.MAC) == 0 && Name.compare(rhs.Name) == 0
				&& Port == rhs.Port && UID.compare(rhs.UID) == 0 && Vendor.compare(rhs.Vendor) == 0
				&& Type == rhs.Type && CameraId.compare(rhs.CameraId) == 0 && Description.compare(rhs.Description) == 0;
		}

		CameraInformation& CameraInformation::operator=( const CameraInformation& rhs )
		{
			IP = rhs.IP;
			MAC = rhs.MAC;
			Name = rhs.Name;
			Port = rhs.Port;
			UID = rhs.UID;
			Vendor = rhs.Vendor;
			Type = rhs.Type;
			CameraId = rhs.CameraId;
			Description = rhs.Description;
			return *this;
		}

		bool CameraInformation::isEmpty()
		{
			return (*this) == CameraInformation();
		}

	};
};
