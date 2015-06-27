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

#ifndef IMAGE_H_
#define IMAGE_H_

#if WIN32
#pragma once
#endif

#include <QRgb>
#include <QImage>
#include <string>
#include <cstdint>
#include "Images/Frame.h"
#include "Images/FrameData.h"
#include "Images/FrameHeader.h"
#include "ContainerDLL.h"
#include "../Defines.hpp"


namespace Container
{
	namespace Images
	{

		class CONTAINER_API Image
		{

		public:
			/**
			* \brief Constructor for Image Class
			*/
			Image();

			/**
			* \brief Constructor for Image Class
			*
			* An Image has an internal Thrift Frame, to help you get more information about the frame
			*
			* \param  Container::Images::FrameData frame The thrift frame data
			* \param  Container::Images::FrameHeader header The thrift frame header
			*/
			Image(FrameData* data, FrameHeader* header);

			/**
			* \brief Constructor for Image Class
			*
			* An Image has an internal Thrift Frame, to help you get more information about the frame
			*
			* \param  Container::Images::Frame frame The thrift frame 
			*/
			Image(Frame* frame, bool copy = true);

			/**
			* \brief Constructor for Image Class
			*
			* An Image has an internal Thrift Frame, to help you get more information about the frame
			*
			* \param  frameSize frameSize The size of frames to create.
			*/
			Image(unsigned int frameSize);

			/**
			* \brief Destructor for Image Class

			*/
			virtual ~Image();

			/**
			* \brief Attaches the ownership from the image
			*
			* \return bool The old ownership state
			*/
			inline Image& attachFrame() 
			{ 
				_isOwner = true; 
				return *this; 
			}

			/**
			* \brief Appends a new frame to the internal frame
			*
			* \return Image& The image instance
			* \param  Container::Images::Frame * frame The frame to append
			*/
			Image& appendFrame(Frame* frame);


			/**
			* \brief Detaches the ownership from the image
			*
			* \return bool The old ownership state
			*/
			inline bool detachFrame() 
			{ 
				bool oldOwner = _isOwner; 
				if(_isOwner) 
					_isOwner = false; 
				return oldOwner; 
			}

			/**
			* \brief Returns the internal frame
			*
			* \param  bool detachFrame Detachs the ownership from the image
			* \return Container::Images::Frame* The internal frame
			*/
			inline Frame* getFrame(bool detachFrame = false)
			{ 
				if(_isOwner)
					_isOwner = !detachFrame; 
					
				return _frame; 
			}

			/**
			* \brief Returns an instance of an internal frame
			*
			* \param unsigned int frameIndex Index of the frame.
			* \return Container::Images::Frame* The internal frame
			*/
			Frame extractFrameInstance(unsigned int frameindex = 0);

			/**
			* \brief Returns a copy of an internal frame
			*
			* \param unsigned int frameIndex Index of the frame.
			* \return Container::Images::Frame* A copy of the extracted frame
			*/
			Frame* extractFrame(unsigned int frameindex = 0);

			/**
			* \brief Sets a new internal frame
			*
			* \param  Container::Images::Frame * frame The new frame storage class
			* \param bool copy True, if the image takes the ownership, else false
			* \param int frameindexDst Index of the frame. -1 indicates, that the whole frame should be replaced. A value >= 0 replaces the frame data at this position
			* \param unsigned int frameindexSrc The source frame data index
			* \param unsigned int frameRange The range to copy
			*/
			void setFrame(Frame* frame, bool copy = false, int frameindexDst = 0, unsigned int frameindexSrc = 0, unsigned int frameRange = 0);

			/**
			* \brief Determines the byte data type size
			*
			* \return int The size of the data type in bytes
			* \param unsigned int frameIndex Index of the frame.
			*/
			int getDataTypeSize( unsigned int frameIndex = 0 )  const;

			/**
			* \brief Save the image to the hdd
			*
			* \return bool true, if successful, else false
			* \param  std::string filename The filename
			* \param layerNumber The layer number to export
			*/
			virtual bool save(std::string filename, unsigned int frameIndex = 0, int layerNumber = 0);

			/**
			* \brief Loads the image to the hdd
			*
			* \return bool true, if successful, else false
			* \param  std::string filename The filename
			*/
			virtual bool load(std::string filename);

			/**
			* \brief Loads bitmap images from the hdd
			*
			* \return bool true, if successful, else false
			* \param  std::list<std::string> filename The filenames
			*/
			virtual bool load(std::list<std::string> filenames);

			/**
			* \brief Clamps the internal frame to a minimum and maximum range
			*
			* \param  float minDepth The minimum depth
			* \param  float maxDepth The maximum depth
			* \param unsigned int frameIndex Index of the frame.
			*/
			void clamp( float minDepth, float maxDepth, unsigned int frameIndex = 0 );

