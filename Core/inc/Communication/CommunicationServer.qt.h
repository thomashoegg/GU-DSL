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
#ifndef __COMMUNICATIONSERVER_H__
#define __COMMUNICATIONSERVER_H__
#include "Communication/CommunicationClientDetector.h"
#include "CoreDLL.h"
#include <QObject>
#include "Communication/PtrDefinitions.h"
#include "Plugin/Interfaces/IPluginObject.qt.h"
#include <vector>
#include "DebugOutput/ModuleDebugLevel.qt.hpp"

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
		class CORE_API CommunicationServer: public QObject, public Core::Plugin::Interfaces::IPluginObject 
		{
			Q_OBJECT
			Q_PROPERTY(Core::DebugOutput::DebugLevel::ModuleDebugLevelFlag DebugLevel READ getDebugLevel WRITE setDebugLevel)
			Q_FLAGS(Core::DebugOutput::DebugLevel::ModuleDebugLevelFlag)
		public:


			/**
			* \brief Constructor
			*
			* Constructor CommunicationServer()
			*
			* \return 
			*/
			CommunicationServer();

			/**
			* \brief Destructor
			*
			* Destructor ~CommunicationServer()
			*
			* \return 
			*/
			virtual ~CommunicationServer() = 0;


			/**
			* \brief Run-Method
			*
			* Implements the run method of QThread
			*
			* \return void
			*/
			virtual void run() = 0;

			/**
			* \brief Getter for Running flag
			*
			*
			* \return bool		The running flag
			*/
			bool isRunning() const;

			/**
			 * \brief get debug level
			 *
			 */
			virtual Core::DebugOutput::DebugLevel::ModuleDebugLevelFlag getDebugLevel();

			/**
			 * \brief set debug level
			 *
			 */
			virtual void setDebugLevel(Core::DebugOutput::DebugLevel::ModuleDebugLevelFlag value );

			/**
			* \brief checks if the module is in the given debug level
			*
			*	\param Core::DebugOutput::ModuleDebugLevel level. The given debug level to compare.
			*	\return bool. true if module is in the given debug level.
			*/
			inline bool isModuleDebugLevel(Core::DebugOutput::DebugLevel::ModuleDebugLevel level);

			/**
			 * \brief initialize thrift server
			 *
			 */
			virtual void init();

			/**
			 * \brief handle image
			 *
			 *	\param Container::Images::Image image. Image to handle.
			 */
			virtual void handleImage(const Container::Images::Image& image) = 0;
		protected slots:
			void doWork();
		public slots:
			void finished();
		public:
		protected:
			bool													_isRunning;						//!< Running Flag - true if running - else false 
			QThread*												_thread;						//<! The Thread to get frames.
			Core::Utilities::WaitCondition*											_waitForDataCondition;			//<! The wait condition.
			QMutex*													_waitMutex;						//<! The wait mutex.
			Core::DebugOutput::DebugLevel::ModuleDebugLevelFlag		_debugLevel;					//<! The module debug level.
			QMutex													_imageMutex;					//<! The image mutex.
			Container::Images::Image*								_currentImage;					//<! The current image.
			int														_port;							//<! The communication client port.
			Core::Communication::CommunicationClientDetector		_clientDetector;				//<! The communication client detector.
		};
	}; //namespace Communication
}; //namespace Core

#endif		// __COMMUNICATIONSERVER_H__
