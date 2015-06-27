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

#ifndef VIEWER2D_H
#define VIEWER2D_H

#include "ViewerDLL.h"
#include "glwindow.qt.h"
#include "BoundingBoxes/BoundingRect2D.h"
#include "Images/DataType.h"
#include "Images/PixelData.h"

namespace Container
{
	namespace Images
	{
		class Image;
	}
}

namespace Viewer
{
	namespace Rendering
	{
		class GLRenderObject;

		typedef boost::shared_ptr<GLRenderObject>		GLRenderObjectPtr;
		typedef boost::shared_ptr<const GLRenderObject>	ConstGLRenderObjectPtr;

		class GLPipeline;

		typedef boost::shared_ptr<GLPipeline>			GLPipelinePtr;
		typedef boost::shared_ptr<const GLPipeline>		ConstGLPipelinePtr;
	}
}



namespace Viewer
{

		class VIEWER_API Viewer2D : public GLWindow
		{
			Q_OBJECT
		public:

			//! Order in which the single image fields arr rendered
			enum RenderOrder
			{
				horizontal,			//!< horizontal alignment
				vertical			//!< vertical alignment
			};

			explicit Viewer2D(QWindow *parent = 0);
			~Viewer2D();

			virtual void setImage(QImage image, unsigned int index = 0, bool addMissingImage = false);

			virtual void setImage(Container::Images::Image image, unsigned int index = 0, bool addMissingImage = false);

			virtual void setImage(const float* data, int width, int height, unsigned int index = 0, bool addMissingImage = false);

			virtual void setImage( void* data, size_t bufferSize, size_t sizeOfElement, int width, int height, int dataType = 0, unsigned int index = 0, bool addMissingImage = false, int pixelType = -1);
			
			void setImage( void* data, size_t bufferSize, size_t sizeOfElement, int width, int height, Container::Images::DataType dataType = Container::Images::DataType::UNDEFINED, unsigned int index = 0, bool addMissingImage = false, Container::Images::PixelData pixelType = Container::Images::PixelData::UNDEFINED);
			void setImage( void* data, size_t bufferSize, size_t sizeOfElement, int width, int height, Container::Images::DataType dataType = Container::Images::DataType::UNDEFINED, unsigned int index = 0, bool addMissingImage = false, int pixelType = -1);


			void render(QPainter *painter);

			virtual int addImageObject(unsigned int textureId, unsigned int witdh, unsigned int height);

			virtual bool removeImageObject(unsigned int textureId);

			//! Get render order.
			RenderOrder renderOrder(void) const { return _renderOrder; };

			//! Set render order.
			void setRenderOrder(RenderOrder value);


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

			/**
			* \brief Rotate the view 90�
			*
			* \param  bool ccw Rotates clockwise or counterclockwise
			*/
			void rotate90(bool ccw);

			//! Resets the viewer
			void resetView();

			size_t frameCount();

			//! Get the number of layers of the currently drawn image
			size_t layerCount(unsigned int frameIndex = 0);

			
			void getCurrentImageMinMaxRange( float &min, float &max, unsigned int frameIndex = 0 );


		protected:

			virtual void addDefaultObject();

			Viewer::Rendering::GLRenderObjectPtr getRenderObjectFromMousePosition(int x, int y);

			//! Helper struct storing texture information.
			struct Texture {

				//! Constructor
				Texture(unsigned int ID=0, unsigned int alphaID=0, unsigned short width=0, unsigned short height=0) 
					: ID(ID), alphaID(alphaID), width(width), height(height), x1(0), y1(0), x2(0), y2(0)
				{};

				unsigned int		ID;				//!< texture ID
				unsigned int		alphaID;		//!< alpha ID
				unsigned short		width;			//!< image width
				unsigned short		height;			//!< image height
				long				intTime;		//!< image integration time	   

				int		x1;
				int		y1;
				int		x2;
				int		y2;
			};

			//! Image list.
			typedef std::pair<unsigned int, Texture> TextureMapPair;
			typedef std::map<unsigned int, Texture>  TextureMap;


			typedef std::pair<unsigned int, Geometry::BoundingBoxes::BoundingRect2Dd>	TextureBoundingMapPair;
			typedef std::map<unsigned int,  Geometry::BoundingBoxes::BoundingRect2Dd>	TextureBoundingMap;


		protected:

			virtual void initialize();
			virtual void resizeEvent(QResizeEvent *event);

			//! Add image pane.
			virtual void setImage(unsigned char iIn, unsigned int ID, unsigned int alphaID, unsigned int width, unsigned int height, bool redraw = true);

			//! Update image.
			void updateImage(unsigned char iIn, unsigned int ID, unsigned int alphaID, unsigned int width, unsigned int height, unsigned long intTime, bool redraw = true);

			//! Remove image pane.
			void removeImage(unsigned char iIn);

			//! Get number of images.
			unsigned char numbImages(void) const { return (unsigned char) _aTexture.size(); };


			//! Get number of horizontal image fields.
			unsigned char horzImgFields(void) const { return _nHorizontal; };

			//! Get number of vertical image fields.
			unsigned char vertImgFields(void) const { return _nVertical; };

			//! Set number of image fields.
			void setNumbImgFields(unsigned char horz, unsigned char vert);

			//! Determine number of image fields.
			virtual void recalcNumbImgFields(void);

			//!< Recalculates the current texture bounding rectangles
			void calculateBoundingRectangles();

			//! Calculate area covered by image fields.
			float calcFieldArea(unsigned char horz, unsigned char vert);

		protected:
			//! Mouse button event handler.
			virtual void mousePressEvent(QMouseEvent *m);

			//! Mouse double click event handler.
			virtual void mouseDoubleClickEvent(QMouseEvent *m);

			//! Mouse event handler.
			virtual void mouseMoveEvent(QMouseEvent *m);

			//! Mouse wheel event handler.
			virtual void wheelEvent(QWheelEvent *m);

			//! Key event handler
			virtual void keyPressEvent(QKeyEvent *m);

		Q_SIGNALS:
			//! Mouse button event handler.
			void onMouseEvent(QMouseEvent *m);

		private:

			TextureMap		_aTexture;							//!< OpenGL texture IDs
			RenderOrder		_renderOrder;						//!< image field render order
			unsigned char	_nHorizontal;						//!< number of horizontal image fields
			unsigned char	_nVertical;							//!< number of vertical image fields

			float			_fRotation;							//!< The texture rotation
			bool			_mirrorX;							//!< Mirrors on the X axis
			bool			_mirrorY;							//!< Mirrors on the Y axis
			bool			_bShowIntegrationTime;				//!< A flag to indicate of the integration time should be drawn

			TextureBoundingMap	_textureBoundingMap;			//!< The current bounding rectangles
			QMutex				_mutexTextureUpdate;			//!< A mutex to lock texture updates

			Viewer::Rendering::GLRenderObjectPtr _currentRenderObject;	//!< Determines the last clicked renderObject
		};

		typedef boost::shared_ptr<Viewer2D>	Viewer2DPtr;

}	// namespace Viewer


#endif	// VIEWER2D_H
