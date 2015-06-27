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


#include "Processing/Component/ComponentDiagram.qt.h"

#include <QThread>
#include <QFile>
#include <QtXml>
#include <QApplication>
#include "DebugOutput/DebugStream.h"
#include "Processing/Object/Class.qt.h"
#include "Processing/Object/Port.qt.h"
#include "Processing/Interfaces/IProcessor.qt.h"
#include "Processing/Interfaces/IProvider.qt.h"
#include "Processing/Object/ObjectFactory.qt.h"
#include "Processing/Object/Sink.qt.h"
#include "Processing/Object/Source.qt.h"
#include "Processing/Object/Filter.qt.h"
#include "Misc/PortConnectionHelper.h"
#include "Mediation/RuntimeEventBus.h"

#include <qtextcodec.h>
#include <qsize.h>
#include <qpoint.h>
#include <qrect.h>
#include <qcoreapplication.h>
#include <QRegularExpression>

namespace Core
{
namespace Processing
{
namespace Component
{

ComponentDiagram::ComponentDiagram(QString uid /*= ""*/, std::string fileName /*= ""*/)
	: IObject(uid),
	_thread(nullptr),
	_isRunning(false)
{
	setFileName(fileName);
}

ComponentDiagram::ComponentDiagram( const ComponentDiagram& rhs )
	: IObject(rhs),
	_thread(nullptr),
	_fileName(rhs._fileName),
	_isRunning(false)
{
	copyObjectsAndConnectPorts(rhs);
}

ComponentDiagram::ComponentDiagram( const ComponentDiagram& rhs, QString uid )
	: IObject(rhs, uid),
	_thread(nullptr),
	_fileName(rhs._fileName),
	_isRunning(false)
{
	copyObjectsAndConnectPorts(rhs);
}

ComponentDiagram::~ComponentDiagram()
{
	//if(_thread)
	//	delete _thread;

	_objects.clear();
}


void ComponentDiagram::run()
{
	// A thread can be started once only
	if(_thread != nullptr)
		return;

	_thread = new QThread();

	connect(_thread, SIGNAL(finished()), this, SLOT(finished()));

	moveToThread(_thread);
	_thread->start();

	QMetaObject::invokeMethod(this, "doWork", Qt::QueuedConnection);

}


bool ComponentDiagram::process( Core::Processing::Interfaces::ConstIObjectParametersPtr parameters )
{
	return true;
}

void ComponentDiagram::doWork()
{
	_isRunning = true;

	//init();

	process(nullptr);
}

void ComponentDiagram::finished()
{
	_isRunning = false;
}

bool ComponentDiagram::stop()
{
	if(!_isRunning || _thread == nullptr)
		return false;

	// Signal the stop event to all the running objects
	emit executionStopped();

	_thread->quit();

	return false;
}

void ComponentDiagram::init()
{
	initComponents();

	for(QList<Core::Processing::Interfaces::IObjectPtr>::iterator oIt = _objects.begin(); oIt != _objects.end(); oIt++)
	{
		connect(this, SIGNAL(executionStopped()), &(*(*oIt)), SLOT(componentStopped()));
	}
}

QList<Core::Processing::Interfaces::IObjectPtr> ComponentDiagram::getObjects()
{
	return _objects;
}

bool ComponentDiagram::saveAs( std::string filename )
{
	bool returnValue = false;
	if(filename.length() > 0)
	{
		setFileName(filename);
		returnValue = save();
	}
	return returnValue;
}

bool ComponentDiagram::save()
{
	bool returnValue = false;
	if(_fileName.length() > 0)
	{
		//create directory if not exists
		QFileInfo fileInfo(_fileName.c_str());
		QDir directory(fileInfo.absoluteDir());
		if(!directory.exists())
		{
			directory.mkpath(directory.absolutePath());
		}

		// Writing to a file
		QFile file(_fileName.c_str());
		if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			qDebug() << "Open the file for writing failed";
		}
		else
		{
		
			// Create a document to write XML
			QDomDocument domDocument;
			QDomElement root = domDocument.createElement(DIAGRAM_FILE);
			root.setAttribute(DIAGRAM_ATTRIBUTE_VERSION, DIAGRAM_FILE_CURRENT_VERSION);
			domDocument.appendChild(root);

			////add diagram file version to root element
			//QDomElement version = domDocument.createElement("DiagramFileVersion");
			//QDomText versionTextElement = domDocument.createTextNode("VersionTextElement");
			//version.appendChild(versionTextElement);
			//versionTextElement.setNodeValue(QString(DIAGRAM_FILE_VERSION));
			//root.appendChild(version);

			//add object group
			QDomElement diagramObjectGroup = domDocument.createElement(DIAGRAM_OBJECT_GROUP);
			root.appendChild(diagramObjectGroup);

			for(Core::Processing::Interfaces::IObjectPtr object : _objects)
			{
				//add object
				QDomElement domObject = domDocument.createElement(DIAGRAM_OBJECT);
				domObject.setAttribute(DIAGRAM_ATTRIBUTE_TYPE, object->metaObject()->className());
				domObject.setAttribute(DIAGRAM_ATTRIBUTE_UID, object->uid());
				domObject.setAttribute(DIAGRAM_ATTRIBUTE_NAME, object->objectName());
				fillProperties(domDocument, domObject, *object.get());
				diagramObjectGroup.appendChild(domObject);

				//add port group
				QDomElement objectPortGroup = domDocument.createElement(DIAGRAM_OBJECT_PORT_GROUP);
				domObject.appendChild(objectPortGroup);
				Core::Processing::Object::ClassPtr classObject = boost::dynamic_pointer_cast<Core::Processing::Object::Class>(object);
				if(classObject)
				{
					Core::Processing::Object::Class::PortMap portmap = classObject->getPortMap();
					//add ports
					for(Core::Processing::Object::Class::PortMapPair portMapEntry : portmap )
					{
						//add port
						Core::Processing::Interfaces::IPortPtr iport = portMapEntry.second;
						if(iport)
						{
							QDomElement portElement = domDocument.createElement(DIAGRAM_OBJECT_PORT);
							portElement.setAttribute(DIAGRAM_ATTRIBUTE_TYPE, iport->metaObject()->className());
							portElement.setAttribute(DIAGRAM_ATTRIBUTE_UID, iport->uid());
							portElement.setAttribute(DIAGRAM_ATTRIBUTE_NAME, QString::fromStdString(portMapEntry.first));
							objectPortGroup.appendChild(portElement);

							Core::Processing::Object::PortPtr port = boost::dynamic_pointer_cast<Core::Processing::Object::Port>(iport);
							if(port)
							{
								Core::Processing::Object::Port::ProviderProcessorListMap connectionMap = port->getProviderProcessorListMap();
								if(connectionMap.size() > 0)
								{
									//add port connection group
									QDomElement portConnectionGroup = domDocument.createElement(DIAGRAM_OBJECT_PORT_CONNECTION_GROUP);
									portElement.appendChild(portConnectionGroup);


									for(Core::Processing::Object::Port::ProviderProcessorListPair connectionMapEntry: connectionMap)
									{
										Core::Processing::Interfaces::IProvider* source = connectionMapEntry.first;
										QList<Core::Processing::Interfaces::IProcessorPtr> targets = connectionMapEntry.second;

										for(Core::Processing::Interfaces::IProcessorPtr target : targets)
										{
											//add port connection
											Core::Processing::Object::Port* targetPort = dynamic_cast<Core::Processing::Object::Port*>(target.get());

											QDomElement portConnection = domDocument.createElement(DIAGRAM_OBJECT_PORT_CONNECTION);
											portConnection.setAttribute(DIAGRAM_ATTRIBUTE_TARGET, targetPort->uid());
											portConnectionGroup.appendChild(portConnection);
										}

									}
								}
							}

						}
					}
				}
			}

			//write to file
			QTextStream stream(&file);
			stream << domDocument.toString();
			file.close();
			qDebug() << "Writing is done";
			returnValue = true;
		}
	}
	return returnValue;
}

bool ComponentDiagram::load( std::string filename )
{
	bool returnValue = false;
	if(!QFile::exists(filename.c_str()))
	{
		E_OUT("Could not load diagram. File does not exist.");
		return false;
	}

	setFileName(filename);
	// open file
	QFile file(filename.c_str());

	if (!file.open(QFile::ReadOnly | QFile::Text)) 
	{
		E_OUT("File could not be opened!");
		return false;
	}

	QDomDocument domDocument;
	if (!domDocument.setContent(&file)) 
	{
		D_OUT("Wrong document format!");
		return false;
	}

	file.close();

	QDomElement root = domDocument.documentElement();

	if (root.tagName().compare(QString(DIAGRAM_FILE)) != 0) 
	{
		E_OUT2(__FUNCTION__, "Wrong file format!");
		return false;
	}

	if(root.hasAttribute(DIAGRAM_ATTRIBUTE_VERSION))
	{
		if(root.attribute(DIAGRAM_ATTRIBUTE_VERSION).compare(QString(DIAGRAM_FILE_VERSION_1_0)) == 0)
		{
			returnValue = loadV1(domDocument);
		}
	}

	if(returnValue)
	{
		init();
	}

	return returnValue;
}

void ComponentDiagram::fillProperties(QDomDocument& domDocument, QDomElement& domObject, QObject& object)
{
	QDomElement propertyGroup = domDocument.createElement(DIAGRAM_OBJECT_PROPERTY_GROUP);
	domObject.appendChild(propertyGroup);

	for( int i = 0; i < object.metaObject()->propertyCount(); i++)
	{
		QMetaProperty metaProperty = object.metaObject()->property(i);
		QString propertyName(metaProperty.name());
		QVariant variable = object.property(propertyName.toStdString().c_str());
		QString propertyValue = variantToString(variable, metaProperty.type());

		QDomElement domProperty = domDocument.createElement(DIAGRAM_OBJECT_PROPERTY);
		domProperty.setAttribute(DIAGRAM_ATTRIBUTE_NAME, propertyName);
		domProperty.setAttribute(DIAGRAM_ATTRIBUTE_VALUE, propertyValue);
		propertyGroup.appendChild(domProperty);
		
		/*QSettings
		QString propertyName = _objects[i]->metaObject()->property(j).name();
		QVariant variable = _objects[i]->property(propertyName.toStdString().c_str());
		QByteArray buffer;
		QDataStream stream(&buffer,	QIODevice::ReadWrite);
		variable.save(stream);
		QByteArray hexBuffer = buffer.toHex();*/
	}
	
	QList<QByteArray> properties = object.dynamicPropertyNames();
	for(QString dynamicProperty : properties)
	{
		QVariant dynamicProerptyVariant = object.property(dynamicProperty.toStdString().c_str());
		QString dynamicProerptyValue = variantToString(dynamicProerptyVariant, QVariant::String);

		QDomElement domProperty = domDocument.createElement(DIAGRAM_OBJECT_PROPERTY);
		domProperty.setAttribute(DIAGRAM_ATTRIBUTE_NAME, dynamicProperty);
		domProperty.setAttribute(DIAGRAM_ATTRIBUTE_VALUE, dynamicProerptyValue);
		propertyGroup.appendChild(domProperty);
	}
}

bool ComponentDiagram::loadV1( QDomDocument& domDocument )
{
	bool returnValue = false;
	QDomElement root = domDocument.documentElement();
	QDomElement diagramObjectGroup = root.firstChildElement(QString(DIAGRAM_OBJECT_GROUP));

	if(!diagramObjectGroup.isNull())
	{
		Core::Processing::Object::ObjectFactoryPtr objectFactory = Core::Processing::Object::ObjectFactory::instance();
		std::vector<Core::Misc::PortConnectionHelper> portConnectionList;
		std::map<QString, Core::Processing::Interfaces::IPortPtr> portMap;

		QDomNodeList diagramObjects = diagramObjectGroup.childNodes();
		for(int i = 0; i < diagramObjects.count(); i++)
		{
			//get diagram object node
			QDomNode diagramObjectNode = diagramObjects.at(i);
			if(!diagramObjectNode.isNull() && diagramObjectNode.nodeName().compare(QString(DIAGRAM_OBJECT)) == 0)
			{
				//get diagram object
				QDomElement diagramObject = diagramObjectNode.toElement();
				QString diagramobjectUid, diagramObjectType, diagramObjectName;

				if(!diagramObject.isNull())
				{
					if(diagramObject.hasAttribute(DIAGRAM_ATTRIBUTE_UID))
					{
						diagramobjectUid = diagramObject.attribute(DIAGRAM_ATTRIBUTE_UID);
					}
					else
					{
						D_OUT2(__FUNCTION__, "Object has no UID attribute");
					}

					if(diagramObject.hasAttribute(DIAGRAM_ATTRIBUTE_TYPE))
					{
						diagramObjectType = diagramObject.attribute(DIAGRAM_ATTRIBUTE_TYPE);
					}
					else
					{
						D_OUT2(__FUNCTION__, "Object has no type attribute");
					}

					if(diagramObject.hasAttribute(DIAGRAM_ATTRIBUTE_NAME))
					{
						diagramObjectName = diagramObject.attribute(DIAGRAM_ATTRIBUTE_NAME);;
					}
					/*else
					{
						D_OUT2(__FUNCTION__, "Object has no name attribute");
					}*/
				}
				else
				{
					E_OUT2(__FUNCTION__, "Failed to read object attributes")
				}

				if(diagramObjectType.length() > 0 && diagramobjectUid.length() > 0)
				{
					//create diagram object
					Core::Processing::Interfaces::IObjectPtr object = objectFactory->createObject(diagramObjectType, diagramobjectUid, true);
					Core::Mediation::RuntimeEventBus::getInstance().postEvent(Core::Mediation::Events::SelectItemEvent(object));
					_objects.append(object);

					//get propertygroup
					QDomElement diagramPropertyGroup = diagramObject.firstChildElement(DIAGRAM_OBJECT_PROPERTY_GROUP);
					if(!diagramPropertyGroup.isNull())
					{
						QDomNodeList diagramPropertyNodes = diagramPropertyGroup.childNodes();
						for(int i = 0; i < diagramPropertyNodes.count(); i++)
						{
							//get property node
							QDomNode diagramPropertyNode = diagramPropertyNodes.at(i);
							if(!diagramPropertyNode.isNull() && diagramPropertyNode.nodeName().compare(QString(DIAGRAM_OBJECT_PROPERTY)) == 0)
							{
								//get proptery
								QDomElement diagramProperty = diagramPropertyNode.toElement();
								if(!diagramProperty.isNull() && diagramProperty.hasAttribute(DIAGRAM_ATTRIBUTE_NAME) && diagramProperty.hasAttribute(DIAGRAM_ATTRIBUTE_VALUE))
								{
									//set property
									QString propertyName = diagramProperty.attribute(DIAGRAM_ATTRIBUTE_NAME);
									QString propertyValue = diagramProperty.attribute(DIAGRAM_ATTRIBUTE_VALUE);
									if(propertyName.length() > 0)
									{
										object->setPropertyInitializationValue(propertyName.toStdString(), stringToVariant(propertyValue));

										if(object->property(propertyName.toStdString().c_str()).isValid())
										{
											int index = object->metaObject()->indexOfProperty(propertyName.toStdString().c_str());
											if(index >= 0)
											{
												QMetaProperty metaProperty = object->metaObject()->property(index);

												QVariant newValue = stringToVariant(propertyValue);
												if(metaProperty.isEnumType())
												{
													newValue.convert(QVariant::Int);
												}
												
												object->setProperty(propertyName.toStdString().c_str(), newValue);

											}
											else
											{
												E_OUT2(__FUNCTION__, "Failed to set attribute due to missing property.");
											}
										}
									}
									else
									{
										E_OUT2(__FUNCTION__, "Failed to set attribute due to empty property name.");
									}
								}
								else
								{
									D_OUT2(__FUNCTION__, "Property has no name or value");
								}
							}
							else
							{
								E_OUT2(__FUNCTION__, "Failed to read property");
							}
						}
					}
					else
					{
						E_OUT2(__FUNCTION__, "Failed to read property group");
					}

					//get port group
					QDomElement diagramPortGroup = diagramObject.firstChildElement(DIAGRAM_OBJECT_PORT_GROUP);
					if(!diagramPortGroup.isNull())
					{
						QDomNodeList diagramPortNodes = diagramPortGroup.childNodes();
						for(int i = 0; i < diagramPortNodes.count(); i++)
						{
							//get port node
							QDomNode diagramPortNode = diagramPortNodes.at(i);
							if(!diagramPortNode.isNull() && diagramPortNode.nodeName().compare(QString(DIAGRAM_OBJECT_PORT)) == 0)
							{
								QDomElement diagramPort = diagramPortNode.toElement();

								if(!diagramPort.isNull() && diagramPort.hasAttribute(DIAGRAM_ATTRIBUTE_NAME) && diagramPort.hasAttribute(DIAGRAM_ATTRIBUTE_TYPE) && diagramPort.hasAttribute(DIAGRAM_ATTRIBUTE_UID))
								{
									QString portName = diagramPort.attribute(DIAGRAM_ATTRIBUTE_NAME);
									QString portType = diagramPort.attribute(DIAGRAM_ATTRIBUTE_TYPE);
									QString portUid = diagramPort.attribute(DIAGRAM_ATTRIBUTE_UID);

									Core::Processing::Interfaces::IPortPtr port = boost::dynamic_pointer_cast<Core::Processing::Interfaces::IPort>(objectFactory->createObject(portType, portUid, true));
									portMap.insert(std::pair<QString, Core::Processing::Interfaces::IPortPtr>(portUid, port));
									Core::Processing::Object::SourcePtr source = boost::dynamic_pointer_cast<Core::Processing::Object::Source>(object);
									if(source)
									{
										source->addPort(port, portName);
									}
									else
									{
										Core::Processing::Object::SinkPtr sink = boost::dynamic_pointer_cast<Core::Processing::Object::Sink>(object);
										if(sink)
										{
											sink->addPort(port, portName);
										}
										else
										{
											Core::Processing::Object::FilterPtr filter = boost::dynamic_pointer_cast<Core::Processing::Object::Filter>(object);
											if(filter)
											{
												filter->addPort(port, portName);
											}
											else
											{
												E_OUT2(__FUNCTION__, "Could not add port. Unknown object type.")
											}
										}
									}

									QDomElement diagramPortConnectionGroup = diagramPort.firstChildElement(DIAGRAM_OBJECT_PORT_CONNECTION_GROUP);

									if(!diagramPortConnectionGroup.isNull())
									{
										QDomNodeList diagramPortConnectionNodes = diagramPortConnectionGroup.childNodes();

										for(int i = 0; i < diagramPortConnectionNodes.count(); i++)
										{
											QDomNode diagramPortConnectionNode = diagramPortConnectionNodes.at(i);
											if(!diagramPortConnectionNode.isNull() && diagramPortConnectionNode.nodeName().compare(QString(DIAGRAM_OBJECT_PORT_CONNECTION)) == 0)
											{
												QDomElement diagramPortConnection = diagramPortConnectionNode.toElement();
												if(!diagramPortConnection.isNull() && diagramPortConnection.hasAttribute(DIAGRAM_ATTRIBUTE_TARGET))
												{
													QString target = diagramPortConnection.attribute(DIAGRAM_ATTRIBUTE_TARGET);
													std::map<QString, Core::Processing::Interfaces::IPortPtr>::iterator it = portMap.find(target);
													if(it != portMap.end())
													{
														port->connectInterfaces(boost::dynamic_pointer_cast<Core::Processing::Interfaces::IProvider>(port), boost::dynamic_pointer_cast<Core::Processing::Interfaces::IProcessor>(it->second));
													}
													else
													{
														portConnectionList.push_back(Core::Misc::PortConnectionHelper(port, target));
													}
												}
												else
												{
													E_OUT2(__FUNCTION__, "Port connection has not target attribute");
												}
											}
											else
											{
												D_OUT2(__FUNCTION__, "Failed to read port connection");
											}
										}
									}
									else
									{
										D_OUT2(__FUNCTION__, "Failed to read port connection group");
									}
									
								}
								else
								{
									E_OUT2(__FUNCTION__, "Port has misses the name, type and/or UID attribute");
								}

							}
							else
							{
								E_OUT2(__FUNCTION__, "Failed to read port");
							}
						}
					}
					else
					{
						E_OUT2(__FUNCTION__, "Failed to read port group");
					}
				}
			}
			else
			{
				E_OUT2(__FUNCTION__, "Failed to read object");
			}
		}
		for(Core::Misc::PortConnectionHelper portEntry: portConnectionList)
		{
			std::map<QString, Core::Processing::Interfaces::IPortPtr>::iterator it = portMap.find(portEntry.TargetUid);
			if(it != portMap.end())
			{
				portEntry.SourcePort->connectInterfaces(boost::dynamic_pointer_cast<Core::Processing::Interfaces::IProvider>(portEntry.SourcePort), boost::dynamic_pointer_cast<Core::Processing::Interfaces::IProcessor>(it->second));
			}
			else
			{
				E_OUT2(__FUNCTION__, "Could not connect port.")
			}
		}
	}
	else
	{
		E_OUT2(__FUNCTION__, "Failed to read object group.");
	}

	return returnValue;
}

void ComponentDiagram::initComponents()
{
	for(Core::Processing::Interfaces::IObjectPtr object : _objects)
	{
		Core::Mediation::RuntimeEventBus::getInstance().postEvent(Core::Mediation::Events::SelectItemEvent(object));
		do
		{
			QThread::msleep(10);
		}while(!object->isSelected());
		object->onDiagramObjectLoaded();
		Core::Mediation::RuntimeEventBus::getInstance().postEvent(Core::Mediation::Events::SelectItemEvent(nullptr));
	}
}

std::string ComponentDiagram::fileExtension()
{
	return DIAGRAM_FILE_EXTENSION;
}

void ComponentDiagram::copyObjectsAndConnectPorts( const ComponentDiagram &rhs )
{
	std::vector<Core::Misc::PortConnectionHelper> portNewTargetConnectionList;
	std::vector<Core::Misc::PortConnectionHelper> portOldTargetConnectionList;
	std::map<QString, Core::Processing::Object::PortPtr> portMap;
	std::map<QString, QString> originalCopyPortMap;

	for(Core::Processing::Interfaces::IObjectPtr originalObject : rhs._objects)
	{
		Core::Processing::Interfaces::IObjectPtr copyObject = originalObject->create();

		for( int j = 0; j < originalObject->metaObject()->propertyCount(); j++)
		{
			const char* name = originalObject->metaObject()->property(j).name();
			copyObject->setProperty(name, originalObject->property(name));
		}

		Core::Processing::Object::ClassPtr copyClassObject = boost::dynamic_pointer_cast<Core::Processing::Object::Class>(copyObject);
		Core::Processing::Object::ClassPtr originalClassObject = boost::dynamic_pointer_cast<Core::Processing::Object::Class>(originalObject);
		if(copyClassObject && originalClassObject)
		{
			Core::Processing::Object::Class::PortMap copyClassPortmap = copyClassObject->getPortMap();
			Core::Processing::Object::Class::PortMap originalClassPortmap = originalClassObject->getPortMap();
			//get ports
			for(Core::Processing::Object::Class::PortMapPair copyClassPortmapEntry : copyClassPortmap )
			{
				//get port
				Core::Processing::Object::Class::PortMap::iterator originalClassPortmapEntry = originalClassPortmap.find(copyClassPortmapEntry.first);
				if(originalClassPortmapEntry != originalClassPortmap.end())
				{
					Core::Processing::Object::PortPtr copyClassPort = boost::dynamic_pointer_cast<Core::Processing::Object::Port>(copyClassPortmapEntry.second);
					Core::Processing::Object::PortPtr originalClassPort = boost::dynamic_pointer_cast<Core::Processing::Object::Port>(originalClassPortmapEntry->second);
					if(copyClassPort && originalClassPort)
					{
						portMap.insert(std::make_pair(copyClassPort->uid(), copyClassPort));
						originalCopyPortMap.insert(std::make_pair(originalClassPort->uid(), copyClassPort->uid()));
						Core::Processing::Object::Port::ProviderProcessorListMap originalConnectionMap = originalClassPort->getProviderProcessorListMap();
						if(originalConnectionMap.size() > 0)
						{
							//add port connection group
							for(Core::Processing::Object::Port::ProviderProcessorListPair originalConnectionMapEntry: originalConnectionMap)
							{
								Core::Processing::Interfaces::IProvider* originalProvider = originalConnectionMapEntry.first;
								QList<Core::Processing::Interfaces::IProcessorPtr> originalTargetProcessors = originalConnectionMapEntry.second;

								for(Core::Processing::Interfaces::IProcessorPtr originalTargetProcessor : originalTargetProcessors)
								{
									//add port connection
									Core::Processing::Object::PortPtr originalTargetPort = boost::dynamic_pointer_cast<Core::Processing::Object::Port>(originalTargetProcessor);
									std::map<QString, QString>::iterator srcCpyIt = originalCopyPortMap.find(originalTargetPort->uid());

									if(srcCpyIt != originalCopyPortMap.end())
									{
										std::map<QString, Core::Processing::Object::PortPtr>::iterator portMapIt = portMap.find(srcCpyIt->second);

										if(portMapIt != portMap.end())
										{
											copyClassPort->connectInterfaces(boost::dynamic_pointer_cast<Core::Processing::Interfaces::IProvider>(copyClassPort), boost::dynamic_pointer_cast<Core::Processing::Interfaces::IProcessor>(portMapIt->second));
										}
										else
										{
											portNewTargetConnectionList.push_back(Core::Misc::PortConnectionHelper(copyClassPort, srcCpyIt->second));
										}
									}
									else
									{
										portOldTargetConnectionList.push_back(Core::Misc::PortConnectionHelper(copyClassPort, originalTargetPort->uid()));
									}
								}
							}
						}
					}
				}
			}
		}

		_objects.push_back(copyObject);
	}

	for(Core::Misc::PortConnectionHelper portEntry: portNewTargetConnectionList)
	{
		std::map<QString, Core::Processing::Object::PortPtr>::iterator it = portMap.find(portEntry.TargetUid);
		if(it != portMap.end())
		{
			portEntry.SourcePort->connectInterfaces(boost::dynamic_pointer_cast<Core::Processing::Interfaces::IProvider>(portEntry.SourcePort), boost::dynamic_pointer_cast<Core::Processing::Interfaces::IProcessor>(it->second));
		}
		else
		{
			E_OUT2(__FUNCTION__, "Could not connect port.")
		}
	}

	for(Core::Misc::PortConnectionHelper oldPortEntry: portOldTargetConnectionList)
	{
		std::map<QString, QString>::iterator srcCpyIt = originalCopyPortMap.find(oldPortEntry.TargetUid);

		if(srcCpyIt != originalCopyPortMap.end())
		{
			std::map<QString, Core::Processing::Object::PortPtr>::iterator it = portMap.find(srcCpyIt->second);
			if(it != portMap.end())
			{
				oldPortEntry.SourcePort->connectInterfaces(boost::dynamic_pointer_cast<Core::Processing::Interfaces::IProvider>(oldPortEntry.SourcePort), boost::dynamic_pointer_cast<Core::Processing::Interfaces::IProcessor>(it->second));
			}
			else
			{
				E_OUT2(__FUNCTION__, "Could not connect port.")
			}
		}
	}
}

std::string ComponentDiagram::getFileName() const
{
	return _fileName;
}

void ComponentDiagram::setFileName( const std::string& fileName )
{
	if(fileName.length() > 0)
	{
		_fileName = fileName;
		QFileInfo fileInfo(fileName.c_str());
		setObjectName(fileInfo.baseName());
	}
}


QString ComponentDiagram::variantToString(const QVariant &v, QVariant::Type type)
{
	QString result;

	switch (type) {
	case QVariant::Invalid:
		result = QLatin1String("@Invalid()");
		break;

	case QVariant::ByteArray: {
		QByteArray a = v.toByteArray();
		result = QLatin1String("@ByteArray(");
		result += QString::fromLatin1(a.constData(), a.size());
		result += QLatin1Char(')');
		break;
							  }
	case QVariant::String:
	case QVariant::LongLong:
	case QVariant::ULongLong:
	case QVariant::Int:
	case QVariant::UInt:
	case QVariant::Bool:
	case QVariant::Double:
	case QVariant::KeySequence: {
		result = v.toString();
		if (result.startsWith(QLatin1Char('@')))
			result.prepend(QLatin1Char('@'));
		break;
								}
	case QVariant::Rect: {
		QRect r = qvariant_cast<QRect>(v);
		result += QLatin1String("@Rect(");
		result += QString::number(r.x());
		result += QLatin1Char(' ');
		result += QString::number(r.y());
		result += QLatin1Char(' ');
		result += QString::number(r.width());
		result += QLatin1Char(' ');
		result += QString::number(r.height());
		result += QLatin1Char(')');
		break;
						 }
	case QVariant::Size: {
		QSize s = qvariant_cast<QSize>(v);
		result += QLatin1String("@Size(");
		result += QString::number(s.width());
		result += QLatin1Char(' ');
		result += QString::number(s.height());
		result += QLatin1Char(')');
		break;
						 }
	case QVariant::Point: {
		QPoint p = qvariant_cast<QPoint>(v);
		result += QLatin1String("@Point(");
		result += QString::number(p.x());
		result += QLatin1Char(' ');
		result += QString::number(p.y());
		result += QLatin1Char(')');
		break;
						  }
	case QVariant::StringList:
		{
			QStringList sl = qvariant_cast<QStringList>(v);
			result += QLatin1String("@StringList(");
			for( int i = 0; i < sl.size(); i++)
			{
				result += QLatin1String("@String>>");
				result += sl[i];
			}
			result += QLatin1Char(')');
			break;
		}
	default: {
		QByteArray a;
		{
			QDataStream s(&a, QIODevice::WriteOnly);
			s.setVersion(QDataStream::Qt_5_0);
			s << v;
		}
		result = QLatin1String("@Variant(");
		result += QString::fromLatin1(a.constData(), a.size());
		result += QLatin1Char(')');
		break;
			 }
	}

	return result;
}


QVariant ComponentDiagram::stringToVariant(const QString &s)
{
	if (s.startsWith(QLatin1Char('@'))) {
		if (s.endsWith(QLatin1Char(')'))) {
			if (s.startsWith(QLatin1String("@ByteArray("))) {
				return QVariant(s.toLatin1().mid(11, s.size() - 12));
			} else if (s.startsWith(QLatin1String("@Variant("))) {
				QByteArray a(s.toLatin1().mid(9));
				QDataStream stream(&a, QIODevice::ReadOnly);
				stream.setVersion(QDataStream::Qt_5_0);
				QVariant result;
				stream >> result;
				return result;
			} else if (s.startsWith(QLatin1String("@Rect("))) {
				QStringList args = ComponentDiagram::splitArgs(s, 5);
				if (args.size() == 4)
					return QVariant(QRect(args[0].toInt(), args[1].toInt(), args[2].toInt(), args[3].toInt()));
			} else if (s.startsWith(QLatin1String("@Size("))) {
				QStringList args = ComponentDiagram::splitArgs(s, 5);
				if (args.size() == 2)
					return QVariant(QSize(args[0].toInt(), args[1].toInt()));
			} else if (s.startsWith(QLatin1String("@Point("))) {
				QStringList args = ComponentDiagram::splitArgs(s, 6);
				if (args.size() == 2)
					return QVariant(QPoint(args[0].toInt(), args[1].toInt()));
			} else if (s.startsWith(QLatin1String("@StringList("))) {
				QStringList args = ComponentDiagram::split(s, 11);
				return QVariant(QStringList(args));
			} else if (s == QLatin1String("@Invalid()")) {
				return QVariant();
			}

		}
		if (s.startsWith(QLatin1String("@@")))
			return QVariant(s.mid(1));
	}

	return QVariant(s);
}

QStringList ComponentDiagram::splitArgs(const QString &s, int idx)
{
	int l = s.length();
	Q_ASSERT(l > 0);
	Q_ASSERT(s.at(idx) == QLatin1Char('('));
	Q_ASSERT(s.at(l - 1) == QLatin1Char(')'));

	QStringList result;
	QString item;

	for (++idx; idx < l; ++idx) {
		QChar c = s.at(idx);
		if (c == QLatin1Char(')')) {
			Q_ASSERT(idx == l - 1);
			result.append(item);
		} else if (c == QLatin1Char(' ')) {
			result.append(item);
			item.clear();
		} else {
			item.append(c);
		}
	}

	return result;
}

QStringList ComponentDiagram::split( const QString &s, int idx )
{
	int l = s.length();
	Q_ASSERT(l > 0);
	Q_ASSERT(s.at(idx) == QLatin1Char('('));
	Q_ASSERT(s.at(l - 1) == QLatin1Char(')'));

	QString substring = s.mid(++idx, l - 1 - idx);
	QStringList splitted = substring.split("@String>>",QString::SkipEmptyParts);

	return splitted;
}

};	// namespace Component
};	// namespace Processing
}; //namespace Core
