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


#include "Viewer3D.qt.h"
#include "MayaCamera.h"
#include "GL/GLU.h"

#include "Rendering/vertexformat.h"
#include "Rendering/GLRenderObject.h"
#include "Rendering/GLPipeline.h"

#include <algorithm> 

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLShader>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>

#include <QtGui/QtEvents>

#include <iostream>
#include <sstream>

#define _USE_MATH_DEFINES
#include "math.h"

Geometry::Primitives::vertex_v3n3 const VertexData[] =
{
	Geometry::Primitives::vertex_v3n3(Geometry::Primitives::vec3(-.5f, -.5f,  .5f), Geometry::Primitives::vec3(0, 0, 1)),
	Geometry::Primitives::vertex_v3n3(Geometry::Primitives::vec3(-.5f,  .5f,  .5f), Geometry::Primitives::vec3(1, 0, 0)),
	Geometry::Primitives::vertex_v3n3(Geometry::Primitives::vec3( .5f,  .5f,  .5f), Geometry::Primitives::vec3(0, 1, 0)),
	Geometry::Primitives::vertex_v3n3(Geometry::Primitives::vec3( .5f, -.5f,  .5f), Geometry::Primitives::vec3(1, 1, 0)),
	Geometry::Primitives::vertex_v3n3(Geometry::Primitives::vec3(-.5f, -.5f, -.5f), Geometry::Primitives::vec3(1, 1, 1)),
	Geometry::Primitives::vertex_v3n3(Geometry::Primitives::vec3(-.5f,  .5f, -.5f), Geometry::Primitives::vec3(1, 0, 0)),
	Geometry::Primitives::vertex_v3n3(Geometry::Primitives::vec3( .5f,  .5f, -.5f), Geometry::Primitives::vec3(1, 0, 1)),
	Geometry::Primitives::vertex_v3n3(Geometry::Primitives::vec3( .5f, -.5f, -.5f), Geometry::Primitives::vec3(0, 0, 1))
};

static GLuint const ElementData[] =
{
	0,2,1,  0,3,2,
	4,3,0,  4,7,3,
	4,1,5,  4,0,1,
	3,6,2,  3,7,6,
	1,6,5,  1,2,6,
	7,5,6,  7,4,5
};

Geometry::Primitives::vertex_v3n3 const PlaneVertexData[] =
{
	Geometry::Primitives::vertex_v3n3(Geometry::Primitives::vec3(-0.5f, 0, -0.5f), Geometry::Primitives::vec3(0, 1, 0)),
	Geometry::Primitives::vertex_v3n3(Geometry::Primitives::vec3(-0.5f, 0,  0.5f), Geometry::Primitives::vec3(0, 1, 0)),
	Geometry::Primitives::vertex_v3n3(Geometry::Primitives::vec3( 0.5f, 0,  0.5f), Geometry::Primitives::vec3(0, 1, 0)),
	Geometry::Primitives::vertex_v3n3(Geometry::Primitives::vec3( 0.5f, 0, -0.5f), Geometry::Primitives::vec3(0, 1, 0)),

	// The normal
	Geometry::Primitives::vertex_v3n3(Geometry::Primitives::vec3(0.0f, 0, 0.0f), Geometry::Primitives::vec3(0, 1, 0)),
	Geometry::Primitives::vertex_v3n3(Geometry::Primitives::vec3(0.0f, 1, 0.0f), Geometry::Primitives::vec3(0, 1, 0)),
};

Geometry::Primitives::vertex_v3n3 const FilledPlaneVertexData[] =
{
	Geometry::Primitives::vertex_v3n3(Geometry::Primitives::vec3(-0.5f, 0, -0.5f), Geometry::Primitives::vec3(0, 1, 0)),
	Geometry::Primitives::vertex_v3n3(Geometry::Primitives::vec3(-0.5f, 0,  0.5f), Geometry::Primitives::vec3(0, 1, 0)),
	Geometry::Primitives::vertex_v3n3(Geometry::Primitives::vec3( 0.5f, 0,  0.5f), Geometry::Primitives::vec3(0, 1, 0)),
	Geometry::Primitives::vertex_v3n3(Geometry::Primitives::vec3( 0.5f, 0, -0.5f), Geometry::Primitives::vec3(0, 1, 0)),
};

static GLuint const FilledPlaneElementData[] =
{
	0,2,1,  0,3,2
};




static GLuint const PlaneElementData[] =
{
	0, 1,
	1, 2,
	2, 3,
	3, 0,
	4, 5
};

const Geometry::Primitives::vec4 ColorGridDefault = Geometry::Primitives::vec4(0.3f, 0.3f, 0.3f, 1);
const Geometry::Primitives::vec4 ColorAxisX = Geometry::Primitives::vec4(0.47f, 0.21f, 0.21f, 1);
const Geometry::Primitives::vec4 ColorAxisY = Geometry::Primitives::vec4(0.41f, 0.57f, 0.41f, 1);
const Geometry::Primitives::vec4 ColorAxisZ = Geometry::Primitives::vec4(0.25f, 0.29f, 0.42f, 1);
const Geometry::Primitives::vec3 TrackballViewTranslation = Geometry::Primitives::vec3(0, 0, -15);


const Geometry::Primitives::vec3 OrientationArrowsViewTranslation = Geometry::Primitives::vec3(-6.5, -3.5, -10);

