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
#ifndef __PROJECT_H__
#define __PROJECT_H__

#include "CoreDLL.h"
#include <QObject>
#include "boost/shared_ptr.hpp"
#include <unordered_map>
#include "Processing/Interfaces/IObject.qt.h"
#include "Processing/PtrDefinitions.h"
#include <QThread>

QT_BEGIN_NAMESPACE
class QDomElement;
class QDomDocument;
QT_END_NAMESPACE

namespace Core
{
	namespace Processing
	{
		namespace Project
		{
			#define PROJECT_FILE_EXTENSION ".cxpf"
			#define PROJECT_FILE_VERSION_1_0 "1.0"
			#define PROJECT_FILE_CURRENT_VERSION PROJECT_FILE_VERSION_1_0
			#define PROJECT_FILE "ProjectFile"
			#define PROJECT_DIAGRAM_GROUP "ProjectDiagramGroup"
			#define PROJECT_DIAGRAM "ProjectDiagram"
			#define PROJECT_ATTRIBUTE_VERSION "Version"
			#define PROJECT_ATTRIBUTE_UID "Uid"
			#define PROJECT_ATTRIBUTE_NAME "Name"
			#define PROJECT_ATTRIBUTE_FILENAME "FileName"

			class CORE_API Project : public Core::Processing::Interfaces::IObject
			{
				Q_OBJECT
				DECLARE_OBJECT(Project)

			public:
				//friend class ProjectExecutor;

			public:

				Project(QString uid = "");
				Project(const Project& rhs);
				Project(const Project& rhs, QString uid);

				~Project();

				void run();

				bool stop();

				/**
				 * \brief get project component diagrams.
				 *
				 *	@return project component diagrams.
				 */
				QList<Core::Processing::Component::ConstComponentDiagramPtr> getDiagrams();

				/**
				* \brief add a component diagram
				*
				* \param  Core::Processing::Interfaces::ConstIObjectPtr diagram	The component diagram to add to the containing list.
				*/
				void AddDiagram(Core::Processing::Interfaces::ConstIObjectPtr diagram);

				/**
				 * \brief save diagram.
				 *
				 *	\param std::string filename. The filename to save the diagram to.
				 *	\return bool. true if successful.
				 */
				bool save(std::string filename);

				/**
				 * \brief save diagram.
				 *
				 *	\return bool. true if successful.
				 */
				bool save();

				/**
				 * \brief load diagram.
				 *
				 *	\param std::string filename. The filename to load the diagram from.
				 *	\return bool. true if successful.
				 */
				bool load(std::string filename);

				/**
				 * \brief get component diagram save file extension.
				 *
				 *	\return std::string The file extension.
				 */
				static std::string fileExtension();

				/**
				 * \brief get file name.
				 *
				 *	\return std::string the project file name.
				 */
				std::string getFileName();

				/**
				 * \brief find installed diagram by uid.
				 *
				 *	\param const String& uid. The component diagram uid to find.
				 *	\return ConstComponentDiagramPtr The found component diagram pointer.
				 */
				Core::Processing::Component::ConstComponentDiagramPtr findDiagramByUid(const QString& uid);
			public slots:
				void doWork();
				void finished();

	Q_SIGNALS:
				void executionStopped();

			protected:
				void init();

				virtual void registerProcessorPortListeners(Core::Processing::Interfaces::IPortPtr port) {/* throw std::logic_error("The method or operation is not implemented, but should be implemented in a subclass.");*/ }

				virtual bool process(Core::Processing::Interfaces:: ConstIObjectParametersPtr parameters );

				virtual void initComponents();

				bool loadV1(QDomDocument& domDocument);

				void copyDiagrams( const Project &rhs );

				/**
				 * \brief set file name.
				 *
				 *	\param std::string fileName.
				 */
				void setFileName(const std::string& fileName);
			protected:
				QThread*															_thread;
				bool																_isRunning;
				std::string															_fileName;				//!< The project file name relative to the application directory.
				QList<Core::Processing::Component::ConstComponentDiagramPtr>		_diagrams;				//!< Contains all the created diagrams.
			};

		};	// namespace Project
	};	// namespace Processing
}; //namespace Core

#endif	// ComponentDiagram_CLASS_H
