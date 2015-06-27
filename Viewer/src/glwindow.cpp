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


#include "glwindow.qt.h"
#include "Rendering/GLPipeline.h"
#include "Rendering/GLRenderObject.h"

#include <algorithm> 

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLShader>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
#include <QOpenGLFunctions>
#include <QtGui/QFont>
#include <private/qopenglextensions_p.h>

#include <QtGui/QtEvents>
#include <GL/GLU.h>

#include "OpenGL/SharedGLContext.h"

#include "Primitives/vertex.h"


const float Viewer::GLWindow::s_bgrColor[4] = {0.3f, 0.3f, 0.3f, 1.0f};


Viewer::GLWindow::GLWindow(QWindow *parent)
    : QWindow(parent)
    , _update_pending(false)
    , _animating(false)
    , _context(0)
    , _device(0)
	, _isDepthTestEnabled(false)
	, _initialized(false)
	,_isInitialized(false)
	, _mutexUpdateVertexData(QMutex::Recursive)
	, _fps(0)
	, _frameCount(0)
	, _lastFrameCount(0)
	, _contextInformation(0, 0)
	, _needRecalc(true)
	, _isCleaned(false)
	, _minRange(-1)
	, _maxRange(-1)
	, _contextLockMutex(nullptr)
{
	_lastTick = GetTickCount();
    setSurfaceType(QWindow::OpenGLSurface);


	QDir dir(qApp->applicationDirPath());

	_filenameVertexShader = dir.filePath("GLSL/depthShaderVS.glsl");
	_filenameFragmentShader = dir.filePath("GLSL/depthShaderFS.glsl");

	_clearColor = new float[4];

	//static const float color[] = { 0.0f, 1.0f, 0.0f, 1.0f };
	memcpy(_clearColor, s_bgrColor, sizeof(float)*4);

	//// Always add a default render object
	//addRenderObject();

	//_renderObjects[0]->setFlag(GLRENDEROBJECT_FLAG_ISDEFAULT);

	_contextLockMutex = new QMutex();
}

Viewer::GLWindow::~GLWindow()
{
	cleanup();

	if(_contextLockMutex != nullptr)
		delete _contextLockMutex;

	//if(_context != nullptr)
	//	delete _context;
}

void Viewer::GLWindow::cleanup()
{
	if(_initialized && !_isCleaned)
	{
		acquireGLContext();

		if(_context != nullptr)
			_context->makeCurrent(this);

		_renderObjects.clear();
		_pipelines.clear();

		delete _device;
		_device = nullptr;

		delete[] _clearColor;
		_clearColor = nullptr;

		_initialized = false;
		_isCleaned = true;

		releaseGLContext();
	}

}

void Viewer::GLWindow::render(QPainter *painter)
{
    Q_UNUSED(painter);

	if(_needRecalc)
	{
		recalcNumbImgFields();
		_needRecalc = false;
	}

	QRect rect = geometry();

	glClearBufferfv(GL_COLOR, 0, _clearColor);

	if(_isDepthTestEnabled)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		float depth(1.0f);
		glClearBufferfv(GL_DEPTH, 0, &depth);
	}

	// Lock the mutex to avoid collisions
	QMutexLocker locker(&_mutexUpdateVertexData);

	
	// Remove all deleted objects and implicitly allow the share_ptr deletion
	_deletedObjects.clear();

	for(GLRenderObjectList::iterator it = _renderObjects.begin(); it != _renderObjects.end(); it++)
	{
		if(!*it) continue;

		(*it)->init(_pipelines[0]);

		Viewer::Rendering::GLRenderObject::Viewport vp = (*it)->getViewport();

		if(vp.width != 0 && vp.height != 0)
			glViewportIndexedf(0, vp.x, vp.y, vp.width, vp.height);
		else glViewportIndexedf(0, 0, 0, rect.width(), rect.height());

		(*it)->enableDepthTest(_isDepthTestEnabled);
		(*it)->updateTransformation(_viewMatrix, _projectionMatrix);
		(*it)->render();
	}

	//TODO render fps
	//UpdateFrameRate();
	//renderText(10,10, QString(_fps));

	locker.unlock();

	if(_isDepthTestEnabled)
	{
		glDisable(GL_DEPTH_TEST);
	}
}

//PtrToCircle adadfaf::addCircle(int radius, float centerX, fldafaf, QMatrix4x4 modelMatrix)
//{
//	addRenderObject();
//
//	// initCircle
//
//
//	return circle;
//
//}

void Viewer::GLWindow::initialize()
{
	if(_initialized)
		return;

	// Always add a default buffer
	//if(_currentFrame->width != 0)
	//	_renderObjects[0]->setImage(*_currentFrame);

	Viewer::Rendering::GLPipelinePtr pipeline(new Viewer::Rendering::GLPipeline(this, _context));
	pipeline->init(_filenameVertexShader, _filenameFragmentShader);

	_pipelines.push_back(pipeline);

	for(GLRenderObjectList::iterator it = _renderObjects.begin(); it != _renderObjects.end(); it++)
	{
		if(!*it) continue;

		(*it)->init(pipeline);
	}

	_viewMatrix.setToIdentity();
	_projectionMatrix.setToIdentity();
	_projectionMatrix.ortho(0, 1, 0, 1, -1.0f, 1.0f);

	_initialized = true;
}

