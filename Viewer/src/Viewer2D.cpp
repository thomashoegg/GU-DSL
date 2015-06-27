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


#define _USE_MATH_DEFINES
#include <math.h>

#include "Viewer2D.qt.h"

#include "Rendering/vertexformat.h"
#include "Rendering/GLRenderObject.h"
#include "Rendering/GLPipeline.h"

#include <algorithm> 
#include <limits>

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLShader>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>

#include <QtGui/QtEvents>

#include "Images/Image.h"

#ifdef DEG2RAD
#undef DEG2RAD
#endif

#define DEG2RAD(a) (((a)/180.0)*M_PI)


static unsigned int const ElementData[] =
{
	0, 1, 2, 
	2, 3, 0
};


Geometry::Primitives::vertex_v2t2 const VertexData[] =
{
	Geometry::Primitives::vertex_v2t2(Geometry::Primitives::vec2(0.0f, 0.0f), Geometry::Primitives::vec2(0.0f,	480.0f)),
	Geometry::Primitives::vertex_v2t2(Geometry::Primitives::vec2(0.0f, 480.0f), Geometry::Primitives::vec2(0.0f,	0.0f)),
	Geometry::Primitives::vertex_v2t2(Geometry::Primitives::vec2(640.0f, 480.0f), Geometry::Primitives::vec2(640.0f,	0.0f)),
	Geometry::Primitives::vertex_v2t2(Geometry::Primitives::vec2(640.0f, 0.0f), Geometry::Primitives::vec2(640.0,	480.0f))
};

Viewer::Viewer2D::Viewer2D(QWindow *parent)
    : GLWindow(parent),
	_renderOrder(horizontal),
	_nHorizontal(0),
	_nVertical(0),
	_fRotation(0),
	_mirrorX(false),
	_mirrorY(false),
	_mutexTextureUpdate(QMutex::Recursive)
{
 
}

Viewer::Viewer2D::~Viewer2D()
{
  
}

Viewer::Rendering::GLRenderObjectPtr Viewer::Viewer2D::getRenderObjectFromMousePosition( int x, int y )
{
	int objectX = x;
	int objectY = height() - y;

	for(TextureBoundingMap::iterator itBounding = _textureBoundingMap.begin();
		itBounding != _textureBoundingMap.end(); 
		itBounding++)
	{
		if(_aTexture[itBounding->first].x1 <= objectX && _aTexture[itBounding->first].x2 >= objectX &&
		   _aTexture[itBounding->first].y1 <= objectY && _aTexture[itBounding->first].y2 >= objectY)
		{
			Viewer::Rendering::GLRenderObjectPtr obj = getRenderObject(itBounding->first);
			if(!obj || !obj->isUseCamera())
				continue;
			
			return obj;
		}
	}

	return Viewer::Rendering::GLRenderObjectPtr();
}


