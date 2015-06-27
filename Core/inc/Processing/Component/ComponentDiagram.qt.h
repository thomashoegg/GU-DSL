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

#ifndef ComponentDiagram_CLASS_H
#define ComponentDiagram_CLASS_H

#ifdef WIN32
#pragma once
#endif

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
		namespace Component
		{
			#define DIAGRAM_FILE_EXTENSION ".cxdf"
			#define DIAGRAM_FILE_VERSION_1_0 "1.0"
			#define DIAGRAM_FILE_CURRENT_VERSION DIAGRAM_FILE_VERSION_1_0
			#define DIAGRAM_FILE "DiagramFile"
			#define DIAGRAM_OBJECT_GROUP "DiagramObjectGroup"
			#define DIAGRAM_OBJECT "DiagramObject"
			#define DIAGRAM_OBJECT_PORT_GROUP "ObjectPortGroup"
			#define DIAGRAM_OBJECT_PORT "Port"
			#define DIAGRAM_OBJECT_PORT_CONNECTION_GROUP "PortConnectionGroup"
			#define DIAGRAM_OBJECT_PORT_CONNECTION "PortConnection"
			#define DIAGRAM_OBJECT_PROPERTY_GROUP "PropertyGroup"
			#define DIAGRAM_OBJECT_PROPERTY "Property"
			#define DIAGRAM_ATTRIBUTE_VERSION "Version"
			#define DIAGRAM_ATTRIBUTE_TYPE "Type"
			#define DIAGRAM_ATTRIBUTE_UID "Uid"
			#define DIAGRAM_ATTRIBUTE_NAME "Name"
			#define DIAGRAM_ATTRIBUTE_TARGET "Target"
			#define DIAGRAM_ATTRIBUTE_VALUE	"Value"

			class ComponentExecutor;

			class CORE_API ComponentDiagram : public Core::Processing::Interfaces::IObject
			{
				Q_OBJECT
				DECLARE_OBJECT(ComponentDiagram)

			public:
				friend class ComponentExecutor;

			public:

				ComponentDiagram(QString uid = "", std::string fileName = "");
				ComponentDiagram(const ComponentDiagram& rhs);
				ComponentDiagram(const ComponentDiagram& rhs, QString uid);

				~ComponentDiagram();

				void run();

				bool stop();

				/**
				 * \brief get component diagram objects.
				 *
				 *	@return component diagram objects.
				 */
				QList<Core::Processing::Interfaces::IObjectPtr> getObjects();

				/**
				 * \brief save diagram.
				 *
				 *	\param std::string filename. The filename to save the diagram to.
				 *	\return bool. true if successful.
				 */
				bool saveAs(std::string filename);

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
				 * \brief get component diagram file name.
				 *
				 *	\return std::string file name.
				 */
				std::string getFileName() const;
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

				void fillProperties(QDomDocument& domDocument, QDomElement& domObject, QObject& object);

				bool loadV1(QDomDocument& domDocument);

				void copyObjectsAndConnectPorts( const ComponentDiagram &rhs );

				/**
				 * \brief set file name.
				 *
				 *	\param std::string fileName.
				 */
				void setFileName(const std::string& fileName);

				/**
				 * \brief convert a QString to QVariant.
				 *
				 *	\param const QString &s. The QString to convert.
				 *	\return QVariant. The converted QString as QVariant.
				 */
				QVariant stringToVariant(const QString &s);

				/**
				 * \brief convert a QVariant to a QString.
				 *
				 *	\param const QVariant &v. The QVariant to convert.
				 *	\param const QVariant::Type type. The QVariant type
				 *	\return QString. The converted QVariant as QString.
				 */
				QString variantToString(const QVariant &v, QVariant::Type type);

				/**
				 * \brief split QString into QStringList for QVariants.
				 *		copied from QSettingsPrivate
				 *
				 */
				QStringList splitArgs(const QString &s, int idx);

				/**
				 * \brief split QString into a QStringList for the QStringList Variant.
				 *
				 */
				QStringList split(const QString &s, int idx);

			protected:
				QThread*											_thread;				//!< The executing thread.
				bool												_isRunning;				//!< Indicator if thread is running.
				std::string											_fileName;				//!< The diagram file name relative to the application directory.
				
				QList<Core::Processing::Interfaces::IObjectPtr>		_objects;				//!< Contains all the created objects
			};

		};	// namespace Component
	};	// namespace Processing
}; //namespace Core

#endif	// ComponentDiagram_CLASS_H
