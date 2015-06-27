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

#include "DebugOutput/DebugOutputWidget.qt.h"

#include "QMenu"



Core::DebugOutput::DebugOutputWidget::DebugOutputWidget(QWidget* parent /* = 0 */)
{
	//Core::DebugOutput::DebugStream::instance().setLevel(Core::DebugOutput::DebugStream::Debug);
	Core::DebugOutput::DebugStream::instance().MessageSig.connect(boost::bind(&Core::DebugOutput::DebugOutputWidget::handleOutputMessage, this, _1, _2, _3));

	this->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu(const QPoint&)));
	connect(this, SIGNAL(messageAdded(QListWidgetItem*)), this, SLOT(handleMessageAdded(QListWidgetItem*)));
}

Core::DebugOutput::DebugOutputWidget::~DebugOutputWidget()
{
	Core::DebugOutput::DebugStream::instance().MessageSig.disconnect_all_slots();
}

void Core::DebugOutput::DebugOutputWidget::handleOutputMessage( std::string debugLevel, std::string functionName, std::string msg )
{
	//TODO: Check if there might be memory leak
	
	std::string output = "" + debugLevel + " " + functionName + ": " + msg;
	QString qoutput = QString::fromLatin1(output.c_str());
	
	QListWidgetItem* pItem = new QListWidgetItem(qoutput);
	if(debugLevel == "[DEBUG]") pItem->setForeground(QColor(154,206,235));
	if(debugLevel == "[ERROR]") pItem->setForeground(Qt::red);
	//this->addItem(pItem);

	//	QAbstractItemView::scrollToBottom();

	emit messageAdded(pItem);
}

void Core::DebugOutput::DebugOutputWidget::setLevel( Core::DebugOutput::DebugStream::DebugLevel level )
{
	Core::DebugOutput::DebugStream::instance().setLevel(level);
}

void Core::DebugOutput::DebugOutputWidget::ShowContextMenu( const QPoint& pos)
{
	QPoint globalPos = this->mapToGlobal(pos);

	QMenu contextMenu;

	contextMenu.addAction("Clear Output");

	QAction* selectedItem = contextMenu.exec(globalPos);
	if(selectedItem)
	{
		//D_OUT2(__FUNCTION__, "Clear selected");
		this->clear();
	}
	else
	{

	}
}

void Core::DebugOutput::DebugOutputWidget::handleMessageAdded(QListWidgetItem* pItem)
{
	if(!pItem)
		return;

	if(this->count() > 200)
	{
		QListWidgetItem* item = this->takeItem(0);
		delete item;
		item = nullptr;
	}

	addItem(pItem);
	QAbstractItemView::scrollToBottom();
}
