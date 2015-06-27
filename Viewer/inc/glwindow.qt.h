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

#ifndef GLWINDOW_H
#define GLWINDOW_H

#include "ViewerDLL.h"

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions_4_3_Core>
#include <QMutex>
#include <QMatrix4x4>

#include <pshpack8.h>

#include "boost/shared_ptr.hpp"

#include "OpenGL/ContextHandleInformation.hpp"

#include "Primitives/Point2.h"
#include "Primitives/Point3.h"
#include "Primitives/Line2.h"
#include "Primitives/Line3.h"
#include "Primitives/Rect2.h"
#include "Primitives/Rect3.h"
#include "Primitives/Triangle2.h"
#include "Primitives/Triangle3.h"
#include "Primitives/Circle2.h"

#include "Rendering/vertexformat.h"
#include "Primitives/Circle3.h"

QT_BEGIN_NAMESPACE
class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;
class QOpenGLShader;
class QOpenGLShaderProgram;
class QMouseEvent;
QT_END_NAMESPACE

typedef unsigned char byte;

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
	


		class VIEWER_API GLWindow : public QWindow, public QOpenGLFunctions_4_3_Core
		{
			Q_OBJECT
			
		public:
			static const float s_bgrColor[4];

			typedef std::vector<Viewer::Rendering::GLRenderObjectPtr> GLRenderObjectList;
			typedef std::vector<Viewer::Rendering::GLPipelinePtr> GLPipelineList;
		public:
			explicit GLWindow(QWindow *parent = 0);
			~GLWindow();

			virtual void render(QPainter *painter);
			virtual void render();

			virtual void initialize();
			virtual void cleanup();

			void setAnimating(bool animating);

			virtual int addRenderObject();
			int addRenderObject(Viewer::GLWindow* window);
			int addRenderObject(int width, int height);
			int addRenderObject(Rendering::GLRenderObjectPtr renderObject);

			virtual bool removeRenderObject(int index);

			virtual bool removeRenderObject(std::string name);

			virtual bool removeRenderObject(Rendering::GLRenderObjectPtr renderObject);

			bool enableRenderObject(int index, bool enable);

			int removeAllRenderObjects();

			//! Determine number of image fields.
			virtual void recalcNumbImgFields(void) {}

			int getRenderObjectCount() const { return (int)_renderObjects.size(); }

			Viewer::Rendering::GLRenderObjectPtr getRenderObject(int index);

			Viewer::Rendering::GLRenderObjectPtr getRenderObject(std::string name);

			/**
			* \brief Returns the current index of the render object in the object list
			*
			* \return int The current index of the render object
			* \param  Viewer::Rendering::GLRenderObjectPtr obj The render object to find
			*/
			int getRenderObjectIndex(Viewer::Rendering::GLRenderObjectPtr obj);

			/**
			* \brief Determines if the depth test is enabled
			*
			* \return true - if successful, else false
			*/
			bool isDepthTestEnabled() const { return _isDepthTestEnabled; }



			/**
			* \brief Activates or deactivates depth testing
			*
			* \param  bool enable	True, to enable depth testing, false to disable it
			*/
			void enableDepthTest(bool enable) { _isDepthTestEnabled = enable; }

			void setViewedLayer(int layerNumb, int objectIndex = 0, int frameIndex = 0);

			void setMinMax(float min, float max, int index = 0);

			void useJetColor(bool use, int index = 0);

			void update();

			Viewer::GLWindow::GLPipelineList pipelines() const { return _pipelines; }

			// Primitives
			int addPoints(std::string name, const std::vector<Geometry::Primitives::Point2f>& points, float pointSize, byte r, byte g, byte b);

			int addPoints(std::string name, const std::vector<Geometry::Primitives::Point3f>& points, float pointSize, byte r, byte g, byte b);

			int addLines(std::string name, const std::vector<Geometry::Primitives::Line2f>& lines, float lineWidth, byte r, byte g, byte b);

			int addLines(std::string name, const std::vector<Geometry::Primitives::Line3f>& lines, float lineWidth, byte r, byte g, byte b);

			int addRectangles(std::string name, const std::vector<Geometry::Primitives::Rect2f>& rectangles, float lineWidth, byte r, byte g, byte b);

			int addRectangles(std::string name, const std::vector<Geometry::Primitives::Rect3f>& rectangles, float lineWidth, byte r, byte g, byte b);

			int addTriangles(std::string name, const std::vector<Geometry::Primitives::Triangle2f>& triangles, float lineWidth, byte r, byte g, byte b);

			int addTriangles(std::string name, const std::vector<Geometry::Primitives::Triangle3f>& triangles, float lineWidth, byte r, byte g, byte b);

			int addCircles(std::string name, const std::vector<Geometry::Primitives::Circle2f>& circles, float lineWidth, byte r, byte g, byte b);

			int addCircles(std::string name, const std::vector<Geometry::Primitives::Circle3f>& circles, float lineWidth, byte r, byte g, byte b);

			int addPolyLines(std::string name, const std::vector<Geometry::Primitives::Polygonf>& polygons, float lineWidth, byte r, byte g, byte b);

		public Q_SLOTS:
			void renderLater();
			void renderNow();



		protected:
			bool event(QEvent *event);

			virtual void resizeEvent(QResizeEvent *event);

			void exposeEvent(QExposeEvent *event);

			void updateTexture();

			void saveToFile(QString path);


			virtual void addDefaultObject();

			void renderText(double x, double y, double z, QString txt);

			void UpdateFrameRate();

			int addPrimitives( std::string name, const std::vector<Geometry::Primitives::Polygonf>& primitives, float lineWidth, byte r, byte g, byte b);

		public:

			bool initContext();

			QOpenGLContext*	acquireGLContext()
			{
				_contextLockMutex->lock();
				return _context;
			}

			void releaseGLContext() { _contextLockMutex->unlock(); }


			QOpenGLContext*	getGLContext() const { return	_context; }

			Viewer::OpenGL::ContextHandleInformation getContextInformation() const { return _contextInformation; }

			void makeCurrent();
			void doneCurrent();

		protected:
			bool _update_pending;
			bool _animating;

			QMutex*				_contextLockMutex;
			QOpenGLContext*		_context;
			QOpenGLPaintDevice*	_device;

			QMutex				_imageMutex;					//!< The image mutex
			bool				_isInitialized;					//!< Determines if the window is initialized

			GLPipelineList		_pipelines;						//!< A list of possible piplines used for rendering

			GLRenderObjectList	_renderObjects;					//!< The buffers used for rendering

			GLRenderObjectList	_deletedObjects;				//!< The buffers used for rendering

			QMatrix4x4			_viewMatrix;					//!< The view matrix
			QMatrix4x4			_projectionMatrix;				//!< The projection matrix

			QString				_filenameVertexShader;			//!< The vertex shader file name
			QString				_filenameFragmentShader;		//!< The fragment shader file name
			bool				_isDepthTestEnabled;			//!< Determines if the depth testing is enabled
			float*				_clearColor;					//!< The current clear color;
			bool				_initialized;					//!< Detemrines if the windows is already initialized

			bool				_needRecalc;					//!< Determines if the layout is recalculated during the next draw call

			QMutex				_mutexUpdateVertexData;			//!< A vertex update mutex to avoid render collisions during an update		

			UINT				_fps;
			UINT				_frameCount;
			UINT				_lastFrameCount;
			DWORD				_lastTick;

			bool				_isCleaned;

			float				_minRange;
			float				_maxRange;

			Viewer::OpenGL::ContextHandleInformation _contextInformation;
		};	

		typedef boost::shared_ptr<GLWindow>	GLWindowPtr;

	
}	// namespace Viewer

#endif	// GLWINDOW_H
