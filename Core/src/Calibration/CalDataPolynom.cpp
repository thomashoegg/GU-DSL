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

#include <stdio.h>
#include <stdexcept>
#include <QtXml>

#include "Calibration/CalDataPolynom.h"
#include "DebugOutput/DebugStream.h"

#include <sstream>

namespace Core
{
	namespace Calibration
	{


/** Constructor **/
CalDataPolynom::CalDataPolynom(void)
	: CalData3DBase(),
	_minDistance(0),
	_maxDistance(0)
{
	_polyParams = new double[NB_POLY_PARAMS];
}


/** Copy Constructor **/
CalDataPolynom::CalDataPolynom(const CalDataPolynom &source)
: 
CalData3DBase(source)
{
	_polyParams = new double[NB_POLY_PARAMS];

	_minDistance = source.minDistance();
	_maxDistance = source.maxDistance();

	for(unsigned int param = 0; param < NB_POLY_PARAMS; param++)
	{
		_polyParams[param] = source.getPolyParamInd(param);
	}
}


/** Destructor **/
CalDataPolynom::~CalDataPolynom(void)
{
	if(_polyParams)
	{
		delete[] _polyParams;
		_polyParams = NULL;
	}
}




/**
* Stores the calibration data into a file.
*
* @param filename   output file
*
* @return
*  \c True if successful otherwise \c false.
*/
bool CalDataPolynom::save(const char* filename) const
{
	FILE *hFile = fopen(filename, "wb");

	if (hFile == NULL) return false;

	fprintf(hFile, "<calib3D>\n");

	// Polynom
	fprintf(hFile,  "\t<Polynom min='%.5f' max='%.5f'", _minDistance, _maxDistance);

	for(unsigned int param = 0; param < NB_POLY_PARAMS; param++)
	{
		fprintf(hFile,  "\n\t\td%d='%.10f'", param, _polyParams[param]);
	}

	fprintf(hFile,  "\t>\n\t</Polynom>\n");

	
	fprintf(hFile, "</calib3D>\n");
	fclose(hFile);

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
bool CalDataPolynom::load(const char* filename)
{
	if(!QFile::exists(filename))
		return false;

	// open file
	QFile file(filename);

	if (!file.open(QFile::ReadOnly | QFile::Text)) 
	{
		E_OUT2(__FUNCTION__, "File could not be opened!");
		return false;
	}

	QDomDocument domDocument;
	if (!domDocument.setContent(&file)) {
		E_OUT2(__FUNCTION__, "Wrong document format!");
		return false;
	}

	QDomElement root = domDocument.documentElement();

	// process file  

	// Polynom
	QDomElement child = root.firstChildElement(QString("Polynom"));

	if (child.isNull()) 
	{
		E_OUT2(__FUNCTION__,  "Polynom missing!");
		return false;
	} 

	if ( !(child.hasAttribute("min") && child.hasAttribute("max")) ) 
	{
		E_OUT2(__FUNCTION__, "Polynom min, max missing!");
		return false;
	}

	_minDistance      = child.attribute("min").toDouble();
	_maxDistance      = child.attribute("max").toDouble();

	for(unsigned int param = 0; param < NB_POLY_PARAMS; param++)
	{
		std::stringstream sstream;
		sstream <<"d"<<param;
		if(!child.hasAttribute(sstream.str().c_str()))
		{
			E_OUT2(__FUNCTION__, "Polynom parameter missing!");
			return false;
		}

		_polyParams[param] = child.attribute(sstream.str().c_str()).toDouble();
	}

	return true;
}


/**
* Corrects the given distance information.
*
* @param x                 x-coordinate
* @param y                 y-coordinate
* @param distance          distance value
* @param bPostAdjustment   enable post-adjustment
* @param intensity         intensity value <b>(ignored for B-Spline fitting)</b>
*
* @throw
*  <tt>invalid_argument</tt> if the given pixel coordinate lies outside the 
*  image plane.
*/
float CalDataPolynom::correctDistance(unsigned short x, unsigned short y, 
									  float distance, bool bPostAdjustment, float intensity) const
{ 
	return distance;
}


}
}