void Viewer::GLWindow::render()
{
    if (!_device)
        _device = new QOpenGLPaintDevice;

    _device->setSize(size());

   // QPainter painter(_device);
    render(nullptr);
}

void Viewer::GLWindow::renderLater()
{
    if (!_update_pending) {
        _update_pending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

bool Viewer::GLWindow::event(QEvent *event)
{
	if(!_isCleaned)
	{
		QEvent::Type type = event->type();
		switch (event->type()) {
		case QEvent::UpdateRequest:
			_update_pending = false;
			renderNow();
			return true;
		case QEvent::Close:
			cleanup();
			break;
		default:
			break;
		}
	}
	
	return QWindow::event(event);
}

void Viewer::GLWindow::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        renderNow();
}

void Viewer::GLWindow::renderNow()
{	
	if(!isExposed())
		return;

	if(!isVisible())
	{
		if(_deletedObjects.size() > 0)
		{
			acquireGLContext();

			makeCurrent();

			_deletedObjects.clear();

			doneCurrent();

			releaseGLContext();
		}
		return;
	}

	bool needsInitialize = false;

	needsInitialize = initContext();

	if(!isVisible() && !needsInitialize)
		return;

	acquireGLContext();

	makeCurrent();

	_deletedObjects.clear();

    render();

    _context->swapBuffers(this);

    if (_animating)
        renderLater();

	doneCurrent();

	releaseGLContext();
}

void Viewer::GLWindow::setAnimating(bool animating)
{
    _animating = animating;

    if (animating)
        renderLater();
}

void Viewer::GLWindow::resizeEvent( QResizeEvent *event )
{
	int width = event->size().width();
	int height = event->size().height();

	int side = qMin(width, height);

	QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
}

int Viewer::GLWindow::addRenderObject()
{
	// Lock the mutex to avoid collisions
 	QMutexLocker locker(&_mutexUpdateVertexData);
	Viewer::Rendering::GLRenderObjectPtr renderObj(new Viewer::Rendering::GLRenderObject(this));

	_renderObjects.push_back(renderObj);

	_minRange = _maxRange = -1;

	/*if(_renderObjects.size() == 1)
	_renderObjects[0]->setFlag(GLRENDEROBJECT_FLAG_ISDEFAULT);*/
	
	_needRecalc = true;
	//recalcNumbImgFields();

	return (int)(_renderObjects.size()-1);
}


int Viewer::GLWindow::addRenderObject(int width, int height)
{
	// Lock the mutex to avoid collisions
 	QMutexLocker locker(&_mutexUpdateVertexData);
	Viewer::Rendering::GLRenderObjectPtr renderObj(new Viewer::Rendering::GLRenderObject(this, width, height));

	_renderObjects.push_back(renderObj);

	_minRange = _maxRange = -1;

	/*if(_renderObjects.size() == 1)
	_renderObjects[0]->setFlag(GLRENDEROBJECT_FLAG_ISDEFAULT);*/
	
	_needRecalc = true;
	//recalcNumbImgFields();

	return (int)(_renderObjects.size()-1);
}


int Viewer::GLWindow::addRenderObject(Viewer::GLWindow* window)
{
	// Lock the mutex to avoid collisions
 	QMutexLocker locker(&_mutexUpdateVertexData);
	Viewer::Rendering::GLRenderObjectPtr renderObj(new Viewer::Rendering::GLRenderObject(window));

	_renderObjects.push_back(renderObj);

	_minRange = _maxRange = -1;

	_needRecalc = true;
	//recalcNumbImgFields();

	/*if(_renderObjects.size() == 1)
	_renderObjects[0]->setFlag(GLRENDEROBJECT_FLAG_ISDEFAULT);*/

	return (int)(_renderObjects.size()-1);
}

int Viewer::GLWindow::addRenderObject( Rendering::GLRenderObjectPtr renderObject )
{
	// Lock the mutex to avoid collisions
 	QMutexLocker locker(&_mutexUpdateVertexData);

	_renderObjects.push_back(renderObject);

	_minRange = _maxRange = -1;

	_needRecalc = true;

	return (int)(_renderObjects.size()-1);
}


bool Viewer::GLWindow::removeRenderObject(std::string name)
{
	return removeRenderObject(getRenderObject(name));
}

bool Viewer::GLWindow::removeRenderObject(Rendering::GLRenderObjectPtr renderObject)
{
	return removeRenderObject(getRenderObjectIndex(renderObject));
}

bool Viewer::GLWindow::removeRenderObject( int index )
{
	QMutexLocker locker(&_mutexUpdateVertexData);

	if(index < 0 || index >= _renderObjects.size())
		return false;

	GLRenderObjectList::iterator it = _renderObjects.begin() + index;

	// Store for deletion
	_deletedObjects.push_back(*it);
	_renderObjects[index] = nullptr;

	_needRecalc = true;


	QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
	return true;
}

bool Viewer::GLWindow::enableRenderObject( int index, bool enable )
{
	// Lock the mutex to avoid collisions
	QMutexLocker locker(&_mutexUpdateVertexData);

	if(index < 0 || index >= _renderObjects.size())
		return false;

	if(!_renderObjects[index])
		return false;

	bool set = _renderObjects[index]->setEnabled(enable);

	_needRecalc = true;
	//recalcNumbImgFields();

	QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));

	return set;
}

