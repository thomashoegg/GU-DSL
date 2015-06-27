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


struct int2Custom
{
	int2Custom()
	{
		x = 0;
		y = 0;
	}
	int2Custom(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	int x, y;
};

struct float3Custom
{
	float x,y,z;
	float3Custom() {};

	float3Custom(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	bool operator==(const float3Custom& rhs) const
	{
		return (this->x == rhs.x && this->y == rhs.y && this->z == rhs.z);
	}
};
struct float4Custom
{
	float x,y,z,w;

	float4Custom() {}

	float4Custom(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	bool operator==(const float4Custom& rhs) const
	{
		return (this->x == rhs.x && this->y == rhs.y && this->z == rhs.z && this->w == rhs.w);
	}
};
struct float8Custom
{
	float x,y,z,w;
	float nx,ny,nz,nw;
};

enum class MorphologyMode
{
	ERODE,
	DILATE
};

enum class MedianFilterFailureStragety
{
	WRITE_NAN,
	WRITE_INPUT
};

enum class KernelClampingMode
{
	CLAMP_TO_BORDER = 0,
	WRITE_NAN = 1
};

// What to do if no filtering can be applied
enum class KernelFilterBorderMode
{
	WRITE_ORIGINAL = 0,
	WRITE_ZERO = 1,
	WRITE_NAN = 2	
};

// What to do if no filtering can be applied
enum class KernelFilterCalculationMode
{
	INVALID_VALUE_SKIP = 0,
	INVALID_VALUE_USE_INPUT = 1,
	INVALID_VALUE_OUTPUT_NAN = 2	
};

enum class PyrDownFilterMethod
{
	//UNFILTERED,
	FILTERED_BILATERAL,
	FILTERED_BINOMIAL,
	FILTERED_MEAN,
	FILTERED_CUSTOM_MEAN,
	NO_FILTER
};

enum class NormalCalculationMethod
{
	CENTRAL_DIFFERENCES,
	CENTRAL_DIFFERENCES_ADVANCED,
	EIGEN_VECTOR
};

enum class PointPairDetectionMethod
{
	SMALL_DISTANCE,
	OVERLAPPING_RADII
};
