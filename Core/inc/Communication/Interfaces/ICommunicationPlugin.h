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
#ifndef __COMMUNICATIONPLUGIN_H__
#define __COMMUNICATIONPLUGIN_H__
#include <string>
#include "CoreDLL.h"
#include <QObject>
#include "Plugin/Interfaces/IPluginObject.qt.h"
#include <boost/shared_ptr.hpp>
#include "Communication/PtrDefinitions.h"
#include "Communication/Message.h"

namespace Core
{
	namespace Communication
	{
		struct CommunicationData;

		namespace Interfaces
		{
			class ICommunicationPlugin;

			typedef boost::shared_ptr<ICommunicationPlugin>				ICommunicationPluginPtr;
			typedef boost::shared_ptr<const ICommunicationPlugin>		ConstICommunicationPluginPtr;

			/**
			* \brief Communication structure to store Communication data.
			*
			*/
			class CORE_API ICommunicationPlugin: public QObject, public Core::Plugin::Interfaces::IPluginObject
			{
			public:
				ICommunicationPlugin();
				virtual ~ICommunicationPlugin() = 0;

				virtual bool isMessageFromPlugin(Core::Communication::Message& message) = 0;
				virtual void decodeMessage(Core::Communication::Message& message, Core::Communication::CommunicationDataPtr& communicationData) = 0;
				virtual Core::Communication::CommunicationClientPtr createCommunicationClient(Core::Communication::CommunicationDataPtr communicationData) = 0;
				virtual bool isEqual(Core::Communication::CommunicationDataPtr& communicationData1, Core::Communication::CommunicationDataPtr& communicationData2 ) = 0;
			};
		}; //namespace Interfaces
	}; //namespace Communication
}; // namespace Core
#endif //__COMMUNICATIONPLUGIN_H__
