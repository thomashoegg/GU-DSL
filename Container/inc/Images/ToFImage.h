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


#pragma  once
#ifndef TOFIMAGE_H_
#define TOFIMAGE_H_

#include "ContainerDLL.h"
#include "Images/Image.h"
#include <QString>
#include "boost/shared_ptr.hpp"

namespace Container
{
	namespace Images
	{
		class CONTAINER_API ToFImage : public ::Container::Images::Image
		{
		public:
			static const  std::string PMD_FILE_EXTENSION;					//!< The PMD file extension (.pmd)

		public:
			/**
			* \brief Constructor of the PMD image class
			*
			* \return 
			*/
			ToFImage();

			/**
			* \brief Constructor of the PMD image class
			*
			* \param  Container::Images::Frame * frame A frame storage instance
			* \param  bool copy Determines if the frame is internally copied or used as storage
			*/
			ToFImage(::Container::Images::Frame* frame, bool copy = true);

							/**
			* \brief Constructor of the PMD image class
			*
			* \return 
			*/
			virtual ~ToFImage();


			/**
			* \brief Save the image to the hdd
			*
			* \return bool true, if successful, else false
			* \param  std::string filename The filename
			*/
			virtual bool save(std::string filename);

		protected:
		private:

		public:
			int count;
		};

		typedef boost::shared_ptr<ToFImage> ToFImagePtr;
		typedef boost::shared_ptr<const ToFImage> ConstToFImagePtr;

	}	// namespace Images
}	// namespace Container

#endif // !PMDIMAGE_H_

