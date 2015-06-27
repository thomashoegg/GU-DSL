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

#include "Communication/CommunicationData.qt.h"

namespace Core
{
	namespace Communication
	{
		CommunicationData::CommunicationData()
			:tag(boost::uuids::nil_uuid()),
			typeIndex(typeid(nullptr))
		{
			description = "";
		}

		CommunicationData::CommunicationData( std::type_index typeId, std::string ip, std::string description )
			: tag(boost::uuids::random_generator()()),
			typeIndex(typeId),
			ip(ip),
			description(description)
		{

		}

		CommunicationData::CommunicationData( CommunicationData const& rhs )
			: tag(rhs.tag),
			typeIndex(rhs.typeIndex),
			description(rhs.description),
			communicationClient(rhs.communicationClient)
		{}

		bool CommunicationData::operator==( const CommunicationData& rhs )
		{
			return (!tag.is_nil() && !rhs.tag.is_nil()) ? tag == rhs.tag : typeIndex == rhs.typeIndex && ip == rhs.ip && description == rhs.description;
		}

		CommunicationData& CommunicationData::operator=( const CommunicationData& rhs )
		{
			tag = rhs.tag;
			typeIndex = rhs.typeIndex;
			description = rhs.description;
			return *this;
		}

		void CommunicationData::generateTag()
		{
			if(tag == boost::uuids::nil_uuid())
			{
				tag = boost::uuids::random_generator()();
			}
		}
	};

};

