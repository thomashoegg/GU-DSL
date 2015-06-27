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

#include "Rendering/GLRenderObject.h"
#include "Rendering/GLPipeline.h"
#include "glwindow.qt.h"
#include "QOpenGLShaderProgram"

#include "Rendering/vertexformat.h"
#include <QOpenGLFramebufferObject>
#include <QThread>
#include <QImage>
#include <QMatrix4x4>

#include <QKeyEvent>
#include <QMouseEvent>

#include <iostream>
#include "OpenGL/SharedGLContext.h"

#include "Images/Image.h"
#include "OpenGL/SharedGLContext.h"
#include "MayaCamera.h"
#include "Primitives/vertex.h"

#define MAX_IMAGE_SIZE		65536


Viewer::Rendering::GLRenderObject::GLRenderObject( Viewer::GLWindow* renderWindow)
	: _glw(renderWindow),
	  _currentImage(nullptr)
{
	initDefaults(-1, -1);

}

Viewer::Rendering::GLRenderObject::GLRenderObject( Viewer::GLWindow* renderWindow, int width, int height)
	: _glw(renderWindow),
	  _currentImage(nullptr)
{
	initDefaults(width, height);

}

Viewer::Rendering::GLRenderObject::GLRenderObject()
{
	_glw = OpenGL::SharedGLContext::getInstance()->getWindow();
	initDefaults(-1, -1);
}

Viewer::Rendering::GLRenderObject::GLRenderObject( int width, int height )
{
	_glw = OpenGL::SharedGLContext::getInstance()->getWindow();
	initDefaults(width, height);

}

Viewer::Rendering::GLRenderObject::~GLRenderObject()
{
	if(_context != nullptr && _context->isValid())
	{
		if(_bufferName[0]) _glw->glDeleteBuffers(buffer::MAX, _bufferName);
		if(_textureName) _glw->glDeleteTextures(1, &_textureName);
		if(_vertexArrayName) _glw->glDeleteVertexArrays(1, &_vertexArrayName);
	}

	if(_indexData) delete[] _indexData;
	if(_vertexData) delete[] _vertexData;

	if(_currentImage) delete _currentImage;

	if(_virtMayaCamera) delete _virtMayaCamera;
}

void Viewer::Rendering::GLRenderObject::initDefaults(int width, int height)
{
	_context = nullptr;
	_imageChanged = false;
	_primitiveType = GL_POINTS;
	_format = VERTEX_FORMAT::V3F;
	_vertexDataUpdated = false;
	_countVertices = 0;
	_countIndices = 0;
	_indexData = nullptr;
	_vertexData = nullptr;
	_useTexture = false;
	_objectInitialized = false;
	_isDepthTestEnabled = false;
	_useFixedVertexColor = false;
	_vertexColor = Geometry::Primitives::vec4(1, 1, 1, 1);
	_ignoreViewTransformation = false;
	_useFixedViewTranslation = false;
	_lineWidth = 1.0f;
	_pointSize = 1.0f;
	_flags = GLRENDEROBJECT_FLAG_NONE;
	_isEnabled = true;
	_viewedLayer = 0;
	_viewedFrameIndex = 0;
	_minClamp = 0;
	_maxClamp = 8.0f;
	_useJetColor = true;
	_isGrayUnsignedShort = false;
	_textureNameExternal = 0;
	_hasCustomPipeline = false;
	_useNormals = false;
	_externalFormat = GL_RED;
	_isSingleChannel = false;
	_interpretAsNormalImage = false;
	_interpretAsVertexImage = false;
	_forceMinMaxValues = false;
	_virtMayaCamera = new MayaCamera();
	_useCamera = false;
	_colorImageConvertSourceFromByteToFloat = false;

	_colorImageConvertSourceFromByteToFloat = false;

	for(int i = 0; i < Viewer::Rendering::buffer::MAX; i++)
		_bufferName[i] = 0;

	_vertexArrayName = 0;
	_textureName = 0;

	_creationThreadId = QThread::currentThreadId();

	_currentImage = new Container::Images::Image();

	_modelMatrix.setToIdentity();

	if(width <= 0 || width > MAX_IMAGE_SIZE)
		_currentImageWidth = -1;
	else _currentImageWidth = width;

	if(height <= 0 || height > MAX_IMAGE_SIZE)
		_currentImageHeight = -1;
	else _currentImageHeight = height;

	_colorImageConvertSourceFromByteToFloat = false;
}



/**
* Mouse event handler.
*
* @param m   mouse event
*/

void Viewer::Rendering::GLRenderObject::mousePressEvent(QMouseEvent *m)
{
	_lastPos = m->pos();

	_virtMayaCamera->setLastPosition(m->x(), m->y());
}

/**
* Mouse event handler.
*
* @param m   mousewheel event
*/
void Viewer::Rendering::GLRenderObject::wheelEvent(QWheelEvent *m)
{
}


/**
* Mouse double click event handler.
*
* @param m   mouse event
*/

void Viewer::Rendering::GLRenderObject::mouseDoubleClickEvent(QMouseEvent *m)
{
	_lastPos = m->pos();
	_virtMayaCamera->setLastPosition(m->x(), m->y());

	// get the buttons involved
	Qt::MouseButtons state = m->buttons();

	bool rb = (state & Qt::RightButton) == Qt::RightButton;

	if(rb) _virtMayaCamera->reset();
}

/**
* Mouse double click event handler.
*
* @param m   mouse event
*/
void Viewer::Rendering::GLRenderObject::keyPressEvent(QKeyEvent *e)
{
	_virtMayaCamera->updateCamera(e->key());
}


/**
* Mouse event handler.
*
* @param m   mouse event
*/

void Viewer::Rendering::GLRenderObject::mouseMoveEvent(QMouseEvent *m)
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
}

bool Viewer::Rendering::GLRenderObject::initArrayBuffer()
{
	bool bValidated(true);

	// Init the index buffer
	_glw->glGenBuffers(1, &_bufferName[buffer::ELEMENT]);
	_glw->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferName[buffer::ELEMENT]);
	_glw->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if(_bufferName[buffer::VERTEX_EXTERNAL] <= 0)
	{
		// Init the vertex buffer
		_glw->glGenBuffers(1, &_bufferName[buffer::VERTEX]);
		_glw->glBindBuffer(GL_ARRAY_BUFFER, _bufferName[buffer::VERTEX]);
		_glw->glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	return bValidated;
}

