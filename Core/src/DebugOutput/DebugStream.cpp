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

#include "DebugOutput/DebugStream.h"

#include <stdlib.h>
#include <stdio.h>

using namespace std;
using namespace Core::DebugOutput;

// Definition and initialisation of static class data
  int                      DebugStream::_level = 
#ifdef DEBUG 
  DebugStream::All&(~DebugStream::KernelTime);
#else
  DebugStream::Error | DebugStream::Info;
#endif

DebugStream::DebugCallback DebugStream::_cb = DebugStream::DefaultHandler::messageCB;
ofstream                   DebugStream::DefaultHandler::ostr;


/**
 * Standard constructor of Debug (not for public use!)
 */
DebugStream::DebugStream (void)
{

}


/**
 * This method controls the current debug level. The debug-level is formatted
 * using an OR-ed notation with the '|' (pipe) symbol
 *
 * @param str  string controlling the debug-level
 *
 */
void
DebugStream::setLevel (const char *str)
{
  size_t len = strlen (str);
  size_t curr = 0;

  _level = None;
  while (curr < len) {
    if (!strncmp (&(str[curr]), "All", strlen ("All"))) {
      _level |= All;
      curr += strlen ("All");
    }
    else if (!strncmp (&(str[curr]), "Info", strlen ("Info"))) {
      _level |= Info;
      curr += strlen ("Info");
    }
    else if (!strncmp (&(str[curr]), "Debug", strlen ("Debug"))) {
      _level |= Debug;
      curr += strlen ("Debug");
    }
    else if (!strncmp (&(str[curr]), "Error", strlen ("Error"))) {
      _level |= Error;
      curr += strlen ("Error");
    }
	else if (!strncmp (&(str[curr]), "KernelTime", strlen ("KernelTime"))) {
		_level |= KernelTime;
		curr += strlen ("KernelTime");
	}
    else if (!strncmp (&(str[curr]), "None", strlen ("None"))) {
      _level |= None;
      curr += strlen ("None");
    }
    else {
      std::cout << "DebugStream::setLevel: Malformed level-definition "
                << &(str[curr]) << endl;
      break;
    }

    if (curr < len && str[curr] != '|') {
      std::cout << "DebugStream::setLevel: Malformed level-definition "
                << str << endl;
      break;
    }
    else {
      curr++;                 // ueberspringe '|'
    }
  }
  return;
}


//---------------------------------------------------------------------------
//! \name Default Message Handler
//! \{
//---------------------------------------------------------------------------

/**
 * Constructor of Debug-Handler
 */
DebugStream::DefaultHandler::DefaultHandler(void)
{ }

/**
 * Destructor of Debug-Handler
 */
DebugStream::DefaultHandler::~DefaultHandler (void)
{
  if (ostr.is_open ()) {
    ostr.flush ();
    ostr.close ();
  }

  cout.flush ();
}

/**
 * Setting out-stream to a file
 *
 * @param fileName  the name of the (new) output file
 *
 * @return
 *  boolean value for success
 *
 */
bool 
DebugStream::DefaultHandler::openFile (const char *fileName)
{
  if (ostr.is_open ()) {
    ostr.flush ();
    ostr.close ();
  }

  if (fileName && strlen (fileName) > 0) {
    ostr.open (fileName);
    if (!ostr) {
      cerr << "Could not open debug-trace file `" << fileName << "'\n";
      return false;
    }
  }
  return true;
}


/**
 * Closing out-stream to a file; after that, cout will be used as default
 *
 */
void
DebugStream::DefaultHandler::closeFile (void)
{
  if (ostr.is_open ()) {
    ostr.flush ();
    ostr.close ();
  }
  return;
}

//!\}------------------------------------------------------------------------
