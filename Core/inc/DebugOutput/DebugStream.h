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

#ifndef DEBUGSTREAM_H_
#define DEBUGSTREAM_H_

#if WIN32
#pragma once
#endif
#include "CoreDLL.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <boost/signals2.hpp>
#include "DebugOutput/ModuleDebugLevel.qt.hpp"
#include "DebugOutput/Logger.h"

#pragma warning( push )
#pragma warning( disable: 4251 )

namespace Core
{

namespace DebugOutput
{



class CORE_API DebugStream
{

public:

	enum DebugLevel
	{ None = 0x00000000, Debug = 0x00000001, Info = 0x00000002, 
	Error = 0x00000004, KernelTime = 0x00000008, All = 0xffffffff };

	typedef boost::signals2::signal<void (const std::string debugLevel, const std::string functionName, const std::string message)> OnMessage;
	OnMessage		MessageSig;

	//! callback function pointer type
	typedef void (*DebugCallback) (DebugLevel l, const std::string functionName, const std::string msg);



	/**
	* \brief Returns the single instance
	*
	* Returns the instance of the singleton PeristentCameraSettings
	*
	* \return DebugStream&		The Instance
	*/
	static DebugStream& instance()
	{
		static DebugStream _instance;
		return _instance;
	}


	/**
	* \brief Destructor
	*
	* Destructor ~DebugStream()
	*
	* \return 
	*/
	//~DebugStream();


	//! returns the current callback for debug output
	inline DebugCallback getCB(void); 

	//! sets the current callback for debug output
	inline void setCB(DebugCallback cb); 

	//! Set the debug level 
	//! @param l the level (bit code according to DebugStream::DebugLevel) 
	inline void setLevel (int l);

	//! Set the debug level according to DebugStream::DebugLevel in clear text 
	//! @param level textual representation (no whitespaces), like  Debug|Info
	static void setLevel (const char *level);

	//! Get the debug level
	//! @return the debug level 
	inline int getLevel (void);


	//---------------------------------------------------------------------------
	//! \name Default Message Handler
	//! \{
	//---------------------------------------------------------------------------

	//! nested class for the default handler
	class DefaultHandler
	{
	public:
		//! Empty default destructor (constructor is private)
		~DefaultHandler(void);

		//! Open an output file
		static bool openFile (const char *fileName);

		//! Close an output file 
		static void closeFile (void);

		//! Get a reference to the currently used output stream 
		static inline std::ostream & getOStr (void);

		//! Set currently to be used output stream 
		static void setOStr (std::ostream& ostr);

		//! Flush all buffered output and exit the program
		static inline void flush (void);

		//! The message handler callback
		static inline void messageCB (DebugLevel l, const std::string functionName, const std::string msg);

	private:
		//! Default constructor 
		DefaultHandler(void);

		static std::ofstream ostr;    //< output stream 
	};

protected:
private:
	DebugStream();
	DebugStream( const DebugStream& ) {};
	DebugStream & operator = (const DebugStream &);


public:
protected:
private:

	static int				_level;			 //!< current debug level
	static DebugCallback	_cb;			//!< callback for debug-output

	
};

#define WRITE(method, a, level) \
{if ( Core::DebugOutput::DebugStream::instance().getLevel() & level ) { \
	Core::DebugOutput::DebugStream::instance().getCB()(level, \
	std::string(method), std::string(a));}}


//! Output if output level is set to allow error output 
#define E_OUT2(method,a) WRITE(method, a, Core::DebugOutput::DebugStream::Error); LOG(ERROR) << a;
#define E_OUT(a) WRITE(__FUNCTION__, a, Core::DebugOutput::DebugStream::Error); LOG(ERROR) << a;
#define CHECK_LEVEL_E_OUT(level1, level2, msg)							\
{if(((unsigned char)level1) & ((unsigned char)level2))					\
{ WRITE(__FUNCTION__, msg, Core::DebugOutput::DebugStream::Error);		\
LOG(ERROR)<<msg;														\
}}

//! Output if output level is set to allow (add.) info output 
#define I_OUT2(method,a) WRITE(method, a, Core::DebugOutput::DebugStream::Info); LOG(INFO) << a;
#define I_OUT(a) WRITE(__FUNCTION__, a, Core::DebugOutput::DebugStream::Info); LOG(INFO) << a;
#define CHECK_LEVEL_I_OUT(level1, level2, msg)							\
{if(((unsigned char)level1) & ((unsigned char)level2))					\
{ WRITE(__FUNCTION__, msg, Core::DebugOutput::DebugStream::Info);		\
LOG(INFO)																\
}}

//! Output if output level is set to allow (add.) debug output
#define D_OUT2(method,a) WRITE(method, a, Core::DebugOutput::DebugStream::Debug); LOG(TRACE) << a;
#define D_OUT(a) WRITE(__FUNCTION__, a, Core::DebugOutput::DebugStream::Debug); LOG(TRACE) << a;
#define CHECK_LEVEL_D_OUT(level1, level2, msg)							\
{if(((unsigned char)level1) & ((unsigned char)level2))					\
{ WRITE(__FUNCTION__, msg, Core::DebugOutput::DebugStream::Debug);		\
LOG(TRACE)																\
}}

//! Output if output level is set to allow (add.) kernel time output
#define KT_OUT2(method,a) WRITE(method, a, Core::DebugOutput::DebugStream::KernelTime); LOG(TRACE) << a;
#define KT_OUT(a) WRITE(__FUNCTION__, a, Core::DebugOutput::DebugStream::KernelTime); LOG(TRACE) << a;
#define CHECK_LEVEL_KT_OUT(level1, level2, msg)								\
{if(((unsigned char)level1) & ((unsigned char)level2))						\
{ WRITE(__FUNCTION__, msg, Core::DebugOutput::DebugStream::KernelTime);		\
LOG(TRACE) << msg;															\
}}


// inline methods
#include "DebugStream.inl"
//
}

}

#pragma warning ( pop )
#endif // !DEBUGSTREAM_H_
