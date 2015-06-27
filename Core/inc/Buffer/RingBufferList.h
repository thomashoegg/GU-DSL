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



#ifndef RINGBUFFERLIST_H_
#define RINGBUFFERLIST_H_

#if WIN32
#pragma once
#endif

#include "CoreDLL.h"


#include <QtCore/QMutex>
#include <list>

namespace Core
{

namespace Buffer
{

template<class T>

	
class RingBufferList
{

public:

	/**
	* \brief Constructor
	*
	*
	* \return 
	* \param  int size Size of the Buffer - Default: 1
	*/
	RingBufferList(int size = 1);

	/**
	* \brief Destructor
	*
	* Destructor ~RingBufferList()
	*
	* \return 
	*/
	~RingBufferList();


	/**
	* \brief Read Value
	*
	* Read value from buffer (FIFO)
	*
	* \return T*		Pointer to the desired Value
	*/
	T* read();

	
	/**
	* \brief Read Value
	*
	* Read value from buffer(FIFO) and keep it in buffer or not
	*
	* \return T*				Pointer to the desired Value		
	* \param  bool remove		Flag for removal - true: remove - false: keep
	*/
	T* read(bool remove);

	/**
	* \brief Stores Value
	*
	* Stores a Value at the end of the buffer
	*
	* \return void
	*/
	void store(T* data);


	/**
	* \brief  Get Buffer-Size
	*
	* Get the current size of the buffer
	*
	* \return int		Current size
	*/
	int getSize() const { return _size; }

	/**
	* \brief Set Buffer-Size
	*
	* Set the Buffer-Size to a new value
	*
	* \return void
	* \param  int val	The new size
	*/
	int setSize(int val) { int oldSize = _size; _size = val; return oldSize; }

	/**
	* \brief Clears the buffer
	*
	*
	* \return void
	*/
	void clear();
	
	/**
	* \brief Gets the number of Elements
	*
	* Gets the current Number of Elements stored in the Buffer
	*
	* \return int		Number of strored Elements
	*/
	int getNumbElements() const;

	/**
	* \brief Resizes the Buffer
	*
	*
	* \return void
	* \param  int size		The new Buffer-Size
	*/
	void resize(int size);
		
	T& operator=(const T& source);

protected:
private:


public:
protected:

private:

	int _size;						//!< Current buffer size
	std::list<T*> _bufferList;		//!< List that keeps the stored elements
	QMutex *_pMutex;				//!< Mutex for multithreaded access

	
	/**
	* \brief Deletes a subset or all elements in the buffer
	*
	*
	* \return void
	* \param  int idxBegin Indicates from which to start when deleting elements
	*/
	void deleteAllElements(int idxBegin = 0); 

};

#include "RingBufferList.inl"

}
}
#endif /*RINGBUFFERLIST_H_*/
