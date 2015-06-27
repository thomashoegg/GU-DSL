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


#ifndef COMMONBASICS_GLPIPELINE_H
#define COMMONBASICS_GLPIPELINE_H

#if WIN32
#pragma once;
#endif
#include "../ModuleConfig.h"
#include "ViewerDLL.h"
#include "QString"

#include "boost/shared_ptr.hpp"
#if OPENGL_VERSION_MAJOR >= 4
	#if OPENGL_VERSION_MINOR >= 3
		#include <QtGui/QOpenGLFunctions_4_3_Core>
	#elif OPENGL_VERSION_MINOR == 2
		#include <QtGui/QOpenGLFunctions_4_2_Core>
	#elif OPENGL_VERSION_MINOR == 1
		#include <QtGui/QOpenGLFunctions_4_1_Core>
	#elif OPENGL_VERSION_MINOR == 0
		#include <QtGui/QOpenGLFunctions_4_0_Core>
	#endif
#else
	#include <QtGui/QOpenGLFunctions_2_0>
#endif

#include "Rendering/definitions.h"

QT_BEGIN_NAMESPACE
class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;
class QOpenGLShader;
class QOpenGLShaderProgram;
QT_END_NAMESPACE


namespace Viewer
{
	namespace Rendering
	{
		class GLPipeline;

		/**
			* \brief Type definition for a shared pointer of a pipeline object
			*/
		typedef boost::shared_ptr<GLPipeline>		GLPipelinePtr;

		/**
			* \brief Type definition for a const shared pointer of a pipeline object
			*/
		typedef boost::shared_ptr<const GLPipeline>	ConstGLPipelinePtr;

			
		/**
			* \brief Type definition for a shared pointer of a shader program
			*/
		typedef boost::shared_ptr<QOpenGLShaderProgram>	QOpenGLShaderProgramPtr;

		/**
		 * \brief This class encapsulated
		 *
		 */
		class VIEWER_API GLPipeline
		{
		public:

#if OPENGL_VERSION_MAJOR >= 4
	#if OPENGL_VERSION_MINOR >= 3
			/**
			* \brief The pipeline object constructor
			*
			* Constructor of a pipeline object.
			*
			* \param  QOpenGLFunctions_4_3_Core * coreFunctions	An object that provides OpenGL functions
			* \param  QOpenGLContext * context The current rendering context
			*/
			GLPipeline(QOpenGLFunctions_4_3_Core* coreFunctions, QOpenGLContext* context);
	#elif OPENGL_VERSION_MINOR == 2
			/**
			* \brief The pipeline object constructor
			*
			* Constructor of a pipeline object.
			*
			* \param  QOpenGLFunctions_4_3_Core * coreFunctions	An object that provides OpenGL functions
			* \param  QOpenGLContext * context The current rendering context
			*/
			GLPipeline(QOpenGLFunctions_4_2_Core* coreFunctions, QOpenGLContext* context);
	#elif OPENGL_VERSION_MINOR == 1
			/**
			* \brief The pipeline object constructor
			*
			* Constructor of a pipeline object.
			*
			* \param  QOpenGLFunctions_4_3_Core * coreFunctions	An object that provides OpenGL functions
			* \param  QOpenGLContext * context The current rendering context
			*/
			GLPipeline(QOpenGLFunctions_4_1_Core* coreFunctions, QOpenGLContext* context);
	#elif OPENGL_VERSION_MINOR == 0
			/**
			* \brief The pipeline object constructor
			*
			* Constructor of a pipeline object.
			*
			* \param  QOpenGLFunctions_4_3_Core * coreFunctions	An object that provides OpenGL functions
			* \param  QOpenGLContext * context The current rendering context
			*/
			GLPipeline(QOpenGLFunctions_4_0_Core* coreFunctions, QOpenGLContext* context);
	#endif
#else
			/**
			* \brief The pipeline object constructor
			*
			* Constructor of a pipeline object.
			*
			* \param  QOpenGLFunctions_4_3_Core * coreFunctions	An object that provides OpenGL functions
			* \param  QOpenGLContext * context The current rendering context
			*/
			GLPipeline(QOpenGLFunctions_2_0* coreFunctions, QOpenGLContext* context);
#endif
			/**
			* Destructor to clean up OpenGL resources
			*
			*/
			~GLPipeline();

			/**
			* Initializes the pipeline using the given shaders
			*
			* \return bool	true - if the init was successful
			* \param  QString vertexShaderFileName		The vertex shader
			* \param  QString fragmentShaderFileName	The fragment shader
			*/
			bool init(QString vertexShaderFileName, QString fragmentShaderFileName);

			/**
			* Initializes the pipeline using the given shaders
			*
			* \return bool	true - if the init was successful
			* \param  QString vertexShaderFileName		The vertex shader
			* \param  QString fragmentShaderFileName	The fragment shader
			* \param  QString geometryShaderFileName	The geometry shader
			*/
			bool init(QString vertexShaderFileName, QString fragmentShaderFileName, QString geometryShaderFileName);

			/**
			* Bind the pipeline object to the active context
			*/
			void bind();

			/**
			* Unbind the pipeline object from the active context
			*/
			void unbind();

			/**
			* \brief Returns the shader program
			*
			* \return The internal shader program
			*/
			QOpenGLShaderProgramPtr getShaderProgram() { return _shaderProgram; }


			/**
			* \brief Returns the attached rendering context
			*
			* \return The attached OpenGL context
			*/
			QOpenGLContext*	getContext() const { return _context; }

#if OPENGL_VERSION_MAJOR >= 4
	#if OPENGL_VERSION_MINOR >= 3
			QOpenGLFunctions_4_3_Core* glFunctions() { return _glf; }
	#elif OPENGL_VERSION_MINOR == 2
			QOpenGLFunctions_4_2_Core* glFunctions() { return _glf; }
	#elif OPENGL_VERSION_MINOR == 1
			QOpenGLFunctions_4_1_Core* glFunctions() { return _glf; }
	#elif OPENGL_VERSION_MINOR == 0
			QOpenGLFunctions_4_0_Core* glFunctions() { return _glf; }
	#endif
#else
			QOpenGLFunctions_2_0* glFunctions() { return _glf; }
#endif
																																																		
		protected:
#if OPENGL_VERSION_MAJOR >= 4
	#if OPENGL_VERSION_MINOR >= 3
			QOpenGLFunctions_4_3_Core*		_glf;							//!< The OpenGL core function object
	#elif OPENGL_VERSION_MINOR == 2
			QOpenGLFunctions_4_2_Core*		_glf;							//!< The OpenGL core function object
	#elif OPENGL_VERSION_MINOR == 1
			QOpenGLFunctions_4_1_Core*		_glf;							//!< The OpenGL core function object
	#elif OPENGL_VERSION_MINOR == 0
			QOpenGLFunctions_4_0_Core*		_glf;							//!< The OpenGL core function object
	#endif
#else
			QOpenGLFunctions_2_0*		_glf;							//!< The OpenGL core function object
#endif			
			QOpenGLContext*					_context;						//!< An OpenGL rendering context
						
			QOpenGLShaderProgramPtr			_shaderProgram;					//!< An instance to a shader program
			QString							_filenameVertexShader;			//!< The vertex shader file name
			QString							_filenameFragmentShader;		//!< The fragment shader file name
			QString							_filenameGeometryShader;		//!< The geometry shader file name

			GLuint _pipelineName;											//!< The internal OpenGL pipeline Id
			GLuint _programName;											//!< The internal OpenGL program Id
		};
	}
}


#endif	// COMMONBASICS_GLPIPELINE_H
