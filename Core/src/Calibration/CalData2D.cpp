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



#include <float.h>
#include <stdio.h>
#include <QtXml>

#include "Calibration/CalData2D.h"
#include "DebugOutput/DebugStream.h"

namespace Core
{
namespace Calibration
{


/**
* Constructor.
*/
CalData2D::CalData2D(void) 
{
	m_imgsize[0]     = 0;
	m_imgsize[1]     = 0;
	m_pxlsize[0]     = 0.0;
	m_pxlsize[1]     = 0.0;
	m_focallength[0] = 0.0;
	m_focallength[1] = 0.0;
	m_center[0]      = 0.0;
	m_center[1]      = 0.0;
	m_radial[0]      = 0.0;
	m_radial[1]      = 0.0;
	m_tangential[0]  = 0.0;
	m_tangential[1]  = 0.0;

}


/**
*  Copy Constructor.
*/
CalData2D::CalData2D(const CalData2D &source) 
{
	m_imgsize[0]     = source.m_imgsize[0];
	m_imgsize[1]     = source.m_imgsize[1];
	m_pxlsize[0]     = source.m_pxlsize[0];
	m_pxlsize[1]     = source.m_pxlsize[1];
	m_focallength[0] = source.m_focallength[0];
	m_focallength[1] = source.m_focallength[1];
	m_center[0]      = source.m_center[0];
	m_center[1]      = source.m_center[1];
	m_radial[0]      = source.m_radial[0];
	m_radial[1]      = source.m_radial[1];
	m_tangential[0]  = source.m_tangential[0];
	m_tangential[1]  = source.m_tangential[1];

}

/* **/
CalData2D& CalData2D::operator=(const CalData2D& source)
{
	m_imgsize[0]     = source.m_imgsize[0];
	m_imgsize[1]     = source.m_imgsize[1];
	m_pxlsize[0]     = source.m_pxlsize[0];
	m_pxlsize[1]     = source.m_pxlsize[1];
	m_focallength[0] = source.m_focallength[0];
	m_focallength[1] = source.m_focallength[1];
	m_center[0]      = source.m_center[0];
	m_center[1]      = source.m_center[1];
	m_radial[0]      = source.m_radial[0];
	m_radial[1]      = source.m_radial[1];
	m_tangential[0]  = source.m_tangential[0];
	m_tangential[1]  = source.m_tangential[1];

	return *this;
}



/** Calculate world coordinate for the given pixel and cart distance. */
void CalData2D::cartesian(unsigned short x, unsigned short y, 
	float cartDist, float *p) const
{
	double _x = (x - m_center[0])/m_focallength[0];
	double _y = (m_center[1] - y)/m_focallength[1];

	p[0] = cartDist*_x;
	p[1] = cartDist*_y;
	p[2] = cartDist;    
}



/** Calculate viewing frustum */
void CalData2D::frustum(float maxDist, float (&frustum)[8][3]) const
{
	const float coord[4][2] = {
		-0.5,               -0.5,       
		m_imgsize[0] - 0.5, -0.5,       
		m_imgsize[0] - 0.5, m_imgsize[1] - 0.5,
		-0.5,               m_imgsize[1] - 0.5
	};

	for (int i = 0; i < 8; i++) 
		cartesian(coord[i%4][0], coord[i%4][1], (i > 3) ? -0.012 : -maxDist, frustum[i]);
}


/**
* Stores the calibration data into a file.
*
* @param filename   output file
*
* @return
* \c True if successful otherwise \c false.
*/
bool CalData2D::save(const char* filename) const
{
	FILE *hFile = fopen(filename, "wb");

	if (hFile == NULL) return false;

	fprintf(hFile, "<calib2D>\n");
	fprintf(hFile, "<imagesize   x='%u' y='%u' />\n", m_imgsize[0], m_imgsize[1]);
	fprintf(hFile, "<pixelsize   x='%3.2f' y='%3.2f' />\n", m_pxlsize[0], m_pxlsize[1]);
	fprintf(hFile, "<focallength x='%3.2f' y='%3.2f' />\n", m_focallength[0], m_focallength[1]);
	fprintf(hFile, "<center      x='%3.2f' y='%3.2f' />\n", m_center[0], m_center[1]);
	fprintf(hFile, "<radial      r1='%2.6f' r2='%2.6f' />\n", m_radial[0], m_radial[1]);
	fprintf(hFile, "<tangential  t1='%2.6f' t2='%2.6f' />\n", m_tangential[0], m_tangential[1]);
	fprintf(hFile, "</calib2D>\n");

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
bool CalData2D::load(const char* filename)
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
	if (!domDocument.setContent(&file)) 
	{
		D_OUT2(__FUNCTION__, "Wrong document format!");
		return false;
	}

	file.close();

	QDomElement root = domDocument.documentElement();

	if (root.tagName().compare(QString("calib2D")) != 0) 
	{
		E_OUT2(__FUNCTION__, "Wrong file format!");
		return false;
	}

	// process file  

	// image size
	QDomElement child = root.firstChildElement(QString("imagesize"));

	if (child.isNull()) 
	{
		E_OUT2(__FUNCTION__, "imagesize missing!");
		return false;
	} 

	if ( !(child.hasAttribute("x") && child.hasAttribute("y")) ) 
	{
		E_OUT2(__FUNCTION__, "imagesize attributes missing!");
		return false;
	}

	m_imgsize[0] = child.attribute("x").toUInt();
	m_imgsize[1] = child.attribute("y").toUInt();


	// pixel size
	child = root.firstChildElement(QString("pixelsize"));

	if (child.isNull())
	{
		E_OUT2(__FUNCTION__, "pixelsize missing!");
		return false;
	} 

	if ( !(child.hasAttribute("x") && child.hasAttribute("y")) ) 
	{
		E_OUT2(__FUNCTION__, "pixelsize attributes missing!");
		return false;
	}

	m_pxlsize[0] = child.attribute("x").toDouble();
	m_pxlsize[1] = child.attribute("y").toDouble();

	if (m_pxlsize[0] < FLT_EPSILON || m_pxlsize[1] < FLT_EPSILON)
	{
		E_OUT2(__FUNCTION__, "pixelsize missing!");
		//return false;
	}

	// focal length
	child = root.firstChildElement(QString("focallength"));

	if (child.isNull()) 
	{
		E_OUT2(__FUNCTION__, "focallength  missing!");
		return false;
	}

	if ( !(child.hasAttribute("x") && child.hasAttribute("y")) )
	{
		E_OUT2(__FUNCTION__, "focallength attributes missing!");
		return false;
	}

	m_focallength[0] = child.attribute("x").toDouble();
	m_focallength[1] = child.attribute("y").toDouble();


	// image center
	child = root.firstChildElement(QString("center"));

	if (child.isNull()) 
	{
		E_OUT2(__FUNCTION__, "center missing!");
		return false;
	}

	if ( !(child.hasAttribute("x") && child.hasAttribute("y")) ) 
	{
		E_OUT2(__FUNCTION__, "center attributes missing!");
		return false;
	}

	m_center[0] = child.attribute("x").toDouble();
	m_center[1] = child.attribute("y").toDouble();

	// radial distortion
	child = root.firstChildElement(QString("radial"));
	if (child.isNull())
	{
		E_OUT2(__FUNCTION__, "radial distortion missing!");
		return false;
	} 

	if ( !(child.hasAttribute("r1") && child.hasAttribute("r2")) )
	{
		E_OUT2(__FUNCTION__, "radial distortion attributes missing!");
		return false;
	}

	m_radial[0] = child.attribute("r1").toDouble();
	m_radial[1] = child.attribute("r2").toDouble();

	// tangential distortion
	child = root.firstChildElement(QString("tangential"));

	if (child.isNull()) 
	{
		E_OUT2(__FUNCTION__, "tangential distortion missing!");
		return false;
	} 

	if ( !(child.hasAttribute("t1") && child.hasAttribute("t2")) )
	{
		E_OUT2(__FUNCTION__, "tangential distortion attributes missing!");
		return false;
	}

	m_tangential[0] = child.attribute("t1").toDouble();
	m_tangential[1] = child.attribute("t2").toDouble();

	return true;
}

}
}