bool Viewer::Rendering::GLRenderObject::initTexture2D(const QOpenGLFramebufferObjectFormat* format)
{
	bool bValidated(true);

	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	int width = _currentImageWidth > 0 ? _currentImageWidth : 100;
	int height = _currentImageHeight > 0 ? _currentImageHeight : 100;

	if(format)
		_frameBuffer = QOpenGLFramebufferObjectPtr(new QOpenGLFramebufferObject(width, height, *format));
	else _frameBuffer = QOpenGLFramebufferObjectPtr(new QOpenGLFramebufferObject(width, height, QOpenGLFramebufferObject::Depth, GL_TEXTURE_RECTANGLE, GL_RGBA32F));

	//_glw->glGenTextures(1, &_textureName);

	assert(wglGetCurrentContext() != NULL);

	_textureName = _frameBuffer->texture();
	_glw->glActiveTexture(GL_TEXTURE0);
	_glw->glBindTexture(GL_TEXTURE_RECTANGLE, _textureName);

	GLenum err = glGetError();
	_glw->glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_SWIZZLE_R, GL_RED);
	_glw->glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
	_glw->glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
	_glw->glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);

	_glw->glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	_glw->glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	_glw->glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	_glw->glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Give the image to OpenGL 
	_glw->glTexImage2D(
						GL_TEXTURE_RECTANGLE,		// target
						0,							// level
						//_frameBuffer->format().internalTextureFormat(),					// internalFormat
						GL_RGBA32F,					// internalFormat
						width,						// width
						height,						// height
						0,							// border
						_externalFormat,			// format
						GL_FLOAT,					// type
						0							// texels
						);

	err = _glw->glGetError();

	_glw->glBindTexture(GL_TEXTURE_RECTANGLE, 0);

	return bValidated;
}

