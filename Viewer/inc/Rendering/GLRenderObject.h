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


#ifndef COMMONBASICS_GLRENDEROBJECT_H
#define COMMONBASICS_GLRENDEROBJECT_H

#if WIN32
#pragma once;
#endif.

#include "ViewerDLL.h"

#include "QObject"
#include "QtGui/QOpenGLFunctions_4_3_Core"

#include "boost/shared_ptr.hpp"

#include "Rendering/definitions.h"
#include "Rendering/vertexformat.h"
#include "Rendering/GLPipeline.h"
#include "glwindow.qt.h"

#include "Primitives/vertex.h"

#include "QMatrix4x4"

#include "Images/Image.h"
#include <QtGui/QOpenGLFramebufferObject>


class MayaCamera;

QT_BEGIN_NAMESPACE
class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;
class QOpenGLShader;
class QOpenGLShaderProgram;
class QOpenGLFramebufferObject;
QT_END_NAMESPACE

#define BUFFER_OFFSET(i) ((char *)NULL + (i))



#define GLRENDEROBJECT_FLAG_NONE			0x00000000
#define GLRENDEROBJECT_FLAG_ISDEFAULT		0x00000001


namespace Viewer
{
	namespace Rendering
	{
		/**
		 * \brief The class encapsulates array buffer functionality
		 *
		 * The class contains the full implementation of vertex, array and element buffer creation and deletion
		 */
		class VIEWER_API GLRenderObject
		{
		public:

			struct Viewport
			{
				Viewport()
				{
					x = y = width = height = 0;
				}

				Viewport(float x, float y, float width, float height)
				{
					this->x = x;
					this->y = y;
					this->width = width;
					this->height = height;
				}
				
				float x;
				float y;

				float width;
				float height;
			};

		public:

			typedef boost::shared_ptr<QOpenGLFramebufferObject>		QOpenGLFramebufferObjectPtr;


			/**
			* \brief The render object constructor
			*
			* Constructor of a render object
			*
			* \param  Viewer::Rendering::GLWindow* coreFunctions The render window
			* \param  QOpenGLContext * context The current rendering context
			*/
			GLRenderObject( Viewer::GLWindow* renderWindow);

			
			/**
			* \brief The render object constructor
			*
			* Constructor of a render object
			*
			* \param  Viewer::Rendering::GLWindow* coreFunctions The render window
			* \param  int width The default texture width
			* \param  int height The default texture height
			*/
			GLRenderObject( Viewer::GLWindow* renderWindow, int width, int height);

			/**
			* \brief Creates a render object in the global shared window
			*
			* Constructor of a render object
			*
			*/
			GLRenderObject();

			
			/**
			* \brief Creates a render object in the global shared window
			*
			* Constructor of a render object
			*
			* \param  int width The default texture width
			* \param  int height The default texture height
			*/
			GLRenderObject(int width, int height);

			/**
			* Destructor of the buffer objects
			*
			*/
			~GLRenderObject();



			/**
			* \brief Initializes the render object to represent 2D texture images
			*
			* \return bool true, if successfully initialized, else false
			* \param  const std::vector<Viewer::Rendering::vertex_v2t2> & vertices A list of vertices
			* \param  const std::vector<unsigned int> & indices A list of indices
			* \param  unsigned int textureId An external texture id
			* \param  unsigned int witdh The new width of the object corresponding to the internal/external texture width
			* \param  unsigned int height The new height of the object corresponding to the internal/external texture height
			*/
			bool init2D(const std::vector<Geometry::Primitives::vertex_v2t2>& vertices, const std::vector<unsigned int>& indices, unsigned int textureId, unsigned int witdh, unsigned int height);



			/**
			* \brief Initialization of the array object.
			*
			* \return true - if successful, else false
			* \param  Viewer::Rendering::GLPipelinePtr pipeline The pipeline to use for rendering
			*/
			bool init(Viewer::Rendering::GLPipelinePtr pipeline);

