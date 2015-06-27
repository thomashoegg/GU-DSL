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


#include "boost/shared_ptr.hpp"


namespace Core
{
	namespace Processing
	{
		namespace Interfaces
		{
	#pragma region Forward declarations
			class IObject;
			class IPort;

			class IInterface;
			class IProcessor;
			class IProvider;

			class IImageProcessor;
			class IImageProvider;

			class IIntegerProcessor;
			class IIntegerProvider;

			class IPrimitivesProcessor;
			class IPrimitivesProvider;

			class IPrimitivesArrayProcessor;
			class IPrimitivesArrayProvider;

			class IObjectParameters;

			class IObjectPlugin;

			class ICommunicationSource;

	#pragma endregion

	#pragma region Type Definitions

			// Interfaces Definitions

			typedef boost::shared_ptr<IObject>	IObjectPtr;
			typedef boost::shared_ptr<const IObject> ConstIObjectPtr;

			typedef boost::shared_ptr<IPort>	IPortPtr;
			typedef boost::shared_ptr<const IPort> ConstIPortPtr;

			typedef boost::shared_ptr<IInterface>	IInterfacePtr;
			typedef boost::shared_ptr<const IInterface> ConstIInterfacePtr;

			typedef boost::shared_ptr<IProcessor>	IProcessorPtr;
			typedef boost::shared_ptr<const IProcessor> ConstIProcessorPtr;

			typedef boost::shared_ptr<IProvider>	IProviderPtr;
			typedef boost::shared_ptr<const IProvider> ConstIProviderPtr;

			typedef boost::shared_ptr<IImageProcessor>	IImageProcessorPtr;
			typedef boost::shared_ptr<const IImageProcessor> ConstIImageProcessorPtr;

			typedef boost::shared_ptr<IImageProvider>	IImageProviderPtr;
			typedef boost::shared_ptr<const IImageProvider> ConstIImageProviderPtr;

			typedef boost::shared_ptr<IIntegerProcessor>	IIntegerProcessorPtr;
			typedef boost::shared_ptr<const IIntegerProcessor> ConstIIntegerProcessorPtr;

			typedef boost::shared_ptr<IIntegerProvider>	IIntegerProviderPtr;
			typedef boost::shared_ptr<const IIntegerProvider> ConstIIntegerProviderPtr;

			typedef boost::shared_ptr<IPrimitivesProcessor>	IPrimitivesProcessorPtr;
			typedef boost::shared_ptr<const IPrimitivesProcessor> ConstIPrimitivesProcessorPtr;

			typedef boost::shared_ptr<IPrimitivesProvider>	IPrimitivesProviderPtr;
			typedef boost::shared_ptr<const IPrimitivesProvider> ConstIPrimitivesProviderPtr;

			typedef boost::shared_ptr<IPrimitivesArrayProcessor>	IPrimitivesArrayProcessorPtr;
			typedef boost::shared_ptr<const IPrimitivesArrayProcessor> ConstIPrimitivesArrayProcessorPtr;

			typedef boost::shared_ptr<IPrimitivesArrayProvider>	IPrimitivesArrayProviderPtr;
			typedef boost::shared_ptr<const IPrimitivesArrayProvider> ConstIPrimitivesArrayProviderPtr;

			typedef boost::shared_ptr<IObjectParameters>	IObjectParametersPtr;
			typedef boost::shared_ptr<const IObjectParameters> ConstIObjectParametersPtr;

			typedef boost::shared_ptr<IObjectPlugin>	IObjectPluginPtr;
			typedef boost::shared_ptr<const IObjectPlugin> ConstIObjectPluginPtr;

			typedef boost::shared_ptr<ICommunicationSource>				ICommunicationSourcePtr;
			typedef boost::shared_ptr<const ICommunicationSource>			ConstICommunicationSourcePtr;

	#pragma endregion
		}; //namespace Interfaces
	}; //namespace Processing
}; //namespace Core

namespace Core
{
	namespace Processing
	{
		namespace Object
		{
	#pragma region Forward declarations

		
			class Class;
			class Filter;
			class Source;
			class Sink;

			class Port;

			class ImageProcessorPort;
			class ImageProcessorPort;

	
			class ObjectFactory;

	#pragma endregion

	#pragma region Type Definitions

			// Object Definitions


			typedef boost::shared_ptr<Class>	ClassPtr;
			typedef boost::shared_ptr<const Class> ConstClassPtr;

			typedef boost::shared_ptr<Filter>	FilterPtr;
			typedef boost::shared_ptr<const Filter> ConstFilterPtr;

			typedef boost::shared_ptr<Sink>	SinkPtr;
			typedef boost::shared_ptr<const Sink> ConstSinkPtr;

			typedef boost::shared_ptr<Source>	SourcePtr;
			typedef boost::shared_ptr<const Source> ConstSourcePtr;

			typedef boost::shared_ptr<Port>	PortPtr;
			typedef boost::shared_ptr<const Port> ConstPortPtr;
		

			typedef boost::shared_ptr<ImageProcessorPort>	ImageProcessorPortPtr;
			typedef boost::shared_ptr<const ImageProcessorPort> ConstImageProcessorPortPtr;

			typedef boost::shared_ptr<ImageProcessorPort>	ImageProcessorPortPtr;
			typedef boost::shared_ptr<const ImageProcessorPort> ConstImageProcessorPortPtr;

			// Misc Definitions
			typedef boost::shared_ptr<ObjectFactory>	ObjectFactoryPtr;

	#pragma endregion

		};	// namespace Objects	
	};	// namespace Processing
}; //namespace Core

namespace Core
{
	namespace Processing
	{
		namespace Component
		{
			class ComponentDiagram;

			typedef boost::shared_ptr<ComponentDiagram>	ComponentDiagramPtr;
			typedef boost::shared_ptr<const ComponentDiagram> ConstComponentDiagramPtr;

		};	// namespace Component	
	};	// namespace Processing
}; //namespace Core

namespace Core
{
	namespace Processing
	{
		namespace Ports
		{
	#pragma region Forward declarations
			class ImageProcessorPort;
			class ImageProcessorPort;


	#pragma endregion

	#pragma region Type Definitions

			// Object Definitions

			typedef boost::shared_ptr<ImageProcessorPort>	ImageProcessorPortPtr;
			typedef boost::shared_ptr<const ImageProcessorPort> ConstImageProcessorPortPtr;

			typedef boost::shared_ptr<ImageProcessorPort>	ImageProcessorPortPtr;
			typedef boost::shared_ptr<const ImageProcessorPort> ConstImageProcessorPortPtr;

	#pragma endregion

		};	// namespace Ports	
	};	// namespace Processing
}; //namespace Core

namespace Core
{
	namespace Processing
	{
		namespace Project
		{
#pragma region Forward declarations
			class Project;


#pragma endregion

#pragma region Type Definitions

			// Object Definitions

			typedef boost::shared_ptr<Project>	ProjectPtr;
			typedef boost::shared_ptr<const Project> ConstProjectPtr;

#pragma endregion

		};	// namespace Project	
	};	// namespace Processing
}; //namespace Core
