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

#include "Calibration/CalData3DBase.h"

#include <QFile>

namespace Core
{

namespace Calibration
{



/** Constructor **/
CalData3DBase::CalData3DBase(void) 
 :
_bValid(false)
{

}

/** Copy Constructor **/
CalData3DBase::CalData3DBase(const CalData3DBase &source) 
 :
_bValid(source._bValid)
{

}


/** Destructor **/
CalData3DBase::~CalData3DBase(void)
{

}


/**
 * Stores the calibration data into a file.
 *
 * @param filename   output file
 *
 * @return
 * \c True if successful otherwise \c false.
 */
bool CalData3DBase::save(const char* filename) const
{
	 return true;
}



/**
 * Restores the calibration data from a given file.
 *
 * @param filename   input file
 *
 * @return
 *  Zero if successful, otherwise an error value is returned (see \see Error).
 */
bool CalData3DBase::load(const char* filename)
{
	if(!QFile::exists(filename))
		return false;

	return true;
}


}
}
