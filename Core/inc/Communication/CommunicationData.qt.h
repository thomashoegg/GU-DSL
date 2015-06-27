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
#ifndef __COMMUNICATIONDATA_H__
#define __COMMUNICATIONDATA_H__
#include <string>
#include <QObject>
#include <QMetaType>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <typeindex>
#include "CoreDLL.h"
#include "Communication/PtrDefinitions.h"

namespace Core
{
	namespace Communication
	{
		/**
		* \brief Communication structure to store Communication data.
		*
		*/
		struct CORE_API CommunicationData : public QObject
		{
			Q_OBJECT
		public:
			boost::uuids::uuid				tag;						//<! unique object id.
			std::type_index					typeIndex;					//<! the communication type index.
			std::string						ip;							//<! the communication ip.
			std::string						description;				//<! description of the Communication.
			CommunicationClientPtr			communicationClient;		//<! the communication client.

			/**
			* \brief default constructor.
			*
			*/
			CommunicationData();
			
			/**
			* \brief constructor.
			*
			*	@param typeId. the type of the communication client.
			*	@param ip ip address of the Communication.
			*	@param description description of the Communication.
			*/
			CommunicationData(std::type_index typeId, std::string ip, std::string description);

			/**
			* \brief Copy constructor.
			*
			*	@param rhs CommunicationData to copy.
			*/
			CommunicationData(CommunicationData const& rhs);

			/**
			* \brief comparison operator
			*
			*/
			bool operator==(const CommunicationData& rhs);

			/**
			* \brief assignment operator
			*
			*/
			CommunicationData& operator=(const CommunicationData& rhs);

			/**
			 * \brief generate tag if not already exists.
			 *
			 */
			void generateTag();
		};	
	}
};
Q_DECLARE_METATYPE(Core::Communication::CommunicationData)
Q_DECLARE_METATYPE(Core::Communication::CommunicationDataPtr)

namespace
{
	void RegisterCommunicationData()	
	{
		qRegisterMetaType<Core::Communication::CommunicationDataPtr>();
		qRegisterMetaType<Core::Communication::CommunicationDataPtr>("CommunicationDataPtr");
		qRegisterMetaType<Core::Communication::CommunicationData>();
		qRegisterMetaType<Core::Communication::CommunicationData>("CommunicationData");
	}

	Q_CONSTRUCTOR_FUNCTION(RegisterCommunicationData)
}

#endif //__COMMUNICATIONDATA_H__
