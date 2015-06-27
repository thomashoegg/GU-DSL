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

#include "Calibration/CalDataDecoupledIntensity.h"
#include "DebugOutput/DebugStream.h"

#include <sstream>



/** Constructor **/
Core::Calibration::CalDataDecoupledIntensity::CalDataDecoupledIntensity(void)
:
CalData3DBase(),
_degree(0),
_minDistance(0),
_maxDistance(0),
_polyDelta(NULL)
{
}


/** Copy Constructor **/
Core::Calibration::CalDataDecoupledIntensity::CalDataDecoupledIntensity(const Core::Calibration::CalDataDecoupledIntensity &source)
: 
CalData3DBase(source),
_polyDelta(NULL)
{
	_degree = source.getDegree();

	if(!_degree)
		return;

	_minDistance = source.minDistance();
	_maxDistance = source.maxDistance();

	if(_polyDelta)
	{
		delete[] _polyDelta;
		_polyDelta = NULL;
	}

	_polyDelta = new float[_degree +1];

	for(unsigned int param = 0; param < _degree +1; param++)
	{
		_polyDelta[param] = source.getPolyDeltaInd(param);
	}
}


/** Destructor **/
Core::Calibration::CalDataDecoupledIntensity::~CalDataDecoupledIntensity(void)
{
	if(_polyDelta)
	{
		delete[] _polyDelta;
		_polyDelta = NULL;
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
bool Core::Calibration::CalDataDecoupledIntensity::save(const char* filename) const
{
	FILE *hFile = fopen(filename, "wb");

	if (hFile == NULL) return false;

	fprintf(hFile, "<calib3D>\n");

	// Polynom
	fprintf(hFile,  "\t<PolynomDelta degree='%d' min='%.5f' max='%.5f'", _degree, _minDistance, _maxDistance);

	for(unsigned int param = 0; param < _degree +1; param++)
	{
		fprintf(hFile,  "\n\t\ta%d='%.10f'", param, _polyDelta[param]);
	}

	fprintf(hFile,  "\t>\n\t</PolynomDelta>\n");

	
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
bool Core::Calibration::CalDataDecoupledIntensity::load(const char* filename)
{
	// open file
	QFile file(filename);

	if (!file.open(QFile::ReadOnly | QFile::Text)) 
	{
		E_OUT2("CalDataDecoupledIntensity::read()", "File could not be opened!");
		return false;
	}

	QDomDocument domDocument;
	if (!domDocument.setContent(&file)) {
		E_OUT2("CalDataDecoupledIntensity::read()", "Wrong document format!");
		return false;
	}

	QDomElement root = domDocument.documentElement();

	// process file  

	// Polynom
	QDomElement child = root.firstChildElement(QString("PolynomDelta"));

	if (child.isNull()) 
	{
		E_OUT2("CalDataDecoupledIntensity::read()",  "Polynom missing!");
		return false;
	} 

	if ( !(child.hasAttribute("degree") && child.hasAttribute("min") && child.hasAttribute("max")) ) 
	{
		E_OUT2("CalDataDecoupledIntensity::read()", "Polynom min, max missing!");
		return false;
	}

	_degree		= child.attribute("degree").toUInt();
	_minDistance	= child.attribute("min").toFloat();
	_maxDistance   = child.attribute("max").toFloat();

	if(_polyDelta)
	{
		delete[] _polyDelta;
		_polyDelta = NULL;
	}
	
	_polyDelta = new float[_degree +1];

	for(unsigned int param = 0; param < _degree +1; param++)
	{
		std::stringstream sstream;
		sstream <<"a"<<param;
		if(!child.hasAttribute(sstream.str().c_str()))
		{
			E_OUT2("CalDataDecoupledIntensity::read()", "Polynom parameter missing!");
			return false;
		}

		_polyDelta[param] = child.attribute(sstream.str().c_str()).toFloat();
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
float Core::Calibration::CalDataDecoupledIntensity::correctDistance(unsigned short x, unsigned short y, 
									  float distance, bool bPostAdjustment, float intensity) const
{ 
	return distance;
}
