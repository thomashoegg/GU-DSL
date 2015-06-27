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


#include "Processing/Project/Project.qt.h"

#include <QThread>
#include <QFile>
#include <QtXml>
#include <QApplication>
#include "DebugOutput/DebugStream.h"
#include "Processing/Object/Class.qt.h"
#include "Processing/Object/Port.qt.h"
#include "Processing/Interfaces/IProvider.qt.h"
#include "Processing/Interfaces/IProcessor.qt.h"
#include "Processing/Object/ObjectFactory.qt.h"
#include "Processing/Object/Sink.qt.h"
#include "Processing/Object/Source.qt.h"
#include "Processing/Object/Filter.qt.h"
#include "Processing/Component/ComponentDiagram.qt.h"
#include "Misc/PortConnectionHelper.h"


namespace Core
{
namespace Processing
{
namespace Project
{

Project::Project(QString uid /*= ""*/)
	: IObject(uid),
	_thread(nullptr),
	_fileName()
{
}

Project::Project( const Project& rhs )
	: IObject(rhs),
	_thread(nullptr),
	_fileName(rhs._fileName)
{
	copyDiagrams(rhs);
}

Project::Project( const Project& rhs, QString uid )
	: IObject(rhs, uid),
	_thread(nullptr),
	_fileName(rhs._fileName)
{
	copyDiagrams(rhs);
}

Project::~Project()
{
	//if(_thread)
	//	delete _thread;

	_diagrams.clear();
}


void Project::run()
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


bool Project::process( Core::Processing::Interfaces::ConstIObjectParametersPtr parameters )
{
	return true;
}

void Project::doWork()
{
	_isRunning = true;

	//init();

	process(nullptr);
}

void Project::finished()
{
	_isRunning = false;
}

bool Project::stop()
{
	if(!_isRunning || _thread == nullptr)
		return false;

	// Signal the stop event to all the running objects
	emit executionStopped();

	_thread->quit();

	return false;
}

void Project::init()
{
	initComponents();

	for(QList<Core::Processing::Component::ConstComponentDiagramPtr>::iterator oIt = _diagrams.begin(); oIt != _diagrams.end(); oIt++)
	{
		connect(this, SIGNAL(executionStopped()), &(*(*oIt)), SLOT(componentStopped()));
	}
}

QList<Core::Processing::Component::ConstComponentDiagramPtr> Project::getDiagrams()
{
	return _diagrams;
}

bool Project::save( std::string filename )
{
	bool returnValue = false;
	if(filename.length() > 0)
	{
		setFileName(filename);
		returnValue = save();
	}
	return returnValue;
}

bool Project::save()
{
	bool returnValue = false;
	if(_fileName.length() > 0)
	{
		QFileInfo fileInfo(_fileName.c_str());
		QDir directory(fileInfo.absoluteDir());

		// Create a document to write XML
		QDomDocument domDocument;
		QDomElement root = domDocument.createElement(PROJECT_FILE);
		root.setAttribute(PROJECT_ATTRIBUTE_VERSION, PROJECT_FILE_CURRENT_VERSION);
		domDocument.appendChild(root);

		////add diagram file version to root element
		//QDomElement version = domDocument.createElement("DiagramFileVersion");
		//QDomText versionTextElement = domDocument.createTextNode("VersionTextElement");
		//version.appendChild(versionTextElement);
		//versionTextElement.setNodeValue(QString(DIAGRAM_FILE_VERSION));
		//root.appendChild(version);

		//add object group
		QDomElement diagramObjectGroup = domDocument.createElement(PROJECT_DIAGRAM_GROUP);
		root.appendChild(diagramObjectGroup);

		for(Core::Processing::Component::ConstComponentDiagramPtr diagram : _diagrams)
		{
			//add object
			QDomElement domObject = domDocument.createElement(PROJECT_DIAGRAM);
			domObject.setAttribute(PROJECT_ATTRIBUTE_NAME, diagram->objectName());
			domObject.setAttribute(PROJECT_ATTRIBUTE_UID, diagram->uid());
			domObject.setAttribute(PROJECT_ATTRIBUTE_FILENAME, directory.relativeFilePath(diagram->getFileName().c_str()));
			diagramObjectGroup.appendChild(domObject);
		}


		//create directory if not exists
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
			QTextStream stream(&file);
			stream << domDocument.toString();
			file.close();
			qDebug() << "Writing is done";
			returnValue = true;
		}
	}
	return returnValue;
}

bool Project::load( std::string filename )
{
	bool returnValue = false;
	if(!QFile::exists(filename.c_str()))
		return false;

	setFileName(filename);

	// open file
	QFile file(filename.c_str());

	if (!file.open(QFile::ReadOnly | QFile::Text)) 
	{
		E_OUT2(__FUNCTION__, "File could not be opened!");
		return false;
	}

	QDomDocument domDocument;
	if (!domDocument.setContent(&file)) 
	{
		D_OUT2(__FUNCTION__, "Wrong document format!");
		return false;
	}

	file.close();

	QDomElement root = domDocument.documentElement();

	if (root.tagName().compare(QString(PROJECT_FILE)) != 0) 
	{
		E_OUT2(__FUNCTION__, "Wrong file format!");
		return false;
	}

	if(root.hasAttribute(PROJECT_ATTRIBUTE_VERSION))
	{
		if(root.attribute(PROJECT_ATTRIBUTE_VERSION).compare(QString(PROJECT_FILE_VERSION_1_0)) == 0)
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

bool Project::loadV1( QDomDocument& domDocument )
{
	bool returnValue = false;
	QDomElement root = domDocument.documentElement();
	QDomElement projectDiagramGroup = root.firstChildElement(QString(PROJECT_DIAGRAM_GROUP));

	if(!projectDiagramGroup.isNull())
	{
		Core::Processing::Object::ObjectFactoryPtr objectFactory = Core::Processing::Object::ObjectFactory::instance();
		std::vector<Core::Misc::PortConnectionHelper> portConnectionList;
		std::map<QString, Core::Processing::Interfaces::IPortPtr> portMap;

		QDomNodeList projectDiagrams = projectDiagramGroup.childNodes();
		for(int i = 0; i < projectDiagrams.count(); i++)
		{
			//get diagram object node
			QDomNode projectDiagramNode = projectDiagrams.at(i);
			if(!projectDiagramNode.isNull() && projectDiagramNode.nodeName().compare(QString(PROJECT_DIAGRAM)) == 0)
			{
				//get diagram object
				QDomElement projectDiagram = projectDiagramNode.toElement();
				QString projectDiagramUid, projectDiagramFileName, projectDiagramName;

				if(!projectDiagram.isNull())
				{
					if(projectDiagram.hasAttribute(PROJECT_ATTRIBUTE_UID))
					{
						projectDiagramUid = projectDiagram.attribute(PROJECT_ATTRIBUTE_UID);
					}
					else
					{
						D_OUT2(__FUNCTION__, "Object has no UID attribute");
					}

					if(projectDiagram.hasAttribute(PROJECT_ATTRIBUTE_FILENAME))
					{
						projectDiagramFileName = projectDiagram.attribute(PROJECT_ATTRIBUTE_FILENAME);
					}
					else
					{
						D_OUT2(__FUNCTION__, "Object has no path attribute");
					}

					if(projectDiagram.hasAttribute(PROJECT_ATTRIBUTE_NAME))
					{
						projectDiagramName = projectDiagram.attribute(PROJECT_ATTRIBUTE_NAME);;
					}
					
				}
				else
				{
					E_OUT2(__FUNCTION__, "Failed to read object attributes")
				}

				if(projectDiagramFileName.length() > 0 && projectDiagramUid.length() > 0)
				{
					//create project diagram
					Core::Processing::Component::ComponentDiagramPtr component(new Core::Processing::Component::ComponentDiagram(projectDiagramUid));

					QFileInfo projectFileInfo(_fileName.c_str());
					QDir projectDirectory(projectFileInfo.absolutePath());
					std::string fileName = projectDirectory.absoluteFilePath(projectDiagramFileName).toStdString();
					component->load(fileName);
					component->setObjectName(projectDiagramName);
					_diagrams.append(component);
				}
			}
			else
			{
				E_OUT2(__FUNCTION__, "Failed to read diagram");
			}
		}
	}
	else
	{
		E_OUT2(__FUNCTION__, "Failed to read object group.");
	}

	return returnValue;
}

void Project::initComponents()
{

}

std::string Project::fileExtension()
{
	return PROJECT_FILE_EXTENSION;
}

void Project::copyDiagrams( const Project &rhs )
{
	for(Core::Processing::Component::ConstComponentDiagramPtr originalDiagram : rhs._diagrams)
	{
		Core::Processing::Component::ComponentDiagramPtr copyDiagram = Core::Processing::Component::ComponentDiagramPtr(new Core::Processing::Component::ComponentDiagram(*originalDiagram.get()));
		_diagrams.push_back(copyDiagram);
	}
}

std::string Project::getFileName()
{
	return _fileName;
}

void Project::setFileName( const std::string& fileName )
{
	if(fileName.length() > 0)
	{
		_fileName = fileName;
		QFileInfo fileInfo(fileName.c_str());
		setObjectName(fileInfo.baseName());
	}
}

void Project::AddDiagram( Core::Processing::Interfaces::ConstIObjectPtr diagram )
{
	if(findDiagramByUid(diagram->uid()) == nullptr)
	{
		Core::Processing::Component::ConstComponentDiagramPtr diagramPtr = boost::dynamic_pointer_cast<const Core::Processing::Component::ComponentDiagram>(diagram);
		_diagrams.push_back(diagramPtr);
	}
}

Core::Processing::Component::ConstComponentDiagramPtr Project::findDiagramByUid( const QString& uid )
{
	Core::Processing::Component::ConstComponentDiagramPtr returnValue;
	for(Core::Processing::Interfaces::ConstIObjectPtr installedDiagram : _diagrams)
	{
		if(installedDiagram->uid() == uid)
		{
			returnValue = boost::dynamic_pointer_cast<const Core::Processing::Component::ComponentDiagram>(installedDiagram);
			break;
		}
	}
	return returnValue;
}

};	// namespace Project
};	// namespace Processing
}; //namespace Core
