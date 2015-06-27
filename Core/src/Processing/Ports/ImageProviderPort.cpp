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
#include "Processing/Ports/ImageProviderPort.qt.h"
#include "Processing/Interfaces/IImageProvider.qt.h"
#include "Processing/Interfaces/IImageProcessor.qt.h"

#include "boost/signals2.hpp"

namespace Core
{
namespace Processing
{
namespace Ports
{

PROCESSING_REGISTER_OBJECT(Core::Processing::Ports::ImageProviderPort)

ImageProviderPort::ImageProviderPort(QString uid /*= ""*/)
	:Port(uid)
{
	
}

ImageProviderPort::ImageProviderPort( const ImageProviderPort& rhs )
	:Port(rhs)
{
	// TODO: Rebind connections
}

ImageProviderPort::ImageProviderPort( const ImageProviderPort& rhs, QString uid )
	: Port (rhs, uid)
{

}


bool ImageProviderPort::connectInterfaceMembers(Core::Processing::Interfaces::IProviderPtr source, Core::Processing::Interfaces::IProcessorPtr target)
{
	Core::Processing::Interfaces::IImageProviderPtr ptrSource = boost::dynamic_pointer_cast<Core::Processing::Interfaces::IImageProvider>(source);
	Core::Processing::Interfaces::IImageProcessorPtr ptrTarget = boost::dynamic_pointer_cast<Core::Processing::Interfaces::IImageProcessor>(target);
	
	if(!ptrSource || !ptrTarget)
		return false;

	MEMBER_SIGNAL_CONNECT_2(provideImage, Core::Processing::Interfaces::IImageProcessor::processImage, ptrTarget, _1, _2);
	MEMBER_SIGNAL_CONNECT_3(provideShowImage, Core::Processing::Interfaces::IImageProcessor::processShowImage, ptrTarget, _1, _2, _3);

	return true;
}

};	// namespace Ports
};	// namespace Processing
}; //namespace Core