			/**
			* \brief Returns the width of the image
			*
			* \return int The width
			* \param unsigned int frameIndex Index of the frame.
			*/
			int width(unsigned int frameIndex = 0) const;

			/**
			* \brief Sets the width of the image
			*
			* \param  int width The new width
			* \param unsigned int frameIndex Index of the frame.
			*/
			void setWidth(int width, unsigned int frameIndex = 0) const;

			/**
			* \brief Returns the height of the image
			*
			* \return int The height
			* \param unsigned int frameIndex Index of the frame.
			*/
			int height(unsigned int frameIndex = 0) const;

			/**
			* \brief Sets the height of the image
			*
			* \param  int height The new height
			* \param unsigned int frameIndex Index of the frame.
			*/
			void setHeight(int height, unsigned int frameIndex = 0) const;

			/**
			* \brief Returns the layerCount of the image
			*
			* \return int The layerCount
			* \param unsigned int frameIndex Index of the frame.
			*/
			int layerCount(unsigned int frameIndex = 0) const;

			/**
			* \brief Sets the layerCount of the image
			*
			* \param  int layerCount The new layerCount
			* \param unsigned int frameIndex Index of the frame.
			*/
			void setLayerCount(int layerCount, unsigned int frameIndex = 0) const;

			/**
			* \brief Returns the size of the image
			*
			* \return int The size
			* \param unsigned int frameIndex Index of the frame.
			*/
			int size(unsigned int frameIndex = 0) const;

			/**
			* \brief Returns the minimum range
			*
			* \return float The minimum range
			* \param unsigned int frameIndex Index of the frame.
			*/
			float rangeMin(unsigned int frameIndex = 0) const;

			/**
			* \brief Sets the minimum range
			*
			* \param  int width The new minimum range
			* \param unsigned int frameIndex Index of the frame.
			*/
			void setRangeMin(float min, unsigned int frameIndex = 0) const;
				
			/**
			* \brief Returns the maximum range
			*
			* \return float The maximum range
			* \param unsigned int frameIndex Index of the frame.
			*/
			float rangeMax(unsigned int frameIndex = 0) const;

			/**
			* \brief Sets the maximum range
			*
			* \param  int width The new maximum range
			* \param unsigned int frameIndex Index of the frame.
			*/
			void setRangeMax(float max, unsigned int frameIndex = 0) const;

			/**
			* \brief Returns the pixel data of the image
			*
			* \return Container::Images::PixelData The pixel data
			* \param unsigned int frameIndex Index of the frame.
			*/
			PixelData pixelData(unsigned int frameIndex = 0) const;

			/**
			* \brief Sets the pixel data of the image
			*
			* \param  Container::Images::PixelData layerCount The new pixel data
			* \param unsigned int frameIndex Index of the frame.
			*/
			void setPixelData(PixelData pixelData, unsigned int frameIndex = 0) const;

			/**
			* \brief Returns the pixel data of the image
			*
			* \return Container::Images::PixelData The pixel data
			* \param unsigned int frameIndex Index of the frame.
			*/
			CameraParameters cameraParameters(unsigned int frameIndex = 0) const;

			/**
			* \brief Sets the layerCount of the image
			*
			* \param  Container::Images::CameraParameters cameraParameters The new layerCount
			* \param unsigned int frameIndex Index of the frame.
			*/
			void setCameraParameters(CameraParameters cameraParameters, unsigned int frameIndex = 0) const;

			/**
			* \brief Returns the data type of the image
			*
			* \return Container::Images::DataType::type The data type
			* \param unsigned int frameIndex Index of the frame.
			*/
			DataType dataType(unsigned int frameIndex = 0) const;

			/**
			* \brief Sets the data type of the image
			*
			* \param  Container::Images::DataType::type dataType The new data type
			*/
			void setDataType(DataType dataType, unsigned int frameIndex = 0) const;

			/**
			* \brief Returns the coordinate type of the image
			*
			* \return Container::Images::CoordinateType The coordinate type
			* \param unsigned int frameIndex Index of the frame.
			*/
			CoordinateType coordinateType(unsigned int frameIndex = 0) const;

			/**
			* \brief Sets the coordinate type of the image
			*
			* \param  Container::Images::CoordinateType coordinateType The new coordinate type
			* \param unsigned int frameIndex Index of the frame.
			*/
			void setCoordinateType(CoordinateType coordinateType, unsigned int frameIndex = 0) const;

			/**
			* \brief Returns the type of the image
			*
			* \return Container::Images::ImageType The image type
			* \param unsigned int frameIndex Index of the frame.
			*/
			ImageType imageType(unsigned int frameIndex = 0) const;

			/**
			* \brief Sets the image type of the image
			*
			* \param  Container::Images::ImageType imageType The new imageType
			* \param unsigned int frameIndex Index of the frame.
			*/
			void setImageType(int imageType, unsigned int frameIndex = 0) const;

