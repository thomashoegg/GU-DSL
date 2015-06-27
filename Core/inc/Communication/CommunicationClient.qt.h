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
#ifndef __COMMUNICATIONCLIENT_H__
#define __COMMUNICATIONCLIENT_H__
#include "CoreDLL.h"
#include <QThread>
#include "Processing/Interfaces/IObject.qt.h"
#include <boost/bind.hpp>
#include <boost/signals2/signal.hpp>
#include "Communication/PtrDefinitions.h"
#include "Communication/Interfaces/ICommunicationInformation.h"


QT_BEGIN_NAMESPACE
class QThread;
class QMutex;
QT_END_NAMESPACE

namespace Core
{
	namespace Utilities
	{
		class WaitCondition;
	};
};

namespace Container
{
	namespace Images
	{
		class Image;
	};
};

namespace Core
{
	namespace Communication
	{
		typedef boost::shared_ptr<boost::signals2::connection>			ConnectionPtr;
		typedef boost::shared_ptr<const boost::signals2::connection>	ConstConnectionPtr;
		typedef boost::weak_ptr<boost::signals2::connection>			ConnectionWeakPtr;


		/**
		 * \brief Communication client
		 *
		 */
		class CORE_API CommunicationClient : public QObject
		{
			Q_OBJECT
			Q_PROPERTY(QString objectName READ objectName WRITE setObjectName DESIGNABLE false)
		public:
			typedef boost::signals2::signal<void (Container::Images::Image)> signal_t;
		public:
			/**
			 * \brief constructor
			 *
			 */
			CommunicationClient();

			/**
			 * \brief destructor
			 *
			 */
			~CommunicationClient();

			/**
			 * \brief init the Communication client.
			 *
			 */
			void init();

		protected:

			/**
			 * \brief connect to thrift.
			 *
			 *	@param data Communication information.
			 */
			virtual bool connect( const Core::Communication::Interfaces::ICommunicationInformation& data ) = 0;

			/**
			 * \brief disconnect from thrift.
			 *
			 */
			virtual void disconnect() = 0;

			/**
			 * \brief get image frame from thrift.
			 *
			 *	@param[out] _return image to return.
			 *	@param remove remove image from thrift.
			 */
			virtual void getImage( Container::Images::Image& _return, const bool remove ) = 0;

			/**
			 * \brief get frame count.
			 *
			 *    @return frame count.
			 */
			virtual int32_t getFrameCount() = 0;


			/**
			 * \brief get Communications information
			 *
			 */
			virtual void getCommunicationsInformation( std::vector<Core::Communication::Interfaces::ICommunicationInformation> & _return ) = 0;

			/**
			 * \brief reset.
			 *
			 */
			virtual void reset() = 0;

			/**
			 * \brief start
			 *
			 */
			virtual void start() = 0;

			/**
			 * \brief stop
			 *
			 */
			virtual void stop() = 0;

			/**
			 * \brief get type
			 *
			 */
			virtual void getType( std::string& _return ) = 0;

			/**
			 * \brief
			 *
			 */
			virtual void executeCommand( std::string& _return, const std::string& command, const std::string& value ) = 0;

			// Initialize the Client
			/**
			 * \brief run
			 *
			 * @return
			 */
			virtual bool run() = 0;

		public:

			/**
			* \brief operator +=
			*
			*	@param rhs boost signal slot type.
			*	@return boost connection smart pointer
			*/
			ConnectionPtr operator+= (const signal_t::slot_type &rhs);

			/**
			* \brief connect on message received event.
			*
			*	@param rhs boost signal slot type.
			*	@return boost connection smart pointer
			*/
			ConnectionPtr bind(const signal_t::slot_type &rhs);
		protected slots:
			void doWork();

		public slots:
			virtual void finished();
		protected:
			bool													_isRunning;												//<! Indicates if thread is running.
			QThread*												_thread;												//<! The Thread to get frames.
			Core::Utilities::WaitCondition*											_waitForDataCondition;									//<! The wait condition.
			QMutex*													_waitMutex;												//<! The wait mutex.
			
			signal_t												onImageReceived;										//!< The image received event.
		};

	}; //namespace Communication
}; //namespace Core
Q_DECLARE_INTERFACE(Core::Communication::CommunicationClient, "de.ce.CommunicationClient/0.1")
#endif		// __COMMUNICATIONCLIENT_H__
