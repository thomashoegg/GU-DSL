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
#ifndef __UNIQUENAME_H__
#define __UNIQUENAME_H__
#include "CoreDLL.h"
#include <string>
#pragma warning(push)
#pragma warning(disable: 4251)

namespace Core
{
	namespace Utilities
	{
		/**
		 * \brief unique name generator class.
		 *
		 */
		class CORE_API UniqueName
		{
		public:
			/**
			 * \brief constructor
			 *
			 *	\param std::string baseName. The base name.
			 */
			UniqueName(std::string baseName);

			/**
			 * \brief destructor
			 *
			 */
			~UniqueName();

			/**
			 * \brief get unique name.
			 *
			 *	\return std::string unique name.
			 */
			std::string getName() const;
		private:
			UniqueName(const UniqueName&);
			UniqueName& operator= (const UniqueName&);
		private:
			const std::string		_uniqueName;	//!< The unique name.
		};
	}; //namespace Utilities
}; //namespace Core
 #pragma warning(pop)
#endif //__UNIQUENAME_H__
