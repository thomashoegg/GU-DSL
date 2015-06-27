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
#ifndef __PROJECTEXECUTOR_H__
#define __PROJECTEXECUTOR_H__

#include "CoreDLL.h"
#include <QObject>
#include "boost/shared_ptr.hpp"
#include <unordered_map>
#include "Processing/Interfaces/IObject.qt.h"
#include "Processing/PtrDefinitions.h"

namespace Core
{
	namespace Processing
	{
		namespace Project
		{
			class CORE_API ProjectExecutor : public QObject
			{
				Q_OBJECT

			public:
				/**
				 * \brief returns the single instance
				 *
				 *	\return ComponentExecutor& instance
				 */
				static ProjectExecutor& instance();

				/**
				 * \brief destructor
				 *
				 */
				~ProjectExecutor();

				/**
				* \brief set project
				*
				* \param  Core::Processing::Project::ProjectPtr project	The project to be set.
				*/
				void setProject(Core::Processing::Project::ProjectPtr project);

				/**
				* \brief Runs the project.
				*
				* \return ProjectPtr the running project.
				*/
				ProjectPtr runProject();

				/**
				* \brief Stops the running project.
				*/
				void stopProject();

				/**
				* \brief Tries to cleanup all components
				*/
				void cleanup();

				/**
				 * \brief get the project.
				 *
				 *	\return Core::Processing::Project::ProjectPtr the running project.
				 */
				Core::Processing::Project::ProjectPtr getProject();

				/**
				* \brief Installs all loaded component diagrams
				*
				* \param  QList<Core::Processing::Interfaces::ConstIObjectPtr> diagrams	The list of component diagrams
				*/
				void installComponentDiagrams(QList<Core::Processing::Interfaces::ConstIObjectPtr> diagrams);

				/**
				* \brief Installs a component diagram
				*
				* \param  Core::Processing::Interfaces::ConstIObjectPtr diagram	The component diagram to add to the containing list.
				*/
				void installComponentDiagram(Core::Processing::Interfaces::ConstIObjectPtr diagram);

				/**
				* \brief Runs all the installed component diagrams
				*
				* \return QList<ComponentDiagramPtr> List of new running component diagrams.
				*/
				QList<Core::Processing::Component::ComponentDiagramPtr> runComponentDiagrams();

				/**
				 * \brief Runs the installed component diagram with the given uid.
				 *
				 *	\param const QString& uid. The component diagram uid to run.
				 *	\return ComponentDiagramPtr The new running component diagram.
				 */
				Core::Processing::Component::ComponentDiagramPtr runComponentDiagram(const QString& uid);

				/**
				* \brief Stops all the running component diagrams
				*/
				void stopComponentDiagrams();

				/**
				 * \brief Stops the running component diagram with the given uid.
				 *
				 *	\param const QString& uid. The component diagram uid to stop.
				 */
				void stopComponentDiagram(const QString& uid);

				/**
				 * \brief get running component diagrams.
				 *
				 *	\return QList<Core::Processing::Component::ComponentDiagramPtr> running component diagrams.
				 */
				QList<Core::Processing::Component::ComponentDiagramPtr> getRunningComponentDiagrams();

			protected:
				/**
				 * \brief cleanup the given object
				 *
				 *	@param Core::Processing::Interfaces::IObjectPtr object The object to cleanup.
				 */
				void cleanupObject( Core::Processing::Interfaces::IObjectPtr object);

				/**
				 * \brief find running diagram by uid.
				 *
				 *	\param const String& uid. The component diagram uid to find.
				 *	\return ConstComponentDiagramPtr The found component diagram pointer.
				 */
				Core::Processing::Component::ComponentDiagramPtr findRunningDiagramByUid(const QString& uid);

				/**
				 * \brief Runs the installed component diagram.
				 *
				 *	\param ConstComponentDiagramPtr componentDiagram. The component diagram to run.
				 *	\return ComponentDiagramPtr The new running component diagram.
				 */
				Core::Processing::Component::ComponentDiagramPtr runComponentDiagram(Core::Processing::Component::ConstComponentDiagramPtr componentDiagram);

				/**
				 * \brief Stops the running component diagram.
				 *
				 *	\param ComponentDiagramPtr runningDiagram. The component diagram to stop.
				 */
				void stopComponentDiagram( Core::Processing::Component::ComponentDiagramPtr runningDiagram );

			private:
				/**
				 * \brief private constructor
				 *
				 */
				ProjectExecutor();

				/**
				 * \brief private copy constructor
				 *
				 */
				ProjectExecutor(const ProjectExecutor& );

				/**
				 * \brief private assignment operator
				 *
				 */
				ProjectExecutor& operator= (const ProjectExecutor& );
			protected:
				Core::Processing::Project::ProjectPtr						_project;						//!< the project.
				QList<Core::Processing::Component::ComponentDiagramPtr>		_runningComponentDiagrams;		
			};

		}	// namespace Project
	}	// namespace Processing
}; //namespace Core
#endif	// __PROJECTEXECUTOR_H__
