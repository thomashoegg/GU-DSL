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

#ifndef SHAREDGL_CONTEXT_H
#define SHAREDGL_CONTEXT_H


#include "ViewerDLL.h"

#include <QThread>
#include <QOpenGLContext>
#include "private/qopenglcontext_p.h"

#include "glwindow.qt.h"
#include "OpenGL/ContextHandleInformation.hpp"

#include <sstream>

#pragma once

namespace Viewer
{
	namespace OpenGL
	{
		// Singleton
		// Pointer Interop Viewer
		// Pointer OpengGLContext


		// makeCurrent()
		class VIEWER_API SharedGLContext
		{
		public:


			static SharedGLContext* getInstance()
			{
				if(!instance)
					instance = new SharedGLContext();

				return instance; // Instantiated on first use.
			}

			void init(Viewer::GLWindowPtr glWindow, QOpenGLContext* qOpenGLContext, Viewer::OpenGL::ContextHandleInformation contextInformation)
			{
				_glWindow = glWindow;
				_qOpenGLContext = qOpenGLContext;
				if(glWindow)
					_glFunctions = glWindow.get();


				_contextInformation = contextInformation;
			}

			//void makeCurrent()
			//{
			//	if(!_qOpenGLContext)
			//		return;

			//	//assert(_isCurrent == false);

			//	lock();

			//	if(_isDebug)
			//	{
			//		std::stringstream ss;
			//		ss << "Called makeCurrent() from Thread " << (int)_lockingThread << std::endl;
			//		OutputDebugStringA(ss.str().c_str());
			//	}

			//	wglMakeCurrent((HDC)_contextInformation._hdc, (HGLRC)_contextInformation._hglrc);
			//	QOpenGLContextPrivate::setCurrentContext(_qOpenGLContext);

			//	if(_isCurrent)
			//		return;

			//	_isCurrent = true;
			//}

			//void doneCurrent()
			//{
			//	if(!_qOpenGLContext)
			//		return;

			//	//assert(_isCurrent == true);

			//	wglMakeCurrent(NULL, NULL);
			//	QOpenGLContextPrivate::setCurrentContext(NULL);

			//	if(_isDebug)
			//	{
			//		std::stringstream ss;
			//		ss << "Called doneCurrent() from Thread " << (int)_lockingThread << std::endl;
			//		OutputDebugStringA(ss.str().c_str());
			//	}


			//	unlock();

			//	if(!_isCurrent)
			//		return;

			//	_isCurrent = false;
			//}

			void makeCurrent()
			{
				makeCurrent(nullptr, _qOpenGLContext, _contextInformation);
			}

			void makeCurrent(QSurface *surface, QOpenGLContext* qOpenGLContext, Viewer::OpenGL::ContextHandleInformation contextInformation)
			{
				if(!qOpenGLContext)
					return;

				if((intptr_t)wglGetCurrentContext() == contextInformation._hglrc &&
				    (intptr_t)wglGetCurrentDC() == contextInformation._hdc)
					return;

			//	assert(_isLocalCurrent == false);

				lock();


				if(_isDebug)
				{
					std::stringstream ss;
					ss << "Called makeCurrent(QOpenGLContext*, OpenGLContextHandleInformation ) from Thread " << (int)_lockingThread << std::endl;
					OutputDebugStringA(ss.str().c_str());
				}

				if (surface == nullptr || qOpenGLContext->thread() != QThread::currentThread())
				{
					assert(contextInformation._hdc != 0 && contextInformation._hglrc != 0);

					wglMakeCurrent((HDC)contextInformation._hdc, (HGLRC)contextInformation._hglrc);
					QOpenGLContextPrivate::setCurrentContext(qOpenGLContext);
				}
				else qOpenGLContext->makeCurrent(surface);

				if(_isLocalCurrent)
					return;

				_isLocalCurrent = true;
			}

			void doneCurrent()
			{
				doneCurrent(_qOpenGLContext);
			}

			void doneCurrent(QOpenGLContext* qOpenGLContext)
			{
				if(!qOpenGLContext)
					return;

			//	assert(_isLocalCurrent == true);

			
				if(_isDebug)
				{
					std::stringstream ss;
					ss << "Called doneCurrent(QOpenGLContext*) from Thread " << (int)_lockingThread << std::endl;
					OutputDebugStringA(ss.str().c_str());
				}

				if (qOpenGLContext->thread() != QThread::currentThread())
				{
					wglMakeCurrent(NULL, NULL);
					QOpenGLContextPrivate::setCurrentContext(NULL);
				}
				else qOpenGLContext->doneCurrent();

				unlock();

				if(!_isLocalCurrent)
					return;

				_isLocalCurrent = false; 
			}

			QOpenGLContext* sharedContext()
			{
				if(!_qOpenGLContext)
					return nullptr;

				//if(!_isLocalCurrent || _lockingThread != QThread::currentThreadId())
				//	throw std::exception("If this happens, someone tries to access the context from a different thread or did not call makeCurrent()");

				return _qOpenGLContext;
			}

			QOpenGLFunctions_4_3_Core* functions()
			{
				return _glFunctions;
			}



			Viewer::GLWindow* getWindow() const { return _glWindow.get(); }

			bool isSharedContextCurrent() const { return _isCurrent; }


		private:
			SharedGLContext() 
				: _contextInformation(0, 0), _mutex(QMutex::Recursive), _isCurrent(false)
			{
				_qOpenGLContext = nullptr;
				_isDebug = false;
				_isLocalCurrent = false;
			}; 

			SharedGLContext(SharedGLContext const&);            // Don't Implement
			SharedGLContext& operator=(SharedGLContext const&); // Don't implement

			void lock()
			{
				_mutex.lock();

				_lockingThread = QThread::currentThreadId();
			}

			void unlock()
			{
				_mutex.unlock();

				_lockingThread = 0;
			}

			QOpenGLFunctions_4_3_Core*    _glFunctions;
			QOpenGLContext* _qOpenGLContext;
			Viewer::GLWindowPtr _glWindow;
			Viewer::OpenGL::ContextHandleInformation _contextInformation;
			bool _isCurrent;
			bool _isLocalCurrent;
			Qt::HANDLE _lockingThread;

			QMutex _mutex;

			static SharedGLContext* instance; // Guaranteed to be destroyed.

			bool _isDebug;

		};

	}

}


#endif		// SHAREDGL_CONTEXT_H