bool Viewer::Rendering::GLRenderObject::initVertexArray()
{
	if(!_objectInitialized)
		return false;

	bool bValidated(true);

	bool enableVertexAttribute = false;
	bool enableNormalAttribute = false;
	bool enableTextureAttribute = false;
	bool enableColorAttribute = false;

	if(_vertexArrayName <= 0)
		_glw->glGenVertexArrays(1, &_vertexArrayName);

	_glw->glBindVertexArray(_vertexArrayName);

	if(_bufferName[buffer::VERTEX_EXTERNAL] != 0)
		_glw->glBindBuffer(GL_ARRAY_BUFFER, _bufferName[buffer::VERTEX_EXTERNAL]);
	else _glw->glBindBuffer(GL_ARRAY_BUFFER, _bufferName[buffer::VERTEX]);


	switch (_format)
	{
	case Viewer::Rendering::VERTEX_FORMAT::V1F:
		enableVertexAttribute = true;
		_glw->glVertexAttribPointer(semantic::attr::POSITION, 1, GL_FLOAT, GL_FALSE, sizeof(float), BUFFER_OFFSET(0));
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V1I:
		enableVertexAttribute = true;
		_glw->glVertexAttribPointer(semantic::attr::POSITION, 1, GL_INT, GL_FALSE, sizeof(int), BUFFER_OFFSET(0));
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V1UI:
		enableVertexAttribute = true;
		_glw->glVertexAttribPointer(semantic::attr::POSITION, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(unsigned int), BUFFER_OFFSET(0));
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V2F:
		enableVertexAttribute = true;
		_glw->glVertexAttribPointer(semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v2), BUFFER_OFFSET(0));
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V3F:
		enableVertexAttribute = true;
		_glw->glVertexAttribPointer(semantic::attr::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v3), BUFFER_OFFSET(0));
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V4F:
		enableVertexAttribute = true;
		_glw->glVertexAttribPointer(semantic::attr::POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v4), BUFFER_OFFSET(0));
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V2F_C4:
		enableVertexAttribute = true;
		enableColorAttribute = true;
		_glw->glVertexAttribPointer(semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v2c4), BUFFER_OFFSET(0));
		_glw->glVertexAttribPointer(semantic::attr::COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v2c4), BUFFER_OFFSET(sizeof(Geometry::Primitives::vec2)));
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V3F_C4:
		enableVertexAttribute = true;
		enableColorAttribute = true;
		_glw->glVertexAttribPointer(semantic::attr::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v3c4), BUFFER_OFFSET(0));
		_glw->glVertexAttribPointer(semantic::attr::COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v3c4), BUFFER_OFFSET(sizeof(Geometry::Primitives::vec3)));
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V2F_T2:
		enableVertexAttribute = true;
		enableTextureAttribute = true;
		_glw->glVertexAttribPointer(semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v2t2), BUFFER_OFFSET(0));
		_glw->glVertexAttribPointer(semantic::attr::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v2t2), BUFFER_OFFSET(sizeof(Geometry::Primitives::vec2)));
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V3F_T2:
		enableVertexAttribute = true;
		enableTextureAttribute = true;
		_glw->glVertexAttribPointer(semantic::attr::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v3t2), BUFFER_OFFSET(0));
		_glw->glVertexAttribPointer(semantic::attr::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v3t2), BUFFER_OFFSET(sizeof(Geometry::Primitives::vec3)));
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V2F_T2_C4:
		enableVertexAttribute = true;
		enableTextureAttribute = true;
		enableColorAttribute = true;
		_glw->glVertexAttribPointer(semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v3t2c4), BUFFER_OFFSET(0));
		_glw->glVertexAttribPointer(semantic::attr::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v3t2c4), BUFFER_OFFSET(sizeof(Geometry::Primitives::vec2)));
		_glw->glVertexAttribPointer(semantic::attr::COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v3t2c4), BUFFER_OFFSET(sizeof(Geometry::Primitives::vec2) + sizeof(Geometry::Primitives::vec2)));
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V3F_T2_C4:
		enableVertexAttribute = true;
		enableTextureAttribute = true;
		enableColorAttribute = true;
		_glw->glVertexAttribPointer(semantic::attr::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v3t2c4), BUFFER_OFFSET(0));
		_glw->glVertexAttribPointer(semantic::attr::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v3t2c4), BUFFER_OFFSET(sizeof(Geometry::Primitives::vec3)));
		_glw->glVertexAttribPointer(semantic::attr::COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v3t2c4), BUFFER_OFFSET(sizeof(Geometry::Primitives::vec3) + sizeof(Geometry::Primitives::vec2)));
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V2F_N2F:
		enableVertexAttribute = true;
		enableNormalAttribute = true;
		_glw->glVertexAttribPointer(semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v2n2), BUFFER_OFFSET(0));
		_glw->glVertexAttribPointer(semantic::attr::NORMAL, 2, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v2n2), BUFFER_OFFSET(sizeof(Geometry::Primitives::vec2)));
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V3F_N3F:
		enableVertexAttribute = true;
		enableNormalAttribute = true;
		_glw->glVertexAttribPointer(semantic::attr::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v3n3), BUFFER_OFFSET(0));
		_glw->glVertexAttribPointer(semantic::attr::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v3n3), BUFFER_OFFSET(sizeof(Geometry::Primitives::vec3)));
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V4F_N4F:
		enableVertexAttribute = true;
		enableNormalAttribute = true;
		_glw->glVertexAttribPointer(semantic::attr::POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v4n4), BUFFER_OFFSET(0));
		_glw->glVertexAttribPointer(semantic::attr::NORMAL, 4, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v4n4), BUFFER_OFFSET(sizeof(Geometry::Primitives::vec4)));
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V2F_N2F_C4:
		enableVertexAttribute = true;
		enableNormalAttribute = true;
		enableColorAttribute = true;
		_glw->glVertexAttribPointer(semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v2n2c4), BUFFER_OFFSET(0));
		_glw->glVertexAttribPointer(semantic::attr::NORMAL, 2, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v2n2c4), BUFFER_OFFSET(sizeof(Geometry::Primitives::vec2)));
		_glw->glVertexAttribPointer(semantic::attr::COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v2n2c4), BUFFER_OFFSET(sizeof(Geometry::Primitives::vec2) + sizeof(Geometry::Primitives::vec2)));
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V3F_N3F_C4:
		enableVertexAttribute = true;
		enableNormalAttribute = true;
		enableColorAttribute = true;
		_glw->glVertexAttribPointer(semantic::attr::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v3n3c4), BUFFER_OFFSET(0));
		_glw->glVertexAttribPointer(semantic::attr::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v3n3c4), BUFFER_OFFSET(sizeof(Geometry::Primitives::vec3)));
		_glw->glVertexAttribPointer(semantic::attr::COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v3n3c4), BUFFER_OFFSET(sizeof(Geometry::Primitives::vec3) + sizeof(Geometry::Primitives::vec3)));
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V2F_N2F_T2:
		enableVertexAttribute = true;
		enableNormalAttribute = true;
		enableTextureAttribute = true;
		_glw->glVertexAttribPointer(semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v2n2t2), BUFFER_OFFSET(0));
		_glw->glVertexAttribPointer(semantic::attr::NORMAL, 2, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v2n2t2), BUFFER_OFFSET(sizeof(Geometry::Primitives::vec2)));
		_glw->glVertexAttribPointer(semantic::attr::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v2n2t2), BUFFER_OFFSET(sizeof(Geometry::Primitives::vec2) + sizeof(Geometry::Primitives::vec2)));
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V3F_N3F_T2:
		enableVertexAttribute = true;
		enableNormalAttribute = true;
		enableTextureAttribute = true;
		_glw->glVertexAttribPointer(semantic::attr::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v3n3t2), BUFFER_OFFSET(0));
		_glw->glVertexAttribPointer(semantic::attr::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v3n3t2), BUFFER_OFFSET(sizeof(Geometry::Primitives::vec3)));
		_glw->glVertexAttribPointer(semantic::attr::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v3n3t2), BUFFER_OFFSET(sizeof(Geometry::Primitives::vec3) + sizeof(Geometry::Primitives::vec3)));
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V2F_N2F_T2_C4:
		enableVertexAttribute = true;
		enableNormalAttribute = true;
		enableTextureAttribute = true;
		enableColorAttribute = true;
		_glw->glVertexAttribPointer(semantic::attr::POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v2n2t2c4), BUFFER_OFFSET(0));
		_glw->glVertexAttribPointer(semantic::attr::NORMAL, 2, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v2n2t2c4), BUFFER_OFFSET(sizeof(Geometry::Primitives::vec2)));
		_glw->glVertexAttribPointer(semantic::attr::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v2n2t2c4), BUFFER_OFFSET(sizeof(Geometry::Primitives::vec2) + sizeof(Geometry::Primitives::vec2)));
		_glw->glVertexAttribPointer(semantic::attr::COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v2n2t2c4), BUFFER_OFFSET(sizeof(Geometry::Primitives::vec2) + sizeof(Geometry::Primitives::vec2) + sizeof(Geometry::Primitives::vec2)));
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V3F_N3F_T2_C4:
		enableVertexAttribute = true;
		enableNormalAttribute = true;
		enableTextureAttribute = true;
		enableColorAttribute = true;
		_glw->glVertexAttribPointer(semantic::attr::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v3n3t2c4), BUFFER_OFFSET(0));
		_glw->glVertexAttribPointer(semantic::attr::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v3n3t2c4), BUFFER_OFFSET(sizeof(Geometry::Primitives::vec3)));
		_glw->glVertexAttribPointer(semantic::attr::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v3n3t2c4), BUFFER_OFFSET(sizeof(Geometry::Primitives::vec3) + sizeof(Geometry::Primitives::vec3)));
		_glw->glVertexAttribPointer(semantic::attr::COLOR, 4, GL_FLOAT, GL_FALSE, sizeof(Geometry::Primitives::vertex_v3n3t2c4), BUFFER_OFFSET(sizeof(Geometry::Primitives::vec3) + sizeof(Geometry::Primitives::vec3) + sizeof(Geometry::Primitives::vec2)));
		break;
	default:
		break;
	}

	if(enableVertexAttribute)
		_glw->glEnableVertexAttribArray(semantic::attr::POSITION);
	else _glw->glDisableVertexAttribArray(semantic::attr::POSITION);

	if(enableNormalAttribute)
	{
		_useNormals = true;
		_glw->glEnableVertexAttribArray(semantic::attr::NORMAL);
	}
	else
	{
		_useNormals = false;
		_glw->glDisableVertexAttribArray(semantic::attr::NORMAL);
	}

	if(enableTextureAttribute)
	{
		_useTexture = true;
		_glw->glEnableVertexAttribArray(semantic::attr::TEXCOORD);
	}
	else
	{
		_useTexture = false;
		_glw->glDisableVertexAttribArray(semantic::attr::TEXCOORD);
	}

	if(enableColorAttribute)
	{
		_useColors = true;
		_glw->glEnableVertexAttribArray(semantic::attr::COLOR);
	}
	else 
	{
		_useColors = false;
		_glw->glDisableVertexAttribArray(semantic::attr::COLOR);
	}

	_glw->glBindVertexArray(0);
	_glw->glBindBuffer(GL_ARRAY_BUFFER, 0);
	return bValidated;
}

