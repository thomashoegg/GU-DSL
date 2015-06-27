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

#ifndef ENUMS_H_
#define ENUMS_H_

#if WIN32
#pragma once
#endif

//enum DataType
//{
//	Undefined = 0x0000,    //!< datatype not defined
//	Char      = 0x1400,    //!< unsigned char
//	UChar     = 0x1401,    //!< char
//	Short     = 0x1402,    //!< unsigned short
//	UShort    = 0x1403,    //!< short
//	Int       = 0x1404,    //!< unsigned int
//	UInt      = 0x1405,    //!< int
//	Float     = 0x1406,    //!< float
//	Double    = 0x140A,    //!< double
//};

enum DataOrder 
{
	Interleaved, //!< interleaved layers
	Separated    //!< separated layers
};
//
//enum CoordinateType
//{
//	UNDEFINED, POLAR, CARTESIAN
//};
//
//enum PixelData
//{
//	Pixel, Point
//};

#endif /*ENUMS_H*/ 