Viewer::Rendering::GLRenderObjectPtr Viewer::GLWindow::getRenderObject( int index )
{
	// Lock the mutex to avoid collisions
	QMutexLocker locker(&_mutexUpdateVertexData);

	if(index < 0 || index >= _renderObjects.size())
		return Viewer::Rendering::GLRenderObjectPtr();

	return _renderObjects[index];

}

Viewer::Rendering::GLRenderObjectPtr Viewer::GLWindow::getRenderObject( std::string name )
{
	// Lock the mutex to avoid collisions
	QMutexLocker locker(&_mutexUpdateVertexData);
	GLRenderObjectList::iterator itObj = std::find_if(_renderObjects.begin(), _renderObjects.end(), [&name](Viewer::Rendering::GLRenderObjectPtr const& o) {
		return o && o->getName() == name; });

	if(itObj == _renderObjects.end())
		return Viewer::Rendering::GLRenderObjectPtr();

	return *itObj;

}

int Viewer::GLWindow::getRenderObjectIndex( Viewer::Rendering::GLRenderObjectPtr obj )
{
	// Lock the mutex to avoid collisions
	QMutexLocker locker(&_mutexUpdateVertexData);

	for(int i = 0; i < _renderObjects.size(); i++)
	{
		if(_renderObjects[i] == obj)
			return i;
	}
	
	return -1;
}


int Viewer::GLWindow::removeAllRenderObjects()
{
	int removedObjects = 0;

	// Lock the mutex to avoid collisions
	QMutexLocker locker(&_mutexUpdateVertexData);

	for(int i = (int)_renderObjects.size()-1; i >= 0; i--)
	{
		if(!_renderObjects[i])
			continue;

		if(!_renderObjects[i]->isFlagSet(GLRENDEROBJECT_FLAG_ISDEFAULT))
		{
			GLRenderObjectList::iterator it = _renderObjects.begin() + i;

			_deletedObjects.push_back(*it);
			_renderObjects.erase(it);

			removedObjects++;
		}
	}

	_needRecalc = true;
	//recalcNumbImgFields();

	QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));

	return removedObjects;
}

bool Viewer::GLWindow::initContext()
{
	if (!_context) 
	{
		acquireGLContext();

		//while(::glGetError())
		//{
		//	
		//}

		int error = ::GetLastError();

		Viewer::OpenGL::SharedGLContext::getInstance()->makeCurrent();

		error = ::GetLastError();

		_context = new QOpenGLContext(this);
		QSurfaceFormat format = requestedFormat();
		format.setMajorVersion(4);
		format.setMinorVersion(3);

		_context->setFormat(format);
		_context->setShareContext(Viewer::OpenGL::SharedGLContext::getInstance()->sharedContext());
		_context->create();

		_context->makeCurrent(this);
		_context->versionFunctions<QOpenGLFunctions_4_3_Core>();

		error = ::GetLastError();

		_contextInformation = Viewer::OpenGL::ContextHandleInformation((intptr_t)wglGetCurrentContext(), (intptr_t)wglGetCurrentDC());

		error = ::GetLastError();

		initializeOpenGLFunctions();

		error = ::glGetError();

		initialize();

		_isInitialized = true;

		_context->doneCurrent();

		Viewer::OpenGL::SharedGLContext::getInstance()->doneCurrent();

		releaseGLContext();
		return true;
	}

	return false;
}



void Viewer::GLWindow::addDefaultObject()
{
	if(_renderObjects.size() <= 0)
	{
		addRenderObject();
		_renderObjects[0]->setFlag(GLRENDEROBJECT_FLAG_ISDEFAULT);
		//if(isVisible() && isExposed())
		//	_renderObjects[0]->init(_pipelines[0]);
	}
}


void Viewer::GLWindow::setViewedLayer( int layerNumb, int objectIndex /*= 0*/, int frameIndex /*= 0*/)
{
	if(objectIndex < 0)
	{
		for(int i = 0; i < _renderObjects.size(); i++)
			_renderObjects[i]->setViewedLayer(layerNumb, frameIndex);
	}
	else if(objectIndex >= 0 && objectIndex < _renderObjects.size())
		_renderObjects[objectIndex]->setViewedLayer(layerNumb, frameIndex);
}

void Viewer::GLWindow::setMinMax( float min, float max, int index/* = 0*/)
{
	_minRange = min;
	_maxRange = max;

	if(index < 0)
	{
		for(int i = 0; i < _renderObjects.size(); i++)
			_renderObjects[i]->setMinMax(min, max);
	}
	else if(index >= 0 && index < _renderObjects.size())
		_renderObjects[index]->setMinMax(min, max);
}

