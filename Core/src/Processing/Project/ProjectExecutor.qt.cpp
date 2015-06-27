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


#include "Processing/Project/ProjectExecutor.qt.h"
#include "Processing/Project/Project.qt.h"
#include "Processing/Component/ComponentDiagram.qt.h"

#include <QApplication>

#include "Processing/Object/Class.qt.h"
#include "Processing/Interfaces/ISupport2DRendering.h"
#include "Processing/Interfaces/ISupportMultiple2DRendering.h"
#include "Processing/Interfaces/ISupport3DRendering.h"
#include "Processing/Interfaces/ISupportMultiple3DRendering.h"

namespace Core
{
namespace Processing
{
namespace Project
{

	ProjectExecutor& ProjectExecutor::instance()
	{
		static ProjectExecutor instance;
		return instance;
	}

	ProjectExecutor::ProjectExecutor()
		:_project(nullptr)
	{
	}


	ProjectExecutor::~ProjectExecutor()
	{
		stopProject();
		cleanup();
	}

	void ProjectExecutor::setProject( Core::Processing::Project::ProjectPtr project )
	{
		if(project != _project)
		{
			stopComponentDiagrams();

			_project = project;

			QList<Core::Processing::Component::ConstComponentDiagramPtr> diagrams = _project->getDiagrams();
			for(Core::Processing::Component::ConstComponentDiagramPtr diagram: diagrams)
			{
				installComponentDiagram(diagram);
				runComponentDiagram(diagram->templateUid());
			}
		}
	}

	Core::Processing::Project::ProjectPtr ProjectExecutor::runProject()
	{
		runComponentDiagrams();	
		return _project;
	}

	void ProjectExecutor::stopProject()
	{
		stopComponentDiagrams();
	}

	void ProjectExecutor::cleanup()
	{
		for(int i = 0; i < _runningComponentDiagrams.count(); i++ )
		{
			QList<Core::Processing::Interfaces::IObjectPtr> objects = _runningComponentDiagrams[i]->getObjects();
			for(int o = 0; o < objects.count(); o++)
			{
				cleanupObject(objects[o]);
			}
		}
	}

	Core::Processing::Project::ProjectPtr ProjectExecutor::getProject()
	{
		return _project;
	}

	void ProjectExecutor::installComponentDiagrams( QList<Core::Processing::Interfaces::ConstIObjectPtr> diagrams )
	{
		if(diagrams.length() > 0)
		{
			for(Core::Processing::Interfaces::ConstIObjectPtr diagram : diagrams)
			{
				installComponentDiagram(diagram);
			}
		}
		else
		{
			installComponentDiagram(Core::Processing::Component::ComponentDiagramPtr(new Core::Processing::Component::ComponentDiagram()));
		}
	}

	void ProjectExecutor::installComponentDiagram( Core::Processing::Interfaces::ConstIObjectPtr diagram )
	{
		if(_project == nullptr)
		{
			_project = Core::Processing::Project::ProjectPtr(new Core::Processing::Project::Project());
		}
		_project->AddDiagram(diagram);
	}

	Core::Processing::Component::ComponentDiagramPtr ProjectExecutor::runComponentDiagram(const QString& uid)
	{
		return runComponentDiagram(_project->findDiagramByUid(uid));
	}

	Core::Processing::Component::ComponentDiagramPtr ProjectExecutor::runComponentDiagram(Core::Processing::Component::ConstComponentDiagramPtr componentDiagram)
	{
		Core::Processing::Component::ComponentDiagramPtr newDiagram;
		if(componentDiagram)
		{
			newDiagram = boost::dynamic_pointer_cast<Core::Processing::Component::ComponentDiagram>(componentDiagram->create());
			_runningComponentDiagrams.append(newDiagram);
		}
		return newDiagram;
	}


	QList<Core::Processing::Component::ComponentDiagramPtr> ProjectExecutor::runComponentDiagrams()
	{
		QList<Core::Processing::Component::ComponentDiagramPtr> runningDiagrams;
		for(Core::Processing::Component::ConstComponentDiagramPtr diagram : _project->getDiagrams())
		{
			runningDiagrams.push_back(runComponentDiagram(diagram));
		}
		return runningDiagrams;
	}

	QList<Core::Processing::Component::ComponentDiagramPtr> ProjectExecutor::getRunningComponentDiagrams()
	{
		return _runningComponentDiagrams;
	}

	void ProjectExecutor::stopComponentDiagrams()
	{
		for(int i = _runningComponentDiagrams.count() -1 ; i >= 0 ; i-- )
		{
			stopComponentDiagram(_runningComponentDiagrams[i]);
		}
		_runningComponentDiagrams.clear();
	}


	void ProjectExecutor::stopComponentDiagram( const QString& uid )
	{
		if(Core::Processing::Component::ComponentDiagramPtr diagram = findRunningDiagramByUid(uid))
		{
			stopComponentDiagram(diagram);
		}
	}

	void ProjectExecutor::stopComponentDiagram( Core::Processing::Component::ComponentDiagramPtr runningDiagram )
	{
		QList<Core::Processing::Interfaces::IObjectPtr> objects = runningDiagram->getObjects();
		for(int o = 0; o < objects.count(); o++)
		{
			cleanupObject(objects[o]);
		}
		runningDiagram->stop();
	}

	Core::Processing::Component::ComponentDiagramPtr ProjectExecutor::findRunningDiagramByUid( const QString& uid )
	{
		Core::Processing::Component::ComponentDiagramPtr returnValue;
		for(Core::Processing::Component::ComponentDiagramPtr runningDiagram : _runningComponentDiagrams)
		{
			if(runningDiagram->uid() == uid)
			{
				returnValue = runningDiagram;
				break;
			}
		}
		return returnValue;
	}


	void ProjectExecutor::cleanupObject( Core::Processing::Interfaces::IObjectPtr object)
	{
		Core::Processing::Object::ClassPtr classObject = boost::dynamic_pointer_cast<Core::Processing::Object::Class>(object);
		if(classObject != nullptr)
		{
			classObject->stop();
		}

		Core::Processing::Interfaces::ISupport2DRendering* supportRendering2D = dynamic_cast<Core::Processing::Interfaces::ISupport2DRendering*>(object.get());
		Core::Processing::Interfaces::ISupportMultiple2DRendering* supportMultipleRendering2D = dynamic_cast<Core::Processing::Interfaces::ISupportMultiple2DRendering*>(object.get());

		if(supportMultipleRendering2D != nullptr)
		{
			supportMultipleRendering2D->setRenderWindows(std::vector<Viewer::Viewer2DPtr>());
		}
		else if(supportRendering2D != nullptr)
		{
			supportRendering2D->setRenderWindow(nullptr);
		}

		Core::Processing::Interfaces::ISupport3DRendering* supportRendering3D = dynamic_cast<Core::Processing::Interfaces::ISupport3DRendering*>(object.get());
		Core::Processing::Interfaces::ISupportMultiple3DRendering* supportMultipleRendering3D = dynamic_cast<Core::Processing::Interfaces::ISupportMultiple3DRendering*>(object.get());

		if(supportMultipleRendering3D != nullptr)
		{
			supportMultipleRendering3D->setRenderWindows(std::vector<Viewer::Viewer3DPtr>());
		}
		else if(supportRendering3D != nullptr)
		{
			supportRendering3D->setRenderWindow(nullptr);
		}
	}
};	// namespace Project
};	// namespace Processing
}; //namespace Core
