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

#include "Utilities/ProgramOptions.h"
#include "../../Config.h"
#include <iostream>

#include <iostream>

namespace Core
{
	namespace Utilities
	{
		ProgramOptions& ProgramOptions::getInstance()
		{
			static ProgramOptions instance;
			return instance;
		}

		ProgramOptions::~ProgramOptions()
		{

		}

		int ProgramOptions::parse( int argc, char* argv[] )
		{
			boost::program_options::options_description allOptions;
			allOptions.add(cmdOptions).add(hiddenOptions);

			boost::program_options::command_line_parser parser(argc, argv);
			parser.allow_unregistered();

			parser.options(allOptions);
			if(postitionalOptionsDescription.max_total_count() > 0)
				parser.positional(postitionalOptionsDescription);

			boost::program_options::store(parser.run(), varaiblesMap);
			boost::program_options::notify(varaiblesMap);
			return print();
		}

		ProgramOptions::ProgramOptions()
			: cmdOptions("Vision runtime options"),
			varaiblesMap()
		{

		}

		int ProgramOptions::print()
		{
			if(varaiblesMap.count("help"))
			{
				std::cout << cmdOptions << std::endl;
				return 1;
			}

			if(varaiblesMap.count("version"))
			{
				std::cout << "Vision runtime "<< VERSION_STRING << std::endl;
				return 1;
			}
			return 0;
		}
	}; //namespace Utilities
}; //namespace Core
