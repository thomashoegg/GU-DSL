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

#ifndef MAYACAMERA_H
#define MAYACAMERA_H

#pragma once
#include <QtGui/QVector3D>
#include "ViewerDLL.h"

class QMatrix4x4;

//! OpenGL helper class
/** 
 * A Maya-like Camera representation for OpenGL.
 *
 * This class provides the basic functionality for an OpenGL camera placed on 
 * a sphere around a certain point of interest.
 */
class VIEWER_API MayaCamera
{
public:
	MayaCamera(void);
	~MayaCamera(void);

	void reset();
public:
	//! Set the last cursor position
	void setLastPosition(int x, int y) { m_lastx = x; m_lasty = y; }

	//! Updates the camera parameters
	void updateCamera(int x = 0, int y = 0, int lButton = 0, int mButton = 0, int rButton = 0, int moveWheel = 0, int zZoom = 0);

	//! Updates the camera transformation
	void updateTransformation(float rotX, float rotY, float rotZ, float tx, float ty, float tz);

	//! Updates the camera transformation
	void setDefaultValues(float rotX, float rotY, float rotZ, float tx, float ty, float tz);

	//! Moves the camera along a line
	void moveCamera(float destPtX, float destPtY, float destPtZ, float dist);

	//! Updated the camera position on x and y
	void updateCameraPosition(char direction);

	void updateTrackingBall(char axis, bool positiveSpin);

	//! Setup the OpenGL camera
	QMatrix4x4 setup();

	//! Set the mirror X flag.
	void setMirrorX(bool value);

	//! Get the mirror X flag.
	bool mirrorX(void) const;

	//! Set the mirror Y flag.
	void setMirrorY(bool value);

	//! Get the mirror Y flag.
	bool mirrorY(void) const;

	//! Set the rotation.
	void setRotation(float value);

	//! Get the rotation.
	float rotation(void) const;

	//! Set the rotation.
	void setRotationX(float value);

	//! Get the rotation.
	float rotationX(void) const { return m_rotx; }

	//! Set the rotation.
	void setRotationY(float value);

	//! Get the rotation.
	float rotationY(void) const { return m_roty; }

	//! Set the rotation.
	void setRotationZ(float value);

	//! Get the rotation.
	float rotationZ(void) const { return m_rotz; }

	//! Set the translation.
	void setTranslationX(float value);

	//! Get the translation.
	float translationX(void) const { return m_tx; }

	//! Set the translation.
	void setTranslationY(float value);

	//! Get the translation.
	float translationY(void) const { return m_ty; }

	//! Set the translation.
	void setTranslationZ(float value);

	//! Get the translation.
	float translationZ(void) const { return m_tz; }

	float clampRotation( float value );

protected:

	float	m_defaultRotx;
	float	m_defaultRoty;
	float	m_defaultRotz;
	float	m_defaultTx;
	float	m_defaultTy;
	float	m_defaultTz;

	float	m_rotx;
	float	m_roty;
	float	m_rotz;
	float	m_tx;
	float	m_ty;
	float	m_tz;
	int		m_lastx;
	int		m_lasty;

	float m_fRotation;				//!< The texture rotation
	bool m_mirrorX;					//!< Mirrors on the X axis
	bool m_mirrorY;					//!< Mirrors on the Y axis
};

#endif // MAYACAMERA_H