bool Viewer::Rendering::GLRenderObject::initUniformBuffer()
{
	bool bValidated(true);

	GLint UniformBufferOffset(0);
	//GLint UniformBlockSize(0);

	_glw->glGetIntegerv(
		GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT,
		&UniformBufferOffset);

	{
		// sizeof 4 x 4 matrix
		GLint UniformBlockSize = sizeof(float)*16; //max(sizeof(float)*16, UniformBufferOffset);

		_glw->glGenBuffers(1, &_bufferName[buffer::TRANSFORM_MODEL]);
		_glw->glBindBuffer(GL_UNIFORM_BUFFER, _bufferName[buffer::TRANSFORM_MODEL]);
		_glw->glBufferData(GL_UNIFORM_BUFFER, UniformBlockSize, NULL, GL_DYNAMIC_DRAW);
		_glw->glBindBuffer(GL_UNIFORM_BUFFER, 0);

		_glw->glGenBuffers(1, &_bufferName[buffer::TRANSFORM_VIEW]);
		_glw->glBindBuffer(GL_UNIFORM_BUFFER, _bufferName[buffer::TRANSFORM_VIEW]);
		_glw->glBufferData(GL_UNIFORM_BUFFER, UniformBlockSize, NULL, GL_DYNAMIC_DRAW);
		_glw->glBindBuffer(GL_UNIFORM_BUFFER, 0);

		_glw->glGenBuffers(1, &_bufferName[buffer::TRANSFORM_PROJECTION]);
		_glw->glBindBuffer(GL_UNIFORM_BUFFER, _bufferName[buffer::TRANSFORM_PROJECTION]);
		_glw->glBufferData(GL_UNIFORM_BUFFER, UniformBlockSize, NULL, GL_DYNAMIC_DRAW);
		_glw->glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	return bValidated;
}

bool Viewer::Rendering::GLRenderObject::initDebugOutput()
{
	bool bValidated(true);

	//glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
	//glDebugMessageControlARB(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	//glDebugMessageCallbackARB(&glf::debugOutput, NULL);

	return bValidated;
}


void Viewer::Rendering::GLRenderObject::setCustomShaders(const QString& filenameVertexShader, const QString& filenameFragmentShader, const QString& filenameGeometryShader )
{
	_customFilenameVertexShader = filenameVertexShader;
	_customFilenameFragmentShader = filenameFragmentShader;
	_customFilenameGeometryShader = filenameGeometryShader;
}


bool Viewer::Rendering::GLRenderObject::init(std::string filenameVertexShader, std::string filenameFragmentShader, int fboWidth, int fboHeight)
{
	return init(filenameVertexShader, filenameFragmentShader, "", fboWidth, fboHeight);
}

bool Viewer::Rendering::GLRenderObject::init(std::string filenameVertexShader, std::string filenameFragmentShader, std::string filenameGeometryShader, int fboWidth, int fboHeight)
{

	if(!_customFilenameVertexShader.isEmpty())
		filenameVertexShader = _customFilenameVertexShader.toStdString();

	if(!_customFilenameFragmentShader.isEmpty())
		filenameFragmentShader = _customFilenameFragmentShader.toStdString();

	if(!_customFilenameGeometryShader.isEmpty())
		filenameGeometryShader = _customFilenameGeometryShader.toStdString();

	Viewer::Rendering::GLPipelinePtr pipeline(new Viewer::Rendering::GLPipeline(_glw, _glw->getGLContext()));
	pipeline->init(filenameVertexShader.c_str(), filenameFragmentShader.c_str(), filenameGeometryShader.c_str());

	//_hasCustomPipeline = true;

	return init(pipeline);
}

bool Viewer::Rendering::GLRenderObject::init(Viewer::Rendering::GLPipelinePtr pipeline)
{
	return init(pipeline, nullptr);
}

bool Viewer::Rendering::GLRenderObject::init(Viewer::Rendering::GLPipelinePtr pipeline, const QOpenGLFramebufferObjectFormat* format)
{
	return init(pipeline, format, GL_RED);
}

bool Viewer::Rendering::GLRenderObject::init(Viewer::Rendering::GLPipelinePtr pipeline, const QOpenGLFramebufferObjectFormat* format, GLuint externalFormat)
{
	if(_objectInitialized)
		return true;


	// If we have custom shaders, then recreate the pipeline
	if(!_customFilenameVertexShader.isEmpty() || !_customFilenameGeometryShader.isEmpty() || !_customFilenameFragmentShader.isEmpty())
	{
		pipeline.reset(new Viewer::Rendering::GLPipeline(_glw, _glw->getGLContext()));
		pipeline->init(_customFilenameVertexShader, _customFilenameFragmentShader, _customFilenameGeometryShader);
	}

	if(!pipeline)
	{
		QString excepString("%1: %2");
		excepString.arg(__FUNCTION__).arg("pipeline parameter null!");

		throw std::exception(excepString.toLatin1());
	}

	_context = pipeline->getContext();

	if(_context == nullptr || !_context->isValid())
	{
#if DEBUG
		QString excepString("%1: %2");
		excepString.arg(__FUNCTION__).arg("OpenGL rendering context null or invalid!");

		throw std::exception(excepString.toLatin1());
#endif
		return false;
	}

	// Activate the assigned context to make sure that the resources are created in the right one


	//bool sharedWasCurrent = OpenGL::SharedGLContext::getInstance()->isSharedContextCurrent();
	//if(sharedWasCurrent)
	//	OpenGL::SharedGLContext::getInstance()->doneCurrent();
	//	
	//_glw->makeCurrent();
	//_context->makeCurrent(_glw);

	_objectInitialized = true;
	_pipeline = pipeline;

	_externalFormat = externalFormat;

	bool bValidated(true);

	if(bValidated)
		bValidated = initTexture2D(format);
	if(bValidated)
		bValidated = initArrayBuffer();
	if(bValidated)
		bValidated = initVertexArray();
	if(bValidated)
		bValidated = initUniformBuffer();

	_imageChanged = true;

	//_glw->doneCurrent();

	//if(sharedWasCurrent)
	//	OpenGL::SharedGLContext::getInstance()->makeCurrent();

	return bValidated;
}


void Viewer::Rendering::GLRenderObject::setImage(Container::Images::Image image)
{
	_imageMutex.lock();

	(*_currentImage) = image;

	if(_viewedLayer >= _currentImage->layerCount() || _viewedLayer < 0)
		_viewedLayer = 0;

	if(_viewedFrameIndex >= (int)_currentImage->frameIndexSize() || _viewedFrameIndex < 0)
		_viewedFrameIndex = 0;

	_currentImageWidth = _currentImage->width(_viewedFrameIndex);
	_currentImageHeight = _currentImage->height(_viewedFrameIndex);

	_imageChanged = true;

	_imageMutex.unlock();

	//// If we are in the same thread as during the creation, then directly update the texture
	//if(QThread::currentThreadId() == _creationThreadId)
	//	updateTexture();
}


void Viewer::Rendering::GLRenderObject::updateTexture()
{
	if(false)
	{
		saveToFile("C:\\temp\\testimage.png");
	}
	//std::cout << "Update Texture called" << std::endl;

	if(!_imageChanged)
		return;

	_imageMutex.lock();

	if(_viewedLayer >= _currentImage->layerCount() || _viewedLayer < 0)
		_viewedLayer = 0;

	if(_viewedFrameIndex >= (int)_currentImage->frameIndexSize() || _viewedFrameIndex < 0)
		_viewedFrameIndex = 0;

	if(_currentImage->width(_viewedFrameIndex) != 0 && _currentImage->height(_viewedFrameIndex) != 0 && 
	   (_currentImage->width(_viewedFrameIndex) != _currentImageWidth || _currentImage->height(_viewedFrameIndex) != _currentImageHeight))
	{
		_currentImageWidth = _currentImage->width(_viewedFrameIndex);
		_currentImageHeight = _currentImage->height(_viewedFrameIndex);
	}

	GLenum err = _glw->glGetError();

	//glEnable(GL_TEXTURE_RECTANGLE);

	_glw->glActiveTexture (GL_TEXTURE0);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	_glw->glBindTexture(GL_TEXTURE_RECTANGLE, textureId());

	_isColorImage = false;
	_isGrayUnsignedShort = false;

	switch(_externalFormat)
	{
	case GL_RGBA:
	case GL_RGB:
		_isColorImage = true;
		_isSingleChannel = false;
		break;
	default:
		_isSingleChannel = true;
		break;
	}

	err = _glw->glGetError();

	if(_currentImage->pixelData(_viewedFrameIndex) == Container::Images::PixelData::COLOR)
	{
		unsigned char* colorMap = (unsigned char*)_currentImage->buffer(_viewedFrameIndex);
		unsigned char* texData = colorMap + (_viewedLayer*_currentImage->width(_viewedFrameIndex)*_currentImage->size(_viewedFrameIndex)*sizeof(unsigned char) * 4);

		if(_currentImage->width(_viewedFrameIndex)*_currentImage->height(_viewedFrameIndex) == 0)
			texData = 0;

		// Give the image to OpenGL
		_glw->glTexImage2D(
			GL_TEXTURE_RECTANGLE,								// target
			0,													// level
			GL_RGBA32F,											// internalFormat
			_currentImageWidth,									// width
			_currentImageHeight,								// height
			0,													// border
			GL_RGBA,											// format
			GL_UNSIGNED_BYTE,									// type
			texData												// texels
			);

		_isColorImage = true;
	}
	else
	{
		if(_currentImage->dataType(_viewedFrameIndex) != Container::Images::DataType::UNDEFINED)
		{
			const void* data = _currentImage->buffer(_viewedFrameIndex);
			int bytesize = 0;
			switch (_currentImage->dataType(_viewedFrameIndex))
			{
			case Container::Images::DataType::CHAR:
			case Container::Images::DataType::UCHAR:
				bytesize = sizeof(char);
				break;
			case Container::Images::DataType::SHORT:
			case Container::Images::DataType::USHORT:
				bytesize = sizeof(short);
				break;
			case Container::Images::DataType::INT:
			case Container::Images::DataType::UINT:
				bytesize = sizeof(int);
				break;
			case Container::Images::DataType::FLOAT:
				bytesize = sizeof(float);
				break;
			case Container::Images::DataType::DOUBLE:
				bytesize = sizeof(double);
				break;
			}

			void* texData = (char*)data + (_viewedLayer)*_currentImage->width(_viewedFrameIndex)*_currentImage->height(_viewedFrameIndex)*bytesize;

			if(_currentImage->width(_viewedFrameIndex)*_currentImage->height(_viewedFrameIndex) == 0)
				texData = 0;

			// Give the image to OpenGL 
			_glw->glTexImage2D(
				GL_TEXTURE_RECTANGLE,								// target
				0,													// level
				GL_RGBA32F,											// internalFormat
				_currentImageWidth,									// width
				_currentImageHeight,								// height
				0,													// border
				_externalFormat,									// format
				(GLuint)_currentImage->dataType(_viewedFrameIndex),	// type
				texData												// texels
				);

			_isGrayUnsignedShort =  _currentImage->pixelData(_viewedFrameIndex) == Container::Images::PixelData::GRAY; // !_isColorImage; //
		}
		else
		{
			//D_OUT2(__FUNCTION__, "could not render undefined datatype");
		}

	}

	err = _glw->glGetError();

	_glw->glBindTexture(GL_TEXTURE_RECTANGLE, NULL);


	_imageChanged = false;

	_imageMutex.unlock();

	//saveToFile("C:\\temp\\testimage.png");
}

void Viewer::Rendering::GLRenderObject::saveToFile(QString path)
{
	int* pixels = new int[_currentImageWidth*_currentImageHeight];

	GLenum err = _glw->glGetError();
	_glw->glBindTexture(GL_TEXTURE_RECTANGLE, textureId());

	err =_glw-> glGetError();
	_glw->glGetTexImage(GL_TEXTURE_RECTANGLE, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	//glGetTexImage(GL_TEXTURE_RECTANGLE, 0, GL_LUMINANCE, GL_FLOAT, pixelsF);

	err = _glw->glGetError();
	_glw->glBindTexture(GL_TEXTURE_RECTANGLE, 0);

	err = _glw->glGetError();



	QImage image((byte*)pixels,_currentImageWidth, _currentImageHeight, QImage::Format_ARGB32);
	image.save(path);

	delete[] pixels;
}

void Viewer::Rendering::GLRenderObject::updateTransformation(QMatrix4x4& view, QMatrix4x4& projection )
{
	updateTransformation(_modelMatrix, view, projection);
}

void Viewer::Rendering::GLRenderObject::updateTransformation( QMatrix4x4& model, QMatrix4x4& view, QMatrix4x4& projection )
{
	// World matrix
	_glw->glBindBuffer(GL_UNIFORM_BUFFER, _bufferName[buffer::TRANSFORM_MODEL]);
	float* Pointer = (float*)_glw->glMapBufferRange(
		GL_UNIFORM_BUFFER, 0,	sizeof(float)*16,
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);

	memcpy(Pointer, model.constData(), sizeof(float)*16);

	// Make sure the uniform buffer is uploaded
	_glw->glUnmapBuffer(GL_UNIFORM_BUFFER);

	// View matrix
	_glw->glBindBuffer(GL_UNIFORM_BUFFER, _bufferName[buffer::TRANSFORM_VIEW]);
	Pointer = (float*)_glw->glMapBufferRange(
		GL_UNIFORM_BUFFER, 0,	sizeof(float)*16,
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);

	memcpy(Pointer, view.constData(), sizeof(float)*16);

	// Make sure the uniform buffer is uploaded
	_glw->glUnmapBuffer(GL_UNIFORM_BUFFER);

	// Projection matrix
	_glw->glBindBuffer(GL_UNIFORM_BUFFER, _bufferName[buffer::TRANSFORM_PROJECTION]);
	Pointer = (float*)_glw->glMapBufferRange(
		GL_UNIFORM_BUFFER, 0,	sizeof(float)*16,
		GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);

	memcpy(Pointer, projection.constData(), sizeof(float)*16);

	// Make sure the uniform buffer is uploaded
	_glw->glUnmapBuffer(GL_UNIFORM_BUFFER);
}

void Viewer::Rendering::GLRenderObject::updateVertexData()
{
	updateVertexData(_vertexData, _indexData);
}

void Viewer::Rendering::GLRenderObject::updateVertexData(void* vertexData, unsigned int* indexData)
{
	if(!_vertexDataUpdated)
		return;

	QMutexLocker locker(&_mutexUpdateVertexData);

	initVertexArray();

	_glw->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferName[buffer::ELEMENT]);
	_glw->glBufferData(GL_ELEMENT_ARRAY_BUFFER, _countIndices * sizeof(unsigned int), indexData, GL_STATIC_DRAW);
	_glw->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if( _bufferName[buffer::VERTEX_EXTERNAL] <= 0)
	{
		int sizeofVertices = sizeOfVertexFormat()*_countVertices;

		_glw->glBindBuffer(GL_ARRAY_BUFFER, _bufferName[buffer::VERTEX]);
		_glw->glBufferData(GL_ARRAY_BUFFER, sizeofVertices, vertexData, GL_STATIC_DRAW);
		_glw->glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	_vertexDataUpdated = false;
}

void Viewer::Rendering::GLRenderObject::setVertexData(const void* vertices, int nVertices)
{
	setVertexData(vertices, nVertices, _indexData, _countIndices, _format);
}

void Viewer::Rendering::GLRenderObject::setVertexData(const void* vertices, int nVertices, const unsigned int* indices, int nIndices, VERTEX_FORMAT::Format format)
{
	// If the vertex format changes, then also reinitialize the vertex array
	if(_format != format)
	{
		_format = format;
		initVertexArray();
	}

	_mutexUpdateVertexData.lock();

	if(_indexData && _indexData != indices) 
		delete[] _indexData;

	if((_primitiveType == GL_POINTS || _primitiveType == GL_LINES) && nIndices <= 0)
		nIndices = nVertices;

	_countIndices = nIndices;
	_indexData = new unsigned int[nIndices];

	if(indices != nullptr)
		memcpy(_indexData, indices, sizeof(unsigned int)*nIndices);
	else if(_primitiveType == GL_POINTS || _primitiveType == GL_LINES)
	{
		for(int i = 0; i < nIndices; i++)
			_indexData[i] = i;
	}
	else memset(_indexData, 0, sizeof(unsigned int)*nIndices);

	if(_vertexData != vertices || _countVertices != nVertices)
		initVertices(nVertices);

	int sizeOfVertices = sizeOfVertexFormat() * nVertices;
	if(vertices != nullptr)
		memcpy(_vertexData, vertices, sizeOfVertices);
	else memset(_vertexData, 0, sizeOfVertices);

	_vertexDataUpdated = true;

	_mutexUpdateVertexData.unlock();

	//// If we are in the same thread as during the creation, then directly update the texture
	//if(QThread::currentThreadId() == _creationThreadId)
	//	updateVertexData();
}

void Viewer::Rendering::GLRenderObject::setVertexData( unsigned int vbo, int countVertices, int countIndices, Viewer::Rendering::VERTEX_FORMAT::Format format )
{
	setVertexData(nullptr, countVertices, nullptr, countIndices, format);

	_bufferName[buffer::VERTEX_EXTERNAL] = vbo;
}

void Viewer::Rendering::GLRenderObject::render()
{
	if(!_isEnabled || _hasCustomPipeline)
		return;

	//if(isDepthTestEnabled())
	//{
	//	_glf->glEnable(GL_DEPTH_TEST);
	//	_glf->glDepthFunc(GL_LESS);

	//	float depth(1.0f);
	//	_glf->glClearBufferfv(GL_DEPTH, 0, &depth);
	//}

	GLenum err = _glw->glGetError();


	_pipeline->bind();

	QOpenGLShaderProgramPtr shaderProgram = _pipeline->getShaderProgram();

	int minMaxVecLocation = shaderProgram->uniformLocation("minMax");
	int useJetColorLocation = shaderProgram->uniformLocation("useJetColor");
	int isColorImageLocation = shaderProgram->uniformLocation("isColorImage");
	int isGrayUnsignedShortLocation = shaderProgram->uniformLocation("isGrayUnsignedShort");
	int useNormalsLocation = shaderProgram->uniformLocation("useNormals");
	int useColorsLocation = shaderProgram->uniformLocation("useColors");	
	int useNormalsAsColorsLocation = shaderProgram->uniformLocation("useNormalsAsColor");
	int useFixedVertexColorLocation = shaderProgram->uniformLocation("useFixedVertexColor");
	int fixedVertexColorLocation = shaderProgram->uniformLocation("fixedVertexColor");
	int useIgnoreVertexViewTranslationLocation = shaderProgram->uniformLocation("useIgnoreVertexViewTranslation");
	int useFixedViewTranslationLocation = shaderProgram->uniformLocation("useFixedViewTranslation");
	int fixedViewTranslationLocation = shaderProgram->uniformLocation("fixedViewTranslation");
	int isSingleChannelLocation = shaderProgram->uniformLocation("isSingleChannel");
	int isNormalMapLocation = shaderProgram->uniformLocation("isNormalMap");
	int isVertexMapLocation = shaderProgram->uniformLocation("isVertexMap");
	int isColorImageConvertSourceFromByteToFloatLocation = shaderProgram->uniformLocation("icColorImageConvertSourceFromByteToFloat");
	int qNanPosition = shaderProgram->uniformLocation("qnan");

	err = _glw->glGetError();

	if(_useTexture)
	{
		updateTexture();

		assert(wglGetCurrentContext() != NULL);

		_glw->glActiveTexture(GL_TEXTURE0);
		_glw->glBindTexture(GL_TEXTURE_RECTANGLE, textureId());
	}

	err = _glw->glGetError();

	updateVertexData();

	err = _glw->glGetError();

	_glw->glBindBufferBase(GL_UNIFORM_BUFFER, semantic::uniform::TRANSFORM_MODEL, _bufferName[buffer::TRANSFORM_MODEL]);
	_glw->glBindBufferBase(GL_UNIFORM_BUFFER, semantic::uniform::TRANSFORM_VIEW, _bufferName[buffer::TRANSFORM_VIEW]);
	_glw->glBindBufferBase(GL_UNIFORM_BUFFER, semantic::uniform::TRANSFORM_PROJECTION, _bufferName[buffer::TRANSFORM_PROJECTION]);
	_glw->glBindVertexArray(_vertexArrayName);
	if(_bufferName[buffer::VERTEX_EXTERNAL] != 0)
		_glw->glBindBuffer(GL_ARRAY_BUFFER, _bufferName[buffer::VERTEX_EXTERNAL]);
	else _glw->glBindBuffer(GL_ARRAY_BUFFER, _bufferName[buffer::VERTEX]);
	_glw->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _bufferName[buffer::ELEMENT]);

	err = _glw->glGetError();

	shaderProgram->setUniformValue(isNormalMapLocation, _interpretAsNormalImage ? 1 : 0);
	shaderProgram->setUniformValue(isVertexMapLocation, _interpretAsVertexImage ? 1 : 0);
	shaderProgram->setUniformValue(isSingleChannelLocation, _isSingleChannel ? 1 : 0);
	shaderProgram->setUniformValue(useJetColorLocation, _useJetColor ? 1 : 0);
	shaderProgram->setUniformValue(isColorImageLocation, _isColorImage ? 1 : 0);
	shaderProgram->setUniformValue(isGrayUnsignedShortLocation, _isGrayUnsignedShort ? 1 : 0);
	shaderProgram->setUniformValue(isColorImageConvertSourceFromByteToFloatLocation, _colorImageConvertSourceFromByteToFloat ? 1 : 0);
	if(_isGrayUnsignedShort)
	{
		shaderProgram->setUniformValue(minMaxVecLocation, 0, USHRT_MAX);
	}
	else shaderProgram->setUniformValue(minMaxVecLocation, _minClamp, _maxClamp);

	shaderProgram->setUniformValue(useColorsLocation, _useColors ? 1 : 0);
	shaderProgram->setUniformValue(useNormalsLocation, _useNormals ? 1 : 0);
	shaderProgram->setUniformValue(useFixedVertexColorLocation, _useFixedVertexColor ? 1 : 0);
	shaderProgram->setUniformValue(useIgnoreVertexViewTranslationLocation, _ignoreViewTransformation ? 1 : 0);
	shaderProgram->setUniformValue(useFixedViewTranslationLocation, _useFixedViewTranslation ? 1 : 0);

	float qnan = std::numeric_limits<float>::quiet_NaN();
	shaderProgram->setUniformValue(qNanPosition, qnan);

	if(_useFixedViewTranslation)
		shaderProgram->setUniformValue(fixedViewTranslationLocation, _vertexFixedViewTranslation.x, _vertexFixedViewTranslation.y, _vertexFixedViewTranslation.z);

	if(_useFixedVertexColor)
		shaderProgram->setUniformValue(fixedVertexColorLocation, _vertexColor.r, _vertexColor.g, _vertexColor.b, _vertexColor.a);	

	if(_lineWidth != 1.0f)
	{
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(_lineWidth);
	}

	if(_pointSize != 1.0f)
	{
		glEnable(GL_PROGRAM_POINT_SIZE);
		glPointSize(_pointSize);
	}

	//float* ptr = (float*)_glw->glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE_ARB);
	//_glw->glUnmapBuffer(GL_ARRAY_BUFFER);

	//_glw->glDrawElementsInstancedBaseVertexBaseInstance(
	//	_primitiveType, _countIndices, GL_UNSIGNED_INT, 0, 1, 0, 0);

	err = _glw->glGetError();
	_glw->glDrawElements(_primitiveType, _countIndices, GL_UNSIGNED_INT, 0);
	err = _glw->glGetError();

	if(_lineWidth != 1.0f)
	{
		glDisable(GL_LINE_SMOOTH);
		glLineWidth(1.0f);
	}

	if(_pointSize != 1.0f)
	{
		glDisable(GL_PROGRAM_POINT_SIZE);
		glPointSize(1.0f);
	}

	//if(isDepthTestEnabled())
	//{
	//	_glf->glDisable(GL_DEPTH_TEST);
	//}
}

int Viewer::Rendering::GLRenderObject::sizeOfVertexFormat() const
{
	switch (_format)
	{
	case Viewer::Rendering::VERTEX_FORMAT::V1F:
		return sizeof(float);
	case Viewer::Rendering::VERTEX_FORMAT::V1I:
		return sizeof(int);
	case Viewer::Rendering::VERTEX_FORMAT::V1UI:
		return sizeof(unsigned int);
	case Viewer::Rendering::VERTEX_FORMAT::V2F:
		return sizeof(Geometry::Primitives::vertex_v2);
	case Viewer::Rendering::VERTEX_FORMAT::V3F:
		return sizeof(Geometry::Primitives::vertex_v3);
	case Viewer::Rendering::VERTEX_FORMAT::V4F:
		return sizeof(Geometry::Primitives::vertex_v4);
	case Viewer::Rendering::VERTEX_FORMAT::V2F_C4:
		return sizeof(Geometry::Primitives::vertex_v2c4);
	case Viewer::Rendering::VERTEX_FORMAT::V3F_C4:
		return sizeof(Geometry::Primitives::vertex_v3c4);
	case Viewer::Rendering::VERTEX_FORMAT::V2F_T2:
		return sizeof(Geometry::Primitives::vertex_v2t2);
	case Viewer::Rendering::VERTEX_FORMAT::V3F_T2:
		return sizeof(Geometry::Primitives::vertex_v3t2);
	case Viewer::Rendering::VERTEX_FORMAT::V2F_T2_C4:
		return sizeof(Geometry::Primitives::vertex_v2t2c4);
	case Viewer::Rendering::VERTEX_FORMAT::V3F_T2_C4:
		return sizeof(Geometry::Primitives::vertex_v3t2c4);
	case Viewer::Rendering::VERTEX_FORMAT::V2F_N2F:
		return sizeof(Geometry::Primitives::vertex_v2n2);
	case Viewer::Rendering::VERTEX_FORMAT::V3F_N3F:
		return sizeof(Geometry::Primitives::vertex_v3n3);
	case Viewer::Rendering::VERTEX_FORMAT::V4F_N4F:
		return sizeof(Geometry::Primitives::vertex_v4n4);
	case Viewer::Rendering::VERTEX_FORMAT::V2F_N2F_C4:
		return sizeof(Geometry::Primitives::vertex_v2n2c4);
	case Viewer::Rendering::VERTEX_FORMAT::V3F_N3F_C4:
		return sizeof(Geometry::Primitives::vertex_v3n3c4);
	case Viewer::Rendering::VERTEX_FORMAT::V2F_N2F_T2:
		return sizeof(Geometry::Primitives::vertex_v2n2t2);
	case Viewer::Rendering::VERTEX_FORMAT::V3F_N3F_T2:
		return sizeof(Geometry::Primitives::vertex_v3n3t2);
	case Viewer::Rendering::VERTEX_FORMAT::V2F_N2F_T2_C4:
		return sizeof(Geometry::Primitives::vertex_v2n2t2c4);
	case Viewer::Rendering::VERTEX_FORMAT::V3F_N3F_T2_C4:
		return sizeof(Geometry::Primitives::vertex_v3n3t2c4);
	default:
		return 0;
	}
}

void Viewer::Rendering::GLRenderObject::initVertices( int count )
{
	if(_vertexData != nullptr)
		delete[] _vertexData;

	_vertexData = nullptr;
	_countVertices = count;
	if(count <= 0)
		return;

	switch (_format)
	{
	case Viewer::Rendering::VERTEX_FORMAT::V1F:
		_vertexData = new float[count];
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V1I:
		_vertexData = new int[count];
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V1UI:
		_vertexData = new unsigned int[count];
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V2F:
		_vertexData = new Geometry::Primitives::vertex_v2[count];
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V3F:
		_vertexData = new Geometry::Primitives::vertex_v3[count];
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V4F:
		_vertexData = new Geometry::Primitives::vertex_v4[count];
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V2F_C4:
		_vertexData = new Geometry::Primitives::vertex_v2c4[count];
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V3F_C4:
		_vertexData = new Geometry::Primitives::vertex_v3c4[count];
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V2F_T2:
		_vertexData = new Geometry::Primitives::vertex_v2t2[count];
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V3F_T2:
		_vertexData = new Geometry::Primitives::vertex_v3t2[count];
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V2F_T2_C4:
		_vertexData = new Geometry::Primitives::vertex_v2t2c4[count];
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V3F_T2_C4:
		_vertexData = new Geometry::Primitives::vertex_v3t2c4[count];
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V2F_N2F:
		_vertexData = new Geometry::Primitives::vertex_v2n2[count];
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V3F_N3F:
		_vertexData = new Geometry::Primitives::vertex_v3n3[count];
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V4F_N4F:
		_vertexData = new Geometry::Primitives::vertex_v4n4[count];
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V2F_N2F_C4:
		_vertexData = new Geometry::Primitives::vertex_v2n2c4[count];
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V3F_N3F_C4:
		_vertexData = new Geometry::Primitives::vertex_v3n3c4[count];
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V2F_N2F_T2:
		_vertexData = new Geometry::Primitives::vertex_v2n2t2[count];
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V3F_N3F_T2:
		_vertexData = new Geometry::Primitives::vertex_v3n3t2[count];
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V2F_N2F_T2_C4:
		_vertexData = new Geometry::Primitives::vertex_v2n2t2c4[count];
		break;
	case Viewer::Rendering::VERTEX_FORMAT::V3F_N3F_T2_C4:
		_vertexData = new Geometry::Primitives::vertex_v3n3t2c4[count];
		break;
	default:
		break;
	}
}

void Viewer::Rendering::GLRenderObject::setViewedLayer( int layerNumb, int frameIndex /*= 0*/ )
{
	QMutexLocker locker(&_imageMutex);

	_viewedLayer = layerNumb;
	_viewedFrameIndex = frameIndex;
}

void Viewer::Rendering::GLRenderObject::setMinMax( float min, float max, bool forceValues /*= false*/)
{
	if(!_forceMinMaxValues)
	{
		_minClamp = min;
		_maxClamp = max;
		_forceMinMaxValues = forceValues;
	}
}

void Viewer::Rendering::GLRenderObject::useJetColor( bool use )
{
	_useJetColor = use;
}

void Viewer::Rendering::GLRenderObject::setExternalTextureID( unsigned int textureId )
{
	_textureNameExternal = textureId;
}

void Viewer::Rendering::GLRenderObject::resizeVertexBuffer(VERTEX_FORMAT::Format format, int numberOfVertices, int numberOfIndices /*= -1*/ )
{
	resizeVertexBuffer(format, nullptr, numberOfVertices, numberOfIndices);
}

void Viewer::Rendering::GLRenderObject::resizeVertexBuffer(VERTEX_FORMAT::Format format, const void* vertices, int numberOfVertices, int numberOfIndices /*= -1*/ )
{
	if(!_objectInitialized || numberOfVertices <= 0)
		return;

	if(numberOfIndices <= 0)
	{
		numberOfIndices = numberOfVertices;
		setPrimitiveType(GL_POINTS);
	}

	//QMutexLocker locker(&_mutexUpdateVertexData);

	setVertexData(vertices, numberOfVertices, nullptr, numberOfIndices, format);
	updateVertexData();
	//updateVertexData(nullptr, nullptr);

}

size_t Viewer::Rendering::GLRenderObject::frameCount()
{
	QMutexLocker locker(&_imageMutex);

	if(_currentImage != nullptr)
		return _currentImage->frameIndexSize();

	return 0;
}

size_t Viewer::Rendering::GLRenderObject::layerCount(unsigned int frameIndex /*= 0*/ )
{
	QMutexLocker locker(&_imageMutex);
	
	if(_currentImage != nullptr && _currentImage->frameIndexSize() > frameIndex)
		return _currentImage->layerCount(frameIndex);
	
	return 0;
}

void Viewer::Rendering::GLRenderObject::getCurrentImageMinMaxRange( float &min, float &max, unsigned int frameIndex /*= 0*/ )
{
	QMutexLocker locker(&_imageMutex);

	if(_currentImage != nullptr && _currentImage->frameIndexSize() > frameIndex)
	{
		min = _currentImage->rangeMin(frameIndex);
		max = _currentImage->rangeMax(frameIndex);
	}
	else
	{
		min = 0;
		max = USHRT_MAX;
	}
}

bool Viewer::Rendering::GLRenderObject::setFBOAttachments( std::vector<GLenum> attachments, std::vector<GLuint> textures)
{
	if(attachments.size() != textures.size())
		return false;

	//GLenum colorAttachments[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};

	// Check if we have valid assignments only
	for(size_t i = 0; i < attachments.size(); i++)
	{
		if((attachments[i] < GL_COLOR_ATTACHMENT0 || attachments[i] > GL_COLOR_ATTACHMENT15) && attachments[i] != GL_DEPTH_ATTACHMENT)
			return false;
	}

	for(size_t i = 0; i < textures.size(); i++)
	{
		if(textures[i] <= 0)
			return false;
	}


	_frameBuffer->bind();

	assert(wglGetCurrentContext() != NULL);

	for(int i = GL_COLOR_ATTACHMENT0; i < GL_COLOR_ATTACHMENT15; i++)
	{
		_glw->glActiveTexture((GLenum)(GL_TEXTURE0 + i));
		_glw->glBindTexture(GL_TEXTURE_RECTANGLE, 0);
		_glw->glFramebufferTexture(GL_FRAMEBUFFER, i, 0, 0);
	}

	for(size_t i = 0; i < attachments.size(); i++)
	{
		_glw->glActiveTexture((GLenum)(GL_TEXTURE0 + i));
		_glw->glBindTexture(GL_TEXTURE_RECTANGLE, textures[i]);
		_glw->glFramebufferTexture(GL_FRAMEBUFFER, attachments[i], textures[i], 0);
	}
	_glw->glDrawBuffers((GLsizei)textures.size(), &(*attachments.begin()));			 // defines an array of buffers into which outputs from the fragment shader data will be written.

	GLuint status = _glw->glCheckFramebufferStatus(GL_FRAMEBUFFER);

	_frameBuffer->release();

	return status == GL_FRAMEBUFFER_COMPLETE;

}

bool Viewer::Rendering::GLRenderObject::init2D( const std::vector<Geometry::Primitives::vertex_v2t2>& vertices, const std::vector<unsigned int>& indices, 
											    unsigned int textureId, unsigned int width, unsigned int height )
{
	if(vertices.size() != 4)
		return false;

	if(indices.size() != 6)
		return false;

	setPrimitiveType(GL_TRIANGLES);
	setVertexData((void*)&(*vertices.begin()), (int)vertices.size(), &(*indices.begin()), (int)indices.size(), Viewer::Rendering::VERTEX_FORMAT::V2F_T2);

	if(textureId >= 0)
		setExternalTextureID(textureId);

	return true;
}
