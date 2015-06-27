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

#ifndef PROCESSING_IOBJECT_H
#define PROCESSING_IOBJECT_H

#ifdef WIN32
#pragma once
#endif

#include "CoreDLL.h"

#include <QObject>
#include <QUuid>
#include <QString>
#include <QVariant>
#include <unordered_map>
#include "boost/shared_ptr.hpp"

#include "Processing/PtrDefinitions.h"
#include "Processing/Object/ObjectFactory.qt.h"
#include "Mediation/EventListeners/BasicEventListener.h"

namespace Core
{
	namespace Processing
	{
		namespace Interfaces
		{
			class IObjectParameters;

			class CORE_API IObject : public QObject
			{
				Q_OBJECT
			public:

				IObject(QString uid);
				IObject(const IObject& rhs);
				IObject(const IObject& rhs, QString uid);

				virtual ~IObject();

				virtual void init() = 0;
				virtual IObjectPtr create(QString uid = "") const = 0;
				virtual bool process(ConstIObjectParametersPtr parameters) = 0;

				const QString uid() const;
				const QString templateUid() const;

				/**
				* \brief property changed.
				*
				*	\param const std::string& propertyName.
				*	\param const std::string& value. The new Value.
				*	\param const bool& dynamicProperty. Indicator if property is a dynamic property.
				*/
				Q_INVOKABLE virtual bool propertyChanged(const std::string& propertyName, const std::string& value, const bool& dynamicProperty );

				/**
				 * \brief get property group of a property.
				 *
				 *	\param QString propertyName. The property name
				 *	\return QString The property group.
				 */
				Q_INVOKABLE virtual QString getPropertyGroup(QString propertyName);

				/**
				 * \brief add property group to the property group list.
				 *
				 *	\param QString propertyName. The property name.
				 *	\param QString propertyGroup. The property group.
				 */
				virtual void addPropertyGroup(QString propertyName, QString propertyGroup);

				/**
				 * \brief send the add dynamic property signal.
				 *
				 *	\param const QString& name. The property name.
				 *	\param const QVariant& value. The property value.
				 *	\param const QString& propertyGroup. The property group.
				 *	\param const bool buldUpdate. Add property is in a bulk update.
				 */
				Q_INVOKABLE void sendAddDynamicProperty(const QString &name, const QVariant &value, const QString& propertyGroup, const bool bulkUpdate = false);

				/**
				 * \brief send the update dynamic property value signal.
				 *
				 *	\param const QString& name. The property name.
				 *	\param const QVariant& value. The property value.
				 *	\param const bool sendChangedEvent. Send the proeperty changed event.
				 *	\param const bool buldUpdate. Update is in a bulk update.
				 */
				Q_INVOKABLE void sendUpdatePropertyValue(const QString& name, const QVariant& value, const bool bulkUpdate = false, const bool sendChangedEvent = false);

				/**
				 * \brief commit a running property bulk update
				 *
				 */
				Q_INVOKABLE void sendCommitPropertyBulkUpdate();

				/**
				 * \brief default handle select item event method.
				 *
				 */
				virtual void handleSelectItemEvent( const Core::Mediation::Events::SelectItemEvent& event );

				/**
				 * \brief default handle close event method.
				 *
				 */
				virtual void handleCloseEvent( const Core::Mediation::Events::CloseEvent& event );

				/**
				 * \brief method to initialize object after object is loaded from diagram file
				 *
				 */
				Q_INVOKABLE virtual void onDiagramObjectLoaded();

				/**
				 * \brief set property initialization value
				 *
				 *	\param std::string propertyName The property name.
				 *	\param QVariant value The property value.
				 */
				Q_INVOKABLE void setPropertyInitializationValue(std::string propertyName, QVariant value);

				/**
				 * \brief get property initialization value
				 *
				 *	\param std::string propertyName The property name.
				 *	\return QVariant The property initialization value.
				 */
				QVariant getPropertyInitializationValue(std::string propertyName);

				/**
				 * \brief returns if object is selected.
				 *
				 *	\return bool true if object is selected.
				 */
				Q_INVOKABLE bool isSelected();

