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


#include "Images/Image.h"

#include <fstream>
#include <sstream>
#include <QFile>
#include <QTextStream>

#define MAX_FRAME_COUNT		100
#include <QtCore/QFileInfo>
#include <QtGui/QImage>
#include <QDateTime>
#include <assert.h>
#include <set>

namespace Container
{
namespace Images
{

Image::Image()
{
	_frame = new Frame();
	_frame->header.push_back(FrameHeader());
	_frame->data.push_back(FrameData());

	_isOwner = true;
}

Image::Image(Frame* frame, bool copy /*= true*/)
{

	if(copy)
	{
		_frame = new Frame(*frame);
		_isOwner = true;
	}
	else
	{
		if(frame != nullptr)
		{
			_frame = frame;
			_isOwner = false;
		}
		else
		{
			frame = new Frame(*frame);
			_isOwner = true;	
		}
	}
	
}

Image::Image(unsigned int frameSize)
{
	_frame = new Frame();
	for(unsigned int i = 0; i < frameSize; i++)
	{
		_frame->header.push_back(FrameHeader());
		_frame->data.push_back(FrameData());
	}

	_isOwner = true;
}

Image::Image(FrameData* data, FrameHeader* header = nullptr)
{
	_frame = new Frame();
	_frame->header.push_back(FrameHeader());
	_frame->data.push_back(FrameData());


	if(data != nullptr)
		_frame->data[0] = *data;

	if(header != nullptr)
		_frame->header[0] = *header;

	_isOwner = true;	
}
//
//Image::Image( const Image* rhs )
//{
//	_frame = new Container::Images::Frame(*rhs->_frame);
//	_isOwner = true;
//}
//
Image::Image( const Image& rhs )
{
	assert(rhs._frame != nullptr && "Image contains no valid Frame");
	if(rhs._frame != nullptr)
	{
		_frame = new Frame(*rhs._frame);
	}
	else
	{
		_frame = new Frame();
	}
	_isOwner = true;
}

Image::Image( const QImage& rhs )
{
	loadBitmapInternal(rhs);
}


Image& Image::operator=( const Image& rhs )
{
	if(_isOwner && _frame!=nullptr)
		delete _frame;
	
	//maybe better to use the same frame?
	_frame = new Frame(*rhs._frame);
	_isOwner = true;
	return *this;
}


Image::~Image()
{
	if(_isOwner && _frame!=nullptr)
		delete _frame;
}

int Image::getDataTypeSize( unsigned int frameIndex /*= 0*/ ) const
{
	switch (_frame->header[frameIndex].dataType)
	{
	case DataType::CHAR:
		return sizeof(char);
	case DataType::UCHAR:
		return sizeof(unsigned char);
	case DataType::SHORT:
		return sizeof(short);
	case DataType::USHORT:
		return sizeof(unsigned short);
	case DataType::INT:
		return sizeof(int);
	case DataType::UINT:
		return sizeof(unsigned int);
	case DataType::FLOAT:
		return sizeof(float);
	case DataType::DOUBLE:
		return sizeof(double);
	default:
		return 0;
	};
}

void Image::clamp( float minDepth, float maxDepth, unsigned int frameIndex /*= 0*/ )
{
	if(!_frame) return;

	int dataTypeByteSize = getDataTypeSize(frameIndex);

	//char* pNewImageDataBuffer = allocEqualBuffer();

	char* pImageDataBufferPos = (char*)_frame->data[frameIndex].buffer.c_str();
	const char* pImageDataBufferEnd = (char*)pImageDataBufferPos + (dataTypeByteSize * _frame->header[frameIndex].width * _frame->header[frameIndex].height);

	while(pImageDataBufferPos != pImageDataBufferEnd)
	{
		switch (_frame->header[frameIndex].dataType)
		{
		case DataType::CHAR:
			{
				char val = *((char*)pImageDataBufferPos);
				if(val < minDepth || val > maxDepth)
					val = 0;

				*((char*)pImageDataBufferPos) = val;
				break;
			}
		case DataType::UCHAR:
			{
				unsigned char val = *((unsigned char*)pImageDataBufferPos);
				if(val < minDepth || val > maxDepth)
					val = 0;

				*((unsigned char*)pImageDataBufferPos) = val;
				break;
			}
		case DataType::SHORT:
			{
				short val = *((short*)pImageDataBufferPos);
				if(val < minDepth || val > maxDepth)
					val = 0;

				*((short*)pImageDataBufferPos) = val;
				break;
			}
		case DataType::USHORT:
			{
				unsigned short val = *((unsigned short*)pImageDataBufferPos);
				if(val < minDepth || val > maxDepth)
					val = 0;

				*((unsigned short*)pImageDataBufferPos) = val;
				break;
			}
		case DataType::INT:
			{
				int val = *((int*)pImageDataBufferPos);
				if(val < minDepth || val > maxDepth)
					val = 0;

				*((int*)pImageDataBufferPos) = val;
				break;
			}
		case DataType::UINT:
			{
				unsigned int val = *((unsigned int*)pImageDataBufferPos);
				if(val < minDepth || val > maxDepth)
					val = 0;

				*((unsigned int*)pImageDataBufferPos) = val;
				break;
			}
		case DataType::FLOAT:
			{
				float val = *((float*)pImageDataBufferPos);
				if(val < minDepth || val > maxDepth)
					val = 0;

				*((float*)pImageDataBufferPos) = val;
				break;
			}
		case DataType::DOUBLE:
			{
				double val = *((double*)pImageDataBufferPos);
				if(val < minDepth || val > maxDepth)
					val = 0;

				*((double*)pImageDataBufferPos) = val;
				break;
			}
		};

		pImageDataBufferPos = (char*)pImageDataBufferPos + dataTypeByteSize;
	}

	//_frame->buffer = std::string((char*)pImageDataBufferPos, _frame->width*_frame->height*dataTypeByteSize);

}

void Image::setBuffer( char* buffer, size_t size, unsigned int frameIndex /*= 0*/ )
{
	if(buffer == nullptr)
		_frame->data[frameIndex].buffer = std::string("");
	else _frame->data[frameIndex].buffer = std::string((char*)buffer, size);
}

void Image::setBuffer( void* buffer, size_t size, unsigned int frameIndex /*= 0*/ )
{
	setBuffer((char*)buffer, size, frameIndex);
}

void Image::setMetadata( char* metadata, size_t size, unsigned int frameIndex /*= 0*/ )
{
	if(metadata == nullptr)
		_frame->data[frameIndex].metadata = std::string("");
	else _frame->data[frameIndex].metadata = std::string((char*)metadata, size);
}

char* Image::layer( int index /*= 0*/, unsigned int frameIndex /*= 0*/ ) const
{
	if(index < 0 || index > _frame->header[frameIndex].layerCount)
		return nullptr;

	char* buf = buffer(frameIndex);

	return (char*)buf + (index * layerSize(frameIndex));
}

const char* Image::getLayer( int index /*= 0*/, unsigned int frameIndex /*= 0*/ )
{
	return layer(index,frameIndex);
}

size_t Image::layerSize(unsigned int frameIndex /*= 0*/) const
{
	return _frame->header[frameIndex].width * _frame->header[frameIndex].height * getDataTypeSize(frameIndex);
}

bool Image::save( std::string filename, unsigned int frameIndex /*= 0*/, int layerNumber /*= 0*/)
{
	QFileInfo fileinfo(filename.c_str());
	QString ext = fileinfo.suffix();

	// Special handling for typical image formats
	if(ext == "png" || ext == "bmp" || ext == "jpg" )
		return saveBitmap(filename, frameIndex, layerNumber);

	QFile file(filename.c_str());
	if(file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		std::ofstream file;
		if (file.fail()) 
			return false;

		for(unsigned int i = 0; i < _frame->header.size(); i++)
		{
			int32_t width = this->width(i);
			int32_t height = this->height(i);
			int32_t layerCount = this->layerCount(i);
			int32_t size = this->size(i);
			int32_t metaSize = (int32_t)this->_frame->data[i].metadata.length();
			float minVal = this->rangeMin(i);
			float maxVal = this->rangeMax(i);
			PixelData pixelData = this->pixelData(i);
			DataType dataType = this->dataType(i);
			CoordinateType coordinateType = this->coordinateType(i);
			ImageType imageType = this->imageType(i);

			file.write((char*) &width, sizeof(int32_t));
			file.write((char*) &height, sizeof(int32_t));
			file.write((char*) &layerCount, sizeof(int32_t));
			file.write((char*) &minVal, sizeof(float));
			file.write((char*) &maxVal, sizeof(float));
			file.write((char*) &size, sizeof(int32_t));
			file.write((char*) &metaSize, sizeof(int32_t));
			file.write((char*) &pixelData, sizeof(PixelData));
			file.write((char*) &dataType, sizeof(DataType));
			file.write((char*) &coordinateType, sizeof(CoordinateType));
			file.write((char*) &imageType, sizeof(ImageType));

			int64_t timestamp = this->timestamp(i);
			int32_t index = this->index(i);
			std::string buffer = this->_frame->data[i].buffer;
			std::string metadata = this->_frame->data[i].metadata;

			file.write((char*) &timestamp, sizeof(int64_t));
			file.write((char*) &index, sizeof(index));
			file.write((char*) &buffer, buffer.length());
			file.write((char*) &metadata, metadata.length());
		}
		file.close();

		return file.good();
	}
	else
		return false;
}

bool Image::load( std::string filename )
{
	QFileInfo fileinfo(filename.c_str());
	QString ext = fileinfo.suffix();

	// Special handling for typical image formats
	if(ext == "png" || ext == "bmp" || ext == "jpg" )
		return loadBitmap(filename);

	QFile file(filename.c_str());
	if(file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		std::ifstream file;
		if (file.fail()) 
			return false;

		int32_t metaSize = 0;
		int32_t size = 0;
		
		_frame->header.clear();
		_frame->data.clear();
		
		while(!file.eof())
		{
			_frame->header.push_back(FrameHeader());
			_frame->data.push_back(FrameData());
			size_t frameIndex = _frame->header.size() -1;
			float minVal = 0;
			float maxVal = 0;

			file.read((char*) &_frame->header[frameIndex]. width, sizeof(int32_t));
			file.read((char*) &_frame->header[frameIndex].height, sizeof(int32_t));
			file.read((char*) &_frame->header[frameIndex].layerCount, sizeof(int32_t));
			file.read((char*) &minVal, sizeof(float));
			file.read((char*) &maxVal, sizeof(float));
			file.read((char*) &size, sizeof(int32_t));
			file.read((char*) &metaSize, sizeof(int32_t));
			file.read((char*) &_frame->header[frameIndex].pixelData, sizeof(PixelData));
			file.read((char*) &_frame->header[frameIndex].dataType, sizeof(DataType));
			file.read((char*) &_frame->header[frameIndex].coordinateType, sizeof(CoordinateType));
			file.read((char*) &_frame->header[frameIndex].imageType, sizeof(ImageType));

			_frame->header[frameIndex].rangeMin = minVal;
			_frame->header[frameIndex].rangeMax = maxVal;

			int64_t timestamp = this->timestamp((unsigned int)frameIndex);
			int32_t index = this->index((unsigned int)frameIndex);
			std::string buffer = this->_frame->data[frameIndex].buffer;
			std::string metadata = this->_frame->data[frameIndex].metadata;

			file.read((char*) &_frame->data[frameIndex].timestamp, sizeof(int64_t));
			file.read((char*) &_frame->data[frameIndex].index, sizeof(index));


			if(size)
			{
				char* pBuf = new char[size];

				file.read((char*) &pBuf, size);
				setBuffer(pBuf, size);

				delete[] pBuf;
			}

			if(metaSize > 0)
			{
				char* pBufferMeta = new char[metaSize];
				file.read((char*) &pBufferMeta, metaSize);

				setMetadata(pBufferMeta, metaSize);

				delete[] pBufferMeta;
			}
		
		}
		file.close();

		return file.good();
	}
	else
		return false;
}

bool Image::load( std::list<std::string> filenames )
{
	return loadBitmaps(filenames);
}

unsigned int Image::frameIndexSize() const
{
	return (unsigned int)_frame->header.size();
}

Frame* Image::extractFrame( unsigned int frameindex /*= 0*/ )
{
	if(frameindex < 0 || frameindex > _frame->data.size())
		return nullptr;

	Image extractedImage(&_frame->data[frameindex], &_frame->header[frameindex]); 

	return extractedImage.getFrame(true);
}

Frame Image::extractFrameInstance( unsigned int frameindex /*= 0*/ )
{
	if(frameindex < 0 || frameindex > _frame->data.size())
		return Frame();

	Image extractedImage(&_frame->data[frameindex], &_frame->header[frameindex]); 
	return *extractedImage.getFrame();
}

void Image::setFrame( Frame* frame, bool copy /*= false*/, int frameindexDst /*= 0*/, unsigned int frameindexSrc /*= 0*/, unsigned int frameRange/* = 0*/)
{

	if(frameindexDst >= 0)
	{
		// In this case, we cannot replace the frame data
		if(frameindexDst > (int)frameIndexSize() || frameindexSrc >= frame->header.size())
			return;

		setFrameIndexSize(frameindexDst + 1 + frameRange);

		if(frameRange < 1)
			frameRange = 1;

		for(unsigned int src = 0; src < frameRange; src++)
		{
			int srcIndex = frameindexSrc + src;
			int dstIndex = frameindexDst + src;

			if(_frame->header.size() < dstIndex && frame->header.size() < srcIndex)
			{
				_frame->header[dstIndex] = frame->header[srcIndex];
				_frame->data[dstIndex] = frame->data[srcIndex];
			}
		}
	}
	else 
	{
		if(_frame && _isOwner)
			delete _frame;

		if(copy)
		{
			_frame = new Frame(*frame);
			_isOwner = true;
		}
		else
		{
			_frame = frame;
			_isOwner = false;
		}
	}
}


Image& Image::appendFrame( Frame* frame )
{
	if(frame != nullptr)
	{
		for(int i = 0; i < frame->header.size(); i++)
		{
			_frame->header.push_back(frame->header[0]);
			_frame->data.push_back(frame->data[0]);
		}
	}



	return *this;
}

void Image::setFrameIndexSize( unsigned int frameIndexSize )
{
	while(frameIndexSize > (unsigned int)_frame->header.size() && _frame->header.size() <= MAX_FRAME_COUNT)
	{
		_frame->header.push_back(FrameHeader());
		_frame->data.push_back(FrameData());
	}
}

Image Image::toCartesian(long frameIndex /*= -1*/, unsigned char distanceOffset/* = 2*/)
{
	Image img = Image(*this);

	if(frameIndex >= 0)
	{
		img.transformToCartesian(frameIndex, -1, distanceOffset);
	}
	else
	{
		frameIndex = _frame->data.size();
		for(unsigned int i = 0; i < (unsigned int)frameIndex; i++)
		{
			img.transformToCartesian(i, -1, distanceOffset);
		}
	}
	return img;
}

void Image::transformToCartesian( unsigned int frameIndex, int layer /*= -1*/, unsigned char distanceOffset/* = 2*/)
{
	setCoordinateType(CoordinateType::CARTESIAN, frameIndex);
	if(!_frame) return;
	
	switch (_frame->header[frameIndex].dataType)
	{
	case DataType::CHAR:
		{
			transformToCartesian<char>(frameIndex, layer, distanceOffset);
			break;
		}
	case DataType::UCHAR:
		{
			transformToCartesian<unsigned char>(frameIndex, layer, distanceOffset);	
			break;
		}
	case DataType::SHORT:
		{
			transformToCartesian<short>(frameIndex, layer, distanceOffset);
			break;
		}
	case DataType::USHORT:
		{
			transformToCartesian<unsigned short>(frameIndex, layer, distanceOffset);	
			break;
		}
	case DataType::INT:
		{
			transformToCartesian<int>(frameIndex, layer, distanceOffset);
			break;
		}
	case DataType::UINT:
		{
			transformToCartesian<unsigned int>(frameIndex, layer, distanceOffset);
			break;
		}
	case DataType::FLOAT:
		{
			transformToCartesian<float>(frameIndex, layer, distanceOffset);
			break;
		}
	case DataType::DOUBLE:
		{
			transformToCartesian<double>(frameIndex, layer, distanceOffset);
			break;
		}
	};
}

Image Image::toPolar(long frameIndex /*= -1*/)
{
	Image img = Image(*this);

	if(frameIndex >= 0)
	{
		img.transformToPolar(frameIndex);
	}
	else
	{
		frameIndex = _frame->data.size();
		for(unsigned int i = 0; i < (unsigned int)frameIndex; i++)
		{
			img.transformToPolar(i);
		}
	}
	return img;
}

void Image::transformToPolar( unsigned int frameIndex )
{
	setCoordinateType(CoordinateType::POLAR, frameIndex);
	if(!_frame) return;

	switch (_frame->header[frameIndex].dataType)
	{
	case DataType::CHAR:
		{
			transformToPolar<char>(frameIndex);
			break;
		}
	case DataType::UCHAR:
		{
			transformToPolar<unsigned char>(frameIndex);	
			break;
		}
	case DataType::SHORT:
		{
			transformToPolar<short>(frameIndex);
			break;
		}
	case DataType::USHORT:
		{
			transformToPolar<unsigned short>(frameIndex);	
			break;
		}
	case DataType::INT:
		{
			transformToPolar<int>(frameIndex);
			break;
		}
	case DataType::UINT:
		{
			transformToPolar<unsigned int>(frameIndex);
			break;
		}
	case DataType::FLOAT:
		{
			transformToPolar<float>(frameIndex);
			break;
		}
	case DataType::DOUBLE:
		{
			transformToPolar<double>(frameIndex);
			break;
		}
	};
}

char* Image::metadata( unsigned int frameIndex /*= 0*/ ) const
{
	return(char*)_frame->data[frameIndex].metadata.c_str();
}

const char* Image::getMetadata( unsigned int frameIndex /*= 0*/ )
{
	return metadata(frameIndex);
}

char* Image::buffer( unsigned int frameIndex /*= 0*/ ) const
{
	return (char*)_frame->data[frameIndex].buffer.c_str();
}

const char* Image::getBuffer( unsigned int frameIndex /*= 0*/ )
{
	return buffer(frameIndex);
}

void Image::setTimestamp( int64_t timestamp, unsigned int frameIndex /*= 0*/ ) const
{
	_frame->data[frameIndex].timestamp = timestamp;
}

int64_t Image::timestamp( unsigned int frameIndex /*= 0*/ ) const
{
	return _frame->data[frameIndex].timestamp;
}

void Image::setIndex( int index, unsigned int frameIndex /*= 0*/ ) const
{
	_frame->data[frameIndex].index = index;
}

int Image::index( unsigned int frameIndex /*= 0*/ ) const
{
	return _frame->data[frameIndex].index;
}

void Image::setImageType( int imageType, unsigned int frameIndex /*= 0*/ ) const
{
	_frame->header[frameIndex].imageType = (Container::Images::ImageType)imageType;
}

ImageType Image::imageType( unsigned int frameIndex /*= 0*/ ) const
{
	return _frame->header[frameIndex].imageType;
}

void Image::setCoordinateType( CoordinateType coordinateType, unsigned int frameIndex /*= 0*/ ) const
{
	_frame->header[frameIndex].coordinateType = coordinateType;
}

CoordinateType Image::coordinateType( unsigned int frameIndex /*= 0*/ ) const
{
	return _frame->header[frameIndex].coordinateType;
}

void Image::setDataType( DataType dataType, unsigned int frameIndex /*= 0*/ ) const
{
	_frame->header[frameIndex].dataType = dataType;
}

DataType Image::dataType( unsigned int frameIndex /*= 0*/ ) const
{
	return _frame->header[frameIndex].dataType;
}

void Image::setCameraParameters( CameraParameters cameraParameters, unsigned int frameIndex /*= 0*/ ) const
{
	_frame->header[frameIndex].cameraParameters = cameraParameters;
}

CameraParameters Image::cameraParameters( unsigned int frameIndex /*= 0*/ ) const
{
	return _frame->header[frameIndex].cameraParameters;
}

void Image::setPixelData( PixelData pixelData, unsigned int frameIndex /*= 0*/ ) const
{
	_frame->header[frameIndex].pixelData = pixelData;
}

PixelData Image::pixelData( unsigned int frameIndex /*= 0*/ ) const
{
	return _frame->header[frameIndex].pixelData;
}

void Image::setRangeMax( float max, unsigned int frameIndex /*= 0*/ ) const
{
	_frame->header[frameIndex].rangeMax = max;
}

float Image::rangeMax( unsigned int frameIndex /*= 0*/ ) const
{
	return (float)_frame->header[frameIndex].rangeMax;
}

void Image::setRangeMin( float min, unsigned int frameIndex /*= 0*/ ) const
{
	_frame->header[frameIndex].rangeMin = min;
}

float Image::rangeMin( unsigned int frameIndex /*= 0*/ ) const
{
	return (float)_frame->header[frameIndex].rangeMin;
}

int Image::size( unsigned int frameIndex /*= 0*/ ) const
{
	return (int)_frame->data[frameIndex].buffer.length();
}

void Image::setLayerCount( int layerCount, unsigned int frameIndex /*= 0*/ ) const
{
	_frame->header[frameIndex].layerCount = layerCount;
}

int Image::layerCount( unsigned int frameIndex /*= 0*/ ) const
{
	return _frame->header[frameIndex].layerCount;
}

void Image::setHeight( int height, unsigned int frameIndex /*= 0*/ ) const
{
	_frame->header[frameIndex].height = height;
}

int Image::height( unsigned int frameIndex /*= 0*/ ) const
{
	return _frame->header[frameIndex].height;
}

void Image::setWidth( int width, unsigned int frameIndex /*= 0*/ ) const
{
	_frame->header[frameIndex].width = width;
}

int Image::width( unsigned int frameIndex /*= 0*/ ) const
{
	return _frame->header[frameIndex].width;
}

Image Image::mergeFramesToLayers( DataType mergedDataType )
{
	Image img = Image(*this);
	img.transformMergeFramesToLayers(mergedDataType);
	return img;
}

void Image::transformMergeFramesToLayers( DataType mergedDataType )
{
	unsigned int mergeableLayers = checkFramesMergeable();
	if( mergeableLayers > 0)
	{
		switch (mergedDataType)
		{
		case DataType::CHAR:
			{
				transformMergeFramesToLayers<char>(mergeableLayers, mergedDataType);
				break;
			}
		case DataType::UCHAR:
			{
				transformMergeFramesToLayers<unsigned char>(mergeableLayers, mergedDataType);	
				break;
			}
		case DataType::SHORT:
			{
				transformMergeFramesToLayers<short>(mergeableLayers, mergedDataType);
				break;
			}
		case DataType::USHORT:
			{
				transformMergeFramesToLayers<unsigned short>(mergeableLayers, mergedDataType);	
				break;
			}
		case DataType::INT:
			{
				transformMergeFramesToLayers<int>(mergeableLayers, mergedDataType);
				break;
			}
		case DataType::UINT:
			{
				transformMergeFramesToLayers<unsigned int>(mergeableLayers, mergedDataType);
				break;
			}
		case DataType::FLOAT:
			{
				transformMergeFramesToLayers<float>(mergeableLayers, mergedDataType);
				break;
			}
		case DataType::DOUBLE:
			{
				transformMergeFramesToLayers<double>(mergeableLayers, mergedDataType);
				break;
			}
		}
	}
}

Image Image::splitLayersToFrames()
{
	Image img = Image(*this);
	img.transformSplitLayersToFrames();
	return img;
}

void Image::transformSplitLayersToFrames()
{
	int layercount = layerCount();
	if(layercount > 1)
	{
		size_t layersize = layerSize();
		char* srcBuffer = (char*)buffer();

		FrameHeader header = _frame->header[0];
		FrameData data = _frame->data[0];
		header.layerCount = 1;
		
		for(int i = 0; i < layercount; i++)
		{
			if(_frame->header.size() < i + 1)
			{
				_frame->header.push_back(FrameHeader(header));
			}
			if(_frame->data.size() < i + 1)
			{
				_frame->data.push_back(FrameData(data));
			}
			setBuffer(srcBuffer, layersize,(unsigned int)i);
			srcBuffer += layersize;
		}
	}
}

unsigned int Image::checkFramesMergeable()
{
	unsigned int imgFrameIndexSize = frameIndexSize();
	if(imgFrameIndexSize > 1)
	{
		unsigned int mergeableLayers = (unsigned int) layerCount(0);
		for(unsigned int i = 1; i < frameIndexSize(); i++)
		{
			if(width(i) * height(i) != width(0) * height(0))
			{
				mergeableLayers = 0;
				break;
			}
			else
			{
				mergeableLayers += (unsigned int)layerCount(i);
			}
		}	
		return mergeableLayers;
	}
	else
	{
		//skip if 1 frame only.
		return 0;
	}
}

std::string Image::cameraId(unsigned int frameIndex /*= 0*/) const
{
	return _frame->header[frameIndex].cameraId;
}

void Image::setCameraId( std::string cameraId, unsigned int frameIndex /*= 0*/ )
{
	_frame->header[frameIndex].cameraId = cameraId;
}

bool Image::saveBitmap( std::string filename, unsigned int frameIndex, int layerNumber/* = 0*/)
{
	switch (_frame->header[frameIndex].dataType)
	{
	case DataType::CHAR:
		{
			return saveBitmapInternal<char>(filename, frameIndex, layerNumber);
		}
	case DataType::UCHAR:
		{
			return saveBitmapInternal<unsigned char>(filename, frameIndex, layerNumber);
		}
	case DataType::SHORT:
		{
			return saveBitmapInternal<short>(filename, frameIndex, layerNumber);
		}
	case DataType::USHORT:
		{
			return saveBitmapInternal<unsigned short>(filename, frameIndex, layerNumber);
		}
	case DataType::INT:
		{
			return saveBitmapInternal<int32_t>(filename, frameIndex, layerNumber);
		}
	case DataType::UINT:
		{
			return saveBitmapInternal<uint32_t>(filename, frameIndex, layerNumber);
		}
	case DataType::FLOAT:
		{
			return saveBitmapInternal<float>(filename, frameIndex, layerNumber);
		}
	case DataType::DOUBLE:
		{
			return saveBitmapInternal<double>(filename, frameIndex, layerNumber);
		}
	};

	return true;
}

bool Image::loadBitmap( std::string filename )
{
	std::list<std::string> list;
	list.push_back(filename);

	return loadBitmaps(list);
}

bool Image::loadBitmaps( std::list<std::string> filenames )
{
	bool returnValue = false;
	unsigned int frameIndex = 0;
	size_t loadedBitmaps = 0;

	for(std::list<std::string>::iterator it = filenames.begin(); it != filenames.end(); ++it)
	{
		QFileInfo fileinfo(it->c_str());
		QString ext = fileinfo.suffix();
		// skip if not png, bmp or jpg
		if(ext != "png" && ext != "bmp" && ext != "jpg" )
		{
			continue;
		}

		//determine frameindex
		QString baseFileName = fileinfo.baseName();
		QStringList splittedFileName = baseFileName.split("_", QString::SkipEmptyParts);
		int frameIndexPosition = splittedFileName.indexOf("FrameIndex");
		if(frameIndexPosition >= 0 && frameIndexPosition + 1 < splittedFileName.size())
		{
			bool ok = false;
			unsigned int readFrameIndex = splittedFileName.at(frameIndexPosition + 1).toUInt(&ok);

			if(ok)
			{
				frameIndex = readFrameIndex;
			}
			else
			{
				frameIndex++;
			}
		}
		QImage image = QImage(QString::fromStdString(*it));
		if(!image.isNull())
		{
			if(loadBitmapInternal(image, frameIndex))
			{
				loadedBitmaps++;
			}
		}
	}

	if(loadedBitmaps == filenames.size())
	{
		returnValue = true;
	}

	return returnValue;
}

bool Image::loadBitmapInternal( const QImage& image, unsigned int frameIndex /*= 0*/)
{
	setWidth(image.width(), frameIndex);
	setHeight(image.height(), frameIndex);
	setDataType(Container::Images::DataType::UCHAR, frameIndex);
	setPixelData(Container::Images::PixelData::COLOR, frameIndex);
	setTimestamp(QDateTime::currentDateTime().toTime_t());
	setImageType((int)Container::Images::ImageType::RGBA, frameIndex);
	setCoordinateType(Container::Images::CoordinateType::CARTESIAN, frameIndex);

	QImage convertedImage = image.convertToFormat(QImage::Format_ARGB32);

	char* buffer = new char[convertedImage.width() * convertedImage.height() * 4];
	for(int y = 0; y < convertedImage.height(); y++)
	{
		char* scanline = (char*)convertedImage.scanLine(y);

		char* lineAdr = buffer + y * (4 * sizeof(char) * convertedImage.width());

		memcpy(lineAdr, scanline,  (4 * sizeof(char) * convertedImage.width()));
	}

	setBuffer(buffer, convertedImage.width() * convertedImage.height() * 4, frameIndex);	
	
	delete [] buffer;
	return true;
}

}	// namespace images
}	// namespace container
