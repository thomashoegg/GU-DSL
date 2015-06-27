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


#define _USE_MATH_DEFINES 1

#include <float.h>
#include <math.h>
#include <QtOpenGL/qgl.h>
#include <GL/glu.h>

#include "QMatrix4x4"

#include "MayaCamera.h"


/**
* Constructor
*
*/
MayaCamera::MayaCamera(void)
{
	setDefaultValues(0.000f, 0.000f, 0.000f, 0.000f, 0.000f, 15.0f);
	reset();
}

/**
* Destructor
*
*/
MayaCamera::~MayaCamera(void)
{
}

/**
* Resets the camera view
*
*/
void MayaCamera::reset()
{

	m_rotx = m_defaultRotx;
	m_roty = m_defaultRoty;
	m_rotz = m_defaultRotz;
	m_tx = m_defaultTx;
	m_ty = m_defaultTy;
	m_tz = m_defaultTz;
	m_lastx = 0;
	m_lasty = 0;

	m_fRotation = 0;
	m_mirrorX = false;
	m_mirrorY = false;
}

void MayaCamera::updateCameraPosition(char direction)
{
	switch(direction)
	{
	// right-direction
	case 'R':
		m_tx += 0.1f;
		break;
	// left-direction
	case 'L':
		m_tx -= 0.1f;
		break;
	// down
	case 'D':
		m_ty -= 0.1f;
		break;
	// up
	case 'U':
		m_ty += 0.1f;
		break;
	// reset 
	case 'Q':
		reset();
		break;
	}
}


/**
* Updates the camera parameters
*
* @param x The new x coordinate
* @param y The new y coordinate
* @param lButton The click count of the left button
* @param mButton The click count of the middle button
* @param rButton The click count of the right button
*
* @return
*
*/
void MayaCamera::updateCamera( int x, int y, int lButton, int mButton, int rButton, int moveWheel, int zZoom)
{
	int diffx= x-m_lastx;
	int diffy= y-m_lasty;
	m_lastx = x;
	m_lasty = y;

	float dist = sqrt((float)(diffx*diffx + diffy*diffy));

	if(diffy < 0) dist *= -1;

	// zoom in and out with mousewheel
	if(moveWheel > 0)
	{
		m_tz += 0.5;
	}
	else if(moveWheel < 0)
	{
		m_tz -= 0.5;
	}

	// reset camera with double right mouse click
	if(rButton > 1)
	{
		reset();
	}
	// zoom in and out with pressed left and right mousebutton
	else if( lButton > 0 && rButton > 0 )
	{
		m_tz -= (float) 0.05f * dist;
	}
	else
	{
		// rotate with pressed left mousebutton
		if( lButton  > 0 )
		{
			m_rotx += (float) 0.5f * diffy;
			m_roty += (float) 0.5f * diffx;		

			//m_rotx += 0.1;
			//m_roty += 0.1;	
		}
		
 		else
 		{
 			// translate with pressed right mousebutton
 			if( rButton > 0 )
 			{
  				m_tx += (float) 0.05f * diffx;
  				m_ty -= (float) 0.05f * diffy;
 			}
 		}
	}
}

void MayaCamera::moveCamera( float destPtX, float destPtY, float destPtZ, float dist )
{
	QVector3D dir(destPtX-m_tx, destPtY-m_ty, destPtZ-(-m_tz));

	dir = dist * dir.normalized();

	m_tx += dir.x();
	m_ty += dir.y();
	m_tz += dir.z();

}


/**
*
* Does the OpenGL camera setup by applying all the transformation parameters
*
*/
QMatrix4x4 MayaCamera::setup()
{
	//QMatrix4x4 matLookAt;

	//matLookAt.lookAt(QVector3D(_x, _y, _z), QVector3D(_x+_lx, _y+_ly, _z+_lz), QVector3D(0.0f, 1.0f,  0.0f));

	QMatrix4x4 mat;

	mat.translate(0, 0, -m_tz);
	mat.translate(m_tx, m_ty, 0);
	mat.rotate(m_rotx, 1, 0, 0);
	mat.rotate(m_roty, 0, 1, 0);
	mat.rotate(m_rotz, 0, 0, 1);

	mat.rotate(rotation(), 0.0f, 0.0f, 1.0f);
	mat.scale(mirrorX() ? -1 : 1, mirrorY() ? -1 : 1, 1.0f);

	//// set camera transformation
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	//glTranslatef(0, 0, -m_tz);
	//glTranslatef(m_tx, m_ty, 0);
	//glRotatef(m_rotx, 1, 0, 0);
	//glRotatef(m_roty, 0, 1, 0);	

	//glRotatef(rotation(), 0.0f, 0.0f, 1.0f);
	//glScalef(mirrorX() ? -1 : 1, mirrorY() ? -1 : 1, 1.0f);

	return mat;
}

void MayaCamera::updateTrackingBall(char axis, bool positiveSpin)
{
	switch(axis)
	{
	case 'x':
		m_rotz += 1.5;
		break;
	case 'y':
		m_rotx += 1.5;
		break;
	case 'z':
		m_roty += 1.5;
		break;
	}
}

/**
* Set the rotation of the single textures in the viewer. 
*
* @param value   rotation
*/
void MayaCamera::setRotation(float value)
{
	m_fRotation = value;
}


/** Get the rotation. **/
float MayaCamera::rotation(void) const
{
	return m_fRotation;
}


/**
* Set the mirror X flag.
*
* @param value   flag
*/
void MayaCamera::setMirrorX(bool value)
{
	m_mirrorX = value;
}


/** Get the mirror X flag. **/
bool MayaCamera::mirrorX(void) const
{
	return m_mirrorX;
}


/**
* Set the mirror Y flag.
*
* @param value   flag
*/
void MayaCamera::setMirrorY(bool value)
{
	m_mirrorY = value;
}


/** Get the mirror Y flag. **/
bool MayaCamera::mirrorY(void) const
{
	return m_mirrorY;
}

void MayaCamera::setRotationX( float value )
{
	m_rotx = clampRotation(value);
}

void MayaCamera::setRotationY( float value )
{
	m_roty = clampRotation(value);
}

void MayaCamera::setRotationZ( float value )
{
	m_rotz = clampRotation(value);
}

float MayaCamera::clampRotation( float value )
{
	float tmpValue = value;
	if(value < 0)
	{
		while (tmpValue + 360 < 0)
		{
			tmpValue += 360;
		}
	}
	else
	{
		while ((tmpValue - 360) > 0)
		{
			tmpValue -= 360;
		}
	}

	return tmpValue;
}

void MayaCamera::setTranslationX( float value )
{
	m_tx = value;
}

void MayaCamera::setTranslationY( float value )
{
	m_ty = value;
}

void MayaCamera::setTranslationZ( float value )
{
	m_tz = value;
}

void MayaCamera::updateTransformation( float rotX, float rotY, float rotZ, float tx, float ty, float tz )
{
	setRotationX(rotX);
	setRotationY(rotY);
	setRotationZ(rotZ);

	setTranslationX(tx);
	setTranslationY(ty);
	setTranslationZ(tz);
}

void MayaCamera::setDefaultValues( float rotX, float rotY, float rotZ, float tx, float ty, float tz )
{
	m_defaultRotx = clampRotation(rotX);
	m_defaultRoty = clampRotation(rotY);
	m_defaultRotz = clampRotation(rotZ);

	m_defaultTx = tx;
	m_defaultTy = ty;
	m_defaultTz = tz;

	reset();
}