Geometry::Primitives::vertex_v3c4 const OrientationArrowsVertexData[] = 
{
	// Orientation arrow on x-axis
	Geometry::Primitives::vertex_v3c4(Geometry::Primitives::vec3(0.f, 0.f,  0.f), ColorAxisX),
	Geometry::Primitives::vertex_v3c4(Geometry::Primitives::vec3(0.5f, 0.f,  0.f), ColorAxisX),

	// Orientation arrow on y-axis
	Geometry::Primitives::vertex_v3c4(Geometry::Primitives::vec3(0.f, 0.f,  0.f), ColorAxisY),
	Geometry::Primitives::vertex_v3c4(Geometry::Primitives::vec3(0.f, 0.5f,  0.f), ColorAxisY),

	// Orientation arrow on z-axis
	Geometry::Primitives::vertex_v3c4(Geometry::Primitives::vec3(0.f, 0.f,  0.f), ColorAxisZ),
	Geometry::Primitives::vertex_v3c4(Geometry::Primitives::vec3(0.f, 0.f,  0.5f), ColorAxisZ)
};

static GLuint const OrientationArrowsElementData[] =
{
	0,1,	2,3,	4,5
};

#define TRACKBALL_NAME		"TRACKBALL"

 Geometry::Primitives::vertex_v3c4 const GridVertexData[82];
 static GLuint const GridElementData[82];

// Geometry::Primitives::vertex_v3 const TrackingBallVertexDataX[72];
/// Geometry::Primitives::vertex_v3 const TrackingBallVertexDataX[72];
// static GLuint const TrackingBallElementDataX[72];
// 
// Geometry::Primitives::vertex_v3 const TrackingBallVertexDataY[72];
// static GLuint const TrackingBallElementDataY[72];
// 
// Geometry::Primitives::vertex_v3 const TrackingBallVertexDataZ[72];
// static GLuint const TrackingBallElementDataZ[72];

Viewer::Viewer3D::Viewer3D(QWindow *parent)
	: Viewer::GLWindow(parent),
	_glMousePosX(INT_MIN),
	_glMousePosY(INT_MIN),
	_glMousePosZ(INT_MIN),
	_lastWheelPosSet(false)
{
	QDir dir(qApp->applicationDirPath());

	_filenameVertexShader = dir.filePath("GLSL/3dShaderVS.glsl");
	_filenameFragmentShader = dir.filePath("GLSL/3dShaderFS.glsl");

	_moveCameraX = 0;
	_moveCameraY = 0;
	_moveCameraZ = 30;

	_virtMayaCamera = new MayaCamera();

	enableDepthTest(true);

	static const float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	memcpy(_clearColor, color, sizeof(float)*4);

	addRenderObject();
	_renderObjects[0]->setFlag(GLRENDEROBJECT_FLAG_ISDEFAULT);
}


Viewer::Viewer3D::~Viewer3D()
{
	delete _virtMayaCamera;
}

/**
* Mouse event handler.
*
* @param m   mouse event
*/

