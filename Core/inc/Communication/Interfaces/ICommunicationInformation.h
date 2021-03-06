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
#ifndef __COMMUNICATIONINFORMATION_H__
#define __COMMUNICATIONINFORMATION_H__
#include <string>
#include "CoreDLL.h"
#include <boost/shared_ptr.hpp>

namespace Core
{
	namespace Communication
	{
		namespace Interfaces
		{
			class ICommunicationInformation;

			typedef boost::shared_ptr<ICommunicationInformation>						ICommunicationInformationPtr;
			typedef boost::shared_ptr<const ICommunicationInformation>					ConstICommunicationInformationPtr;

			/**
			* \brief Communication structure to store Communication data.
			*
			*/
			class CORE_API ICommunicationInformation
			{
			public:
				ICommunicationInformation();
				virtual ~ICommunicationInformation() = 0;
			};
		}; //namespace Interfaces
	}; //namespace Communication
}; // namespace Core
#endif //__COMMUNICATIONINFORMATION_H__