void Viewer::GLWindow::useJetColor( bool use, int index/* = 0*/)
{
	if(index < 0)
	{
		for(int i = 0; i < _renderObjects.size(); i++)
			_renderObjects[i]->useJetColor(use);
	}
	else if(index >= 0 && index < _renderObjects.size())
		_renderObjects[index]->useJetColor(use);
}

void Viewer::GLWindow::renderText(double x, double y, double z, QString txt)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	//renderText(x, y, z, txt, QFont("Arial", 12, QFont::Bold, false) );
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}

/**
 * \brief Update frame rate
 *
 */
void Viewer::GLWindow::UpdateFrameRate()
{
	_frameCount++;

	DWORD tickCount = GetTickCount();
	DWORD span      = tickCount - _lastTick;
	if (span >= 1000)
	{
		_fps            = (UINT)((double)(_frameCount - _lastFrameCount) * 1000.0 / (double)span + 0.5);
		_lastTick       = tickCount;
		_lastFrameCount = _frameCount;
	}
}
void Viewer::GLWindow::update()
{
	QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
}

void Viewer::GLWindow::doneCurrent()
{
	Viewer::OpenGL::SharedGLContext::getInstance()->doneCurrent(_context);
}

void Viewer::GLWindow::makeCurrent()
{
	Viewer::OpenGL::SharedGLContext::getInstance()->makeCurrent(this, _context, _contextInformation);
}

int Viewer::GLWindow::addPoints( std::string name, const std::vector<Geometry::Primitives::Point2f>& points, float pointSize, byte r, byte g, byte b )
{
	QMutexLocker locker(&_mutexUpdateVertexData);

	Viewer::Rendering::GLRenderObjectPtr renderObject = getRenderObject(name);
	if(renderObject)
		return -1;

	int index = addRenderObject();

	Geometry::Primitives::vertex_v2c4* vertices = new Geometry::Primitives::vertex_v2c4[points.size()];

	int i = 0;
	std::vector<Geometry::Primitives::Point2f>::const_iterator it;
	for(it = points.begin(), i = 0; it != points.end(); it++, i++)
	{
		vertices[i].Position = Geometry::Primitives::vec2(it->x, it->y);
		vertices[i].Color = Geometry::Primitives::vec4((float)r/255.0f, (float)g/255.0f, (float)b/255.0f, 1.0f);
	}

	QDir dir(qApp->applicationDirPath());

	QString filenameVertexShader = dir.filePath("GLSL/PrimitiveVS.vert");
	QString filenameFragmentShader = dir.filePath("GLSL/PrimitiveFS.frag");


	_renderObjects[index]->setPrimitiveType(GL_POINTS);
	_renderObjects[index]->setVertexData(vertices, (int)points.size(), nullptr, 0, Viewer::Rendering::VERTEX_FORMAT::V2F_C4);
	_renderObjects[index]->setName(name);
	_renderObjects[index]->setCustomShaders(filenameVertexShader, filenameFragmentShader, "");
	_renderObjects[index]->setPointSize(pointSize);

	delete[] vertices;

	return index;
}

int Viewer::GLWindow::addPoints( std::string name, const std::vector<Geometry::Primitives::Point3f>& points, float pointSize, byte r, byte g, byte b )
{

	QMutexLocker locker(&_mutexUpdateVertexData);

	Viewer::Rendering::GLRenderObjectPtr renderObject = getRenderObject(name);
	if(renderObject)
		return -1;

	int index = addRenderObject();

	Geometry::Primitives::vertex_v3c4* vertices = new Geometry::Primitives::vertex_v3c4[points.size()];

	int i = 0;
	std::vector<Geometry::Primitives::Point3f>::const_iterator it;
	for(it = points.begin(), i = 0; it != points.end(); it++, i++)
	{
		vertices[i].Position = Geometry::Primitives::vec3(it->x, it->y, it->z);
		vertices[i].Color = Geometry::Primitives::vec4((float)r/255.0f, (float)g/255.0f, (float)b/255.0f, 1.0f);
	}

	QDir dir(qApp->applicationDirPath());

	QString filenameVertexShader = dir.filePath("GLSL/PrimitiveVS.vert");
	QString filenameFragmentShader = dir.filePath("GLSL/PrimitiveFS.frag");

	_renderObjects[index]->setPrimitiveType(GL_POINTS);
	_renderObjects[index]->setVertexData(vertices, (int)points.size(), nullptr, 0, Viewer::Rendering::VERTEX_FORMAT::V3F_C4);
	_renderObjects[index]->setName(name);
	_renderObjects[index]->setCustomShaders(filenameVertexShader, filenameFragmentShader, "");
	_renderObjects[index]->setPointSize(pointSize);

	delete[] vertices;

	return index;
}