void Viewer::Viewer3D::mousePressEvent(QMouseEvent *m)
{
	
/*		m->MouseButtonPress*/

 	_lastPos = m->pos();
	
 	_virtMayaCamera->setLastPosition(m->x(), m->y());
 
 	// get current mouse position
 	QPoint  currPos = m->pos();
 
//  	setCursor(Qt::OpenHandCursor);
//  	
//  	int counter = 0;
//  	//GLdouble moveToX, moveToY, moveToZ;
//  	int wheelVal;
//  
//  	_lastPos = m->pos();
//  	_virtMayaCamera->setLastPosition(m->x(), m->y());
//  
//  	GLint viewport[4];
//  	GLdouble modelview[16];
//  	GLdouble projection[16];
//  	GLfloat winX, winY, winZ;
//  	GLdouble posX, posY, posZ;
//  
//  	QRect rect = geometry();
//  
//  	QMatrix4x4 pmvMatrixModel = _virtMayaCamera->setup();
//  	QMatrix4x4 pmvMatrixProjection;
//  	pmvMatrixProjection.perspective(45.0f,(GLfloat)rect.width()/(GLfloat)rect.height(), 0.1f,1000.0f);
//  
//  	const float* matData = pmvMatrixModel.constData();
//  	const float* perspMatData = pmvMatrixProjection.constData();
//  	for(int i = 0; i < 16; i++)
//  	{
//  		modelview[i] = matData[i];
//  		projection[i] = perspMatData[i];
//  	}
//  
//  	glGetIntegerv( GL_VIEWPORT, viewport );
//  
//  	winX = (GLfloat)currPos.x();
//  	winY = (float)viewport[3] - (float)currPos.y();
//  	glReadPixels( currPos.x(), int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
//  
//  	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
//  
//  	bool rotateOnlyX = false;
//  	bool rotateOnlyY = false;
//  	bool rotateOnlyZ = false;
// 	
//  	std::cout << "x: " << posX << " y: " << posY << " z: " << posZ << std::endl;
// 
//  	for(int i = 0; i < 361; i++)
//  	{
//  		if(((_trackingBallX[i].Position.x > posX-5)
// 			/*&&(posZ == 0.0)*/
// 			&&(_trackingBallY[i].Position.y > posY-5))
//  			&&((_trackingBallX[i].Position.x < posX+5)
// 			/*&&(posZ == 0.0)*/
// 			&&(_trackingBallY[i].Position.y < posY+5)))
// 
//  		{
//  			std::cout << " X - PUNKT BER�HRT!!! " << std::endl;
// 			rotateOnlyX = true;
// 			setCursor(Qt::OpenHandCursor);
// 
//  			_virtMayaCamera->updateCamera(rotateOnlyX);
//  		}
//  
//  		else if(((_trackingBallY[i].Position.y > posY-5)
// 			/*&&(posX == 0.0)*/
// 			&&(_trackingBallZ[i].Position.z > posZ-5))
//  			&&((_trackingBallY[i].Position.y < posY+5)
// 			/*&&(posX == 0.0)*/
// 			&&(_trackingBallZ[i].Position.z < posZ+5)))
//  		{
//  			std::cout << " Y - PUNKT BER�HRT!!! " << std::endl;
// 			rotateOnlyY = true;
// 			setCursor(Qt::OpenHandCursor);
// 
//  			_virtMayaCamera->updateCamera(rotateOnlyY);
//  		}
//  
//  		else if(((_trackingBallX[i].Position.x > posX-5)
// 			/*&&(posY == 0.0)*/
// 			&&(_trackingBallZ[i].Position.z > posZ-5))
//  			&&((_trackingBallX[i].Position.x < posX+5)
// 			/*&&(posY == 0.0)*/
// 			&&(_trackingBallZ[i].Position.z < posZ+5)))
//  		{
//  			std::cout << " Z - PUNKT BER�HRT!!! " << std::endl;
// 			rotateOnlyZ = true;
// 			setCursor(Qt::OpenHandCursor);
//  			
//  			_virtMayaCamera->updateCamera(rotateOnlyZ);
//  		}
//  	}
// 
// 	if(m->MouseButtonRelease)
// 	{
// 		rotateOnlyX = false;
// 		rotateOnlyY = false;
// 		rotateOnlyZ = false;
// 	}
	

 //	for(int i = 0; i < 361; i++)
 //	{
 //		if(((_trackingBallX[i].Position.x > posX-5)
	//		/*&&(posZ == 0.0)*/
	//		&&(_trackingBallY[i].Position.y > posY-5))
 //			&&((_trackingBallX[i].Position.x < posX+5)
	//		/*&&(posZ == 0.0)*/
	//		&&(_trackingBallY[i].Position.y < posY+5)))

 //		{
 //			std::cout << " X - PUNKT BER�HRT!!! " << std::endl;
	//		rotateOnlyX = true;
	//		setCursor(Qt::OpenHandCursor);

 //			_virtMayaCamera->updateCamera(rotateOnlyX);
 //		}
 //
 //		else if(((_trackingBallY[i].Position.y > posY-5)
	//		/*&&(posX == 0.0)*/
	//		&&(_trackingBallZ[i].Position.z > posZ-5))
 //			&&((_trackingBallY[i].Position.y < posY+5)
	//		/*&&(posX == 0.0)*/
	//		&&(_trackingBallZ[i].Position.z < posZ+5)))
 //		{
 //			std::cout << " Y - PUNKT BER�HRT!!! " << std::endl;
	//		rotateOnlyY = true;
	//		setCursor(Qt::OpenHandCursor);

 //			_virtMayaCamera->updateCamera(rotateOnlyY);
 //		}
 //
 //		else if(((_trackingBallX[i].Position.x > posX-5)
	//		/*&&(posY == 0.0)*/
	//		&&(_trackingBallZ[i].Position.z > posZ-5))
 //			&&((_trackingBallX[i].Position.x < posX+5)
	//		/*&&(posY == 0.0)*/
	//		&&(_trackingBallZ[i].Position.z < posZ+5)))
 //		{
 //			std::cout << " Z - PUNKT BER�HRT!!! " << std::endl;
	//		rotateOnlyZ = true;
	//		setCursor(Qt::OpenHandCursor);
 //			
 //			_virtMayaCamera->updateCamera(rotateOnlyZ);
 //		}
 //	}

	//if(m->MouseButtonRelease)
	//{
	//	rotateOnlyX = false;
	//	rotateOnlyY = false;
	//	rotateOnlyZ = false;
	//}
	


	QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
}

