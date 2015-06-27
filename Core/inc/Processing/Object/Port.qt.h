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

#ifndef PROCESSING_PORT_H
#define PROCESSING_PORT_H

#ifdef WIN32
#pragma once
#endif

#include "CoreDLL.h"

#include <unordered_map>

#include "QObject"
#include "Processing/Interfaces/IObject.qt.h"
#include "Processing/Interfaces/IInterface.qt.h"
#include "Processing/Interfaces/IPort.qt.h"
#include "Processing/Object/PortSignalArgs.qt.h"
#include "Processing/PtrDefinitions.h"

#include "boost/shared_ptr.hpp"
#include "boost/signals2.hpp"

namespace Core
{
	namespace Processing
	{
		namespace Object
		{
			class CORE_API Port : public Core::Processing::Interfaces::IPort
			{
				Q_OBJECT
				DECLARE_OBJECT(Port)
			public:
				typedef boost::signals2::signal<void (Core::Processing::Interfaces::ConstIObjectPtr sender, const PortSignalArgs* args)> OnEvent;
				typedef std::unordered_map<Core::Processing::Interfaces::IProvider*, QList<Core::Processing::Interfaces::IProcessorPtr>> ProviderProcessorListMap; 
				typedef ProviderProcessorListMap::value_type ProviderProcessorListPair;

			public:

				Port(QString uid);
				Port(const Port& rhs);
				Port(const Port& rhs, QString uid);

				virtual bool addInterface(Core::Processing::Interfaces::IInterfacePtr iPtr);

				virtual bool connectInterfaces(Core::Processing::Interfaces::IProviderPtr source, Core::Processing::Interfaces::IProcessorPtr target);

				virtual bool disconnectInterfaces(Core::Processing::Interfaces::IProviderPtr source, Core::Processing::Interfaces::IProcessorPtr target);

				QString getName() const { return _name; }

				void setName(const QString& name) { _name = name; }

				const ProviderProcessorListMap getProviderProcessorListMap() const;

				virtual void requestData() {};

			protected:
				virtual void init() { throw std::logic_error("The method or operation is not implemented."); }

				virtual bool process( Core::Processing::Interfaces::ConstIObjectParametersPtr parameters ) { throw std::logic_error("The method or operation is not implemented."); }

			private:

				virtual bool connectInterfaceMembers(Core::Processing::Interfaces::IProviderPtr source, Core::Processing::Interfaces::IProcessorPtr target) { throw std::logic_error("The method or operation is not implemented."); }

			protected:

				ProviderProcessorListMap	_sourceTargetMap;

				QList<Core::Processing::Interfaces::IInterfacePtr>	_interfaces;

				QList<Core::Processing::Interfaces::IInterfacePtr>	_providers;
				QList<Core::Processing::Interfaces::IInterfacePtr>	_processors;

				QString					_name;
			};
		};	// namespace Object
	};	// namespace Processing
}; //namespace Core

#define MEMBER_SIGNAL_CONNECT_BIND(SignalName, boostFuncBind)	\
	SignalName ## Sig.connect(SignalName ## Sig);

