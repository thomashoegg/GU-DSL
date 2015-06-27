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
#ifndef __ICAMERA_H__
#define __ICAMERA_H__
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/signals2.hpp>
#include "CoreDLL.h"
#include <qglobal.h>
#include "Plugin/Interfaces/IPluginObject.qt.h"
#include "DebugOutput/ModuleDebugLevel.qt.hpp"
#include <QObject>

namespace Container
{
	namespace Images
	{
		class Image;
		class Frame;
	};
};

namespace Viewer
{
	class GLWindow;
};

namespace Core
{
	namespace Processing
	{
		namespace Interfaces
		{
			class IObject;

			typedef boost::weak_ptr<IObject>	IObjectWeakPtr;
		};
	};

	namespace Buffer
	{
		template <class T> class RingBufferList;
	};

	namespace Camera
	{
		typedef boost::shared_ptr<boost::signals2::connection>			ConnectionPtr;
		typedef boost::shared_ptr<const boost::signals2::connection>	ConstConnectionPtr;
		typedef boost::weak_ptr<boost::signals2::connection>			ConnectionWeakPtr;

		class CameraInterface;
		struct CameraInformation;

		/**
		 * \brief Camera interface
		 *
		 */
		class CORE_API ICamera: public QObject, public Core::Plugin::Interfaces::IPluginObject
		{
			Q_OBJECT
			Q_PROPERTY(QString objectName READ objectName WRITE setObjectName DESIGNABLE false)
			Q_PROPERTY(QtWrapperModuleDebugLevelFlag DebugLevel READ getDebugLevel WRITE setDebugLevel)
			Q_FLAGS(QtWrapperModuleDebugLevelFlag)
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
			typedef boost::shared_ptr<ICamera> Ptr;
			typedef boost::shared_ptr<const ICamera> ConstPtr;
			typedef boost::weak_ptr<ICamera> WeakPtr;

		public:
			typedef boost::signals2::signal<void (Container::Images::Image)> signal_t;
			typedef boost::signals2::signal<void (int)> signal_t_status;
		protected:
			signal_t													onImageReceived;			//!< The image received event.
			signal_t_status												onStatus;					//!< The status received event.
			Core::DebugOutput::DebugLevel::ModuleDebugLevelFlag			_debugLevel;				//!< The module debug level.
			Core::Processing::Interfaces::IObjectWeakPtr				_cameraSource;				//!< The corresponding camera source
		public:
			/**
			 * \brief constructor
			 *
			 */
			ICamera();

			/**
			 * \brief pure virtual destructor.
			 *
			 */
			virtual ~ICamera() = 0;

			/**
			 * \brief connect to camera.
			 *
			 *	\param const Container::Images::CameraInformation& camInfo
			 */
			virtual bool connect(const CameraInformation& camInfo) = 0;

			/**
			 * \brief disconnect from camera.
			 *
			 */
			virtual void disconnect() = 0;

			/**
			 * \brief open camera data stream
			 *
			 */
			virtual void openDatastream();

			/**
			 * \brief get cameras information.
			 *
			 *	\param std::vector<Container::Images::CameraInformation>& _return
			 */
			virtual void getCamerasInformation( std::vector<CameraInformation>& _return ) = 0;

			/**
			 * \brief get frame.
			 *
			 * \param Container::Images::Frame& _return
			 * \param const bool remove
			 */
			virtual void getFrame( Container::Images::Frame& _return, const bool remove ) = 0;

			/**
			 * \brief Triggers the camera.
			 *
			 */
			virtual bool trigger();;

			/**
			 * \brief execute command.
			 *
			 * \param std::string& _return
			 * \param const std::string& command
			 * \param const std::string& value
			 */
			virtual void executeCommand( std::string& _return, const std::string& command, const std::string& value ) = 0;

			/**
			 * \brief get buffer list.
			 *
			 *	\return Core::Buffer::RingBufferList<Container::Images::Image>*
			 */
			virtual Core::Buffer::RingBufferList<Container::Images::Image>* getBufferList() = 0;
				
			/**
			 * \brief get frame count.
			 *
			 *	\return int
			 */
			virtual int getFrameCount() const = 0;

			/**
			 * \brief get ring buffer size.
			 *
			 *	\return int
			 */
			virtual int getRingBufferSize() const = 0;

			/**
			* \brief clears the ring buffer.
			*
			*/
			virtual void clearRingBuffer() = 0;

			/**
			 * \brief 
			 *
			 */
			virtual bool isConnected() const = 0;

			/**
			 * \brief
			 *
			 */
			virtual bool isRunning() const = 0;

			/**
			 * \brief
			 *
			 */
			virtual long startProcessing() = 0;

			/**
			 * \brief
			 *
			 */
			virtual long stopProcessing() = 0 ;

			/**
			 * \brief
			 *
			 */
			virtual long resetProcessing() = 0;

			/**
			 * \brief
			 *
			 */
			virtual void getType( std::string& _return ) = 0;

			/**
			 * \brief add dynamic properties to the camera manager
			 *
			 *	\param QObject* object. Pointer to the QObject the properties should be added to.
			 */
			virtual void addDynamicProperties(QObject* object);

			/**
			* \brief property changed.
			*
			*	\param const std::string& propertyName.
			*	\param const std::string& value. The new Value.
			*	\param Core::Camera::CameraInterface* const cameraInterface 
			*	\param const bool& dynamicProperty. Indicator if property is a dynamic property.
			*/
			virtual bool propertyChanged( const std::string& propertyName, const std::string& value, const bool& dynamicProperty , Core::Processing::Interfaces::IObject* const cameraInterface );

			/**
			* \brief operator +=
			*
			*	@param rhs boost signal slot type.
			*	@return boost connection smart pointer
			*/
			virtual ConnectionPtr operator+= (const signal_t::slot_type &rhs);

			/**
			* \brief connect on message received event.
			*
			*	@param rhs boost signal slot type.
			*	@return boost connection smart pointer
			*/
			virtual ConnectionPtr bind(const signal_t::slot_type &rhs);

			
			/**
			* \brief operator +=
			*
			*	@param rhs boost signal slot type.
			*	@return boost connection smart pointer
			*/
			virtual ConnectionPtr operator+= (const signal_t_status::slot_type &rhs);

			/**
			* \brief connect on message received event.
			*
			*	@param rhs boost signal slot type.
			*	@return boost connection smart pointer
			*/
			virtual ConnectionPtr bindStatus(const signal_t_status::slot_type &rhs);


			/**
			 * \brief get debug level
			 *
			 */
			virtual QtWrapperModuleDebugLevelFlag getDebugLevel();

			/**
			 * \brief set debug level
			 *
			 */
			virtual void setDebugLevel(QtWrapperModuleDebugLevelFlag value );

			/**
			* \brief checks if the module is in the given debug level
			*
			*	\param Core::DebugOutput::ModuleDebugLevel level. The given debug level to compare.
			*	\return bool. true if module is in the given debug level.
			*/
			inline bool isModuleDebugLevel(Core::DebugOutput::DebugLevel::ModuleDebugLevel level);

			/**
			 * \brief Sets the reference to the corresponding CameraSource implementation
			 *
			 *	\param CameraSourceWeakPtr cameraSource. A reference to the parent class
			 */
			void setCameraSource(Core::Processing::Interfaces::IObjectWeakPtr cameraSource);

		};
	}; //namespace Camera
}; //namespace Core
#endif
