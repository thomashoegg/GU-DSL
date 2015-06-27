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


#include "boost/shared_ptr.hpp"

namespace Core
{
	namespace Communication
	{
#pragma region Forward declarations
		class CommunicationClient;

		struct CommunicationData;

		class CommunicationServer;
#pragma endregion

#pragma region Type Definitions

		typedef boost::shared_ptr<CommunicationClient>							CommunicationClientPtr;
		typedef boost::shared_ptr<const CommunicationClient>					ConstCommunicationClientPtr;

		typedef boost::shared_ptr<CommunicationData>							CommunicationDataPtr;
		typedef boost::shared_ptr<const CommunicationData>						ConstCommunicationDataPtr;

		typedef boost::shared_ptr<CommunicationServer>							CommunicationServerPtr;
		typedef boost::shared_ptr<const CommunicationServer>					ConstCommunicationServerPtr;

#pragma endregion
	}	//namespace Communication
} //namespace Core