int Viewer::GLWindow::addLines( std::string name, const std::vector<Geometry::Primitives::Line2f>& lines, float lineWidth, byte r, byte g, byte b )
{
	std::vector<Geometry::Primitives::Polygonf> primitives(lines.begin(), lines.end());
	return addPrimitives(name, primitives, lineWidth, r, g, b);

	/*QMutexLocker locker(&_mutexUpdateVertexData);

	Viewer::Rendering::GLRenderObjectPtr renderObject = getRenderObject(name);
	if(renderObject)
	return -1;

	int index = addRenderObject();

	Geometry::Primitives::vertex_v2c4* vertices = new Geometry::Primitives::vertex_v2c4[lines.size()*2];

	int i = 0;
	std::vector<Geometry::Primitives::Line2f>::const_iterator it;
	for(it = lines.begin(), i = 0; it != lines.end(); it++, i += 2)
	{
	vertices[i].Position   = Geometry::Primitives::vec2(it->getX1(), it->getY1());
	vertices[i+1].Position = Geometry::Primitives::vec2(it->getX2(), it->getY2());
	vertices[i].Color = vertices[i+1].Color = Geometry::Primitives::vec4((float)r/255.0f, (float)g/255.0f, (float)b/255.0f, 1.0f);
	}

	QDir dir(qApp->applicationDirPath());

	QString filenameVertexShader = dir.filePath("GLSL/PrimitiveVS.vert");
	QString filenameFragmentShader = dir.filePath("GLSL/PrimitiveFS.frag");

	_renderObjects[index]->setPrimitiveType(GL_LINES);
	_renderObjects[index]->setVertexData(vertices, (int)lines.size() * 2, nullptr, 0, Viewer::Rendering::VERTEX_FORMAT::V2F_C4);
	_renderObjects[index]->setName(name);
	_renderObjects[index]->setCustomShaders(filenameVertexShader, filenameFragmentShader, "");
	_renderObjects[index]->setLineWidth(lineWidth);

	delete[] vertices;

	return index;*/
}

int Viewer::GLWindow::addLines( std::string name, const std::vector<Geometry::Primitives::Line3f>& lines, float lineWidth, byte r, byte g, byte b )
{
	std::vector<Geometry::Primitives::Polygonf> primitives(lines.begin(), lines.end());
	return addPrimitives(name, primitives, lineWidth, r, g, b);

	//QMutexLocker locker(&_mutexUpdateVertexData);

	//Viewer::Rendering::GLRenderObjectPtr renderObject = getRenderObject(name);
	//if(renderObject)
	//	return -1;

	//int index = addRenderObject();

	//Geometry::Primitives::vertex_v3c4* vertices = new Geometry::Primitives::vertex_v3c4[lines.size()*2];

	//int i = 0;
	//std::vector<Geometry::Primitives::Line3f>::const_iterator it;
	//for(it = lines.begin(), i = 0; it != lines.end(); it++, i += 2)
	//{
	//	vertices[i].Position   = Geometry::Primitives::vec3(it->getX1(), it->getY1(), it->getZ1());
	//	vertices[i+1].Position = Geometry::Primitives::vec3(it->getX2(), it->getY2(), it->getZ2());
	//	vertices[i].Color = vertices[i+1].Color = Geometry::Primitives::vec4((float)r/255.0f, (float)g/255.0f, (float)b/255.0f, 1.0f);
	//}

	//QDir dir(qApp->applicationDirPath());

	//QString filenameVertexShader = dir.filePath("GLSL/PrimitiveVS.vert");
	//QString filenameFragmentShader = dir.filePath("GLSL/PrimitiveFS.frag");

	//_renderObjects[index]->setPrimitiveType(GL_LINES);
	//_renderObjects[index]->setVertexData(vertices, (int)lines.size() * 2, nullptr, 0, Viewer::Rendering::VERTEX_FORMAT::V3F_C4);
	//_renderObjects[index]->setName(name);
	//_renderObjects[index]->setCustomShaders(filenameVertexShader, filenameFragmentShader, "");
	//_renderObjects[index]->setLineWidth(lineWidth);

	//delete[] vertices;

	//return index;
}