			/**
			* \brief Initialization of the array object.
			*
			* \return true - if successful, else false
			* \param  Viewer::Rendering::GLPipelinePtr pipeline The pipeline to use for rendering
			* \param const QOpenGLFramebufferObjectFormat* format The framebuffer format
			*/
			bool init(Viewer::Rendering::GLPipelinePtr pipeline, const QOpenGLFramebufferObjectFormat* format);

			/**
			* \brief Initialization of the array object.
			*
			* \return true - if successful, else false
			* \param  Viewer::Rendering::GLPipelinePtr pipeline The pipeline to use for rendering
			* \param const QOpenGLFramebufferObjectFormat* format The framebuffer format
			* \param GLuint externalFormat The external texture format
			*/
			bool init(Viewer::Rendering::GLPipelinePtr pipeline, const QOpenGLFramebufferObjectFormat* format, GLuint externalFormat);

			/**
			* \brief Initializes the render object with a custom rendering pipeline. Note: This deactivates the standard rendering function
			*
			* \return bool
			* \param  std::string filenameVertexShader	The vertex shader file name
			* \param  std::string filenameFragmentShader The fragment shader file name
			*/
			bool init(std::string filenameVertexShader, std::string filenameFragmentShader, int fboWidth, int fboHeight);

			/**
			* \brief Initializes the render object with a custom rendering pipeline. Note: This deactivates the standard rendering function
			*
			* \return bool
			* \param  std::string filenameVertexShader	The vertex shader file name
			* \param  std::string filenameFragmentShader The fragment shader file name
			* \param  std::string filenameGeometryShader The geometry shader file name
			*/
			bool init(std::string filenameVertexShader, std::string filenameFragmentShader, std::string filenameGeometryShader, int fboWidth, int fboHeight);

			
			/**
			* \brief Sets custom shaders which are used during initialization
			*
			* \return bool
			* \param  const QString& filenameVertexShader	The vertex shader file name
			* \param  const QString& filenameFragmentShader The fragment shader file name
			* \param  const QString& filenameGeometryShader The geometry shader file name
			*/
			void setCustomShaders(const QString&filenameVertexShader, const QString& filenameFragmentShader, const QString& filenameGeometryShader);


			/**
			* \brief Set an external texture id
			*
			* Sets an external texture id. In threaded environments, just a flag is set and the data is stored.
			* Then before the rendering occurs, the texture data is updated. In non threaded environments, the texture
			* is directly updated
			* 
			* \param  unsigned int textureId	The new texture id
			*/
			void setExternalTextureID(unsigned int textureId);

			/**
			* \brief Set the current texture data
			*
			* Sets the current texture data. In threaded environments, just a flag is set and the data is stored.
			* Then before the rendering occurs, the texture data is updated. In non threaded environments, the texture
			* is directly updated
			* 
			* \param  Container::Images::Image image	The new image
			*/
			void setImage(Container::Images::Image image);

			/**
			* \brief Returns a reference to the internally stored model matrix
			*
			* \return QMatrix4x4& The internal model matrix
			*/
			QMatrix4x4& getModelMatrix() { return _modelMatrix; }

			/**
			* \brief Sets the internal model matrix
			*
			* \param  const QMatrix4x4 & modelMatrix The new model matrix
			*/
			void setModelMatrix(const QMatrix4x4& modelMatrix) { _modelMatrix = modelMatrix; }

			/**
			* \brief Updates all transformation matrices
			*
			* Updates all transformation matrices and copies the content to the GPU. The internally stored
			* model matrix is used.
			*
			* \return void
			* \param  QMatrix4x4 & view		The view transformation matrix
			* \param  QMatrix4x4 & projection	The projection matrix
			*/
			virtual void updateTransformation(QMatrix4x4& view, QMatrix4x4& projection);

			
			/**
			* \brief Updates all transformation matrices
			*
			* Updates all transformation matrices and copies the content to the GPU
			*
			* \return void
			* \param  QMatrix4x4 & model	The model transformation matrix
			* \param  QMatrix4x4 & view		The view transformation matrix
			* \param  QMatrix4x4 & projection	The projection matrix
			*/
			virtual void updateTransformation(QMatrix4x4& model, QMatrix4x4& view, QMatrix4x4& projection);