			/**
			* \brief Returns the index of the image
			*
			* \return int The width
			*/
			int index(unsigned int frameIndex = 0) const;

			/**
			* \brief Sets the index of the image
			*
			* \param  int index The new index
			* \param unsigned int frameIndex Index of the frame.
			*/
			void setIndex(int index, unsigned int frameIndex = 0) const;

			/**
			* \brief Returns the timestamp of the image
			*
			* \return int64_t The timestamp
			*/
			int64_t timestamp(unsigned int frameIndex = 0) const;

			/**
			* \brief Sets the timestamp of the image
			*
			* \param  int64_t timestamp The new timestamp
			* \param unsigned int frameIndex Index of the frame.
			*/
			void setTimestamp(int64_t timestamp, unsigned int frameIndex = 0) const;
				
			/**
			* \brief Returns the byte size of the chosen layer
			*
			* \return size_t Byte size of the chosen layer
			* \param unsigned int frameIndex Index of the frame.
			*/
			size_t layerSize(unsigned int frameIndex = 0) const;

			/**
			* \brief Returns a pointer to the chosen layer
			*
			* \return char* The chosen layer buffer address
			* \param  int index The layer index
			* \param unsigned int frameIndex Index of the frame.
			*/
			char* layer(int index = 0, unsigned int frameIndex = 0) const;

			/**
			* \brief Returns a pointer to the chosen layer
			*
			* \return char* The chosen layer buffer address
			* \param  int index The layer index
			* \param unsigned int frameIndex Index of the frame.
			*/
			const char* getLayer(int index = 0, unsigned int frameIndex = 0);

			/**
			* \brief Returns the buffer of the image
			*
			* \return char* The buffer
			* \param unsigned int frameIndex Index of the frame.
			*/
			char* buffer(unsigned int frameIndex = 0) const;

			/**
			* \brief Returns the buffer of the image
			*
			* \return char* The buffer
			* \param unsigned int frameIndex Index of the frame.
			*/
			const char* getBuffer(unsigned int frameIndex = 0);

			/**
			* \brief Sets the buffer of the image
			*
			* \param  void * buffer The new buffer
			* \param  int size The byte size of the buffer
			* \param unsigned int frameIndex Index of the frame.
			*/
			void setBuffer(char* buffer, size_t size, unsigned int frameIndex = 0);

			/**
			* \brief Sets the buffer of the image
			*
			* \param  void * buffer The new buffer
			* \param  int size The byte size of the buffer
			* \param unsigned int frameIndex Index of the frame.
			*/
			DEPRECATED(void setBuffer(void* buffer, size_t size, unsigned int frameIndex = 0));

			/**
			* \brief Returns the width of the image
			*
			* \return char* The metadata
			* \param unsigned int frameIndex Index of the frame.
			*/
			char* metadata(unsigned int frameIndex = 0)  const;

			/**
			* \brief Returns the width of the image
			*
			* \return char* The metadata
			* \param unsigned int frameIndex Index of the frame.
			*/
			const char* getMetadata(unsigned int frameIndex = 0);

			/**
			* \brief Sets the metadata of the image
			*
			* \param  void * metadata The new metadata
			* \param  int size The byte size of the metadata
			* \param unsigned int frameIndex Index of the frame.
			*/
			void setMetadata(char* metadata, size_t size, unsigned int frameIndex = 0);

			/**
				* \brief get frame index size
				*
				*	\return unsigned int The frame index size.
				*/
			unsigned int frameIndexSize() const;

			/**
				* \brief resize frame index size, if necessary
				*
				*/
			void setFrameIndexSize(unsigned int frameIndexSize);

			/**
				* \brief get camera id.
				*
				*	\return std::string. The camera id.
				*/
			std::string cameraId(unsigned int frameIndex = 0) const;

			/**
				* \brief set camera id.
				*
				*	\param std::string cameraId to be set.
				*/
			void setCameraId(std::string cameraId, unsigned int frameIndex = 0);

			/**
				* \brief transforms a copy in cartesian coordinates
				*
				*	\return Image. A transformed copy.
				*	\param long frameIndex. The Index of the frame to transform.
				*	\param unsigned char distanceOffset. Offset between two distance values.
				*	-1 all frames.
				*/
			Image toCartesian(long frameIndex = -1, unsigned char distanceOffset = 2);

			/**
				* \brief transforms a copy in polar coordinates
				*
				* \return Image. A transformed copy.
				*	\param long frameIndex. The Index of the frame to transform. 
				*	-1 all frames.
				*/
			Image toPolar(long frameIndex = -1);

			/**
				* \brief transforms the current image into cartesian coordinates
				*
				*	\param long frameIndex. The Index of the frame to transform.
				*  \param int layer. The layer to transform.
				*	\param unsigned char distanceOffset. Offset between two distance values.
				*	-1 all frames.
				*/
			void transformToCartesian(unsigned int frameIndex, int layer = -1, unsigned char distanceOffset = 2);