void Viewer::Viewer2D::render(QPainter *painter)
{
    //Q_UNUSED(painter);

	if(_needRecalc)
	{
		recalcNumbImgFields();
		_needRecalc = false;
	}

	_viewMatrix.setToIdentity();
	_projectionMatrix.setToIdentity();
	_projectionMatrix.ortho(0, width(), 0, height(), -1.0f, 1.0f);

	{
		QMutexLocker locker(&_mutexTextureUpdate);

		unsigned char cH = 0, cV = 0;

		for(int i = 0; i < _renderObjects.size(); i++)
		{
			setImage(i, _renderObjects[i]->textureId(), -1, _renderObjects[i]->currentImageWidth(),  _renderObjects[i]->currentImageHeight(), false);	
		}

		for(TextureBoundingMap::iterator itBounding = _textureBoundingMap.begin();
			itBounding != _textureBoundingMap.end(); 
			itBounding++)
		{
			Texture texture = _aTexture[itBounding->first];
			int nWidth = itBounding->second.width();
			int nHeight = itBounding->second.height();
			int textureWidth = texture.width;
			int textureHeight = texture.height;

			if(nHeight == 0 || nWidth == 0)
				continue;

			// calculate the aspect ratio
			float ratioImage = (float) nHeight/nWidth;
			float ratioFrame = (float) (height()*_nHorizontal)/(width()*_nVertical);
			float ratioIx = (float)textureWidth/(float)nWidth;
			float ratioIy = -(float)textureHeight/(float)nHeight;

			if(mirrorX())
				ratioIy *= -1;

			if(mirrorY())
				ratioIx *= -1;

			// draw quad
			// texture coords are flipped as OpenGL normally expects bottom-up images 
			// and video frames are defined top-down

			float x1 = 0;
			float x  = 0;
			float y  = 0;
			float y1 = 0;

			if (ratioFrame > ratioImage)
			{
				x1 = (0.5f/_nHorizontal)*width();
				x  = ((float) cH/_nHorizontal)*width() + x1;
				y  = ((float) ((_nVertical-1) - cV + 0.5f)/_nVertical)*height();
				y1 = (0.5f*ratioImage/(ratioFrame*_nVertical))*height();
			}
			else 
			{
				x  = ((float) (cH + 0.5f)/_nHorizontal)*width();
				x1 = (0.5f*ratioFrame/(ratioImage*_nHorizontal))*width();
				y1 = (0.5f/_nVertical)*height();
				y  = (((float) (_nVertical-1 - cV)/_nVertical))*height() + y1;
			}

			_aTexture[itBounding->first].x1 = x-x1;
			_aTexture[itBounding->first].y1 = y-y1;
			_aTexture[itBounding->first].x2 = x+x1;
			_aTexture[itBounding->first].y2 = y+y1;

			Viewer::Rendering::GLRenderObjectPtr obj = getRenderObject(itBounding->first);
			if(!obj || !obj->isEnabled())
				continue;

			obj->setPrimitiveType(GL_TRIANGLES);

			//obj->setViewport(-x1 + x, y - y1, 2*abs(x1), 2*abs(y1));

			QMatrix4x4 modelMat;
			modelMat.setToIdentity();

			modelMat.translate(x, y, 0);
			modelMat.rotate(rotation(), 0.0f, 0.0f, 1.0f);
			modelMat.scale(ratioIx, ratioIy, 1.0f);

			obj->setModelMatrix(modelMat);

			Geometry::Primitives::vertex_v2t2 const VertexData[] =
			{
				Geometry::Primitives::vertex_v2t2(Geometry::Primitives::vec2(-x1, y1),	Geometry::Primitives::vec2(0, textureHeight)),
				Geometry::Primitives::vertex_v2t2(Geometry::Primitives::vec2(x1, y1),		Geometry::Primitives::vec2(textureWidth, textureHeight)),
				Geometry::Primitives::vertex_v2t2(Geometry::Primitives::vec2(x1, -y1),	Geometry::Primitives::vec2(textureWidth, 0)),
				Geometry::Primitives::vertex_v2t2(Geometry::Primitives::vec2(-x1, -y1),	Geometry::Primitives::vec2(0, 0))
			};

			obj->setVertexData((void*)VertexData, 4, ElementData, 6, Viewer::Rendering::VERTEX_FORMAT::V2F_T2);

			// step field counter
			if (_renderOrder == horizontal) 
			{
				if (++cH == _nHorizontal)
				{
					cV++; cH = 0;
				}
			}
			else 
			{
				if (++cV == _nVertical)
				{
					cH++; cV = 0;
				}
			}		
		}
	}
	GLWindow::render(painter);
}

void Viewer::Viewer2D::initialize()
{
	GLWindow::initialize();

	//// Lock the mutex to avoid collisions
	//QMutexLocker locker(&_mutexUpdateVertexData);

	//_renderObjects[0]->setVertexData((void*)VertexData, 4, ElementData, 6, Geometry::Primitives::VERTEX_FORMAT::V2F_T2);
}


void Viewer::Viewer2D::resizeEvent( QResizeEvent *event )
{
	GLWindow::resizeEvent(event);

	_needRecalc = true;
	update();
	//recalcNumbImgFields();
}


/// MultiTexture-Handling


