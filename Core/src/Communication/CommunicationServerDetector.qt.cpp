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

#include "Communication/UdpCommunication.h"
#include "Communication/Message.h"
#include "Communication/CommunicationServerDetector.qt.h"
#include "Communication/MessageType.hpp"
#include <boost/asio.hpp>
#include "DebugOutput/DebugStream.h"
#include "Plugin/Factory/PluginFactoryManager.qt.h"
#include "Communication/CommunicationClient.qt.h"
#include "Mediation/RuntimeEventBus.h"
#include "Mediation/Events/NewCommunicationDetectedEvent.h"
#include "Mediation/Events/RemoveCommunicationDetectedEvent.h"

namespace Core
{
	namespace Communication
	{
		CommunicationServerDetector::CommunicationServerDetector()
			:_udpCommunication(new Core::Communication::UdpCommunication(9650,9655, boost::asio::ip::address_v4::broadcast(), true))
		{
			unsigned int hello = (unsigned int)Core::Communication::MessageType::Hello;
			unsigned int bye = (unsigned int)Core::Communication::MessageType::Bye;
			char buffer[4];
			memcpy(buffer,&hello,4);
			_hello = new Core::Communication::Message(buffer, 4);
			memcpy(buffer,&bye,4);
			_bye = new Core::Communication::Message(buffer, 4);
			_udpCommunication->connect(boost::bind(&CommunicationServerDetector::handleMessage, this, _1));

			Core::Plugin::Factory::PluginFactoryManager::instance().loadPlugins();
			std::vector<Core::Plugin::Interfaces::IPluginObjectPtr> plugins = Core::Plugin::Factory::PluginFactoryManager::instance().create(typeid(Core::Communication::Interfaces::ICommunicationPlugin));
			for(Core::Plugin::Interfaces::IPluginObjectPtr plugin : plugins)
			{
				Core::Communication::Interfaces::ICommunicationPluginPtr communicationPlugin = boost::dynamic_pointer_cast<Core::Communication::Interfaces::ICommunicationPlugin>(plugin);
				_communicationPlugins.push_back(communicationPlugin);
			}
		}

		CommunicationServerDetector::~CommunicationServerDetector()
		{
			delete _udpCommunication;
			_communicationPlugins.clear();
			delete _hello;
			_hello = nullptr;
			delete _bye;
			_bye = nullptr;
		}

		void CommunicationServerDetector::start()
		{
			_udpCommunication->start();
			_udpCommunication->broadcast(*_hello);
		}

		void CommunicationServerDetector::stop()
		{
			_udpCommunication->broadcast(*_bye);
			_udpCommunication->stop();
		}

		void CommunicationServerDetector::handleMessage( Core::Communication::Message message )
		{
			D_OUT("HandleMessage:");

			CommunicationDataPtr Communication = nullptr;
			std::vector<std::string> localIps = _udpCommunication->getLocalIpAddresses();

			for(Core::Communication::Interfaces::ICommunicationPluginPtr plugin: _communicationPlugins)
			{
				if(plugin->isMessageFromPlugin(message))
				{
					plugin->decodeMessage(message, Communication);

					if(Communication)
					{
						Communication->ip = message.sender();
						unsigned int messagetype = 0;
						memcpy(&messagetype, message.body(), 4);

						std::vector<CommunicationDataPtr>::iterator item = _CommunicationList.begin();
						for( ; item != _CommunicationList.end(); ++item)
						{
							if( plugin->isEqual((*item), Communication))
							{
								break;
							}
						}

						if(messagetype == (unsigned int)Core::Communication::MessageType::Hello)
						{
							if(item == _CommunicationList.end() ) // new Communication found
							{
								bool discard = false;
								if(_discardOtherCommunications)
								{
									// Discard all Communications from foreign systems
									if(std::find(localIps.begin(), localIps.end(), Communication->ip) == localIps.end())
										discard = true;
								}

								if(!discard)
								{
									Communication->generateTag();
									Communication->communicationClient = plugin->createCommunicationClient(Communication);

									//TODO: get Communication information
									/*std::vector<Container::Images::CommunicationInformation> caminfo;
									Communication.CommunicationClient->getCommunicationsInformation(caminfo);*/

									_CommunicationList.push_back(Communication);
									Core::Mediation::RuntimeEventBus::getInstance().postEvent(Core::Mediation::Events::NewCommunicationDetectedEvent(Communication));
								}
							}
						}
						else if(messagetype == (unsigned int)Core::Communication::MessageType::Bye)
						{
							if(item != _CommunicationList.end()) // Communication to remove found
							{
								Core::Mediation::RuntimeEventBus::getInstance().postEvent(Core::Mediation::Events::RemoveCommunicationDetectedEvent(*item));
								_CommunicationList.erase(item);
							}
						}
					}
					break;
				}
			}
		}

		std::vector<CommunicationDataPtr> CommunicationServerDetector::CommunicationList()
		{
			return _CommunicationList;
		}

		void CommunicationServerDetector::cleanupCommunicationClients()
		{
			for(std::vector<CommunicationDataPtr>::iterator it = _CommunicationList.begin(); it != _CommunicationList.end(); it++)
			{
				if(Core::Communication::CommunicationClientPtr client = (*it)->communicationClient)
				{
					client->finished();
				}
			}
		}

	};
};