/**
* Mouse event handler.
*
* @param m   mousewheel event
*/
 void Viewer::Viewer3D::wheelEvent(QWheelEvent *m)
 {
	 int counter = 0;
	 GLdouble moveToX, moveToY, moveToZ;
	 int wheelVal;
	 _lastPos = m->pos();
	 _virtMayaCamera->setLastPosition(m->x(), m->y());

	 // get current mouse position
	 QPoint  currPos = m->pos();
	 // get delta movement to last position
	 QPoint deltaPos = currPos - _lastPos;
	 QPoint dis = m->angleDelta();
	 wheelVal = dis.ry();
	 GLint viewport[4];
	 GLdouble modelview[16];
	 GLdouble projection[16];
	 GLfloat winX, winY, winZ;
	 GLdouble posX, posY, posZ;
	 QRect rect = geometry();
	 QMatrix4x4 pmvMatrixModel = _virtMayaCamera->setup();
	 QMatrix4x4 pmvMatrixProjection;
	 pmvMatrixProjection.perspective(45.0f,(GLfloat)rect.width()/(GLfloat)rect.height(), 0.1f,1000.0f);
	 const float* matData = pmvMatrixModel.constData();
	 const float* perspMatData = pmvMatrixProjection.constData();

	 for(int i = 0; i < 16; i++)
	 {
		 modelview[i] = matData[i];
		 projection[i] = perspMatData[i];
	 }

	 glGetIntegerv( GL_VIEWPORT, viewport );
	 winX = (GLfloat)currPos.x();
	 winY = (float)viewport[3] - (float)currPos.y();
	 glReadPixels( currPos.x(), int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
	 gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	 moveToX = (posX/2);
	 moveToY = (posY/2);
	 moveToZ = (posZ/2);

	 std::cout << "X: " << posX <<  " Y: " << posY << " Z: " << posZ << std::endl;

	 int lb = 0;
	 int mb = 0;
	 int rb = 0;

	 _virtMayaCamera->updateCamera(moveToX, moveToY, lb, mb, rb, wheelVal, moveToZ);
	 QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));


	// int counter = 0;

	// QPoint dis = m->angleDelta();
	// int wheelVal = dis.ry();

	//// get delta movement to last position
	//QPoint deltaPos = m->pos() - _lastWheelPos;

	//// no movement at all
	//if (deltaPos.manhattanLength() > 25 || !_lastWheelPosSet)
	//{
	//	// get current mouse position
	//	_lastWheelPosSet = true;		
	//	_lastWheelPos = m->pos();

	//	QPoint curPos = m->pos();

	//	GLint viewport[4];
	//	GLdouble modelview[16];
	//	GLdouble projection[16];
	//	GLfloat winX, winY, winZ;

	//	QRect rect = geometry();

	//	QMatrix4x4 pmvMatrixModel = _virtMayaCamera->setup();
	//	QMatrix4x4 pmvMatrixProjection;
	//	pmvMatrixProjection.perspective(45.0f,(GLfloat)rect.width()/(GLfloat)rect.height(), 0.1f,1000.0f);

	//	const float* matData = pmvMatrixModel.constData();
	//	const float* perspMatData = pmvMatrixProjection.constData();
	//	for(int i = 0; i < 16; i++)
	//	{
	//		modelview[i] = matData[i];
	//		projection[i] = perspMatData[i];
	//	}

	//	glGetIntegerv( GL_VIEWPORT, viewport );

	//	winX = (GLfloat)curPos.x();
	//	winY = (float)viewport[3] - (float)curPos.y();
	//	glReadPixels( (int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	//	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &_glMousePosX, &_glMousePosY, &_glMousePosZ);

	//	std::cout << "X: " << _glMousePosX <<  " Y: " << _glMousePosY << " Z: " << _glMousePosZ << std::endl;

	//}

	// _virtMayaCamera->moveCamera((float)_glMousePosX, (float)_glMousePosY, (float)_glMousePosZ, wheelVal < 0 ? 1 : -1);

	// QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
}
	 

/**
* Mouse double click event handler.
*
* @param m   mouse event
*/

void Viewer::Viewer3D::mouseDoubleClickEvent(QMouseEvent *m)
{
	_lastPos = m->pos();
	_virtMayaCamera->setLastPosition(m->x(), m->y());

	// get the buttons involved
	Qt::MouseButtons state = m->buttons();

	bool rb = (state & Qt::RightButton) == Qt::RightButton;

	if(rb) _virtMayaCamera->reset();

	QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
}

/**
* Mouse double click event handler.
*
* @param m   mouse event
*/
void Viewer::Viewer3D::keyPressEvent(QKeyEvent *e)
{
	_virtMayaCamera->updateCamera(e->key());
	char direction;
	char axis;
	bool positiveSpin;
	
	switch(e->key())
	{
		// Transformation settings
	case Qt::Key_A : 
	case Qt::Key_Left :
		direction = 'L';
		_virtMayaCamera->updateCameraPosition(direction);
		QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
		break;
	case Qt::Key_S : 
	case Qt::Key_Down :
		direction = 'D';
		_virtMayaCamera->updateCameraPosition(direction);
		QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
		break;
	case Qt::Key_D : 
	case Qt::Key_Right :
		direction = 'R';
		_virtMayaCamera->updateCameraPosition(direction);
		QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
		break;
	case Qt::Key_W : 
	case Qt::Key_Up :
		direction = 'U';
		_virtMayaCamera->updateCameraPosition(direction);
		QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
		break;
	case Qt::Key_R : 
	case Qt::Key_Backspace :
		direction = 'Q';
		_virtMayaCamera->updateCameraPosition(direction);
		QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
		break;

		// Tracking ball settings
	case Qt::Key_X :
		axis = 'x';
		positiveSpin = true;
		_virtMayaCamera->updateTrackingBall(axis, positiveSpin);
		QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
		break;
	case Qt::Key_Y :
		axis = 'y';
		positiveSpin = true;
		_virtMayaCamera->updateTrackingBall(axis, positiveSpin);
		QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
		break;
	case Qt::Key_Z :
		axis = 'z';
		positiveSpin = true;
		_virtMayaCamera->updateTrackingBall(axis, positiveSpin);
		QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
		break;
	}
}


/**
* Mouse event handler.
*
* @param m   mouse event
*/