// Member without parameters
#define MEMBER_SIGNAL(ReturnType, SignalName)	\
	typedef boost::signals2::signal<ReturnType (> On ## SignalName;	\
	On ## SignalName SignalName ## Sig;	\
	ReturnType SignalName() { SignalName ## Sig(); }

// Member with 1 parameter
#define MEMBER_SIGNAL_1(ReturnType, SignalName, p1T, p1V)	\
	typedef boost::signals2::signal<ReturnType (p1T p1V)> On ## SignalName;	\
	On ## SignalName SignalName ## Sig;	\
	ReturnType SignalName(p1T p1V) { SignalName ## Sig(p1V); }

// Member with 2 parameters
#define MEMBER_SIGNAL_2(ReturnType, SignalName, p1T, p1V, p2T, p2V)	\
	typedef boost::signals2::signal<ReturnType (p1T p1V, p2T p2V)> On ## SignalName;	\
	On ## SignalName SignalName ## Sig;	\
	ReturnType SignalName(p1T p1V, p2T p2V) { SignalName ## Sig(p1V, p2V); }

// Member with 3 parameters
#define MEMBER_SIGNAL_3(ReturnType, SignalName, p1T, p1V, p2T, p2V, p3T, p3V)	\
	typedef boost::signals2::signal<ReturnType (p1T p1V, p2T p2V, p3T p3V)> On ## SignalName;	\
	On ## SignalName SignalName ## Sig;	\
	ReturnType SignalName(p1T p1V, p2T p2V, p3T p3V) { SignalName ## Sig(p1V, p2V, p3V); }

// Member with 4 parameters
#define MEMBER_SIGNAL_4(ReturnType, SignalName, p1T, p1V, p2T, p2V, p3T, p3V, p4T, p4V)	\
	typedef boost::signals2::signal<ReturnType (p1T p1V, p2T p2V, p3T p3V, p4T p4V)> On ## SignalName;	\
	On ## SignalName SignalName ## Sig;	\
	ReturnType SignalName(p1T p1V, p2T p2V, p3T p3V, p4T p4V) { SignalName ## Sig(p1V, p2V, p3V, p4V); }

// Member with 5 parameters
#define MEMBER_SIGNAL_5(ReturnType, SignalName, p1T, p1V, p2T, p2V, p3T, p3V, p4T, p4V, p5T, p5V)	\
	typedef boost::signals2::signal<ReturnType (p1T p1V, p2T p2V, p3T p3V, p4T p4V, p5T p5V)> On ## SignalName;	\
	On ## SignalName SignalName ## Sig;	\
	ReturnType SignalName(p1T p1V, p2T p2V, p3T p3V, p4T p4V, p5T p5V) { SignalName ## Sig(p1V, p2V, p3V, p4V, p5V); }

// Member with 6 parameters
#define MEMBER_SIGNAL_6(ReturnType, SignalName, p1T, p1V, p2T, p2V, p3T, p3V, p4T, p4V, p5T, p5V, p6T, p6V)	\
	typedef boost::signals2::signal<ReturnType (p1T p1V, p2T p2V, p3T p3V, p4T p4V, p5T p5V, p6T p6V)> On ## SignalName;	\
	On ## SignalName SignalName ## Sig;	\
	ReturnType SignalName(p1T p1V, p2T p2V, p3T p3V, p4T p4V, p5T p5V, p6T p6V) { SignalName ## Sig(p1V, p2V, p3V, p4V, p5V, p6V); }

// Member with 7 parameters
#define MEMBER_SIGNAL_7(ReturnType, SignalName, p1T, p1V, p2T, p2V, p3T, p3V, p4T, p4V, p5T, p5V, p6T, p6V, p7T, p7V)	\
	typedef boost::signals2::signal<ReturnType (p1T p1V, p2T p2V, p3T p3V, p4T p4V, p5T p5V, p6T p6V, p7T p7V)> On ## SignalName;	\
	On ## SignalName SignalName ## Sig;	\
	ReturnType SignalName(p1T p1V, p2T p2V, p3T p3V, p4T p4V, p5T p5V, p6T p6V, p7T p7V) { SignalName ## Sig(p1V, p2V, p3V, p4V, p5V, p6V, p7V); }

// Member with 8 parameters
#define MEMBER_SIGNAL_8(ReturnType, SignalName, p1T, p1V, p2T, p2V, p3T, p3V, p4T, p4V, p5T, p5V, p6T, p6V, p7T, p7V, p8T, p8V)	\
	typedef boost::signals2::signal<ReturnType (p1T p1V, p2T p2V, p3T p3V, p4T p4V, p5T p5V, p6T p6V, p7T p7V, p8T p8V)> On ## SignalName;	\
	On ## SignalName SignalName ## Sig;	\
	ReturnType SignalName(p1T p1V, p2T p2V, p3T p3V, p4T p4V, p5T p5V, p6T p6V, p7T p7V, p8T p8V) { SignalName ## Sig(p1V, p2V, p3V, p4V, p5V, p6V, p7V, p8V); }


// Member with 9 parameters
#define MEMBER_SIGNAL_9(ReturnType, SignalName, p1T, p1V, p2T, p2V, p3T, p3V, p4T, p4V, p5T, p5V, p6T, p6V, p7T, p7V, p8T, p8V, p9T, p9V)	\
	typedef boost::signals2::signal<ReturnType (p1T p1V, p2T p2V, p3T p3V, p4T p4V, p5T p5V, p6T p6V, p7T p7V, p8T p8V)> On ## SignalName;	\
	On ## SignalName SignalName ## Sig;	\
	ReturnType SignalName(p1T p1V, p2T p2V, p3T p3V, p4T p4V, p5T p5V, p6T p6V, p7T p7V, p8T p8V, p8T p8V) { SignalName ## Sig(p1V, p2V, p3V, p4V, p5V, p6V, p7V, p8V, p8V); }


#define MEMBER_SIGNAL_CONNECT(SignalNameFrom, SignalNameTo, Target)	\
	SignalNameFrom ## Sig.connect(boost::bind(&SignalNameTo, Target));

#define MEMBER_SIGNAL_CONNECT_1(SignalNameFrom, SignalNameTo, Target, p1)	\
	SignalNameFrom ## Sig.connect(boost::bind(&SignalNameTo, Target, _1));

#define MEMBER_SIGNAL_CONNECT_2(SignalNameFrom, SignalNameTo, Target, p1, p2)	\
	SignalNameFrom ## Sig.connect(boost::bind(&SignalNameTo, Target, _1, _2));

#define MEMBER_SIGNAL_CONNECT_3(SignalNameFrom, SignalNameTo, Target, p1, p2, p3)	\
	SignalNameFrom ## Sig.connect(boost::bind(&SignalNameTo, Target, _1, _2, _3));

#define MEMBER_SIGNAL_CONNECT_4(SignalNameFrom, SignalNameTo, Target, p1, p2, p3, p4)	\
	SignalNameFrom ## Sig.connect(boost::bind(&SignalNameTo, Target, _1, _2, _3, _4));

#define MEMBER_SIGNAL_CONNECT_5(SignalNameFrom, SignalNameTo, Target, p1, p2, p3, p4, p5)	\
	SignalNameFrom ## Sig.connect(boost::bind(&SignalNameTo, Target, _1, _2, _3, _4, _5));

#define MEMBER_SIGNAL_CONNECT_6(SignalNameFrom, SignalNameTo, Target, p1, p2, p3, p4, p5, p6)	\
	SignalNameFrom ## Sig.connect(boost::bind(&SignalNameTo, Target, _1, _2, _3, _4, _5, _6));

#define MEMBER_SIGNAL_CONNECT_7(SignalNameFrom, SignalNameTo, Target, p1, p2, p3, p4, p5, p6, p7)	\
	SignalNameFrom ## Sig.connect(boost::bind(&SignalNameTo, Target, _1, _2, _3, _4, _5, _6, _7));

#define MEMBER_SIGNAL_CONNECT_8(SignalNameFrom, SignalNameTo, Target, p1, p2, p3, p4, p5, p6, p7, p8)	\
	SignalNameFrom ## Sig.connect(boost::bind(&SignalNameTo, Target, _1, _2, _3, _4, _5, _6, _7, _8));

#define MEMBER_SIGNAL_CONNECT_9(SignalNameFrom, SignalNameTo, Target, p1, p2, p3, p4, p5, p6, p7, p8, p9)	\
	SignalNameFrom ## Sig.connect(boost::bind(&SignalNameTo, Target, _1, _2, _3, _4, _5, _6, _7, _8, _9));

#define REQUEST_PORT_DATA																						\
	for(Core::Processing::Object::Class::PortMap::iterator it = _ports.begin(); it != _ports.end(); it++)		\
	{																											\
		it->second->requestData();																				\
	}


#define CALL_INTERFACE_BEGIN(InterfaceName)																			\
for(Core::Processing::Object::Class::PortMap::iterator it = _ports.begin(); it != _ports.end(); it++)				\
{																													\
	InterfaceName ## Ptr interfaceProvider = boost::dynamic_pointer_cast<InterfaceName>(it->second);				\
	if(interfaceProvider)																							\
	{

#define CALL_INTERFACE_END																							\
	}																												\
}

#define CALL_INTERFACE(InterfaceName, MethodName)								\
	if(isModuleDebugLevel(Core::DebugOutput::DebugLevel::InterfaceCalls))		\
	{																			\
		D_OUT("Call Interface " #InterfaceName ": " #MethodName)				\
	}																			\
	CALL_INTERFACE_BEGIN(InterfaceName)											\
	interfaceProvider->MethodName();											\
	CALL_INTERFACE_END

#define CALL_INTERFACE_1(InterfaceName, MethodName, a1)							\
	if(isModuleDebugLevel(Core::DebugOutput::DebugLevel::InterfaceCalls))		\
	{																			\
		D_OUT("Call Interface " #InterfaceName "." #MethodName"(" #a1 ")")		\
	}																			\
	CALL_INTERFACE_BEGIN(InterfaceName)											\
	interfaceProvider->MethodName(a1);											\
	CALL_INTERFACE_END

#define CALL_INTERFACE_2(InterfaceName, MethodName, a1, a2)								\
	if(isModuleDebugLevel(Core::DebugOutput::DebugLevel::InterfaceCalls))				\
	{																					\
		D_OUT("Call Interface " #InterfaceName "." #MethodName "(" #a1 "," #a2 ")")		\
	}																					\
	CALL_INTERFACE_BEGIN(InterfaceName)													\
	interfaceProvider->MethodName(a1, a2);												\
	CALL_INTERFACE_END

#define CALL_INTERFACE_3(InterfaceName, MethodName, a1, a2, a3)									\
	if(isModuleDebugLevel(Core::DebugOutput::DebugLevel::InterfaceCalls))						\
	{																							\
		D_OUT("Call Interface " #InterfaceName "." #MethodName "(" #a1 "," #a2 "," #a3 ")")		\
	}																							\
	CALL_INTERFACE_BEGIN(InterfaceName)															\
	interfaceProvider->MethodName(a1, a2, a3);													\
	CALL_INTERFACE_END

#define CALL_INTERFACE_4(InterfaceName, MethodName, a1, a2, a3, a4)										\
	if(isModuleDebugLevel(Core::DebugOutput::DebugLevel::InterfaceCalls))								\
	{																									\
		D_OUT("Call Interface " #InterfaceName "." #MethodName "(" #a1 "," #a2 "," #a3 "," #a4 ")")		\
	}																									\
	CALL_INTERFACE_BEGIN(InterfaceName)																	\
	interfaceProvider->MethodName(a1, a2, a3, a4);														\
	CALL_INTERFACE_END

#define CALL_INTERFACE_5(InterfaceName, MethodName, a1, a2, a3, a4, a5)											\
	if(isModuleDebugLevel(Core::DebugOutput::DebugLevel::InterfaceCalls))										\
	{																											\
		D_OUT("Call Interface " #InterfaceName "." #MethodName "(" #a1 "," #a2 "," #a3 "," #a4 "," #a5 ")")		\
	}																											\
	CALL_INTERFACE_BEGIN(InterfaceName)																			\
	interfaceProvider->MethodName(a1, a2, a3, a4, a5);															\
	CALL_INTERFACE_END

#define CALL_INTERFACE_6(InterfaceName, MethodName, a1, a2, a3, a4, a5, a6)												\
	if(isModuleDebugLevel(Core::DebugOutput::DebugLevel::InterfaceCalls))												\
	{																													\
		D_OUT("Call Interface " #InterfaceName "." #MethodName "(" #a1 "," #a2 "," #a3 "," #a4 "," #a5 "," #a6 ")")		\
	}																													\
	CALL_INTERFACE_BEGIN(InterfaceName)																					\
	interfaceProvider->MethodName(a1, a2, a3, a4, a5, a6);																\
	CALL_INTERFACE_END

#define CALL_INTERFACE_7(InterfaceName, MethodName, a1, a2, a3, a4, a5, a6, a7)													\
	if(isModuleDebugLevel(Core::DebugOutput::DebugLevel::InterfaceCalls))														\
	{																															\
		D_OUT("Call Interface " #InterfaceName "." #MethodName "(" #a1 "," #a2 "," #a3 "," #a4 "," #a5 "," #a6, "," #a7 ")")	\
	}																															\
	CALL_INTERFACE_BEGIN(InterfaceName)																							\
	interfaceProvider->MethodName(a1, a2, a3, a4, a5, a6, a7);																	\
	CALL_INTERFACE_END

#define CALL_INTERFACE_8(InterfaceName, MethodName, a1, a2, a3, a4, a5, a6, a7, a8)														\
	if(isModuleDebugLevel(Core::DebugOutput::DebugLevel::InterfaceCalls))																\
	{																																	\
	D_OUT("Call Interface " #InterfaceName "." #MethodName "(" #a1 "," #a2 "," #a3 "," #a4 "," #a5 "," #a6, "," #a7 "," #a8 ")")		\
	}																																	\
	CALL_INTERFACE_BEGIN(InterfaceName)																									\
	interfaceProvider->MethodName(a1, a2, a3, a4, a5, a6, a7, a8);																		\
	CALL_INTERFACE_END

#define CALL_INTERFACE_9(InterfaceName, MethodName, a1, a2, a3, a4, a5, a6, a7, a8, a9)															\
	if(isModuleDebugLevel(Core::DebugOutput::DebugLevel::InterfaceCalls))																		\
	{																																			\
		D_OUT("Call Interface " #InterfaceName "." #MethodName "(" #a1 "," #a2 "," #a3 "," #a4 "," #a5 "," #a6, "," #a7 "," #a8 "," #a9 ")")	\
	}																																			\
	CALL_INTERFACE_BEGIN(InterfaceName)																											\
	interfaceProvider->MethodName(a1, a2, a3, a4, a5, a6, a7, a8, a9);																			\
	CALL_INTERFACE_END

#endif	// PROCESSING_PORT_H