			/**
			* \brief Set the vertex and index data
			*
			* \param  const vertex_v2fv2f * vertices	The new vertex data
			* \param  int nVertices The number of vertices
			*/
			void setVertexData(const void* vertices, int nVertices);

			/**
			* \brief Set the vertex and index data
			*
			* \param  const vertex_v2fv2f * vertices	The new vertex data
			* \param  int nVertices The number of vertices
			* \param  const unsigned int * indices The corresponding indices
			* \param  int nIndices The number of inidices
			*/
			void setVertexData(const void* vertices, int nVertices, const unsigned int* indices, int nIndices, VERTEX_FORMAT::Format format);

			/**
			* \brief Adds new vertex data to the viewer
			*
			* \param  unsigned int vbo The vertex data as VBO
			* \param  int countVertices The number of vertices
			* \param  int countIndices The number of indices
			*/
			void setVertexData(unsigned int vbo, int countVertices, int countIndices, Viewer::Rendering::VERTEX_FORMAT::Format format);

			/**
			* \brief Set the rendering primitive type
			*
			* \param  int primitiveType The primitive type, e.g. GL_POINTS, GL_LINES, GL_TRIANGLES, ...
			*/
			void setPrimitiveType(int primitiveType) { _primitiveType = primitiveType; }

			/**
			* \brief Renders the array buffer
			*/
			virtual void render();

			/**
			* \brief Sets the color attachments to the fbo
			*
			* \param  std::vector<GLenum> attachments A list FBO attachments
			* \param  std::vector<GLuint> textures A list of corresponding texture elements
			*/
			virtual bool setFBOAttachments(std::vector<GLenum> attachments, std::vector<GLuint> textures);


			/**
			* \brief Returns a true, to indicate that the buffer is rendered
			*
			* \return true, if enabled, else false
			*/
			bool isEnabled() const { return _isEnabled; }


			/**
			* \brief Determines if the buffer should be rendered
			*
			* \return bool true, if the buffer was previously rendered, else false
			* \param  bool enable Determines if the buffer is enabled
			*/
			bool setEnabled(bool enable) { bool oldVal = _isEnabled; _isEnabled = enable; return oldVal; }

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

			/**
			* \brief Returns the currently set vertex color
			*
			* \return The fixed vertex color
			*/
			Geometry::Primitives::vec4 getVertexColor() const { return _vertexColor; }


			/**
			* \brief Sets the fixed vertex color
			*
			* \param  Geometry::Primitives::vec4 color The fixed vertex color
			*/
			void setVertexColor(Geometry::Primitives::vec4 color) { _vertexColor = color; }

			/**
			* \brief Returns the currently set line width
			*
			* \return The line width
			*/
			float getLineWidth() const { return _lineWidth; }

			/**
			* \brief Sets the line width
			*
			* \param  float lineWidth The line width
			*/
			void setLineWidth(float lineWidth) { _lineWidth = lineWidth; }

			
			/**
			* \brief Returns the currently set point size
			*
			* \return The point size
			*/
			float getPointSize() const { return _pointSize; }

			/**
			* \brief Sets the point size
			*
			* \param  float pointSize The point size
			*/
			void setPointSize(float pointSize) { _pointSize = pointSize; }

			/**
			* \brief Determines if the fixed view translation is used
			*
			* \return true - if the view translation is used, else false
			*/
			bool isUseFixedViewTranslation() const { return _useFixedViewTranslation; }

			/**
			* \brief Enables or disables the usage of a fixed view translation
			*
			* \param  bool use true to enable the usage of a fixed view translation
			*/
			void setUseFixedViewTranslation(bool use) { _useFixedViewTranslation = use; }

			
			/**
			* \brief Returns the fixed view translation
			*
			* \return The fixed view translation
			*/
			Geometry::Primitives::vec3 getFixedViewTranslation() const { return _vertexFixedViewTranslation; }


