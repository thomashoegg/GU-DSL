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


#pragma once;
#ifndef __COMMUNICATIONSERVERDETECTOR_H__
#define __COMMUNICATIONSERVERDETECTOR_H__
#include <vector>
#include <string>
#include <QObject>
#include "CoreDLL.h"
#include "Communication/CommunicationData.qt.h"
#include "Communication/Interfaces/ICommunicationPlugin.h"
#include "CoreDLL.h"

namespace Core
{
	namespace Communication
	{
		class Message;
		class UdpCommunication;
		/**
		 * \brief Communication detector module.
		 *		detects all available Communications.
		 *
		 */
		class CORE_API CommunicationServerDetector : public QObject
		{
			Q_OBJECT
		public:
		
		private:
			Core::Communication::UdpCommunication*										_udpCommunication;					//<! the udp communication module.
			Core::Communication::Message*												_hello;								//<! the broadcast hello message.
			Core::Communication::Message*												_bye;								//<! the broadcast bye message.
			std::vector<CommunicationDataPtr>											_CommunicationList;					//<! the Communication list.
			std::vector<Core::Communication::Interfaces::ICommunicationPluginPtr>		_communicationPlugins;				//<! the communication plugin list.
			bool																		_discardOtherCommunications;		//<! Determines if Communications from other systems then the current host are discarded
		private:
			/**
			 * \brief copy constructor
			 *	disabled
			 *	
			 */
			CommunicationServerDetector(CommunicationServerDetector const &); //don't implement

			/**
			 * \brief assignment operator
			 *	disabled
			 */
			void operator=(CommunicationServerDetector const&); //don't implement
		
			/**
			 * \brief constructor.
			 *
			 */
			CommunicationServerDetector();
		protected:
			/**
			 * \brief handle received messages
			 *
			 *	@param message received message.
			 */
			void handleMessage(Core::Communication::Message message);
		public:
			static CommunicationServerDetector& getInstance()
			{
				static CommunicationServerDetector instance;	// Guaranteed to be destroyed.
												// Instantiated on first use.
				return instance;
			}

			/**
			 * \brief destructor
			 *
			 */
			~CommunicationServerDetector();

			/**
			* \brief Determines if other Communications are discared during registration
			*
			* \param  bool discard Discard other Communications during registration
			*/
			void setDiscardOtherCommunications(bool discard) { _discardOtherCommunications = discard; }

			/**
			* \brief Returns if other Communications are discared during registration
			*
			* \return bool True, if other Communications are discarded during registration
			*/
			bool isDiscardOtherCommunications() const { return _discardOtherCommunications; }

			/**
			 * \brief starts the Communication detector module.
			 *
			 */
			void start();

			/**
			 * \brief stops the Communication detector module.
			 *
			 */
			void stop();

			/**
			 * \brief get all detected Communications.
			 *
			 */
			std::vector<CommunicationDataPtr> CommunicationList();

			/**
			 * \brief Clean up all Communication clients
			 *
			 */
			void cleanupCommunicationClients();
		};
	};
};

#endif // __COMMUNICATIONSERVERDETECTOR_H__
