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

#include "Processing/ProcessingPlugin.qt.h"
#include "Processing/Ports/IntegerProviderPort.qt.h"
#include "Processing/Interfaces/IIntegerProvider.qt.h"
#include "Processing/Interfaces/IIntegerProcessor.qt.h"

#include "boost/signals2.hpp"

PROCESSING_REGISTER_OBJECT(Core::Processing::Ports::IntegerProviderPort)

namespace Core
{
namespace Processing
{
namespace Ports
{
	IntegerProviderPort::IntegerProviderPort( QString uid /*= ""*/ )
		:Port(uid)
	{

	}

	IntegerProviderPort::IntegerProviderPort( const IntegerProviderPort& rhs )
		:Port(rhs)
	{

	}

	IntegerProviderPort::IntegerProviderPort( const IntegerProviderPort& rhs, QString uid )
		:Port(rhs, uid)
	{

	}

	bool IntegerProviderPort::connectInterfaceMembers( Core::Processing::Interfaces::IProviderPtr source, Core::Processing::Interfaces::IProcessorPtr target )
	{
		Core::Processing::Interfaces::IIntegerProviderPtr ptrSource = boost::dynamic_pointer_cast<Core::Processing::Interfaces::IIntegerProvider>(source);
		Core::Processing::Interfaces::IIntegerProcessorPtr ptrTarget = boost::dynamic_pointer_cast<Core::Processing::Interfaces::IIntegerProcessor>(target);

		if(!ptrSource || !ptrTarget)
			return false;

		MEMBER_SIGNAL_CONNECT_2(provideInteger, Core::Processing::Interfaces::IIntegerProcessor::processInteger, ptrTarget, _1, _2);

		return true;
	}

};
};
};