			Q_SIGNALS:
				/**
				 * \brief send the add dynamic property signal.
				 *
				 *	\param const QString& name. The property name.
				 *	\param const QVariant& value. The property value.
				 *	\param const QString& propertyGroup. The property group.
				 *	\param const bool buldUpdate. Add property is in a bulk update.
				 */
				void addDynamicProperty(const QString &name, const QVariant &value, const QString& propertyGroup, const bool bulkUpdate = false);

				/**
				 * \brief send the update dynamic property value signal.
				 *
				 *	\param const QString& name. The property name.
				 *	\param const QVariant& value. The property value.
				 *	\param const bool sendChangedEvent. Send the property changed event.
				 *	\param const bool buldUpdate. Update is in a bulk update.
				 */
				void updatePropertyValue(const QString& name, const QVariant& value, const bool bulkUpdate = false, const bool sendChangedEvent = false);

				/**
				 * \brief commit a running property bulk update
				 *
				 */
				void commitPropertyBulkUpdate();

			public:
				virtual void registerProcessorPortListeners() { /*throw std::logic_error("Only class objects are permit to call the method registerProcessorPortListeners.");*/ };
			protected:
				QUuid														_uid;
				QUuid														_templateUid;
				std::unordered_map<std::string, std::string>				_propertyGroupMap;				//!< Map to group properties.
				bool														_disablePropertyUpdate;			//!< Disable the dynamic property update.
				Core::Mediation::EventListeners::BasicEventListener			_basicEventListener;			//!< The select item event listener.
				bool														_isSelected;					//!< Indicator if this object ist selected.
				std::unordered_map<std::string, QVariant>					_propertyInitializationValue;	//!< The property initialization value.
			};
		};	// namespace Object
	};	// namespace Processing
}; //namespace Core

Q_DECLARE_METATYPE(Core::Processing::Interfaces::IObjectPtr)


#ifdef DECLARE_OBJECT
#undef DECLARE_OBJECT
#endif
#define DECLARE_OBJECT(ObjectType)																											\
		public: static Core::Processing::Interfaces::IObjectPtr createObject(QString uid)													\
		{																																	\
			return boost::shared_ptr<ObjectType>(new ObjectType(uid));																		\
		}																																	\
																																			\
		public: static Core::Processing::Interfaces::IObjectPtr createObject()																\
		{																																	\
			return boost::shared_ptr<ObjectType>(new ObjectType(""));																		\
		}																																	\
																																			\
		public: static Core::Processing::Interfaces::IObjectPtr createObject(const ObjectType& parent, QString uid)							\
		{																																	\
			Core::Processing::Interfaces::IObjectPtr createdObject = boost::shared_ptr<ObjectType>(new ObjectType(parent, uid));			\
			createdObject->registerProcessorPortListeners();																					\
			Core::Processing::Object::ObjectFactory::instance()->emitObjectCreated(createdObject);											\
			return createdObject;																											\
		}																																	\
																																			\
		public: static Core::Processing::Interfaces::IObjectPtr createObject(const ObjectType& parent)										\
		{																																	\
			Core::Processing::Interfaces::IObjectPtr createdObject = boost::shared_ptr<ObjectType>(new ObjectType(parent));					\
			createdObject->registerProcessorPortListeners();																					\
			Core::Processing::Object::ObjectFactory::instance()->emitObjectCreated(createdObject);											\
			return createdObject;																											\
		}																																	\
																																			\
		public: virtual Core::Processing::Interfaces::IObjectPtr create(QString uid = "")	const											\
		{																																	\
			return ObjectType::createObject(*this, uid);																					\
		}

#define DECLARE_OBJECT_FULL(ObjectType)																										\
	DECLARE_OBJECT(ObjectType)																												\
		protected:																															\
			virtual void init() {}																											\
			virtual bool process( Core::Processing::Interfaces::ConstIObjectParametersPtr parameters ) { throw std::logic_error("The method or operation is not implemented."); }


#endif	// PROCESSING_IOBJECT_H