			/**
				* \brief transforms the current image into polar coordinates
				*
				*	\param long frameIndex. The Index of the frame to transform. 
				*	-1 all frames.
				*/
			void transformToPolar(unsigned int frameIndex);

			/**
				* \brief a copy with merged frames into layers
				*
				*	/return Image. A merged copy.
				*  /param Container::Images::DataType::type mergedDataType. The copy image data type.
				*/
			Image mergeFramesToLayers(DataType mergedDataType);

			/**
				* \brief a copy with splitted layers into frames
				*
				*	/return Image. A merged copy.
				*/
			Image splitLayersToFrames();

			/**
				* \brief merge frames into layers
				*
				*  /param Container::Images::DataType::type mergedDataType. The copy image data type.
				*/
			void transformMergeFramesToLayers(DataType mergedDataType);

			/**
				* \brief a split layers into frames
				*
				*/
			void transformSplitLayersToFrames();
		public:
			/**
			* \brief Copy Constructor for Image Class
			*
			* \param const Image* rhs frame The image to copy
			*/
			Image(const Image& rhs);

			/**
			 * \brief Copy Constructor from QImage
			 *
			 *	\param const QImage& rhs The QImage to copy
			 */
			Image(const QImage& rhs);

			/**
			* \brief Assignment operator overload
			*
			* \param const Image* rhs frame The image to copy
			*/
			Image& operator=(const Image& rhs);
		protected:

			template<class T> QRgb getRGBValue(T* buffer, int x, int y, int width, int channels, float minRange, float MaxRange);

			/**
			* \brief Save the image as bitmap to the hdd
			*
			* \return bool true, if successful, else false
			* \param  std::string filename The filename
			* \param layerNumber The layer number to export
			*/
			bool saveBitmap(std::string filename, unsigned int frameIndex, int layerNumber = 0);

			/**
			* \brief Save the image as bitmap to the hdd
			*
			* \return bool true, if successful, else false
			* \param  std::string filename The filename
			* \param layerNumber The layer number to export
			*/
			template<class T> bool saveBitmapInternal(std::string filename, unsigned int frameIndex, int layerNumber = 0);

			/**
			* \brief Load the image as bitmap from the hdd
			*
			* \return bool true, if successful, else false
			* \param  std::string filename The filename
			*/
			bool loadBitmap(std::string filename);

			/**
			* \brief Load the image as bitmap from the hdd
			*
			* \return bool true, if successful, else false
			* \param  std::list<std::string> filenames The list of filenames
			*/
			bool loadBitmaps(std::list<std::string> filenames);

			/**
			* \brief Load the image as bitmap from the hdd
			*
			* \return bool true, if successful, else false
			* \param const QImage& image The image to load
			* \param unsigned int frameIndex The frameindex.
			*/
			bool loadBitmapInternal(const QImage& image, unsigned int frameIndex = 0);

			/**
				* \brief transforms the current image into cartesian coordinates
				*
				*	\param long frameIndex. The Index of the frame to transform.
				*	-1 all frames.
				*  \param int layer. The layer to transform.
				*	\param unsigned char distanceOffset. Offset between two distance values.
				*/
			template<class T>inline void transformToCartesian(unsigned int frameIndex, int layer = -1, unsigned char distanceOffset = 2);

			/**
				* \brief transforms the current image into polar coordinates
				*
				*	\param long frameIndex. The Index of the frame to transform. 
				*	-1 all frames.
				*  \param int layer. The layer to transform.
				*/
			template<class T>inline void transformToPolar(unsigned int frameIndex, int layer = -1);

			/**
				* \brief merge frames into layers
				*
				*	/param unsigned int mergeableLayers. The number of mergeable layers.
				*  /param Container::Images::DataType::type mergedDataType. The copy image data type.
				*/
			template<class T> inline void transformMergeFramesToLayers(unsigned int mergeableLayers, DataType mergedDataType);

			/**
				* \brief executes the merge frames into layers
				*
				*	/param unsigned int mergeableLayers. The number of mergeable layers.
				*  /param Container::Images::DataType::type mergedDataType. The copy image data type.
				*/
			template<class T, class S> inline void executeTransformMergeFramesToLayers(unsigned int i, T* &bufferPos);

			/**
				* \brief check if it is possible to merge the frames into layers.
				*
				*	/return the number of mergeable layers.
				*/
			unsigned int checkFramesMergeable();

		public:
		protected:
			Frame*					_frame;						//!< The internal data storage class
			bool					_isOwner;					//!< Determines if the image is the owner of the frame
		};


		#include "Image.inl"
	}	// namespace Images
}	// namespace container

#endif // !IMAGE_H_
