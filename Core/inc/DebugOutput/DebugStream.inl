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

/**
 * This method returns the currently used debug callback method
 *
 * @return  the current debug callback
 *
 */
Core::DebugOutput::DebugStream::DebugCallback 
	Core::DebugOutput::DebugStream::getCB(void)
{ return _cb; } 

/**
 * This method sets the currently used debug callback method; if param is
 * NULL, the default handler is used
 *
 * @param cb  the new debug callback
 *
 */
void
Core::DebugOutput::DebugStream::setCB(DebugCallback cb)
{ 
	if ( cb != NULL ) {
		Core::DebugOutput::DebugStream::_cb = cb;
	}
	else {
		Core::DebugOutput::DebugStream::_cb = Core::DebugOutput::DebugStream::DefaultHandler::messageCB;
	}
}

/**
 * This method sets the currently used debug callback method; if param is
 * NULL, the default handler is used
 *
 * @param cb  the new debug callback
 *
 */
void 
Core::DebugOutput::DebugStream::setLevel (int l)
{
  _level = l;
}

/**
 * Returns the current level (|-ORed combination of DebugLevels) which
 * indicate the current levels that are passed to the output
 *
 * @return current level
 *
 */
int 
Core::DebugOutput::DebugStream::getLevel (void)
{
  return _level;
}

//**
// * Flushes the streams of the default handler
// */
void 
Core::DebugOutput::DebugStream::DefaultHandler::flush (void)
{ 
	if (ostr.is_open ()) {
		ostr.flush ();
		ostr.close ();
	}
}


//**
// * Returns the currently used output stream
// *
// * @return
// *  current output stream (stdout or file output-stream)
// */
std::ostream & 
Core::DebugOutput::DebugStream::DefaultHandler::getOStr (void)
{
	if (Core::DebugOutput::DebugStream::DefaultHandler::ostr.is_open ()) {
		return (Core::DebugOutput::DebugStream::DefaultHandler::ostr);
	}
	else {
		return (std::cout);
	}

}

//**
// * Prints a message (info, debug, error) to the specified output stream. 
// *
// * @param l  debug level
// *
// * @param 
// *
// */
void 
DebugStream::DefaultHandler::messageCB (DebugLevel l, const std::string functionName, const std::string msg)
{
	static char *levelText[] =  { "", "DEBUG", "INFO", "", "ERROR", "", "", "", "KERNELTIME", "", "", "", "", "", "", "", "", "", "", "", ""};


	if(msg.empty())
		return;

	//if ( l == Core::DebugOutput::DebugStream::Debug ) {  // some additional info here
	//	getOStr() << "[" << levelText[l] << "] " 
	//		//              <<__FILE__ <<", " 
	//		//              << std::setfill('0')<<std::setw( 5) 
	//		//              << std::setiosflags(std::ios::right) 
	//		//              << __LINE__ << "> " 
	//		//              << std::resetiosflags(std::ios::left) 
	//		<< msg << std::endl;

	//}
	//else {
	//	getOStr() << "[" << levelText[l] << "] " << msg << std::endl;
	//}

	/*switch( l )
	{
	case Core::DebugOutput::DebugStream::None:
		LOG(INFO)<< functionName << msg;
		break;
	case Core::DebugOutput::DebugStream::Debug:
		LOG(TRACE)<< functionName << msg;
		break;
	case Core::DebugOutput::DebugStream::Info:
		LOG(INFO)<< functionName << msg;
		break;
	case Core::DebugOutput::DebugStream::Error:
		LOG(ERROR)<< functionName << msg;
		break;
	case Core::DebugOutput::DebugStream::KernelTime:
		LOG(INFO)<< functionName << msg;
		break;
	case Core::DebugOutput::DebugStream::All:
		LOG(INFO)<< functionName << msg;
		break;
	default:
		LOG(INFO)<< functionName << msg;
		break;
	}*/

	//Core::DebugOutput::DebugStream::_messageSig(l, functionName, msg);
	Core::DebugOutput::DebugStream::instance().MessageSig("[" + std::string(levelText[l]) + "]", functionName, msg);
}
