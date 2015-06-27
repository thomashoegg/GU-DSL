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

#ifndef PROCESSING_ISUPPORTMULTIPLE2DRENDERING_H
#define PROCESSING_ISUPPORTMULTIPLE2DRENDERING_H

#ifdef WIN32
#pragma once
#endif

#include "Viewer2D.qt.h"
#include "Processing/Interfaces/ISupport2DRendering.h"
#include "boost/shared_ptr.hpp"

namespace Core
{
	namespace Processing
	{
		namespace Interfaces
		{
			class ISupportMultiple2DRendering : public ISupport2DRendering
			{
			public:
				virtual ~ISupportMultiple2DRendering() {}

				virtual unsigned int required2DViewers() = 0;

				virtual unsigned int defaultViewerIndex() { return 0; }

				virtual bool setRenderWindows(std::vector<Viewer::Viewer2DPtr> renderWindows) = 0;
			};

		};	// namespace Interfaces
	};	// namespace Processing
}; //namespace Core
#endif	// PROCESSING_ISUPPORTMULTIPLE2DRENDERING_H
