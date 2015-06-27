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


#ifndef RINGBUFFER_H_
#define RINGBUFFER_H_


#if WIN32
#pragma once
#endif

#include <QtCore/QMutex>
#include <type_traits>
#include "CoreDLL.h"
#include <iostream>

namespace Core
{

namespace Buffer
{


template<class T>
class RingBuffer
{
	
public:

	/**
	* \brief Constructor
	*
	*
	* \return 
	* \param size_t size Size of the Buffer - Default: 1
	* \param bool overrideTail If true override Tail on insert else skip insert. - Default: true.
	*/
	RingBuffer(size_t size = 1, bool overrideTail = true);

	/**
	* \brief Destructor
	*
	* Destructor ~RingBuffer()
	*
	* \return 
	*/
	~RingBuffer();

	/**
	* \brief Return and remove last element from buffer.
	*
	* Read value from buffer (FIFO)
	*
	* \return T	Last element in the buffer.
	*/
	T pop();

	/**
	 * \brief Return last element in the buffer.
	 *
	 * \return T Last element in the buffer.
	 */
	T peek();

	/**
	 * \brief Return first element in the buffer.
	 *
	 * \return T first element in the buffer.
	 */
	T peekAtHead();

	/**
	* \brief Read at a special location
	*
	* Reads the Value at a special location.
	*
	* \return T	The Value at the desired position.
	* \param  size_t position Position relative to tail to read.
	*/
	T peekAt(const size_t position = 0);

	/**
	* \brief Stores Value
	*
	* Stores a Value at the end of the buffer
	*
	* \return void
	*/
	void push(const T);

	/**
	* \brief  Get Buffer-Size
	*
	* Get the current size of the buffer
	*
	* \return size_t		Current size
	*/
	size_t getSize() const;

	/**
	 * \brief get the number of stored elements.
	 *
	 *	\return size_t The current number of stored elements.
	 */
	size_t getNumberOfElements() const;

	/**
	* \brief Clears the buffer
	*
	*
	* \return void
	*/
	void clear();

	/**
	* \brief Resizes the Buffer
	*
	*
	* \return void
	* \param  int size		The new Buffer-Size
	*/
	void resize(const size_t size);

	/**
	 * \brief Indicates if ring buffer is empty.
	 *
	 *	\return bool Is the ring buffer empty.
	 */
	bool isEmpty() const;

	/**
	 * \brief Indicates if the ring buffer is full.
	 *
	 * \return bool Is the ring buffer full.
	 */
	bool isFull() const;
protected:
	/**
	 * \brief Ring buffer element structure.
	 *
	 *	A doubly link node element of the ring buffer.
	 */
	template <class R>
	struct Element
	{
		friend struct Deallocator;

		Element<R>* next;	//!< next ring buffer element.
		Element<R>* prev;	//!< previous ring buffer element.
		bool invalid;		//!< is element invalid.
		R element;			//!< the element.

		/**
		 * \brief Constructor
		 *
		 */
		Element()
		{
			invalid = true;
		}

		/**
		 * \brief Destructor
		 *
		 */
		~Element()
		{
		}
	};
private:
	/**
	 * \brief internal insert ring buffer element.
	 *
	 *	Insert element structure in the ring buffer (pre tail).
	 */
	inline void insertElement();
	
	/**
	 * \brief remove ring buffer element.
	 *
	 *	Remove a element structure from the ring buffer.
	 */
	inline void removeElement();

private:
 	size_t _size;				//!< Actual Buffer size.
	size_t _numberOfElements;	//!< Actual number of stored Elements.
 	Element<T>* _head;			//!< Number of the head element.
 	Element<T>* _tail;			//!< Number of the tail element.
	bool _overrideTail;			//!< If true override tail on insert else skip insert. 
	bool _isFull;				//!< Is the ring buffer full.
	QMutex* _mutex;				//!< Mutex for multithreaded access
};

#include "RingBuffer.inl"

}
}

#endif /* RINGBUFFER_H_ */
