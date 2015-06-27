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
#include "Processing/Ports/PrimitivesArrayProviderPort.qt.h"
#include "Processing/Interfaces/IPrimitivesArrayProvider.qt.h"
#include "Processing/Interfaces/IPrimitivesArrayProcessor.qt.h"

#include "boost/signals2.hpp"

PROCESSING_REGISTER_OBJECT(Core::Processing::Ports::PrimitivesArrayProviderPort)

namespace Core
{
namespace Processing
{
namespace Ports
{
	PrimitivesArrayProviderPort::PrimitivesArrayProviderPort( QString uid /*= ""*/ )
		:Port(uid)
	{

	}

	PrimitivesArrayProviderPort::PrimitivesArrayProviderPort( const PrimitivesArrayProviderPort& rhs )
		:Port(rhs)
	{

	}

	PrimitivesArrayProviderPort::PrimitivesArrayProviderPort( const PrimitivesArrayProviderPort& rhs, QString uid )
		:Port(rhs, uid)
	{

	}

	bool PrimitivesArrayProviderPort::connectInterfaceMembers( Core::Processing::Interfaces::IProviderPtr source, Core::Processing::Interfaces::IProcessorPtr target )
	{
		Core::Processing::Interfaces::IPrimitivesArrayProviderPtr ptrSource = boost::dynamic_pointer_cast<Core::Processing::Interfaces::IPrimitivesArrayProvider>(source);
		Core::Processing::Interfaces::IPrimitivesArrayProcessorPtr ptrTarget = boost::dynamic_pointer_cast<Core::Processing::Interfaces::IPrimitivesArrayProcessor>(target);

		if(!ptrSource || !ptrTarget)
			return false;

		MEMBER_SIGNAL_CONNECT_3(provideBool, Core::Processing::Interfaces::IPrimitivesArrayProcessor::processBool, ptrTarget, _1, _2, _3);
		MEMBER_SIGNAL_CONNECT_3(provideChar, Core::Processing::Interfaces::IPrimitivesArrayProcessor::processChar, ptrTarget, _1, _2, _3);
		MEMBER_SIGNAL_CONNECT_3(provideWChar, Core::Processing::Interfaces::IPrimitivesArrayProcessor::processWChar, ptrTarget, _1, _2, _3);
		MEMBER_SIGNAL_CONNECT_3(provideSChar, Core::Processing::Interfaces::IPrimitivesArrayProcessor::processSChar, ptrTarget, _1, _2, _3);
		MEMBER_SIGNAL_CONNECT_3(provideShort, Core::Processing::Interfaces::IPrimitivesArrayProcessor::processShort, ptrTarget, _1, _2, _3);
		MEMBER_SIGNAL_CONNECT_3(provideInt, Core::Processing::Interfaces::IPrimitivesArrayProcessor::processInt, ptrTarget, _1, _2, _3);
		MEMBER_SIGNAL_CONNECT_3(provideLong, Core::Processing::Interfaces::IPrimitivesArrayProcessor::processLong, ptrTarget, _1, _2, _3);
		MEMBER_SIGNAL_CONNECT_3(provideLongLong, Core::Processing::Interfaces::IPrimitivesArrayProcessor::processLongLong, ptrTarget, _1, _2, _3);
		MEMBER_SIGNAL_CONNECT_3(provideUChar, Core::Processing::Interfaces::IPrimitivesArrayProcessor::processUChar, ptrTarget, _1, _2, _3);
		MEMBER_SIGNAL_CONNECT_3(provideUShort, Core::Processing::Interfaces::IPrimitivesArrayProcessor::processUShort, ptrTarget, _1, _2, _3);
		MEMBER_SIGNAL_CONNECT_3(provideUInt, Core::Processing::Interfaces::IPrimitivesArrayProcessor::processUInt, ptrTarget, _1, _2, _3);
		MEMBER_SIGNAL_CONNECT_3(provideULong, Core::Processing::Interfaces::IPrimitivesArrayProcessor::processULong, ptrTarget, _1, _2, _3);
		MEMBER_SIGNAL_CONNECT_3(provideULongLong, Core::Processing::Interfaces::IPrimitivesArrayProcessor::processULongLong, ptrTarget, _1, _2, _3);
		MEMBER_SIGNAL_CONNECT_3(provideFloat, Core::Processing::Interfaces::IPrimitivesArrayProcessor::processFloat, ptrTarget, _1, _2, _3);
		MEMBER_SIGNAL_CONNECT_3(provideDouble, Core::Processing::Interfaces::IPrimitivesArrayProcessor::processDouble, ptrTarget, _1, _2, _3);
		
		return true;
	}
};
};
};