			/**
			* \brief Sets the fixed view translation
			*
			* \param  Geometry::Primitives::vec4 color The fixed view translation
			*/
			void setFixedViewTranslation(Geometry::Primitives::vec3 translation) { _vertexFixedViewTranslation = translation; }

			/**
			* \brief Determines if the fixed vertex color is used
			*
			* \return true - if the fixed color is used, else false
			*/
			bool isUseFixedVertexColor() const { return _useFixedVertexColor; }

			/**
			* \brief Enables or disables the usage of a fixed vertex color
			*
			* \param  bool use true to enable the usage of a fixed vertex color
			*/
			void setUseFixedVertexColor(bool use) { _useFixedVertexColor = use; }

			/**
			* \brief Determines if the texture is interpreted as normal map
			*
			* \return true - if the texture is interpreted as normal map
			*/
			bool isInterpretAsNormalImage() const { return _interpretAsNormalImage; }

			/**
			* \brief Determines if the texture is interpreted as normal map
			*
			* \param  bool value true  to interpret the texture as normal map
			*/
			void setInterpretAsNormalImage(bool value) { _interpretAsNormalImage = value; }

			
			/**
			* \brief Determines if the texture is interpreted as Vertex map
			*
			* \return true - if the texture is interpreted as Vertex map
			*/
			bool isInterpretAsVertexImage() const { return _interpretAsVertexImage; }

			/**
			* \brief Determines if the texture is interpreted as Vertex map
			*
			* \param  bool value true  to interpret the texture as Vertex map
			*/
			void setInterpretAsVertexImage(bool value) { _interpretAsVertexImage = value; }


			/**
			* \brief Determines if the view transformation matrix is always set to the identity matrix
			*
			* \return true - if set, else false
			*/
			bool isIgnoreViewTransformation() const { return _ignoreViewTransformation; }


			/**
			* \brief Determines if the view transformation matrix is always set to the identity matrix
			*
			* \param  bool ignore use true to ignore the view transformation
			*/
			void setIgnoreViewTransformation(bool ignore) { _ignoreViewTransformation = ignore; }

			/**
			* \brief Returns the name of the object
			*
			* \return std::string The object name
			*/
			std::string getName() const { return _name; }

			/**
			* \brief Sets the name of the object
			*
			* \param  std::string name The object name
			*/
			void setName(std::string name) { _name = name; }

			
			/**
			* \brief Returns the current viewport
			*
			* \return Viewer::Rendering::GLRenderObject::Viewport The currently set Viewport
			*/
			Viewport getViewport() const { return _viewport; }

			/**
			* \brief Sets the viewport
			*
			* \param  float x	The x-coordinate
			* \param  float y	The y-coordinate
			* \param  float width 	The width
			* \param  float height 	The height
			*/
			void setViewport(float x, float y, float width, float height) { _viewport = Viewport(x, y, width, height); }

			/**
			* \brief Sets the viewport position. The size is kept
			*
			* \param  float x	The x-coordinate
			* \param  float y	The y-coordinate
			*/
			void setViewport(float x, float y) { _viewport.x = x; _viewport.y = y; }

			/**
			* \brief Sets the viewport.
			*
			* \param  Viewport viewport The viewport
			*/
			void setViewport(Viewport viewport) { _viewport = viewport; }


			/**
			* \brief Returns the set flags
			*
			* \return int The flag mask
			*/
			int getFlags() const { return _flags; }


			/**
			* \brief Applies a new flag to the flag mask
			*
			* \param  int flag The new flag to apply
			*/
			void setFlag(int flag) { _flags |= flag; }

			/**
			* \brief Tests if a flag is set
			*
			* \return bool true, if the flag is set, else false
			* \param  int flag The test flag
			*/
			bool isFlagSet(int flag) const { return ((_flags & flag) == flag); }