/**
* Adds a new image to the display list.
*
* @param iIn      image ID (input channel)
* @param ID       texture ID
* @param width    image width
* @param height   image height
*/
void Viewer::Viewer2D::setImage(unsigned char iIn, GLuint ID, GLuint alphaID, unsigned int width, 
						   unsigned int height, bool redraw/* = true*/)
{

	QMutexLocker locker(&_mutexTextureUpdate);
	if (ID == 0) return;

	if (_aTexture.size() >= 255u) 
		return;

	// add new texture container
	TextureMap::iterator iter = _aTexture.find(iIn);

	if (iter != _aTexture.end()) 
	{
		updateImage(iIn, ID, alphaID, width, height, 0, redraw);
	//	iter->second = Texture(ID, alphaID, width, height);
	}
	else
		_aTexture.insert(TextureMapPair(iIn, Texture(ID, alphaID, width, height)));

	// update images fields
	if (_aTexture.size() != _nHorizontal*_nVertical)
		_needRecalc = true;
	//recalcNumbImgFields();

	if(redraw)
		QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
}


/**
* Update image.
*
* @param iIn      image ID (input channel)
* @param ID       texture ID
* @param width    image width
* @param height   image height
*/
void Viewer::Viewer2D::updateImage(unsigned char iIn, GLuint ID, GLuint alphaID, unsigned int width, unsigned int height, unsigned long intTime,  bool redraw /*= true*/)
{
	QMutexLocker locker(&_mutexTextureUpdate);
	// find texture container
	TextureMap::iterator iter = _aTexture.find(iIn);

	if (iter == _aTexture.end())
		return;

	bool needRecalcFields = false;

	if(iter->second.width != width || iter->second.height != height)
		needRecalcFields = true;

	// update data
	iter->second.ID			= ID;
	iter->second.alphaID	= alphaID;
	iter->second.width		= width;
	iter->second.height		= height;
	iter->second.intTime	= intTime;

	if(needRecalcFields)
		_needRecalc = true;
		//recalcNumbImgFields();

	if(redraw)
		QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
}

/**
* Remove an image from the display list.
*
* @param ID   texture ID
*/
void Viewer::Viewer2D::removeImage(unsigned char iIn)
{
	QMutexLocker locker(&_mutexTextureUpdate);

	// remove texture container
	_aTexture.erase(iIn);

	// update number of images fields
	if (_aTexture.size() != _nHorizontal + _nVertical)
		_needRecalc = true;
		//recalcNumbImgFields();

	QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
}


/**
* Set render order.
*/
void Viewer::Viewer2D::setRenderOrder(Viewer::Viewer2D::RenderOrder value) 
{ 
	_renderOrder = value; 
};


/** Set number of image fields. **/
void Viewer::Viewer2D::setNumbImgFields(unsigned char horz, unsigned char vert)
{

	if (horz + vert < (int) _aTexture.size()) 
	{
		//D_OUT2(__FUNCTION__, "Insufficient number of image fields specified.");
		return;
	}

	_nHorizontal = horz; 
	_nVertical   = vert; 

	QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
};


/**
* Determine number of images fields.
*/
void Viewer::Viewer2D::recalcNumbImgFields(void)
{
	QMutexLocker locker(&_mutexTextureUpdate);

	calculateBoundingRectangles();

	size_t textureCount = 0;
	
	for(TextureBoundingMap::iterator itBounding = _textureBoundingMap.begin(); 
		itBounding != _textureBoundingMap.end(); 
		itBounding++)
	{
		Viewer::Rendering::GLRenderObjectPtr obj = getRenderObject(itBounding->first);
		if(!obj || !obj->isEnabled())
			continue;

		textureCount++;
	}

	float last, current; // area size

	if (_renderOrder == horizontal) 
	{
		// set initial guess
		float horz = textureCount;
		unsigned char rHorz, vert = 1;

		// improve iteratively
		current = calcFieldArea((unsigned char) horz, vert);

		do
		{
			last  = current;

			horz  = 0.5f*ceil(horz);
			rHorz = (unsigned char) ceil(horz);

			while (unsigned int(vert*rHorz) < (unsigned int) textureCount)
				vert++; 

			current = calcFieldArea(rHorz, vert);

		} while (current > last);

		// re-adjust
		_nHorizontal = (unsigned char) 2.0f*horz;
		_nVertical   = (unsigned char) ceil((float) textureCount/_nHorizontal);
	}
	else
	{
		// set initial guess
		float vert = textureCount;
		unsigned char rVert, horz = 1;

		// improve iteratively
		current = calcFieldArea(horz, vert);

		do
		{
			last = current;

			vert  = 0.5f*ceil(vert);
			rVert = (unsigned char) ceil(vert);

			while (unsigned int(rVert*horz) < textureCount)
				horz++; 

			current = calcFieldArea(horz, rVert);

		} while (current > last);

		// re-adjust
		_nVertical   = (unsigned char) 2.0f*vert;
		_nHorizontal = (unsigned char) ceil((float) textureCount/_nVertical);
	}
}