int Viewer::GLWindow::addRectangles( std::string name, const std::vector<Geometry::Primitives::Rect2f>& rectangles, float lineWidth, byte r, byte g, byte b )
{
	std::vector<Geometry::Primitives::Polygonf> primitives(rectangles.begin(), rectangles.end());
	return addPrimitives(name, primitives, lineWidth, r, g, b);

	//QMutexLocker locker(&_mutexUpdateVertexData);

	//Viewer::Rendering::GLRenderObjectPtr renderObject = getRenderObject(name);
	//if(renderObject)
	//	return -1;

	//int index = addRenderObject();

	//Geometry::Primitives::vertex_v2c4* vertices = new Geometry::Primitives::vertex_v2c4[rectangles.size()*4];
	//unsigned int* indices = new unsigned int[rectangles.size()*8];

	//int i = 0;
	//int eli = 0;
	//std::vector<Geometry::Primitives::Rect2f>::const_iterator it;
	//for(it = rectangles.begin(), i = 0, eli = 0; it != rectangles.end(); it++, i += 4, eli += 8)
	//{
	//	vertices[i].Position   = Geometry::Primitives::vec2(it->getX1(), it->getY1());
	//	vertices[i+1].Position = Geometry::Primitives::vec2(it->getX2(), it->getY2());
	//	vertices[i+2].Position = Geometry::Primitives::vec2(it->getX3(), it->getY3());
	//	vertices[i+3].Position = Geometry::Primitives::vec2(it->getX4(), it->getY4());
	//	vertices[i].Color = vertices[i+1].Color = vertices[i+2].Color = vertices[i+3].Color = Geometry::Primitives::vec4((float)r/255.0f, (float)g/255.0f, (float)b/255.0f, 1.0f);

	//	indices[eli+0] = i+0;
	//	indices[eli+1] = i+1;
	//	indices[eli+2] = i+1;
	//	indices[eli+3] = i+2;
	//	indices[eli+4] = i+2;
	//	indices[eli+5] = i+3;
	//	indices[eli+6] = i+3;
	//	indices[eli+7] = i+0;
	//}

	//QDir dir(qApp->applicationDirPath());

	//QString filenameVertexShader = dir.filePath("GLSL/PrimitiveVS.vert");
	//QString filenameFragmentShader = dir.filePath("GLSL/PrimitiveFS.frag");

	//_renderObjects[index]->setPrimitiveType(GL_LINES);
	//_renderObjects[index]->setVertexData(vertices, (int)rectangles.size() * 4, indices, (int)rectangles.size()*8, Viewer::Rendering::VERTEX_FORMAT::V2F_C4);
	//_renderObjects[index]->setName(name);
	//_renderObjects[index]->setCustomShaders(filenameVertexShader, filenameFragmentShader, "");
	//_renderObjects[index]->setLineWidth(lineWidth);

	//delete[] vertices;

	//return index;
}

int Viewer::GLWindow::addRectangles( std::string name, const std::vector<Geometry::Primitives::Rect3f>& rectangles, float lineWidth, byte r, byte g, byte b )
{
	std::vector<Geometry::Primitives::Polygonf> primitives(rectangles.begin(), rectangles.end());
	return addPrimitives(name, primitives, lineWidth, r, g, b);

	/*QMutexLocker locker(&_mutexUpdateVertexData);

	Viewer::Rendering::GLRenderObjectPtr renderObject = getRenderObject(name);
	if(renderObject)
	return -1;

	int index = addRenderObject();

	Geometry::Primitives::vertex_v3c4* vertices = new Geometry::Primitives::vertex_v3c4[rectangles.size()*4];
	unsigned int* indices = new unsigned int[rectangles.size()*8];

	int i = 0;
	int eli = 0;
	std::vector<Geometry::Primitives::Rect3f>::const_iterator it;
	for(it = rectangles.begin(), i = 0, eli = 0; it != rectangles.end(); it++, i += 4, eli += 8)
	{
	vertices[i].Position   = Geometry::Primitives::vec3(it->getX1(), it->getY1(), it->getZ1());
	vertices[i+1].Position = Geometry::Primitives::vec3(it->getX2(), it->getY2(), it->getZ2());
	vertices[i+2].Position = Geometry::Primitives::vec3(it->getX3(), it->getY3(), it->getZ3());
	vertices[i+3].Position = Geometry::Primitives::vec3(it->getX4(), it->getY4(), it->getZ4());
	vertices[i].Color = vertices[i+1].Color = vertices[i+2].Color = vertices[i+3].Color = Geometry::Primitives::vec4((float)r/255.0f, (float)g/255.0f, (float)b/255.0f, 1.0f);

	indices[eli+0] = i+0;
	indices[eli+1] = i+1;
	indices[eli+2] = i+1;
	indices[eli+3] = i+2;
	indices[eli+4] = i+2;
	indices[eli+5] = i+3;
	indices[eli+6] = i+3;
	indices[eli+7] = i+0;
	}

	QDir dir(qApp->applicationDirPath());

	QString filenameVertexShader = dir.filePath("GLSL/PrimitiveVS.vert");
	QString filenameFragmentShader = dir.filePath("GLSL/PrimitiveFS.frag");

	_renderObjects[index]->setPrimitiveType(GL_LINES);
	_renderObjects[index]->setVertexData(vertices, (int)rectangles.size() * 4, indices, (int)rectangles.size()*8, Viewer::Rendering::VERTEX_FORMAT::V3F_C4);
	_renderObjects[index]->setName(name);
	_renderObjects[index]->setCustomShaders(filenameVertexShader, filenameFragmentShader, "");
	_renderObjects[index]->setLineWidth(lineWidth);

	delete[] vertices;

	return index;*/
}