void Viewer::Viewer3D::mouseMoveEvent(QMouseEvent *m)
{
	// get current mouse position
	QPoint  currPos = m->pos();

	// get delta movement to last position
	QPoint deltaPos = currPos - _lastPos;

	// no movement at all
	if (deltaPos.manhattanLength() == 0)
		return;

	// get the buttons involved
	Qt::MouseButtons state = m->buttons();

	int lb = (state & Qt::LeftButton)  == Qt::LeftButton;
	int mb = (state & Qt::MidButton)   == Qt::MidButton;
	int rb = (state & Qt::RightButton) == Qt::RightButton;

	_virtMayaCamera->updateCamera(m->x(), m->y(), lb, mb, rb);

	QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
}


void Viewer::Viewer3D::render(QPainter *painter)
{
	Q_UNUSED(painter);
	QRect rect = geometry();

	_projectionMatrix.setToIdentity();
	_projectionMatrix.perspective(45.0f,(GLfloat)rect.width()/(GLfloat)rect.height(), 0.1f,1000.0f);
	_viewMatrix = _virtMayaCamera->setup();

	Viewer::GLWindow::render(painter);
}


void Viewer::Viewer3D::initialize()
{
	// Lock the mutex to avoid collisions
	QMutexLocker locker(&_mutexUpdateVertexData);

	addGrid(21);
	addTrackingBall('x');
	addTrackingBall('y');
	addTrackingBall('z');
	addOrientationArrows();

	// Add all render objects before calling Viewer::GLWindow::initialize();
	//int defaultObject = 0;


	Viewer::GLWindow::initialize();

	QRect rect = geometry();
	_projectionMatrix.perspective(45.0f,(GLfloat)rect.width()/(GLfloat)rect.height(), 0.1f,1000.0f);

	//int vertexCount = sizeof(VertexData)/sizeof(Geometry::Primitives::vertex_v3n3);
	//int elementCount = sizeof(ElementData)/sizeof(unsigned int);

	//int gridVertexCount = sizeof(GridVertexData)/sizeof(Geometry::Primitives::vertex_v3c4);
	//int gridElementCount = sizeof(GridElementData)/sizeof(unsigned int);
	//
	//_renderObjects[defaultObject]->setVertexData((void*)VertexData, vertexCount, ElementData, elementCount, Geometry::Primitives::VERTEX_FORMAT::V3F_N3F);
}

void Viewer::Viewer3D::addGrid(unsigned int lineCount)
{	
	if(lineCount < 1)
		return;

	int gridObjectIndex = addRenderObject();

	if(_initialized)
		_renderObjects[gridObjectIndex]->init(_pipelines[0]);

	// If we have an even number of lines, then increase the line count
	if(lineCount%2 == 0)
		lineCount++;

	float halfLineCount = lineCount/2;
	float x1 = -halfLineCount;
	float z1 = -halfLineCount;

	int centerLineIndex = lineCount/2;

	using namespace Geometry::Primitives;

	std::vector<vertex_v3c4> vertices;
	std::vector<unsigned int> indices;

	// Line direction: z
	for(unsigned int x = 0; x < lineCount; x++)
	{
		vertex_v3c4 v1(vec3(x1 + x, 0.0f , -z1), ColorGridDefault);
		vertex_v3c4 v2(vec3(x1 + x, 0.0f , z1), ColorGridDefault);

		//// draw the z-axis in blue
 	//	if(x == centerLineIndex)
 	//	{
 	//		v1.Color = ColorAxisZ;
 	//		v2.Color = ColorAxisZ;
 	//	}

		vertices.push_back(v1);
		vertices.push_back(v2);

		indices.push_back((unsigned int)indices.size());
		indices.push_back((unsigned int)indices.size());
	}

	// Line direction: x
	for(unsigned int z = 0; z < lineCount; z++)
	{
		vertex_v3c4 v1(vec3(-x1, 0.0f, z1 + z), ColorGridDefault);
		vertex_v3c4 v2(vec3(x1, 0.0f , z1 + z), ColorGridDefault);

		//// draw the x-axis in red
 	//	if(z == centerLineIndex)
 	//	{
 	//		v1.Color = ColorAxisX;
 	//		v2.Color = ColorAxisX;
 	//	}

		vertices.push_back(v1);
		vertices.push_back(v2);

		indices.push_back((unsigned int)indices.size());
		indices.push_back((unsigned int)indices.size());
	}

	_renderObjects[gridObjectIndex]->setPrimitiveType(GL_LINES);
	_renderObjects[gridObjectIndex]->setVertexData(&(*vertices.begin()), (unsigned int)vertices.size(), &(*indices.begin()), (unsigned int)indices.size(), Viewer::Rendering::VERTEX_FORMAT::V3F_C4);
	_renderObjects[gridObjectIndex]->setFlag(GLRENDEROBJECT_FLAG_ISDEFAULT);

	// Add the axis as separate object to allow a thicker line width

	vertices.clear();
	indices.clear();

	// draw the x-axis in red
	vertex_v3c4 v1(vec3(0, 0, 0), ColorAxisX);
	vertex_v3c4 v2(vec3(2, 0, 0), ColorAxisX);
	vertices.push_back(v1);
	vertices.push_back(v2);

	indices.push_back((unsigned int)indices.size());
	indices.push_back((unsigned int)indices.size());

	// draw the y-axis in green
	v1 = vertex_v3c4(vec3(0, 0, 0), ColorAxisY);
	v2 = vertex_v3c4(vec3(0, 2, 0), ColorAxisY);
	vertices.push_back(v1);
	vertices.push_back(v2);

	indices.push_back((unsigned int)indices.size());
	indices.push_back((unsigned int)indices.size());

	// draw the z-axis in blue
	v1 = vertex_v3c4(vec3(0, 0, 0), ColorAxisZ);
	v2 = vertex_v3c4(vec3(0, 0, 2), ColorAxisZ);
	vertices.push_back(v1);
	vertices.push_back(v2);

	indices.push_back((unsigned int)indices.size());
	indices.push_back((unsigned int)indices.size());

	int axisObjectIndex = addRenderObject();
	_renderObjects[axisObjectIndex]->setPrimitiveType(GL_LINES);
	_renderObjects[axisObjectIndex]->setVertexData(&(*vertices.begin()), (unsigned int)vertices.size(), &(*indices.begin()), (unsigned int)indices.size(), Viewer::Rendering::VERTEX_FORMAT::V3F_C4);
	_renderObjects[axisObjectIndex]->setLineWidth(3);
	_renderObjects[axisObjectIndex]->setFlag(GLRENDEROBJECT_FLAG_ISDEFAULT);


}