			/**
			* \brief Sets the layer to view
			*
			*
			* \return void
			* \param  int layerNumb The layer number
			*/
			void setViewedLayer(int layerNumb, int frameIndex = 0);


			/**
			* \brief Returns the currently shown layer index
			*
			* \return unsigned int The layer index
			*/
			unsigned int viewedLayer() { return _viewedLayer; }


			/**
			* \brief Returns the currently shown frame index
			*
			* \return unsigned int The frame index
			*/
			unsigned int viewedFrameIndex() { return _viewedFrameIndex; }


			void setMinMax(float min, float max, bool forceValues = false);


			void useJetColor(bool use);

			
			/**
			* \brief Returns the currently active texture id
			*
			* \return unsigned it the currently active texture id
			*/
			unsigned int textureId() { return _textureNameExternal ? _textureNameExternal : _textureName; }

			

			/**
			* \brief Returns the internal framebuffer object
			*
			* \return Viewer::Rendering::GLRenderObject::QOpenGLFramebufferObjectPtr The internal framebuffer object
			*/
			QOpenGLFramebufferObjectPtr frameBuffer() const { return _frameBuffer; }


			/**
			* \brief Returns the internal rendering pipeline
			*
			* \return Viewer::Rendering::GLPipelinePtr The default internal pipeline
			*/
			GLPipelinePtr pipeline() const { return _pipeline; }

			/**
			* \brief Returns the vertex array buffer object id
			*
			* \return GLuint The vertex array buffer object id
			*/
			GLuint vertexArray() const { return _vertexArrayName; }

			/**
			* \brief Returns the vertex buffer object id
			*
			* \return GLuint The vertex buffer object id
			*/
			GLuint vertexBuffer() const { return _bufferName[buffer::VERTEX]; }

			/**
			* \brief Resizes the internal vertex buffer object to the passed size
			*
			* \param  VERTEX_FORMAT::Format format The new vertex format
			* \param  int numberOfVertices The number of vertices
			* \param  int numberOfIndices The number of indices
			*/
			void resizeVertexBuffer(VERTEX_FORMAT::Format format, int numberOfVertices, int numberOfIndices = -1);

			/**
			* \brief Resizes the internal vertex buffer object to the passed size
			*
			* \param  VERTEX_FORMAT::Format format The new vertex format
			* \param  const void* vertices The vertex buffer
			* \param  int numberOfVertices The number of vertices
			* \param  int numberOfIndices The number of indices
			*/
			void resizeVertexBuffer(VERTEX_FORMAT::Format format, const void* vertices, int numberOfVertices, int numberOfIndices = -1);

			
			/**
			* \brief Sets the image to be treated as a color image
			*
			* \param  bool isColor true, if the image should be drawn as colored image, false to use jet coloring
			*/
			void setAsColorImage(bool isColor, bool convertSourceFromByteToFloat = false) { _isColorImage = isColor; _colorImageConvertSourceFromByteToFloat = convertSourceFromByteToFloat; }

			/**
			* \brief Returns the number of frames of the current image 
			*
			* \return size_t The number of frames of the current image
			*/
			size_t frameCount();

			/**
			* \brief Determines the layer count of the currently attached image
			*
			* \param  unsigned int frameIndex The selected frame index
			* \return size_t The layer count
			*/
			size_t layerCount(unsigned int frameIndex = 0);


			/**
			* \brief Tries to receive the current image min and max range value
			*
			* \return void
			* \param  float & min The default min range value
			* \param  float & max The default max range value
			* \param  unsigned int frameIndex The selected frame index
			*/
			void getCurrentImageMinMaxRange( float &min, float &max, unsigned int frameIndex = 0 );

			
			/**
			* \brief Returns the width of the currently active image
			*
			* \return int The width of the image
			*/
			int currentImageWidth() const { return _currentImageWidth; }

			/**
			* \brief Returns the height of the currently active image
			*
			* \return int The height of the image
			*/
			int currentImageHeight() const { return _currentImageHeight; }

