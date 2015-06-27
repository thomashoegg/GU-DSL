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

template<class T> RingBuffer<T>::RingBuffer(size_t size/* = 1*/, bool overrideTail /*= true*/)
{
	_mutex = new QMutex();
	if(size < 1)
	{
		size = 1;
		std::cerr << "Minimum size is 1. Reset size of ringbuffer to 1." <<std::endl;
	}
	//create first element
	//set head and tail on first and only element
	_tail = new Element<T>();
	_head = _tail;

	//set next and prev from first and only element on himself
	_tail->next = _tail;
	_tail->prev = _tail;
		

	for(size_t i= 1; i<size; i++)
	{
		insertElement();
	}
	_size = size;
	_isFull = false;
	_overrideTail = overrideTail;
	_numberOfElements = 0;
}

template<class T> RingBuffer<T>::~RingBuffer()
{
	for(size_t i = 1; i < _size; i++)
	{
		removeElement();
	}
	delete _tail;
	_tail = nullptr;
	_head = nullptr;
	_size = 0;
}

template<class T> void RingBuffer<T>::insertElement()
{
	//add new element before tail.
	Element<T>* tmp = new Element<T>();
	tmp->prev = _tail->prev;
	tmp->next = _tail;
	_tail->prev = tmp;
	tmp->prev->next = tmp;
}

template<class T> void RingBuffer<T>::removeElement()
{
	Element<T>* tmp = nullptr;

	if(_tail->prev != _head)
	{
		//remove element before tail
		tmp = _tail->prev;
	}
	else if(_overrideTail)
	{
		//remove element tail
		tmp = _tail;
		//set new tail
		_tail = tmp->next;
	}
	else
	{
		//remove element head
		tmp = _head;
		//set new head
		_head = tmp->prev;
	}

	if(tmp != nullptr)
	{
		_tail->prev = tmp->prev;
		tmp->prev->next = _tail;
		tmp->prev = nullptr;
		tmp->next = nullptr;
		delete tmp;
		tmp = nullptr;
	}
}

template<class T> T RingBuffer<T>::pop()
{
	QMutexLocker locker(_mutex);
	Element<T>* tmp = _tail;
	if(!isEmpty() || !tmp->invalid)
	{
		_tail = _tail->next;
		tmp->invalid = true;
		_numberOfElements--;
		if(_isFull)
		{
			_isFull = false;
		}
		return tmp->element;
	}
	else
	{
		if(std::is_pointer<T>::value)
		{
			return nullptr;
		}
		else
		{
			return T();
		}
	}
}

template<class T> T RingBuffer<T>::peek()
{
	QMutexLocker locker(_mutex);
	Element<T>* tmp = _tail;
	if(!isEmpty() || !tmp->invalid)
	{
		return tmp->element;
	}
	else
	{
		if(std::is_pointer<T>::value)
		{
			return nullptr;
		}
		else
		{
			return T();
		}
	}
}

template<class T> T RingBuffer<T>::peekAtHead()
{
	QMutexLocker locker(_mutex);
	Element<T>* tmp = _head->prev;
	if(!isEmpty() || !tmp->invalid)
	{
		return tmp->element;
	}
	else
	{
		if(std::is_pointer<T>::value)
		{
			return nullptr;
		}
		else
		{
			return T();
		}
	}
}

template<class T> T RingBuffer<T>::peekAt(const size_t position /*= 0*/)
{
	QMutexLocker locker(_mutex);
	Element<T>* tmp = _tail;
	if(!isEmpty())
	{
		for(size_t i = 0; i<position; i++)
		{
			tmp = tmp->next;
		}

		if(!tmp->invalid)
		{
			return tmp->element;
		}
	}
	if(std::is_pointer<T>::value)
	{
		return nullptr;
	}
	else
	{
		return T();
	}
}

template<class T> void RingBuffer<T>::push(const T value)
{
	//skip insert if buffer is full and not override tail 
	if(_isFull && !_overrideTail)
	{
		return;
	}
	QMutexLocker locker(_mutex);
	_head->element = value;
	_head->invalid = false;
	_head = _head->next;
	//if full move _tail
	if(_isFull)
	{
		_tail = _tail->next;
	}
	else
	{
		_numberOfElements++;
	}
	//check if ring buffer is full
	if(_head == _tail)
	{
		_isFull = true;
	}
}


template<class T> void RingBuffer<T>::clear()
{
	QMutexLocker locker(_mutex);
	_head = _tail;
	Element<T>* tmp = _tail;
	do
	{
		tmp->invalid = true;
		tmp = tmp->prev;
	}
	while(tmp != _tail);
	_numberOfElements = 0;
	_isFull = false;
}

template<class T> void RingBuffer<T>::resize(const size_t size )
{
	if(size > 1)
	{
		QMutexLocker locker(_mutex);
		//expand
		if(size > _size)
		{
			for(size_t i = _size; i < size; i++)
			{
				insertElement();
			}
			if(_isFull)
			{
				_isFull = false;
			}
		}
		//shrink
		else if(size < _size)
		{
			for(size_t i = size; i < _size; i++)
			{
				removeElement();
			}
			//check if ring buffer is full
			if(_head == _tail)
			{
				_isFull = true;
			}
		}
		_size = size;
	}
}

template<class T> size_t RingBuffer<T>::getSize() const 
{ 
	return _size;
}

template<class T> size_t RingBuffer<T>::getNumberOfElements() const
{
	return _numberOfElements;
}

template<class T> bool RingBuffer<T>::isEmpty() const 
{
	return !_isFull && _head == _tail;
}

template<class T> bool RingBuffer<T>::isFull() const 
{
	return _isFull;
}
