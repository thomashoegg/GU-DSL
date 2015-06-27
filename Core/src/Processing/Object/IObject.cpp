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


#include "CoreDLL.h"

#include "Processing/PtrDefinitions.h"
#include <QMetaType>

#include "Processing/Interfaces/IObject.qt.h"
#include <boost/bind.hpp>
#include "Mediation/RuntimeEventBus.h"
#include <QVariant>
#include <QThread>
#include "DebugOutput/DebugStream.h"


namespace Core
{
	namespace Processing
	{
		namespace Interfaces
		{
			IObject::IObject(QString uid)
				:_disablePropertyUpdate(false),
				_isSelected(false),
				_basicEventListener()
			{ 
				uid.length() > 0 ? _uid = QUuid(uid) : _uid = QUuid::createUuid();
				_basicEventListener.setSelectItemEventHandle(boost::bind(&IObject::handleSelectItemEvent, this, _1));
				_basicEventListener.setCloseEventHandle(boost::bind(&IObject::handleCloseEvent, this, _1));
				Core::Mediation::RuntimeEventBus::getInstance().registerListener(&_basicEventListener);
			}

			IObject::IObject(const IObject& rhs)
				:_disablePropertyUpdate(rhs._disablePropertyUpdate),
				_isSelected(false),
				_basicEventListener()
			{
				_uid = QUuid::createUuid();
				_templateUid = rhs.templateUid();
				setObjectName(rhs.objectName());
				_basicEventListener.setSelectItemEventHandle(boost::bind(&IObject::handleSelectItemEvent, this, _1));
				_basicEventListener.setCloseEventHandle(boost::bind(&IObject::handleCloseEvent, this, _1));
				Core::Mediation::RuntimeEventBus::getInstance().registerListener(&_basicEventListener);
				_propertyInitializationValue.insert(rhs._propertyInitializationValue.begin(), rhs._propertyInitializationValue.end());
			}

			IObject::IObject( const IObject& rhs, QString uid )
				:_disablePropertyUpdate(rhs._disablePropertyUpdate),
				_isSelected(false),
				_basicEventListener()
			{
				uid.length() > 0 ? _uid = QUuid(uid) : _uid = QUuid::createUuid();
				_templateUid = rhs.templateUid();
				setObjectName(rhs.objectName());
				_basicEventListener.setSelectItemEventHandle(boost::bind(&IObject::handleSelectItemEvent, this, _1));
				_basicEventListener.setCloseEventHandle(boost::bind(&IObject::handleCloseEvent, this, _1));
				Core::Mediation::RuntimeEventBus::getInstance().registerListener(&_basicEventListener);
				_propertyInitializationValue.insert(rhs._propertyInitializationValue.begin(), rhs._propertyInitializationValue.end());
			}

			IObject::~IObject()
			{
				Core::Mediation::RuntimeEventBus::getInstance().unregisterListener(&_basicEventListener);
				_basicEventListener.removeCloseEventHandle();
				_basicEventListener.removeSelectItemEventHandle();
			}

			const QString IObject::uid() const
			{ 
				return _uid.toString();
			}

			const QString IObject::templateUid() const
			{
				return _templateUid.isNull() ? _uid.toString() : _templateUid.toString();
			}		
			
			
			//void IObject::setTemplateId( const QString& templUid )
			//{
			//	if(templUid.length() > 0 && templUid.compare(uid()) != 0)
			//	{
			//		_templateUid = QUuid(templUid);	
			//	}
			//}

			QString IObject::getPropertyGroup(QString propertyName)
			{
				std::unordered_map<std::string, std::string>::iterator item = _propertyGroupMap.find(propertyName.toStdString());
				if(item != _propertyGroupMap.end())
				{
					return QString::fromStdString(item->second);
				}
				return "";
			}