			/**
			* \brief If true, it is not possible to set the min max values again
			*
			* \return bool True, if the min max values are forced
			*/
			bool isForceMinMaxValues() const { return _forceMinMaxValues; }

			/**
			* \brief If true, it is not possible to set the min max values again
			*
			* \param  bool val If true, the min max values are forced
			*/
			void setForceMinMaxValues(bool val) { _forceMinMaxValues = val; }


			/**
			* \brief Updates the texture data
			*
			* The method updates the first available texture data and determines if the 
			* image is colored.
			*/
			void updateTexture();

			/**
			* \brief Saves the current texture to a file
			*
			* \return void
			* \param  QString path The destination file path.
			*/
			void saveToFile(QString path);

			/**
			* \brief Determines if the object has virtual camera support
			*
			* \return true - if the object has virtual camera support
			*/
			bool isUseCamera() const { return _useCamera; }

			/**
			* \brief Enables or disables virtual camera support
			*
			* \param  bool use true to enable virtual camera support
			*/
			void setUseCamera(bool use) { _useCamera = use; }


			MayaCamera* getCamera() const { return _virtMayaCamera; }

			
			/**
			* \brief Updates the vertex data on the GPU
			*
			*/
			void updateVertexData();

		public:
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

		protected:

			/**
			* \brief Initializes the object with default values
			*
			* \param  int width The required width
			* \param  int height The required height
			*/
			void initDefaults(int width, int height);


			/**
			* \brief Initializes the array buffer
			*
			* This method initializes the OpenGL array buffer
			*
			* \return bool true - if successful
			*/
			bool initArrayBuffer();

			/**
			* \brief Initializes the array buffer
			*
			* This method initializes the OpenGL texture
			*
			* \param const QOpenGLFramebufferObjectFormat* format The requested frame buffer format
			*
			* \return bool true - if successful
			*/
			bool initTexture2D(const QOpenGLFramebufferObjectFormat* format);

			/**
			* \brief Initializes the array buffer
			*
			* This method initializes the OpenGL vertex array
			*
			* \return bool true - if successful
			*/
			bool initVertexArray();

			/**
			* \brief Initializes the array buffer
			*
			* This method initializes the OpenGL uniform buffers 
			*
			* \return bool true - if successful
			*/
			bool initUniformBuffer();

			/**
			* \brief Initializes the debug output
			*
			* This method initializes the OpenGL debug output
			*
			* \return bool true - if successful
			*/
			bool initDebugOutput();


			/**
			* \brief Updates the vertex data on the GPU
			*
			* \return void
			* \param  void* vertexData The new vertex data
			* \param  unsigned int* indexData The new index data
			*/
			void updateVertexData(void*	vertexData, unsigned int* indexData);


			/**
			* \brief Returns the size in bytes of the currently selected vertex format
			*
			* \return int The number of bytes of the used vertex format
			*/
			int sizeOfVertexFormat() const;

			
			/**
			* \brief Initializes the vertex data structure with the selected format
			*/
			void initVertices(int count);

			/**
			* \brief Indicates if the object is already initialized
			*
			* \return bool true, if already initialized, else false
			*/
			bool isObjectInitialized() const { return _objectInitialized; }

		protected:

			Viewer::GLWindow*				_glw;				//!< The OpenGL core function object
			QOpenGLContext*					_context;			//!< An OpenGL rendering context

			GLuint _vertexArrayName;							//!< The OpenGL vertex array Id
			GLuint _bufferName[buffer::MAX];					//!< All intialized buffers
			GLuint _textureName;								//!< The OpenGL texture Id
			GLuint _textureNameExternal;						//!< An external texture Id

			bool _isColorImage;									//!< Determines if the current texture data is colored
			bool _colorImageConvertSourceFromByteToFloat;		//!< Just a hack!!
			bool _isGrayUnsignedShort;							//!< Determines if the current texture data is colored
			bool _isSingleChannel;								//!< Determines if the current texture data is a single channel only

			Container::Images::Image *_currentImage;	//!< The current image