/**
* Calculate area covered by image fields.
*
* @return
*  size of covered area 0 <= x <= 1
*/
float Viewer::Viewer2D::calcFieldArea(unsigned char horz, unsigned char vert) 
{
	QMutexLocker locker(&_mutexTextureUpdate);

	float sum = 0;
	float ratioFrame = (float) (height()*horz)/(width()*vert);

	for(TextureBoundingMap::iterator itBounding = _textureBoundingMap.begin(); 
		itBounding != _textureBoundingMap.end(); 
		itBounding++)
	{
		Viewer::Rendering::GLRenderObjectPtr obj = getRenderObject(itBounding->first);
		if(!obj || !obj->isEnabled())
			continue;

		float ratioImage = (float) itBounding->second.height()/itBounding->second.width();

		if (ratioImage < ratioFrame)
			sum += ratioImage/(ratioFrame*vert*horz);
		else
			sum += ratioFrame/(ratioImage*vert*horz);
	}

	return sum;
}

/*!
* \brief
* Recalculates the bounding rectangles of all given valid textures
*
* \return
*/
void Viewer::Viewer2D::calculateBoundingRectangles() 
{
	QMutexLocker locker(&_mutexTextureUpdate);

	double rotation = DEG2RAD(_fRotation);

	QMatrix rotationMat(cos(rotation), -sin(rotation), sin(rotation), cos(rotation), 0, 0);

	_textureBoundingMap.clear();

	TextureMap::const_iterator texture;
	for (texture = _aTexture.begin(); texture != _aTexture.end(); texture++) 
	{
		if (texture->second.width == 0 || texture->second.height == 0)
			continue;

		Geometry::BoundingBoxes::BoundingRect2Dd boundingRect;

		QPoint vec2(0, texture->second.height);
		vec2 = vec2 * rotationMat;

		boundingRect.addPoint(vec2.x(), vec2.y());

		vec2 = QPoint(texture->second.width, texture->second.height);
		vec2 = vec2 * rotationMat;

		boundingRect.addPoint(vec2.x(), vec2.y());

		vec2 = QPoint(texture->second.width, 0);
		vec2 = vec2 * rotationMat;

		boundingRect.addPoint(vec2.x(), vec2.y());

		_textureBoundingMap.insert(TextureBoundingMapPair(texture->first, boundingRect));
	}
}

int Viewer::Viewer2D::addImageObject(unsigned int textureId, unsigned int witdh, unsigned int height)
{
	int renderObjectIndex = GLWindow::addRenderObject(witdh, height);

	if(renderObjectIndex >= 0)
	{
		QMutexLocker locker(&_mutexTextureUpdate);

		Viewer::Rendering::GLRenderObjectPtr renderObject = getRenderObject(renderObjectIndex);
		
		renderObject->setPrimitiveType(GL_TRIANGLES);
		renderObject->setVertexData((void*)VertexData, 4, ElementData, 6, Viewer::Rendering::VERTEX_FORMAT::V2F_T2);

		if(textureId >= 0)
			renderObject->setExternalTextureID(textureId);

		setImage(renderObjectIndex, renderObject->textureId(), -1, witdh, height);
	}
	
	return renderObjectIndex;
}

bool Viewer::Viewer2D::removeImageObject(unsigned int textureId)
{
	QMutexLocker locker(&_mutexTextureUpdate);

	// find texture container
	TextureMap::iterator iter = _aTexture.end();
	int index = -1;
	TextureMap::const_iterator texture;
	for (texture = _aTexture.begin(); texture != _aTexture.end(); texture++, index) 
	{
		if(texture->second.ID == textureId)
			break;
	}

	if (iter == _aTexture.end())
		return false;

	bool removed = GLWindow::removeRenderObject(index);

	if(removed)
	{
		removeImage(index);
	}

	return removed;
}


