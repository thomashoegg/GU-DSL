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


#ifndef DEBUGOUTPUTWIDGET_H_
#define DEBUGOUTPUTWIDGET_H_

#if WIN32
#pragma once
#endif


#include "CoreDLL.h"
#include <QListWidget>

#include "DebugOutput/DebugStream.h"

namespace Core
{

	namespace DebugOutput
	{
		class CORE_API DebugOutputWidget : public QListWidget
		{
			Q_OBJECT

		public:
			/**
			* \brief Constructor
			*
			* Constructor DebugOutputWidget()
			*
			* \return 
			*/
			DebugOutputWidget(QWidget* parent = 0);


			/**
			* \brief Destructor
			*
			* Destructor ~DebugOutputWidget()
			*
			* \return 
			*/
			~DebugOutputWidget();

			/**
			* \brief Sets the Level
			*
			* Sets the desired level that should be shown in the Widget
			*
			* \return void
			* \param  Core::DebugOutput::DebugStream::DebugLevel level The Level
			*/
			void setLevel(Core::DebugOutput::DebugStream::DebugLevel level);

		protected:
		private:

		/**
		* \brief Handles Output Messages
		*
		* Callback to handle Output Messages given by DebugStream, i.e. D_OUT, E_OUT etc.
		*
		* \return void
		* \param  std::string debugLevel		Level of the output 
		* \param  std::string functionName		Name of the function where Output takes place
		* \param  std::string msg				Output Message
		*/
		void handleOutputMessage(std::string debugLevel, std::string functionName, std::string msg);

		signals:
			void messageAdded(QListWidgetItem* pItem);

		public slots:
			/**
			* \brief 
			*
			* Shows the context menu when right clicked
			*
			* \return void
			* \param  const QPoint & pos Position where you clicked
			*/
			void ShowContextMenu(const QPoint& pos);

			/**
			* \brief Handles the message added signal
			*
			*/
			void handleMessageAdded(QListWidgetItem* pItem);

		public:
		protected:
		private:

		};
	}
}

#endif // !DEBUGOUTPUTWIDGET_H_