			bool					_imageChanged;				//!< Determines if the texture has changed since the last rendering call
			QMutex					_imageMutex;				//!< The image mutex
			Qt::HANDLE				_creationThreadId;			//!< The thread id during the creation of the object

			GLPipelinePtr			_pipeline;					//!< The rendering pipeline
			int _primitiveType;									//!< The rendering primitive type


			QMutex			_mutexUpdateVertexData;				//!< The mutex for changing vertex data
			int				_countIndices;						//!< The number of indices
			unsigned int*	_indexData;							//!< The current indices
			int				_countVertices;						//!< The number of vertices
			void*			_vertexData;						//!< The vertex data
			bool			_vertexDataUpdated;					//!< Determines if the vertex data was updated

			Geometry::Primitives::vec3			_vertexFixedViewTranslation;		//!< The fixed vertex view translation
			Geometry::Primitives::vec4			_vertexColor;						//!< The fixed vertex color
			bool			_useFixedVertexColor;				//!< Determines if the fixed vertex color should be used
			bool			_useFixedViewTranslation;			//!< Determines if the fixed view translation should be used

			bool			_isEnabled;							//!< Determines if the array buffer should be drawn
			VERTEX_FORMAT::Format _format;						//!< The currently used vertex format
			bool			_useTexture;						//!< Determines if a textures is used
			bool			_useColors;							//!< Determines if colors are used
			bool			_useNormals;						//!< Determines if normals are available
			bool			_isDepthTestEnabled;				//!< Determines if the depth testing is enabled
			bool			_objectInitialized;					//!< Determines if all GL objects are initialized
			QMatrix4x4		_modelMatrix;						//!< The model transformation matrix of the object
			bool			_ignoreViewTransformation;			//!< If this variable is set, the view transformation is set to identity
			int				_currentImageWidth;					//!< The current image width
			int				_currentImageHeight;				//!< The current image height
			std::string		_name;								//!< The object name
			float			_lineWidth;							//!< The current line width
			float			_pointSize;							//!< The current point size
			Viewport		_viewport;							//!< The object viewport

			int				_flags;								//!< A flag field

			int				_viewedLayer;						//!< The layer to view
			int				_viewedFrameIndex;					//!< The frame to view
			bool			_interpretAsNormalImage;			//!< Determines if the texture colors are interpreted as normals
			bool			_interpretAsVertexImage;			//!< Determines if the texture colors are interpreted as vertex image
			bool			_useJetColor;						//!< Determines if jet coloring is used
			float			_minClamp;							//!< The minimum clamp distance
			float			_maxClamp;							//!< The maximum clamp distance
			bool			_hasCustomPipeline;					//!< Determines if a custom pipeline is used

			QOpenGLFramebufferObjectPtr	_frameBuffer;			//!< The internal used framebuffer
			int				_fboWidth;							//!< The framebuffer object width
			int				_fboHeight;							//!< The framebuffer object height

			GLuint			_externalFormat;					//!< The external image format

			bool			_forceMinMaxValues;					//!< Forces the min and max values

			MayaCamera*		_virtMayaCamera;					//!< The object specific camera
			QPoint			_lastPos;							//!< last cursor position
			bool			_useCamera;							//!< Virtual camera support required

			QString			_customFilenameVertexShader;		//!< Defines a custom vertex shader used for initialization
			QString			_customFilenameFragmentShader;		//!< Defines a custom fragment shader used for initialization
			QString			_customFilenameGeometryShader;		//!< Defines a custom geometry shader used for initialization
		};

		/**
		* \brief Type definition for a shared pointer of an array object
		*/
		typedef boost::shared_ptr<GLRenderObject>		GLRenderObjectPtr;

		/**
		* \brief Type definition for a const shared pointer of an array object
		*/
		typedef boost::shared_ptr<const GLRenderObject>	ConstGLRenderObjectPtr;

	}	// namespace Rendering
}	// Basics

#endif	// COMMONBASICS_GLRENDEROBJECT_H