void Viewer::Viewer3D::addTrackingBall(char trackingBallAxis)
{
	int trackingBallObjectIndex = addRenderObject();

	if(_initialized)
		_renderObjects[trackingBallObjectIndex]->init(_pipelines[0]);

	using namespace Geometry::Primitives;

	std::vector<vertex_v3> vertices;
	std::vector<unsigned int> indices;

	float radiusCircle = 2.5f;

	float x1 = .0f;
	float y1 = .0f;
	float z1 = .0f;

	for(int angle = 0; angle <= 360; angle += 1)
	{
		float angleRad = angle * M_PI / 180.0f;
		
		float x = sinf(angleRad) * radiusCircle;
		float y = cosf(angleRad) * radiusCircle;

		vertex_v3 v;

		switch(trackingBallAxis)
		{
		case 'x':
			v = vertex_v3(vec3(x1 + x, y1 + y, 0.0f));
			_trackingBallX.push_back(v);
			break;
		case 'y':
			v = vertex_v3(vec3(0.0f, x1 + x, z1 + y));
			_trackingBallY.push_back(v);
			break;
		case 'z':
			v = vertex_v3(vec3(x1 + x, 0.0f, z1 + y));
			_trackingBallZ.push_back(v);
			break;
		}

		vertices.push_back(v);
		indices.push_back((unsigned int)indices.size());
	}

	_renderObjects[trackingBallObjectIndex]->setUseFixedVertexColor(true);

	switch(trackingBallAxis)
	{
	case 'x':
		_renderObjects[trackingBallObjectIndex]->setVertexColor(ColorAxisX);
		break;

	case 'y': 
		_renderObjects[trackingBallObjectIndex]->setVertexColor(ColorAxisY);
		break;

	case 'z':
		_renderObjects[trackingBallObjectIndex]->setVertexColor(ColorAxisZ);
		break;
	}

	_renderObjects[trackingBallObjectIndex]->setUseFixedViewTranslation(true);
	_renderObjects[trackingBallObjectIndex]->setFixedViewTranslation(TrackballViewTranslation);
	_renderObjects[trackingBallObjectIndex]->setIgnoreViewTransformation(true);
	_renderObjects[trackingBallObjectIndex]->setPrimitiveType(GL_LINE_LOOP);
	_renderObjects[trackingBallObjectIndex]->setVertexData(&(*vertices.begin()), (unsigned int)vertices.size(), &(*indices.begin()), (unsigned int)indices.size(), Viewer::Rendering::VERTEX_FORMAT::V3F);
	_renderObjects[trackingBallObjectIndex]->setFlag(GLRENDEROBJECT_FLAG_ISDEFAULT);

	//std::stringstream s;
	//s << TRACKBALL_NAME << trackingBallAxis;

	//_renderObjects[trackingBallObjectIndex]->setName(s.str());

	//QRect rect = geometry();
	//int wNew = ((float)rect.width()/(float)rect.height())*100;
	//_renderObjects[trackingBallObjectIndex]->setViewport(0, 0, wNew, 100);
}

void Viewer::Viewer3D::addOrientationArrows()
{
	int orientationArrowsObjectIndex = addRenderObject();

	if(_initialized)
		_renderObjects[orientationArrowsObjectIndex]->init(_pipelines[0]);

	int vertexCount = sizeof(OrientationArrowsVertexData)/sizeof(Geometry::Primitives::vertex_v3c4);
	int elementCount = sizeof(OrientationArrowsElementData)/sizeof(unsigned int);

	_renderObjects[orientationArrowsObjectIndex]->setUseFixedViewTranslation(true);
	_renderObjects[orientationArrowsObjectIndex]->setFixedViewTranslation(OrientationArrowsViewTranslation);
	_renderObjects[orientationArrowsObjectIndex]->setIgnoreViewTransformation(true);
	_renderObjects[orientationArrowsObjectIndex]->setPrimitiveType(GL_LINES);
	_renderObjects[orientationArrowsObjectIndex]->setVertexData((void*)OrientationArrowsVertexData, vertexCount, OrientationArrowsElementData, elementCount, Viewer::Rendering::VERTEX_FORMAT::V3F_C4);
	_renderObjects[orientationArrowsObjectIndex]->setLineWidth(3);
}

