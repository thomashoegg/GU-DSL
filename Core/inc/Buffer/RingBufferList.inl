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


#include <QtCore/QMutexLocker>
#include <QtCore/QMutex>


#define TEMPLATEDEF_RINGLIST template<class T>
#define CLASSDEF_RINGLIST    RingBufferList<T>




TEMPLATEDEF_RINGLIST CLASSDEF_RINGLIST::RingBufferList(int size/* = 1*/)
{
	_size = size;

	_pMutex = new QMutex();
}

TEMPLATEDEF_RINGLIST CLASSDEF_RINGLIST::~RingBufferList()
{
	{
		QMutexLocker locker(_pMutex);

		//typename std::list<T*>::const_iterator it;

		//for (
		//	it = _bufferList.begin();
		//	it != _bufferList.end();
		//it++)
		//{
		//	delete *it;
		//}

		deleteAllElements();

		_bufferList.clear();
	}

	delete _pMutex;

	_pMutex = nullptr;


}

TEMPLATEDEF_RINGLIST T* CLASSDEF_RINGLIST::read()
{
	return read(true);
}

TEMPLATEDEF_RINGLIST T* CLASSDEF_RINGLIST::read(bool remove)
{
	if(_pMutex == nullptr)
		return nullptr;

	QMutexLocker locker(_pMutex);

	if(_bufferList.size() <= 0)
		return nullptr;

	T* myObject =  _bufferList.front();
	if(remove) _bufferList.pop_front();
	return myObject;
}

TEMPLATEDEF_RINGLIST void CLASSDEF_RINGLIST::store(T* data)
{
	if(_pMutex == nullptr)
		return;

	QMutexLocker locker(_pMutex);

	if(_bufferList.size() < _size)
		_bufferList.push_back(data);
	else
	{
		T* myObject =  _bufferList.front();
		_bufferList.pop_front();
		if(myObject != nullptr)
			delete myObject;

		_bufferList.push_back(data);
	}
}



TEMPLATEDEF_RINGLIST void CLASSDEF_RINGLIST::clear()
{
	if(_pMutex == nullptr)
		return;

	QMutexLocker locker(_pMutex);

	deleteAllElements();

	_bufferList.clear();
}

TEMPLATEDEF_RINGLIST void CLASSDEF_RINGLIST::deleteAllElements(int idxBegin)
{
	typename std::list<T*>::const_iterator it = _bufferList.begin();

	std::advance(it, idxBegin);
	//for(it = std::advance(_bufferList.begin(), idxBegin); it != _bufferList.end(); it++)
	for( ; it != _bufferList.end(); it++)
	{
		delete (*it);
	}
}


TEMPLATEDEF_RINGLIST void CLASSDEF_RINGLIST::resize(int size)
{
	if(_pMutex == nullptr)
		return;

	QMutexLocker locker(_pMutex);


	if(size < _size)
	{
		deleteAllElements(size);
		// TODO: also remove from list!
	}

	_size = size;
}

TEMPLATEDEF_RINGLIST int CLASSDEF_RINGLIST::getNumbElements() const
{
	if(_pMutex == nullptr)
		return 0;

	QMutexLocker locker(_pMutex);

	return (int)_bufferList.size();
}