/**
* Set the mirror X flag.
*
* @param value   flag
*/
void Viewer::Viewer2D::setMirrorX(bool value)
{
	_mirrorX = value;
	_needRecalc = true;
	update();
	//recalcNumbImgFields();
}


/** Get the mirror X flag. **/
bool Viewer::Viewer2D::mirrorX(void) const
{
	return _mirrorX;
}


/**
* Set the mirror Y flag.
*
* @param value   flag
*/
void Viewer::Viewer2D::setMirrorY(bool value)
{
	_mirrorY = value;
	_needRecalc = true;
	update();
	//recalcNumbImgFields();
}


/** Get the mirror Y flag. **/
bool Viewer::Viewer2D::mirrorY(void) const
{
	return _mirrorY;
}


/**
* Set the rotation of the single textures in the viewer. 
*
* @param value   rotation
*/
void Viewer::Viewer2D::setRotation(float value)
{
	_fRotation = value;
	_needRecalc = true;
	update();
	//recalcNumbImgFields();
}


/** Get the rotation. **/
float Viewer::Viewer2D::rotation(void) const
{
	return _fRotation;
}



void Viewer::Viewer2D::setImage(Container::Images::Image image, unsigned int index /* = 0*/, bool addMissingImage /*= false)*/)
{
	_imageMutex.lock();

	addDefaultObject();

	if(image.width() == 2 && image.pixelData() == Container::Images::PixelData::POINT)
	{
		Container::Images::Image image2D;

		int maxRange = image.rangeMax()*100;

		image2D.setDataType(Container::Images::DataType::FLOAT);
		image2D.setLayerCount(1);
		image2D.setPixelData(Container::Images::PixelData::GRAY);
		image2D.setCoordinateType(Container::Images::CoordinateType::CARTESIAN);
		image2D.setWidth(maxRange);	
		image2D.setHeight(maxRange);	
		float* buffer2D = new float[maxRange*maxRange];
		float* buffer = (float*)image.buffer();

		std::fill_n(buffer2D, image2D.width()*image2D.height(), 0);

		for( int i = 0; i < image.height(); i++)
		{
			if(abs(buffer[2*i]) >= maxRange/2 || abs(buffer[2*i+1]) >= maxRange/2)
				continue;

			buffer2D[((int)buffer[2*i]+maxRange/2)+(-(int)buffer[2*i+1]+maxRange/2)*maxRange] = 1;


		}


		image2D.setBuffer(buffer2D, image2D.width()*image2D.height()*sizeof(float));
		//_renderWindow->setImage(newImage);

		image = image2D;

		delete[] buffer2D;
		//delete buffer;
	}

//	(*_currentFrame) = frame;

	if(index >= 0 && index < _renderObjects.size())
	{
		unsigned int viewedFrameIndex = _renderObjects[index]->viewedFrameIndex();

		if(viewedFrameIndex >= (int)image.frameIndexSize() || viewedFrameIndex < 0)
			viewedFrameIndex = 0;

		setImage(index, _renderObjects[index]->textureId(), 0, image.width(viewedFrameIndex), image.height(viewedFrameIndex));
		_renderObjects[index]->setImage(image);
	}

	while(addMissingImage && index >= _renderObjects.size())
		addImageObject(0, image.width(), image.height());

	_imageMutex.unlock();

	QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
}


void Viewer::Viewer2D::setImage(const float* data, int width, int height, unsigned int index /* = 0*/, bool addMissingImage/* = false*/)
{
	addDefaultObject();

	Container::Images::Image newImage;
	newImage.setWidth(width);
	newImage.setHeight(height);

	newImage.setBuffer((char*)(data), width * height * sizeof(float));	

	_imageMutex.lock();

	while(addMissingImage && index >= _renderObjects.size())
		addImageObject(0, newImage.width(), newImage.height());

	if(index >= 0 && index < _renderObjects.size())
	{
		setImage(index, _renderObjects[index]->textureId(), -1, newImage.width(), newImage.height());
		_renderObjects[index]->setImage(newImage);
	}

	_imageMutex.unlock();

	QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
}


