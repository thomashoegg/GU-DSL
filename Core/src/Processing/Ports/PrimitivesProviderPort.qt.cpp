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
#include "Processing/Ports/PrimitivesProviderPort.qt.h"
#include "Processing/Interfaces/IPrimitivesProvider.qt.h"
#include "Processing/Interfaces/IPrimitivesProcessor.qt.h"

#include "boost/signals2.hpp"

PROCESSING_REGISTER_OBJECT(Core::Processing::Ports::PrimitivesProviderPort)

namespace Core
{
namespace Processing
{
namespace Ports
{
	PrimitivesProviderPort::PrimitivesProviderPort( QString uid /*= ""*/ )
		:Port(uid)
	{

	}

	PrimitivesProviderPort::PrimitivesProviderPort( const PrimitivesProviderPort& rhs )
		:Port(rhs)
	{

	}

	PrimitivesProviderPort::PrimitivesProviderPort( const PrimitivesProviderPort& rhs, QString uid )
		:Port(rhs, uid)
	{

	}

	bool PrimitivesProviderPort::connectInterfaceMembers(Core::Processing::Interfaces::IProviderPtr source, Core::Processing::Interfaces::IProcessorPtr target)
	{
		Core::Processing::Interfaces::IPrimitivesProviderPtr ptrSource = boost::dynamic_pointer_cast<Core::Processing::Interfaces::IPrimitivesProvider>(source);
		Core::Processing::Interfaces::IPrimitivesProcessorPtr ptrTarget = boost::dynamic_pointer_cast<Core::Processing::Interfaces::IPrimitivesProcessor>(target);

		if(!ptrSource || !ptrTarget)
			return false;

		MEMBER_SIGNAL_CONNECT_2(provideBool, Core::Processing::Interfaces::IPrimitivesProcessor::processBool, ptrTarget, _1, _2);
		MEMBER_SIGNAL_CONNECT_2(provideChar, Core::Processing::Interfaces::IPrimitivesProcessor::processChar, ptrTarget, _1, _2);
		MEMBER_SIGNAL_CONNECT_2(provideWChar, Core::Processing::Interfaces::IPrimitivesProcessor::processWChar, ptrTarget, _1, _2);
		MEMBER_SIGNAL_CONNECT_2(provideSChar, Core::Processing::Interfaces::IPrimitivesProcessor::processSChar, ptrTarget, _1, _2);
		MEMBER_SIGNAL_CONNECT_2(provideShort, Core::Processing::Interfaces::IPrimitivesProcessor::processShort, ptrTarget, _1, _2);
		MEMBER_SIGNAL_CONNECT_2(provideInt, Core::Processing::Interfaces::IPrimitivesProcessor::processInt, ptrTarget, _1, _2);
		MEMBER_SIGNAL_CONNECT_2(provideLong, Core::Processing::Interfaces::IPrimitivesProcessor::processLong, ptrTarget, _1, _2);
		MEMBER_SIGNAL_CONNECT_2(provideLongLong, Core::Processing::Interfaces::IPrimitivesProcessor::processLongLong, ptrTarget, _1, _2);
		MEMBER_SIGNAL_CONNECT_2(provideUChar, Core::Processing::Interfaces::IPrimitivesProcessor::processUChar, ptrTarget, _1, _2);
		MEMBER_SIGNAL_CONNECT_2(provideUShort, Core::Processing::Interfaces::IPrimitivesProcessor::processUShort, ptrTarget, _1, _2);
		MEMBER_SIGNAL_CONNECT_2(provideUInt, Core::Processing::Interfaces::IPrimitivesProcessor::processUInt, ptrTarget, _1, _2);
		MEMBER_SIGNAL_CONNECT_2(provideULong, Core::Processing::Interfaces::IPrimitivesProcessor::processULong, ptrTarget, _1, _2);
		MEMBER_SIGNAL_CONNECT_2(provideULongLong, Core::Processing::Interfaces::IPrimitivesProcessor::processULongLong, ptrTarget, _1, _2);
		MEMBER_SIGNAL_CONNECT_2(provideFloat, Core::Processing::Interfaces::IPrimitivesProcessor::processFloat, ptrTarget, _1, _2);
		MEMBER_SIGNAL_CONNECT_2(provideDouble, Core::Processing::Interfaces::IPrimitivesProcessor::processDouble, ptrTarget, _1, _2);

		return true;
	}

};
};
};