int Viewer::GLWindow::addTriangles( std::string name, const std::vector<Geometry::Primitives::Triangle2f>& triangles, float lineWidth, byte r, byte g, byte b )
{
	std::vector<Geometry::Primitives::Polygonf> primitives(triangles.begin(), triangles.end());
	return addPrimitives(name, primitives, lineWidth, r, g, b);

	/*QMutexLocker locker(&_mutexUpdateVertexData);

	Viewer::Rendering::GLRenderObjectPtr renderObject = getRenderObject(name);
	if(renderObject)
		return -1;

	int index = addRenderObject();

	Geometry::Primitives::vertex_v2c4* vertices = new Geometry::Primitives::vertex_v2c4[triangles.size()*3];
	unsigned int* indices = new unsigned int[triangles.size()*6];

	int i = 0;
	int eli = 0;
	std::vector<Geometry::Primitives::Triangle2f>::const_iterator it;
	for(it = triangles.begin(), i = 0, eli = 0; it != triangles.end(); it++, i += 4, eli += 6)
	{
		vertices[i].Position   = Geometry::Primitives::vec2(it->getX1(), it->getY1());
		vertices[i+1].Position = Geometry::Primitives::vec2(it->getX2(), it->getY2());
		vertices[i+2].Position = Geometry::Primitives::vec2(it->getX3(), it->getY3());
		vertices[i].Color = vertices[i+1].Color = vertices[i+2].Color = Geometry::Primitives::vec4((float)r/255.0f, (float)g/255.0f, (float)b/255.0f, 1.0f);

		indices[eli+0] = i+0;
		indices[eli+1] = i+1;
		indices[eli+2] = i+1;
		indices[eli+3] = i+2;
		indices[eli+4] = i+2;
		indices[eli+5] = i+0;
	}

	QDir dir(qApp->applicationDirPath());

	QString filenameVertexShader = dir.filePath("GLSL/PrimitiveVS.vert");
	QString filenameFragmentShader = dir.filePath("GLSL/PrimitiveFS.frag");

	_renderObjects[index]->setPrimitiveType(GL_LINES);
	_renderObjects[index]->setVertexData(vertices, (int)triangles.size() * 3, indices, (int)triangles.size()*6, Viewer::Rendering::VERTEX_FORMAT::V2F_C4);
	_renderObjects[index]->setName(name);
	_renderObjects[index]->setCustomShaders(filenameVertexShader, filenameFragmentShader, "");
	_renderObjects[index]->setLineWidth(lineWidth);

	delete[] vertices;

	return index;*/
}

int Viewer::GLWindow::addTriangles( std::string name, const std::vector<Geometry::Primitives::Triangle3f>& triangles, float lineWidth, byte r, byte g, byte b )
{
	std::vector<Geometry::Primitives::Polygonf> primitives(triangles.begin(), triangles.end());
	return addPrimitives(name, primitives, lineWidth, r, g, b);


	/*QMutexLocker locker(&_mutexUpdateVertexData);

	Viewer::Rendering::GLRenderObjectPtr renderObject = getRenderObject(name);
	if(renderObject)
	return -1;

	int index = addRenderObject();

	Geometry::Primitives::vertex_v3c4* vertices = new Geometry::Primitives::vertex_v3c4[triangles.size()*3];
	unsigned int* indices = new unsigned int[triangles.size()*6];

	int i = 0;
	int eli = 0;
	std::vector<Geometry::Primitives::Triangle3f>::const_iterator it;
	for(it = triangles.begin(), i = 0, eli = 0; it != triangles.end(); it++, i += 4, eli += 6)
	{
	vertices[i].Position   = Geometry::Primitives::vec3(it->getX1(), it->getY1(), it->getZ1());
	vertices[i+1].Position = Geometry::Primitives::vec3(it->getX2(), it->getY2(), it->getZ2());
	vertices[i+2].Position = Geometry::Primitives::vec3(it->getX3(), it->getY3(), it->getZ3());
	vertices[i].Color = vertices[i+1].Color = vertices[i+2].Color = Geometry::Primitives::vec4((float)r/255.0f, (float)g/255.0f, (float)b/255.0f, 1.0f);

	indices[eli+0] = i+0;
	indices[eli+1] = i+1;
	indices[eli+2] = i+1;
	indices[eli+3] = i+2;
	indices[eli+4] = i+2;
	indices[eli+5] = i+0;
	}

	QDir dir(qApp->applicationDirPath());

	QString filenameVertexShader = dir.filePath("GLSL/PrimitiveVS.vert");
	QString filenameFragmentShader = dir.filePath("GLSL/PrimitiveFS.frag");

	_renderObjects[index]->setPrimitiveType(GL_LINES);
	_renderObjects[index]->setVertexData(vertices, (int)triangles.size() * 3, indices, (int)triangles.size()*6, Viewer::Rendering::VERTEX_FORMAT::V3F_C4);
	_renderObjects[index]->setName(name);
	_renderObjects[index]->setCustomShaders(filenameVertexShader, filenameFragmentShader, "");
	_renderObjects[index]->setLineWidth(lineWidth);

	delete[] vertices;

	return index;*/
}