void Viewer::Viewer2D::setImage( void* data, size_t bufferSize, size_t sizeOfElement, int width, int height, Container::Images::DataType dataType /*= 0*/, unsigned int index /*= 0*/, bool addMissingImage/* = false*/, int pixelType /*= -1*/)
{
	setImage(data, bufferSize, sizeOfElement, width, height, (int)dataType, index, addMissingImage, pixelType);
}


void Viewer::Viewer2D::setImage( void* data, size_t bufferSize, size_t sizeOfElement, int width, int height, Container::Images::DataType dataType /*= 0*/, unsigned int index /*= 0*/, bool addMissingImage/* = false*/, Container::Images::PixelData pixelType)
{
	setImage(data, bufferSize, sizeOfElement, width, height, (int)dataType, index, addMissingImage, static_cast<int>(pixelType));
}

void Viewer::Viewer2D::setImage( void* data, size_t bufferSize, size_t sizeOfElement, int width, int height, int dataType /*= 0*/, unsigned int index /*= 0*/, bool addMissingImage/* = false*/, int pixelType /*= -1*/)
{
	addDefaultObject();
	Container::Images::Image newImage;
	newImage.setWidth(width);
	newImage.setHeight(height);
	newImage.setDataType((Container::Images::DataType)dataType);
	::size_t dataSize = 0;
	
	switch (dataType)
	{
	case Container::Images::DataType::SHORT:
	case Container::Images::DataType::USHORT:
		dataSize = 2;
		break;
	case Container::Images::DataType::INT:
	case Container::Images::DataType::UINT:
	case Container::Images::DataType::FLOAT:
		dataSize = 4;
		break;
	case Container::Images::DataType::DOUBLE:
		dataSize = 8;
		break;
	case Container::Images::DataType::UNDEFINED:
	case Container::Images::DataType::CHAR:
	case Container::Images::DataType::UCHAR:
	default:
		dataSize = 1;
		break;
	}

	switch(pixelType)
	{
	case Container::Images::PixelData::UNDEFINED:
	case Container::Images::PixelData::PIXEL:
	case Container::Images::PixelData::POINT:
	case Container::Images::PixelData::COLOR:
	case Container::Images::PixelData::GRAY:
		newImage.setPixelData((Container::Images::PixelData)pixelType);
		break;

	}

	if(pixelType >= (int)Container::Images::PixelData::USER)
	{
			newImage.setPixelData((Container::Images::PixelData)pixelType);
	}

	if(sizeOfElement > 0)
	{
		dataSize = sizeOfElement;
	}

	newImage.setBuffer((char*)data, (int)bufferSize);

	_imageMutex.lock();

	while(addMissingImage && index >= _renderObjects.size())
		addImageObject(0, newImage.width(), newImage.height());

	if(index >= 0 && index < _renderObjects.size())
	{
		setImage(index, _renderObjects[index]->textureId(), -1, newImage.width(), newImage.height());
		_renderObjects[index]->setImage(newImage);
	}

	_imageMutex.unlock();

	QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
}

void Viewer::Viewer2D::setImage(QImage image, unsigned int index /*= 0*/, bool addMissingImage /*= false*/)
{
	addDefaultObject();

	Container::Images::Image newImage;
	newImage.setWidth(image.width());
	newImage.setHeight(image.height());
	newImage.setDataType(Container::Images::DataType::UCHAR);
	newImage.setPixelData(Container::Images::PixelData::COLOR);

	QImage convertedImage = image.convertToFormat(QImage::Format_ARGB32);

	char* buffer = new char[convertedImage.width() * convertedImage.height() * 4];
	for(int y = 0; y < convertedImage.height(); y++)
	{
		char* scanline = (char*)convertedImage.scanLine(y);

		char* lineAdr = buffer + y * (4 * sizeof(char) * convertedImage.width());

		memcpy(lineAdr, scanline,  (4 * sizeof(char) * convertedImage.width()));
	}


	newImage.setBuffer(buffer, convertedImage.width() * convertedImage.height() * 4);	

	_imageMutex.lock();

	while(addMissingImage && index >= _renderObjects.size())
		addImageObject(0, newImage.width(), newImage.height());

	if(index >= 0 && index < _renderObjects.size())
	{
		setImage(index, _renderObjects[index]->textureId(), -1, newImage.width(), newImage.height());
		_renderObjects[index]->setImage(newImage);
	}

	_imageMutex.unlock();

	QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
}

