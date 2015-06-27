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

template<class T>
void Container::Images::Image::transformToPolar( unsigned int frameIndex, int layer /*= -1*/ )
{
	int imgSize = width(frameIndex) * height(frameIndex);
	int numberOfLayersToTransform = layerCount(frameIndex);
	int startLayer = 0;

	if(layer >= 0)
	{
		numberOfLayersToTransform = 1;
		startLayer = layer;
	}

	T* pImageDataBufferPos = (T*)buffer(frameIndex) + imgSize * startLayer;
	const T* pImageDataBufferEnd = pImageDataBufferPos + imgSize * numberOfLayersToTransform;

	CameraParameters cameraParameters = this->cameraParameters(frameIndex);

	int x = 0;
	int y = height(frameIndex) -1;
	
	while(pImageDataBufferPos < pImageDataBufferEnd)
	{
		T imgDistance = *pImageDataBufferPos;
		double X_pos = (x - cameraParameters.principalPointX) / cameraParameters.focalLengthX * imgDistance;
		double Y_pos = (cameraParameters.principalPointY - y) / cameraParameters.focalLengthY * imgDistance; //image y axis is inverted

		(*pImageDataBufferPos) = (T) sqrt(X_pos*X_pos + Y_pos*Y_pos + imgDistance * imgDistance);

		pImageDataBufferPos++;
		x++;
		if(x >= width(frameIndex))
		{
			x = 0;
			y--;
		}
	}
}

template<class T>
void Container::Images::Image::transformToCartesian( unsigned int frameIndex, int layer /*= -1*/, unsigned char distanceOffset/* = 2*/ )
{
	int imgSize = width(frameIndex) * height(frameIndex);
	int numberOfLayersToTransform = layerCount(frameIndex);
	int startLayer = 0;

	if(layer >= 0)
	{
		numberOfLayersToTransform = 1;
		startLayer = layer;
	}

	T* pImageDataBufferPos = (T*)buffer(frameIndex) + imgSize * startLayer;
	const T* pImageDataBufferEnd = pImageDataBufferPos + imgSize * numberOfLayersToTransform;

	CameraParameters cameraParameters = this->cameraParameters(frameIndex);

	int x = 0;
	int y = height(frameIndex) -1;


	while(pImageDataBufferPos < pImageDataBufferEnd)
	{
		if(y < 0)
			y = height(frameIndex) -1;

		double x_imagePlan = (x - cameraParameters.principalPointX) / cameraParameters.focalLengthX;
		double y_imagePlan = (cameraParameters.principalPointY - y) / cameraParameters.focalLengthY; //image y axis is inverted
		float pmd_distance = *(pImageDataBufferPos + distanceOffset);

		double dot = x_imagePlan*x_imagePlan + y_imagePlan*y_imagePlan;
		(*pImageDataBufferPos) =  (T)pmd_distance/sqrt(1.0f + dot); 

		pImageDataBufferPos++;

		x++;
		if(x >= width(frameIndex))
		{
			x = 0;
			y--;
		}
	}
}

template<class T>
void Container::Images::Image::transformMergeFramesToLayers(unsigned int mergeableLayers, DataType mergedDataType)
{
	unsigned int bufferSize = width(0) * height(0) * mergeableLayers;
	char* destBuffer = (char*)new T[bufferSize];
	memset(destBuffer, 0, bufferSize);
	T* bufferPos = (T*)destBuffer;

	for(unsigned int i = 0; i < frameIndexSize(); i++)
	{
		switch(dataType(i))
		{
		case DataType::CHAR:
			{
				executeTransformMergeFramesToLayers<T, char>(i, bufferPos);
				break;
			}
		case DataType::UCHAR:
			{
				executeTransformMergeFramesToLayers<T, unsigned char>(i, bufferPos);
				break;
			}
		case DataType::SHORT:
			{
				executeTransformMergeFramesToLayers<T, short>(i, bufferPos);
				break;
			}
		case DataType::USHORT:
			{
				executeTransformMergeFramesToLayers<T, unsigned short>(i, bufferPos);
				break;
			}
		case DataType::INT:
			{
				executeTransformMergeFramesToLayers<T, int>(i, bufferPos);
				break;
			}
		case DataType::UINT:
			{
				executeTransformMergeFramesToLayers<T, unsigned int>(i, bufferPos);
				break;
			}
		case DataType::FLOAT:
			{
				executeTransformMergeFramesToLayers<T, float>(i, bufferPos);
				break;
			}
		case DataType::DOUBLE:
			{
				executeTransformMergeFramesToLayers<T, double>(i, bufferPos);
				break;
			}
		}
	}

	FrameHeader header = _frame->header[0];
	FrameData data = _frame->data[0];
	_frame->header.clear();
	_frame->data.clear();

	header.layerCount = mergeableLayers;
	header.dataType = mergedDataType;

	_frame->header.push_back(header);
	_frame->data.push_back(data);

	setBuffer(destBuffer, bufferSize * sizeof(T));
}


