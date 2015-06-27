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
#ifndef __PROGRAMOPTIONS_H__
#define __PROGRAMOPTIONS_H__
#include <boost/program_options.hpp>
#include "CoreDLL.h"

#pragma warning(push)
#pragma warning(disable: 4251)

namespace Core
{
	namespace Utilities
	{
		#define DIAGRAM_FOLDER "Diagrams"

		class CORE_API ProgramOptions
		{
		public:
			boost::program_options::options_description cmdOptions;
			boost::program_options::options_description hiddenOptions;
			boost::program_options::variables_map varaiblesMap;
			boost::program_options::positional_options_description postitionalOptionsDescription;
		public:
			/**
			 * \brief get static instance
			 *
			 *	\return ProgramOptions
			 */
			static ProgramOptions& getInstance();

			/**
			 * \brief destructor
			 *
			 */
			~ProgramOptions();

			/**
			 * \brief parse command line
			 *
			 */
			int parse(int argc, char* argv[]);
		private:
			/**
			* \brief constructor
			*
			*/
			ProgramOptions();
			
			/**
			* \brief copy constructor
			*		not implemented
			*/
			ProgramOptions(ProgramOptions const&);    // Don't Implement
			
			/**
			* \brief assignment operator
			*		not implemented
			*/
			void operator=(ProgramOptions const&);		// Don't implement

			/**
			 * \brief print help or version.
			 *
			 */
			int print();
		};
	}; //namespace Utilities
}; //namespace Core

#pragma warning(pop)
#endif //__PROGRAMOPTIONS_H__
