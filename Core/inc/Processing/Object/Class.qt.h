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

#ifndef PROCESSING_CLASS_H
#define PROCESSING_CLASS_H

#ifdef WIN32
#pragma once
#endif

#include "CoreDLL.h"
#include <QObject>
#include <QFlags>
#include "boost/shared_ptr.hpp"
#include <unordered_map>
#include "Processing/Interfaces/IObject.qt.h"
#include "Processing/PtrDefinitions.h"
#include "DebugOutput/ModuleDebugLevel.qt.hpp"

namespace Core
{
	namespace Utilities
	{
		class StopWatch;
	};
};

namespace Core
{
	namespace Processing
	{
		namespace Object
		{
			class CORE_API Class : public Core::Processing::Interfaces::IObject
			{
				Q_OBJECT
				DECLARE_OBJECT(Class)
				Q_PROPERTY(QString objectName READ objectName WRITE setObjectName DESIGNABLE false)
				Q_PROPERTY(QtWrapperModuleDebugLevelFlag DebugLevel READ getDebugLevel WRITE setDebugLevel)
				Q_PROPERTY(bool EnableViewer READ getEnableViewer WRITE setEnableViewer)
				Q_PROPERTY(bool Enabled READ getEnabled WRITE setEnabled)
				Q_FLAGS(QtWrapperModuleDebugLevelFlag)
				
			public:
				typedef	std::unordered_map<std::string, Core::Processing::Interfaces::IPortPtr> PortMap;
				typedef PortMap::value_type PortMapPair;
			public:
				enum QtWrapperModuleDebugLevel: unsigned char
				{
					Off					=	0x00,
					Timing				=	0x01,
					Minimum				=	0x02,
					Basic				=	0x04,
					Advanced			=	0x08,
					InterfaceCalls		=	0x10,
					All					=	0xff
				};

				Q_DECLARE_FLAGS(QtWrapperModuleDebugLevelFlag, QtWrapperModuleDebugLevel)
			public:

				Class(QString uid);
				Class(const Class& rhs);
				Class(const Class& rhs, QString uid);

				virtual ~Class();

				virtual void	stop() {  /* Should be handled by every object */  }

				virtual bool	addPort(Core::Processing::Interfaces::IPortPtr port, QString portName);

				virtual bool	addInterface(QString portName, Core::Processing::Interfaces::IInterfacePtr interfacePtr);
				virtual bool	addAsyncInterface(QString portName, Core::Processing::Interfaces::IInterfacePtr interfacePtr);

				const PortMap   getPortMap() const;

				virtual void registerProcessorPortListeners();

				/**
				 * \brief get if image sink is enabled.
				 *
				 *	@return Indicates if module is enabled.
				 */
				virtual bool getEnabled();

				/**
				 * \brief set if module is enabled
				 *
				 *	\param bool enabled
				 */
				virtual void setEnabled(bool enabled);

				/**
				 * \brief Determines if the processing mode is event based
				 *
				 */
				bool getEventBased() const { return _isEventBased; }
			
				/**
				 * \brief Determines if the processing mode is event based
				 *
				 *	\param bool eventBased
				 */
				void setEventBased(bool eventBased) { _isEventBased = eventBased; }

				/**
				 * \brief default handle close event method.
				 *
				 */
				virtual void handleCloseEvent( const Core::Mediation::Events::CloseEvent& event );

			protected Q_SLOTS:

				virtual void	componentStopped() { /* Should be handled by every object */ };

			protected:

				virtual void registerProcessorPortListeners(Core::Processing::Interfaces::IPortPtr port) {/* throw std::logic_error("The method or operation is not implemented, but should be implemented in a subclass.");*/ }

				virtual void init() { throw std::logic_error("The method or operation is not implemented, but should be implemented in a subclass."); }

				virtual bool process(Core::Processing::Interfaces:: ConstIObjectParametersPtr parameters ) { throw std::logic_error("The method or operation is not implemented, but should be implemented in a subclass."); }

				virtual Class::QtWrapperModuleDebugLevelFlag getDebugLevel();
				virtual void setDebugLevel(Class::QtWrapperModuleDebugLevelFlag value );

				bool getEnableViewer();
				void setEnableViewer(bool value);

				/**
				 * \brief checks if the module is in the given debug level
				 *
				 *	\param Core::DebugOutput::ModuleDebugLevel level. The given debug level to compare.
				 *	\return bool. true if module is in the given debug level.
				 */
				inline bool isModuleDebugLevel(Core::DebugOutput::DebugLevel::ModuleDebugLevel level);

				/**
				 * \brief internal handle close event
				 *
				 */
				Q_INVOKABLE void doHandleCloseEvent(int exitCode);

			protected:
				PortMap													_ports;								//!< The port map.
				Core::Utilities::StopWatch*								_stopWatch;							//!< The stop watch to track the filter timing.
				Core::DebugOutput::DebugLevel::ModuleDebugLevelFlag		_debugLevel;						//!< The module debug level.
				bool													_enableViewer;						//!< Indicator if viewer are enabled.
				bool													_enabled;							//!< Indicates if module is enabled.
				bool													_isEventBased;						//!< Determines if the component runs in an event driven mode
			};

		};	// namespace Object
	};	// namespace Processing
}; //namespace Core
#endif	// PROCESSING_CLASS_H