template<class T, class S>
void Container::Images::Image::executeTransformMergeFramesToLayers(unsigned int i, T* &bufferPos)
{
	S* srcBuffer =  (S*)buffer(i);
	int srcBufferSize = width(i) * height(i) * layerCount(i);
	S* srcBufferEnd = srcBuffer + srcBufferSize;

	while(srcBuffer < srcBufferEnd)
	{
		*bufferPos = (T)(*srcBuffer);
		srcBuffer++;
		bufferPos++;
	}
}

template<class T> bool Container::Images::Image::saveBitmapInternal( std::string filename, unsigned int frameIndex, int layerNumber /*= 0*/)
{
	QImage img(width(frameIndex), height(frameIndex), QImage::Format_RGB32);

	int channels = 1;

	if(_frame->header[frameIndex].imageType == ImageType::RAWDATA ||
		_frame->header[frameIndex].imageType == ImageType::RAWIMAGE)
	{
		channels = 1;
	}
	else if(_frame->header[frameIndex].imageType == ImageType::RGB)
	{
		channels = 3;
	}
	else if(_frame->header[frameIndex].imageType == ImageType::RGBA ||
		_frame->header[frameIndex].imageType == ImageType::ARGB)
	{
		channels = 4;
	}

	int w = width(frameIndex);
	float minRange = rangeMin(frameIndex);
	float maxRange = rangeMax(frameIndex);

	bool returnValue = true;

	if(layerNumber >= layerCount(frameIndex))
		layerNumber = -1;
	
	int layerStart = layerNumber;
	int layerEnd = layerNumber;

	if(layerNumber < 0)
	{
		layerStart = 0; 
		layerEnd = layerCount(frameIndex)-1;
	}

	QFileInfo fileinfo(filename.c_str());
	std::string ext = fileinfo.suffix().toStdString();
	std::string filePath = fileinfo.absolutePath().toStdString();
	std::string filenameBase = fileinfo.baseName().toStdString();

	for(int i = layerStart; i <= layerEnd; i++)
	{
		T* imageBuffer = (T*)layer(i, frameIndex);

		std::stringstream ss;

		ss << filePath << "/";
		ss << filenameBase;

		if(layerNumber < 0)
		{
			ss << "_layer" << i << "." << ext;
		}
		else ss << "." << ext;


		for (unsigned short y = 0; y < height(); y++)																						
		{
			for (unsigned short x = 0; x < width (); x++)																						
			{
				img.setPixel(x, y, getRGBValue(imageBuffer, x, y, w, channels, minRange, maxRange));  
			}
		}

		returnValue |= img.save(ss.str().c_str());
	}

	return returnValue;
}

template<class T> QRgb Image::getRGBValue(T* buffer, int x, int y, int width, int channels, float minRange, float maxRange)
{
	T* pos = buffer + (((y*width) + x)*channels);
	switch(channels)
	{
	default:
	case 1:
		{
			double v0 = (double)pos[0];
			double v0Norm = (pos[0]-minRange)/(double)(maxRange-minRange);

			return qRgb((int)(v0Norm*255), (int)(v0Norm*255), (int)(v0Norm*255));
		}
	case 3:
		{
			double v0 = (double)pos[2];
			double v0Norm = (pos[2]-minRange)/(double)(maxRange-minRange);

			double v1 = (double)pos[1];
			double v1Norm = (pos[1]-minRange)/(double)(maxRange-minRange);

			double v2 = (double)pos[0];
			double v2Norm = (pos[0]-minRange)/(double)(maxRange-minRange);

			return qRgb((int)(v0Norm*255), (int)(v1Norm*255), (int)(v2Norm*255));
		}
	case 4:
		{
			double v0 = (double)pos[2];
			double v0Norm = (pos[2]-minRange)/(double)(maxRange-minRange);

			double v1 = (double)pos[1];
			double v1Norm = (pos[1]-minRange)/(double)(maxRange-minRange);

			double v2 = (double)pos[0];
			double v2Norm = (pos[0]-minRange)/(double)(maxRange-minRange);

			double vA = (double)pos[3];
			double vANorm = (pos[3]-minRange)/(double)(maxRange-minRange);

			return qRgba((int)(v0Norm*255), (int)(v1Norm*255), (int)(v2Norm*255), (int)(vANorm*255));
		}
	}

}