void Viewer::Viewer3D::resizeEvent( QResizeEvent *event )
{
/*
	std::stringstream sX;
	std::stringstream sY;
	std::stringstream sZ;
	sX << TRACKBALL_NAME << "x";
	sY << TRACKBALL_NAME << "y";
	sZ << TRACKBALL_NAME << "z";

	int width = event->size().width();
	int height = event->size().height();

	int wNew = ((float)width/(float)height)*100;

	Geometry::Primitives::GLRenderObjectPtr trackBallObjX = getRenderObject(sX.str());
	Geometry::Primitives::GLRenderObjectPtr trackBallObjY = getRenderObject(sY.str());
	Geometry::Primitives::GLRenderObjectPtr trackBallObjZ = getRenderObject(sZ.str());

	if(trackBallObjX)
		trackBallObjX->setViewport(0, 0, wNew, 100);

	if(trackBallObjY)
		trackBallObjY->setViewport(0, 0, wNew, 100);

	if(trackBallObjZ)
		trackBallObjZ->setViewport(0, 0, wNew, 100);
*/
	Viewer::GLWindow::resizeEvent(event);

}

void Viewer::Viewer3D::setVertexData( const Geometry::Primitives::vec3* vertexData, const Geometry::Primitives::vec3* normalData, const unsigned int* indexData, int countVertices, int countIndices)
{
	QMutexLocker locker(&_mutexUpdateVertexData);

	addDefaultObject();


	unsigned int* indexDataTmp = new unsigned int[countIndices];
	memcpy(indexDataTmp, indexData, countIndices * sizeof(unsigned int));

	Geometry::Primitives::vertex_v3n3* vertexDataTmp = new Geometry::Primitives::vertex_v3n3[countVertices];
	for(int i = 0; i < countVertices; i++)
	{
		vertexDataTmp[i] = Geometry::Primitives::vertex_v3n3(vertexData[i], normalData[i]);
	}

	_renderObjects[0]->setVertexData(vertexDataTmp, countVertices, indexDataTmp, countIndices, Viewer::Rendering::VERTEX_FORMAT::V3F_N3F);

	delete[] indexDataTmp;
	delete[] vertexDataTmp;

	QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
}

bool Viewer::Viewer3D::addPlane( std::string name, const Geometry::Plane::PlaneCoefficients& coefficients )
{
	QVector3D planeNormal(coefficients.values[0], coefficients.values[1], coefficients.values[2]);
	planeNormal.normalize();

	planeNormal = coefficients.values[3] * planeNormal;

	Geometry::Primitives::vec3 point(planeNormal.x(), planeNormal.y(), planeNormal.z());

	return addPlane(name, point, coefficients);
}

bool Viewer::Viewer3D::addPlane( std::string name, Geometry::Primitives::vec3 point, const Geometry::Plane::PlaneCoefficients& coefficients )
{
	return addPlane(name, point, coefficients, false);
}

bool Viewer::Viewer3D::addPlane( std::string name, const Geometry::Plane::PlaneCoefficients& coefficients, bool filled, int planeSize /*= 1*/, byte r /*= 255*/, byte g /*= 255*/, byte b /*= 255*/ )
{
	QVector3D planeNormal(coefficients.values[0], coefficients.values[1], coefficients.values[2]);
	planeNormal.normalize();

	planeNormal = coefficients.values[3] * planeNormal;

	Geometry::Primitives::vec3 point(planeNormal.x(), planeNormal.y(), planeNormal.z());

	return addPlane(name, point, coefficients, filled, planeSize, r, g, b);
}

bool Viewer::Viewer3D::addPlane( std::string name, Geometry::Primitives::vec3 point, const Geometry::Plane::PlaneCoefficients& coefficients, bool filled, int planeSize /*= 1*/, byte r /*= 255*/, byte g /*= 255*/, byte b /*= 255*/ )
{

	QMutexLocker locker(&_mutexUpdateVertexData);

	PlaneMap::iterator itFound = _planeMap.find(name);		

	// Check if a plane with the given name is already added
	if(itFound != _planeMap.end())
		return false;

	QVector3D planeNormal(coefficients.values[0], coefficients.values[1], coefficients.values[2]);
	QVector3D yNormal(0, 1, 0);

	QVector3D rotNormal = QVector3D::crossProduct(yNormal, planeNormal);

	planeNormal.normalize();

	float rotAngle = acos(QVector3D::dotProduct(yNormal, planeNormal)) * 180.0f / M_PI;

	QMatrix4x4 planeModelMatrix;
	planeModelMatrix.rotate(rotAngle, rotNormal);

	planeModelMatrix.setColumn(3, QVector4D(point.x, point.y, point.z, 1.0f));

	int index = addRenderObject();

	if(filled)
	{
		int vertexCount = sizeof(FilledPlaneVertexData)/sizeof(Geometry::Primitives::vertex_v3n3);
		int elementCount = sizeof(FilledPlaneElementData)/sizeof(unsigned int);

		Geometry::Primitives::vertex_v3n3 *scaledFilledPlaneVertexData = new Geometry::Primitives::vertex_v3n3[vertexCount];

		for(int i = 0; i < vertexCount; i++)
		{
			Geometry::Primitives::vertex_v3n3 v = FilledPlaneVertexData[i];
			Geometry::Primitives::vec3 position(v.Position.x * planeSize, v.Position.y * planeSize, v.Position.z * planeSize);

			scaledFilledPlaneVertexData[i] = Geometry::Primitives::vertex_v3n3(position, v.Normal);
		}
		
		_renderObjects[index]->setPrimitiveType(GL_TRIANGLES);
		_renderObjects[index]->setVertexData(scaledFilledPlaneVertexData, vertexCount, FilledPlaneElementData, elementCount, Viewer::Rendering::VERTEX_FORMAT::V3F_N3F);

	}
	else
	{

		int vertexCount = sizeof(PlaneVertexData)/sizeof(Geometry::Primitives::vertex_v3n3);
		int elementCount = sizeof(PlaneElementData)/sizeof(unsigned int);

		_renderObjects[index]->setPrimitiveType(GL_LINES);
		_renderObjects[index]->setVertexData(PlaneVertexData, vertexCount, PlaneElementData, elementCount, Viewer::Rendering::VERTEX_FORMAT::V3F_N3F);
	}

	_renderObjects[index]->setModelMatrix(planeModelMatrix);
	_renderObjects[index]->setUseFixedVertexColor(true);
	_renderObjects[index]->setVertexColor(Geometry::Primitives::vec4(r/255.0f, g/255.0f, b/255.0f, 1));

	PlaneData planeData;
	planeData.coefficients = coefficients;
	planeData.renderObjectIndex = index;

	_planeMap.insert(PlaneMapPair(name, planeData));

	_renderObjects[index]->setName(name);

	return true;
}


