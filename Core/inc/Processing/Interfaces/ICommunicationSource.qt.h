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
#ifndef __ICOMMUNICATIONSOURCE_H__
#define __ICOMMUNICATIONSOURCE_H__
#include "Processing/PtrDefinitions.h"
#include "Communication/PtrDefinitions.h"
#include "Viewer2D.qt.h"

/**
 * \namespace Core
 * \brief Namespace to group core functionality.
 *
 */
namespace Core
{
	/**
	 * \namespace Core:Processing
	 * \brief Namespace to group data processors.
	 *
	 */
	namespace Processing
	{
		/**
		 * \namespace Core::Processing:Interfaces
		 * \brief Namespace to group processing interfaces.
		 *
		 */
		namespace Interfaces
		{
			/**
			 * \brief Data provider interface.
			 *
			 */
			class ICommunicationSource
			{
			public:
				ICommunicationSource(){}
				~ICommunicationSource(){}

				/**
				 * \brief get camera client.
				 *
				 *	@return camera client pointer
				 */
				virtual Core::Communication::CommunicationClientPtr client() const = 0;

				/**
				 * \brief set camera client.
				 *
				 *	@param client
				 */
				virtual void setCommunicationClient(Core::Communication::CommunicationClientPtr client) = 0;

				/**
				 * \brief start.
				 *
				 */
				virtual void start() = 0;

				/**
				 * \brief stop.
				 *
				 */
				virtual void stop() = 0;

				/**
				 * \brief set render window.
				 *
				 *	@param renderWindow render window to set.
				 */
				virtual bool setRenderWindow( Viewer::Viewer2DPtr renderWindow ) { return false; };
			};

		};	//namespace Interfaces
	}; //namespace Processing
}; //namespace Core

#endif // __IDATAPROVIDER_H__