void Viewer::Viewer2D::rotate90( bool ccw )
{
	if(ccw) _fRotation = _fRotation + 90;
	else _fRotation = _fRotation - 90;

	while(_fRotation < 0)
		_fRotation += 360;

	_fRotation = fmod(_fRotation, 360);

	_needRecalc = true;
	update();
	//recalcNumbImgFields();
}

void Viewer::Viewer2D::resetView()
{
	_mirrorX = false;
	_mirrorY = false;
	_fRotation = 0.0f;

	_needRecalc = true;
	update();
	//recalcNumbImgFields();
}

size_t Viewer::Viewer2D::frameCount()
{
	size_t frameCount = 0;
	for(int i = 0; i < _renderObjects.size(); i++)
	{
		Viewer::Rendering::GLRenderObjectPtr renderObject = _renderObjects[i];
		if(!renderObject)
			continue;

		frameCount = std::max(frameCount, renderObject->frameCount());
	}

	return frameCount;
}

size_t Viewer::Viewer2D::layerCount(unsigned int frameIndex /*= 0 */)
{
	size_t layerCount = 0;
	for(int i = 0; i < _renderObjects.size(); i++)
	{
		Viewer::Rendering::GLRenderObjectPtr renderObject = _renderObjects[i];
		if(!renderObject)
			continue;

		layerCount = std::max(layerCount, renderObject->layerCount(frameIndex));
	}

	return layerCount;
}

void Viewer::Viewer2D::getCurrentImageMinMaxRange( float &minVal, float &maxVal, unsigned int frameIndex /*= 0*/)
{
	// check if the min max range is manually set
	if(_minRange >= 0 && _maxRange >= 0)
	{
		minVal = _minRange;
		maxVal = _maxRange;

		return;
	}


	minVal = USHRT_MAX;
	maxVal = -USHRT_MAX;

	bool minMaxSet = false;


	for(int i = 0; i < _renderObjects.size(); i++)
	{
		Viewer::Rendering::GLRenderObjectPtr renderObject = _renderObjects[i];
		if(!renderObject)
			continue;

		float localMin;
		float localMax;

		renderObject->getCurrentImageMinMaxRange(localMin, localMax, frameIndex);

		minVal = std::min(0.0f, localMin);
		maxVal = std::max(0.0f, localMax);

		minMaxSet = true;
	}

	if(!minMaxSet)
	{
		minVal = 0;
		maxVal = USHRT_MAX;
	}

}

void Viewer::Viewer2D::addDefaultObject()
{
	if(_renderObjects.size() <= 0)
	{
		addImageObject(0, 200, 200);	
		_renderObjects[0]->setFlag(GLRENDEROBJECT_FLAG_ISDEFAULT);
	}
}

void Viewer::Viewer2D::mousePressEvent( QMouseEvent *m )
{
	_currentRenderObject = getRenderObjectFromMousePosition(m->x(), m->y());
	if(_currentRenderObject)
	{
		_currentRenderObject->mousePressEvent(m);

		emit onMouseEvent(m);

		update();
	}
}

void Viewer::Viewer2D::mouseDoubleClickEvent( QMouseEvent *m )
{
	_currentRenderObject = getRenderObjectFromMousePosition(m->x(), m->y());
	if(_currentRenderObject)
	{
		_currentRenderObject->mouseDoubleClickEvent(m);

		emit onMouseEvent(m);

		update();
	}
}

void Viewer::Viewer2D::mouseMoveEvent( QMouseEvent *m )
{
	if(_currentRenderObject)
	{
		_currentRenderObject->mouseMoveEvent(m);

		emit onMouseEvent(m);

		update();
	}
}

void Viewer::Viewer2D::wheelEvent( QWheelEvent *m )
{
	if(_currentRenderObject)
	{
		_currentRenderObject->wheelEvent(m);

		update();
	}
}

void Viewer::Viewer2D::keyPressEvent( QKeyEvent *m )
{
	if(_currentRenderObject)
	{
		_currentRenderObject->keyPressEvent(m);

		update();
	}
}