bool Viewer::Viewer3D::removePlane( std::string name )
{
	QMutexLocker locker(&_mutexUpdateVertexData);

	PlaneMap::iterator itFound = _planeMap.find(name);		

	// Check if a plane with the given name is already added
	if(itFound == _planeMap.end())
		return false;

	Viewer::Rendering::GLRenderObjectPtr planeObject = getRenderObject(name);

	GLRenderObjectList::iterator itObj = std::find_if(_renderObjects.begin(), _renderObjects.end(), [&name](Viewer::Rendering::GLRenderObjectPtr const& o) {
																											return o && o->getName() == name; });

	if(itObj != _renderObjects.end())
	{
		// Store for deletion
		_deletedObjects.push_back(*itObj);
		_renderObjects.erase(itObj);
	}

	if(planeObject && planeObject->getName() == name)
		_planeMap.erase(itFound);

	return true;
}

int Viewer::Viewer3D::addVertexData( std::string name, const Geometry::Primitives::vec3* vertexData, const Geometry::Primitives::vec3* normalData, const unsigned int* indexData, int countVertices, int countIndices, int primitiveType)
{
	Geometry::Primitives::vertex_v3n3* vertexDataTmp = new Geometry::Primitives::vertex_v3n3[countVertices];
	for(int i = 0; i < countVertices; i++)
	{
		vertexDataTmp[i] = Geometry::Primitives::vertex_v3n3(vertexData[i], normalData[i]);
	}

	return addVertexData(name, vertexDataTmp, indexData, countVertices, countIndices, primitiveType);
}

int Viewer::Viewer3D::addVertexData( std::string name, const Geometry::Primitives::vertex_v3n3* vertexData, const unsigned int* indexData, int countVertices, int countIndices, int primitiveType)
{
	QMutexLocker locker(&_mutexUpdateVertexData);

	GLRenderObjectList::iterator itObj = std::find_if(_renderObjects.begin(), _renderObjects.end(), [&name](Viewer::Rendering::GLRenderObjectPtr const& o) {
		return o && o->getName() == name; });

	// Check, if we have already a cloud with this name
	if(itObj != _renderObjects.end())
		return -1;

	int index = addRenderObject();

	unsigned int* indexDataTmp = new unsigned int[countIndices];
	memcpy(indexDataTmp, indexData, countIndices * sizeof(unsigned int));

	_renderObjects[index]->setName(name);
	_renderObjects[index]->setVertexData(vertexData, countVertices, indexDataTmp, countIndices, Viewer::Rendering::VERTEX_FORMAT::V3F_N3F);
	_renderObjects[index]->setPrimitiveType(primitiveType);

	delete[] indexDataTmp;

	QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));

	return index;
}

int Viewer::Viewer3D::addVertexData( std::string name, unsigned int vbo, int countVertices, int countIndices, Viewer::Rendering::VERTEX_FORMAT::Format format)
{
	QMutexLocker locker(&_mutexUpdateVertexData);

	GLRenderObjectList::iterator itObj = std::find_if(_renderObjects.begin(), _renderObjects.end(), [&name](Viewer::Rendering::GLRenderObjectPtr const& o) {
															return o && o->getName() == name; });

	// Check, if we have already a cloud with this name
	if(itObj != _renderObjects.end())
		return -1;

	int index = addRenderObject();

	_renderObjects[index]->setName(name);
	_renderObjects[index]->setVertexData(vbo, countVertices, countIndices, format);
	_renderObjects[index]->setPrimitiveType(GL_POINTS);

	QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));

	return index;
}

bool Viewer::Viewer3D::removeVertexData( std::string name )
{
	// Lock the mutex to avoid collisions
	QMutexLocker locker(&_mutexUpdateVertexData);

	GLRenderObjectList::iterator itObj = std::find_if(_renderObjects.begin(), _renderObjects.end(), [&name](Viewer::Rendering::GLRenderObjectPtr const& o) {
		return o && o->getName() == name; });

	// Check, if we have a cloud with this name
	if(itObj == _renderObjects.end())
		return false;
	
	// Store for deletion
	_deletedObjects.push_back(*itObj);
	_renderObjects.erase(itObj);

	return true;
}