			void IObject::addPropertyGroup( QString propertyName, QString propertyGroup )
			{
				if(propertyGroup.length() > 0 && propertyName.length() > 0)
				{
					_propertyGroupMap.insert(std::pair<std::string, std::string>(propertyName.toStdString(), propertyGroup.toStdString()));
				}
			}

			void IObject::sendAddDynamicProperty( const QString &name, const QVariant &value, const QString& propertyGroup,  const bool bulkUpdate /*= false*/ )
			{
				if(this->thread() == QThread::currentThread())
				{
					addPropertyGroup(name, propertyGroup);
					emit addDynamicProperty(name, value, propertyGroup, bulkUpdate);
				}
				else
				{
					QMetaObject::invokeMethod(this, "sendAddDynamicProperty", Qt::AutoConnection, Q_ARG(const QString&, name), Q_ARG(const QVariant&, value),  Q_ARG(const QString&, propertyGroup), Q_ARG(const bool, bulkUpdate));
				}
			}

			bool IObject::propertyChanged( const std::string& propertyName, const std::string& value, const bool& dynamicProperty )
			{
				return false;
			}

			void IObject::sendUpdatePropertyValue( const QString& name, const QVariant& value, const bool bulkUpdate /*= false*/, const bool sendChangedEvent /*= false*/ )
			{
				if(!_disablePropertyUpdate)
				{
					if(this->thread() == QThread::currentThread())
					{
						emit updatePropertyValue(name, value, bulkUpdate, sendChangedEvent);
					}
					else
					{
						QMetaObject::invokeMethod(this, "sendUpdatePropertyValue", Qt::AutoConnection, Q_ARG(const QString&, name), Q_ARG(const QVariant&, value), Q_ARG(const bool, bulkUpdate), Q_ARG(const bool, sendChangedEvent));
					}
					
				}
			}

			void IObject::sendCommitPropertyBulkUpdate()
			{
				if(!_disablePropertyUpdate)
				{
					if(this->thread() == QThread::currentThread())
					{
						emit commitPropertyBulkUpdate();
					}
					else
					{
						QMetaObject::invokeMethod(this, "sendCommitPropertyBulkUpdate", Qt::AutoConnection);
					}
				}
			}


			void IObject::handleSelectItemEvent( const Core::Mediation::Events::SelectItemEvent& event )
			{
				Core::Mediation::Events::SelectItemEvent selectItemEvent = const_cast<Core::Mediation::Events::SelectItemEvent&>(event);
				if(selectItemEvent.getObject().get() == this)
				{
					_isSelected = true;
				}
				else
				{
					_isSelected = false;
				}
			}


			void IObject::handleCloseEvent( const Core::Mediation::Events::CloseEvent& event )
			{

			}

			Q_INVOKABLE void IObject::onDiagramObjectLoaded()
			{}

			Q_INVOKABLE void IObject::setPropertyInitializationValue( std::string propertyName, QVariant value )
			{
				std::unordered_map<std::string, QVariant>::iterator it = _propertyInitializationValue.find(propertyName);
				if( it == _propertyInitializationValue.end())
				{
					_propertyInitializationValue.insert(std::pair<std::string, QVariant>(propertyName, value));
				}
				else
				{
					D_OUT("property initialization value already set. Overriding old value");
					it->second = value;
				}
			}

			QVariant IObject::getPropertyInitializationValue( std::string propertyName )
			{
				QVariant returnValue;
				std::unordered_map<std::string, QVariant>::iterator it = _propertyInitializationValue.find(propertyName);
				if( it != _propertyInitializationValue.end())
				{
					returnValue = it->second;
				}
				return returnValue;
			}

			Q_INVOKABLE bool IObject::isSelected()
			{
				return _isSelected;
			}

		}; //namespace Interfaces
	}; //namespace Processing
}; //namespace Core

namespace
{
	void RegisterTypes()	
	{
		qRegisterMetaType<Core::Processing::Interfaces::IObjectPtr>();
	}
	
	Q_CONSTRUCTOR_FUNCTION(RegisterTypes)
}
