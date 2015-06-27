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

#include "Rendering/GLPipeline.h"

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLShader>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>

#include <exception>


#if OPENGL_VERSION_MAJOR >= 4
	#if OPENGL_VERSION_MINOR >= 3
Viewer::Rendering::GLPipeline::GLPipeline( QOpenGLFunctions_4_3_Core* coreFunctions, QOpenGLContext* context)
	: _glf(coreFunctions),
	_context(context),
	_programName(0)
{
	if(_context == nullptr)
	{
		QString excepString("%1: %2");
		excepString.arg(__FUNCTION__).arg("source parameter null!");

		throw std::exception(excepString.toLatin1());
	}
}
	#elif OPENGL_VERSION_MINOR == 2
Viewer::Rendering::GLPipeline::GLPipeline( QOpenGLFunctions_4_2_Core* coreFunctions, QOpenGLContext* context)
	: _glf(coreFunctions),
	_context(context),
	_programName(0)
{
	if(_context == nullptr)
	{
		QString excepString("%1: %2");
		excepString.arg(__FUNCTION__).arg("source parameter null!");

		throw std::exception(excepString.toLatin1());
	}
}
	#elif OPENGL_VERSION_MINOR == 1
Viewer::Rendering::GLPipeline::GLPipeline( QOpenGLFunctions_4_1_Core* coreFunctions, QOpenGLContext* context)
	: _glf(coreFunctions),
	_context(context),
	_programName(0)
{
	if(_context == nullptr)
	{
		QString excepString("%1: %2");
		excepString.arg(__FUNCTION__).arg("source parameter null!");

		throw std::exception(excepString.toLatin1());
	}
}
	#elif OPENGL_VERSION_MINOR == 0
Viewer::Rendering::GLPipeline::GLPipeline( QOpenGLFunctions_4_0_Core* coreFunctions, QOpenGLContext* context)
	: _glf(coreFunctions),
	_context(context),
	_programName(0)
{
	if(_context == nullptr)
	{
		QString excepString("%1: %2");
		excepString.arg(__FUNCTION__).arg("source parameter null!");

		throw std::exception(excepString.toLatin1());
	}
}
	#endif
#else
Viewer::Rendering::GLPipeline::GLPipeline( QOpenGLFunctions_2_0* coreFunctions, QOpenGLContext* context)
	: _glf(coreFunctions),
	_context(context),
	_programName(0)
{
	if(_context == nullptr)
	{
		QString excepString("%1: %2");
		excepString.arg(__FUNCTION__).arg("source parameter null!");

		throw std::exception(excepString.toLatin1());
	}
}
#endif

Viewer::Rendering::GLPipeline::~GLPipeline()
{
	if(_shaderProgram)
		_shaderProgram->removeAllShaders();

	if(_pipelineName) _glf->glDeleteProgramPipelines(1, &_pipelineName);
	if(_programName) _glf->glDeleteProgram(_programName);
}


bool Viewer::Rendering::GLPipeline::init(QString vertexShaderFileName, QString fragmentShaderFileName)
{
	return init(vertexShaderFileName, fragmentShaderFileName, "");
}

bool Viewer::Rendering::GLPipeline::init(QString vertexShaderFileName, QString fragmentShaderFileName, QString geometryShaderFileName)
{
	int  error = ::glGetError();
	_glf->glGenProgramPipelines(1, &_pipelineName);
	_glf->glBindProgramPipeline(_pipelineName);

	GLbitfield stages = 0;
	bool bValidated = true;

	if(bValidated)
	{
		_shaderProgram = QOpenGLShaderProgramPtr(new QOpenGLShaderProgram());

		_glf->glProgramParameteri(_shaderProgram->programId(), GL_PROGRAM_SEPARABLE, GL_TRUE);

		if(!vertexShaderFileName.isEmpty())
		{
			_filenameVertexShader = vertexShaderFileName;
			QOpenGLShader* vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
			vertexShader->compileSourceFile(_filenameVertexShader);
			if(vertexShader->isCompiled())
			{
				if(_shaderProgram->addShader(vertexShader))
					stages |= GL_VERTEX_SHADER_BIT;
			}
		}

		if(!fragmentShaderFileName.isEmpty())
		{
			_filenameFragmentShader = fragmentShaderFileName;
			QOpenGLShader* fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
			fragmentShader->compileSourceFile(_filenameFragmentShader);

			if(fragmentShader->isCompiled())
			{
				if(_shaderProgram->addShader(fragmentShader))
					stages |= GL_FRAGMENT_SHADER_BIT;
			}
		}

		if(!geometryShaderFileName.isEmpty())
		{
			_filenameGeometryShader = geometryShaderFileName;
			QOpenGLShader* geoShader = new QOpenGLShader(QOpenGLShader::Geometry);
			geoShader->compileSourceFile(_filenameGeometryShader);

			if(geoShader->isCompiled())
			{
				if(_shaderProgram->addShader(geoShader))
					stages |= GL_GEOMETRY_SHADER_BIT;
			}
		}

		bValidated = _shaderProgram->link();
	}

	GLenum err = _glf->glGetError();
	if(bValidated)
		_glf->glUseProgramStages(_pipelineName, stages, _shaderProgram->programId());

	err = _glf->glGetError();

	_glf->glBindProgramPipeline(0);

	_programName = _shaderProgram->programId();

	return bValidated;
}


void Viewer::Rendering::GLPipeline::bind()
{
	_shaderProgram->bind();
	_glf->glBindProgramPipeline(_pipelineName);
}



void Viewer::Rendering::GLPipeline::unbind()
{
	_glf->glBindProgramPipeline(0);
}