int Viewer::GLWindow::addCircles( std::string name, const std::vector<Geometry::Primitives::Circle2f>& circles, float lineWidth, byte r, byte g, byte b )
{
	std::vector<Geometry::Primitives::Polygonf> primitives(circles.begin(), circles.end());
	return addPrimitives(name, primitives, lineWidth, r, g, b);

	/*QMutexLocker locker(&_mutexUpdateVertexData);

	Viewer::Rendering::GLRenderObjectPtr renderObject = getRenderObject(name);
	if(renderObject)
	return -1;

	int index = addRenderObject();


	int nbCirclePoints = 0;
	int nbCircleIndices = 0;
	std::vector<Geometry::Primitives::Circle2f>::const_iterator it;
	for(it = circles.begin(); it != circles.end(); it++)
	{
	nbCirclePoints += (int)it->points().size();
	nbCircleIndices += 2*(int)(it->points().size());
	}

	Geometry::Primitives::vertex_v2c4* vertices = new Geometry::Primitives::vertex_v2c4[nbCirclePoints];
	unsigned int* indices = new unsigned int[nbCircleIndices];

	int vertexIndex = 0;
	int eli = 0;

	for(it = circles.begin(); it != circles.end(); it++)
	{
	const std::vector<Geometry::Primitives::Point3f>& points = it->points();

	int circleZeroIndex = vertexIndex;

	for(int i = 0; i < points.size(); i++)
	{
	indices[eli++] = vertexIndex;

	if(i < points.size()-1)
	indices[eli++] = vertexIndex+1;
	else indices[eli++] = circleZeroIndex;

	vertices[vertexIndex].Position = Geometry::Primitives::vec2(points[i].x, points[i].y);
	vertices[vertexIndex].Color = Geometry::Primitives::vec4((float)r/255.0f, (float)g/255.0f, (float)b/255.0f, 1.0f);
	vertexIndex++;
	}
	}

	QDir dir(qApp->applicationDirPath());

	QString filenameVertexShader = dir.filePath("GLSL/PrimitiveVS.vert");
	QString filenameFragmentShader = dir.filePath("GLSL/PrimitiveFS.frag");

	_renderObjects[index]->setPrimitiveType(GL_LINES);
	_renderObjects[index]->setVertexData(vertices, nbCirclePoints, indices, nbCircleIndices, Viewer::Rendering::VERTEX_FORMAT::V2F_C4);
	_renderObjects[index]->setName(name);
	_renderObjects[index]->setCustomShaders(filenameVertexShader, filenameFragmentShader, "");
	_renderObjects[index]->setLineWidth(lineWidth);

	delete[] vertices;

	return index;*/
}

int Viewer::GLWindow::addCircles( std::string name, const std::vector<Geometry::Primitives::Circle3f>& circles, float lineWidth, byte r, byte g, byte b )
{
	std::vector<Geometry::Primitives::Polygonf> primitives(circles.begin(), circles.end());
	return addPrimitives(name, primitives, lineWidth, r, g, b);
}


int Viewer::GLWindow::addPolyLines( std::string name, const std::vector<Geometry::Primitives::Polygonf>& polygons, float lineWidth, byte r, byte g, byte b )
{
	return addPrimitives(name, polygons, lineWidth, r, g, b);
}

int Viewer::GLWindow::addPrimitives( std::string name, const std::vector<Geometry::Primitives::Polygonf>& primitives, float lineWidth, byte r, byte g, byte b)
{
	QMutexLocker locker(&_mutexUpdateVertexData);

	Viewer::Rendering::GLRenderObjectPtr renderObject = getRenderObject(name);
	if(renderObject)
		return -1;

	int index = addRenderObject();


	int nbPoints = 0;
	int nbIndices = 0;
	std::vector<Geometry::Primitives::Polygonf>::const_iterator it;
	for(it = primitives.begin(); it != primitives.end(); it++)
	{
		nbPoints += (int)it->points().size();
		nbIndices += 2*(int)(it->points().size());
	}

	Geometry::Primitives::vertex_v3c4* vertices = new Geometry::Primitives::vertex_v3c4[nbPoints];
	unsigned int* indices = new unsigned int[nbIndices];

	int vertexIndex = 0;
	int eli = 0;

	for(it = primitives.begin(); it != primitives.end(); it++)
	{
		const std::vector<Geometry::Primitives::Point3f>& points = it->points();

		int circleZeroIndex = vertexIndex;

		for(int i = 0; i < points.size(); i++)
		{
			indices[eli++] = vertexIndex;

			if(i < points.size()-1)
				indices[eli++] = vertexIndex+1;
			else indices[eli++] = circleZeroIndex;

			vertices[vertexIndex].Position = Geometry::Primitives::vec3(points[i].x, points[i].y, points[i].z);
			vertices[vertexIndex].Color = Geometry::Primitives::vec4((float)r/255.0f, (float)g/255.0f, (float)b/255.0f, 1.0f);
			vertexIndex++;
		}
	}

	QDir dir(qApp->applicationDirPath());

	QString filenameVertexShader = dir.filePath("GLSL/PrimitiveVS.vert");
	QString filenameFragmentShader = dir.filePath("GLSL/PrimitiveFS.frag");

	_renderObjects[index]->setPrimitiveType(GL_LINES);
	_renderObjects[index]->setVertexData(vertices, nbPoints, indices, nbIndices, Viewer::Rendering::VERTEX_FORMAT::V3F_C4);
	_renderObjects[index]->setName(name);
	_renderObjects[index]->setCustomShaders(filenameVertexShader, filenameFragmentShader, "");
	_renderObjects[index]->setLineWidth(lineWidth);

	delete[] vertices;

	return index;
}
