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


#pragma once
#ifndef __IEVENT_H__
#define __IEVENT_H__
#include "CoreDLL.h"
#include <typeindex>
#include <vector>

namespace Core
{
	namespace Mediation
	{
		namespace Interfaces
		{
			/**
			 * \brief Event interface.
			 *
			 */
			class CORE_API IEvent
			{
			public:
				IEvent();
				virtual ~IEvent() = 0;

				/**
				 * \brief get event base types
				 *
				 *	\retuns std::vector<std::type_index> list of the base types.
				 */
				static std::vector<std::type_index> EventBaseTypes();

				/**
				 * \brief get event type
				 *
				 *	\returns std::type_index The event type index.
				 */
				virtual std::type_index getEventType() const = 0;

				/**
				 * \brief get event type
				 *
				 *	\returns std::type_index The event type index.
				 */
				virtual std::type_index getEventType() = 0;

				/**
				 * \brief get event vase type
				 *
				 *	\returns std::vector<std::type_index> The event base type index list.
				 */
				virtual std::vector<std::type_index> getEventBaseTypes() const = 0;
			};
		}; // namespace Interfaces
	}; // namespace Mediation
}; // namespace Core

#ifdef DECLARE_EVENT
#undef DECLARE_EVENT
#endif
#define DECLARE_EVENT															\
	public: static std::vector<std::type_index> EventBaseTypes();				\
	public: static std::type_index getType();									\
	public: virtual std::type_index getEventType() const;						\
	public: virtual std::type_index getEventType();								\
	public: virtual std::vector<std::type_index> getEventBaseTypes() const;		



#ifdef IMPLEMENT_EVENT
#undef IMPLEMENT_EVENT
#endif
#define IMPLEMENT_EVENT(Event, BaseEvent)										\
	std::vector<std::type_index> Event::EventBaseTypes()						\
	{																			\
		std::vector<std::type_index> tmp;										\
		tmp.push_back(typeid(BaseEvent));										\
		std::vector<std::type_index> baseTmp = BaseEvent::EventBaseTypes();		\
		for(int i = 0; i < baseTmp.size(); i++)									\
		{																		\
			tmp.push_back(baseTmp[i]);											\
		}																		\
		return tmp;																\
	}																			\
																				\
	std::vector<std::type_index> Event::getEventBaseTypes() const				\
	{																			\
		return EventBaseTypes();												\
	}																			\
																				\
	std::type_index Event::getType()											\
	{																			\
		return typeid(Event);													\
	}																			\
	std::type_index Event::getEventType() const									\
	{																			\
		return Event::getType();												\
	}																			\
	std::type_index Event::getEventType()										\
	{																			\
		return Event::getType();												\
	}

#endif // __IEVENT_H__
