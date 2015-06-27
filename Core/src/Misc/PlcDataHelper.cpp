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

#include "Misc/PlcDataHelper.h"
#include <assert.h>

namespace Core
{
namespace Misc
{
	PlcDataHelper::PlcDataHelper( const std::string& name, const std::type_index& type, const size_t& size, const void* data, const std::string& task, const std::string& path /*= ""*/)
		:_arraySize(size),
		_name(name),
		_type(type),
		_path(path),
		_task(task)
	{
		if(path.length() <= 0)
		{
			_path = _name;
		}

		if(_type == typeid(bool) || _type == typeid(bool*))
		{
			_value.b = new bool[_arraySize];
			_byteSize = _arraySize * sizeof(bool);
			memcpy(_value.b, data, _byteSize);
		}
		else if(_type == typeid(int) || _type == typeid(int*))
		{
			_value.i = new int[_arraySize];
			_byteSize = _arraySize * sizeof(int);
			memcpy(_value.i, data, _byteSize);
		}
		else if(_type == typeid(short) || _type == typeid(short*))
		{
			_value.si = new short[_arraySize];
			_byteSize = _arraySize * sizeof(short);
			memcpy(_value.si, data, _byteSize);
		}
		else if(_type == typeid(long) || _type == typeid(long*))
		{
			_value.li = new long[_arraySize];
			_byteSize = _arraySize * sizeof(long);
			memcpy(_value.li, data, _byteSize);
		}
		else if(_type == typeid(long long) || _type == typeid(long long*))
		{
			_value.lli = new long long[_arraySize];
			_byteSize = _arraySize * sizeof(long long);
			memcpy(_value.lli, data, _byteSize);
		}
		else if(_type == typeid(signed char) || _type == typeid(signed char*))
		{
			_value.sc = new signed char[_arraySize];
			_byteSize = _arraySize * sizeof(signed char);
			memcpy(_value.sc, data, _byteSize);
		}
		else if(_type == typeid(unsigned int) || _type == typeid(unsigned int*))
		{
			_value.ui = new unsigned int[_arraySize];
			_byteSize = _arraySize * sizeof(unsigned int);
			memcpy(_value.ui, data, _byteSize);
		}
		else if(_type == typeid(unsigned short) || _type == typeid(unsigned short*))
		{
			_value.usi = new unsigned short[_arraySize];
			_byteSize = _arraySize * sizeof(unsigned short);
			memcpy(_value.usi, data, _byteSize);
		}
		else if(_type == typeid(unsigned long) || _type == typeid(unsigned long*))
		{
			_value.uli = new unsigned long[_arraySize];
			_byteSize = _arraySize * sizeof(unsigned long);
			memcpy(_value.uli, data, _byteSize);
		}
		else if(_type == typeid(unsigned long long) || _type == typeid(unsigned long long*))
		{
			_value.ulli = new unsigned long long[_arraySize];
			_byteSize = _arraySize * sizeof(unsigned long long);
			memcpy(_value.ulli, data, _byteSize);
		}
		else if(_type == typeid(unsigned char) || _type == typeid(unsigned char*))
		{
			_value.uc = new unsigned char[_arraySize];
			_byteSize = _arraySize * sizeof(unsigned char);
			memcpy(_value.uc, data, _byteSize);
		}
		else if(_type == typeid(float) || _type == typeid(float*))
		{
			_value.f = new float[_arraySize];
			_byteSize = _arraySize * sizeof(float);
			memcpy(_value.f, data, _byteSize);
		}
		else if(_type == typeid(double) || _type == typeid(double*))
		{
			_value.d = new double[_arraySize];
			_byteSize = _arraySize * sizeof(double);
			memcpy(_value.d, data, _byteSize);
		}
		else if(_type == typeid(char) || _type == typeid(char*))
		{
			_value.s = new char[_arraySize];
			_byteSize = _arraySize * sizeof(char);
			memcpy(_value.s, data, _byteSize);
		}
	}

	PlcDataHelper::PlcDataHelper( const PlcDataHelper& rhs )
		:_arraySize(rhs._arraySize),
		_name(rhs._name),
		_type(rhs._type),
		_path(rhs._path),
		_task(rhs._task)
	{
		if(_type == typeid(bool) || _type == typeid(bool*))
		{
			_value.b = new bool[_arraySize];
			_byteSize = _arraySize * sizeof(bool);
			memcpy(_value.b, rhs._value.b, _byteSize);
		}
		else if(_type == typeid(int) || _type == typeid(int*))
		{
			_value.i = new int[_arraySize];
			_byteSize = _arraySize * sizeof(int);
			memcpy(_value.i, rhs._value.i, _byteSize);
		}
		else if(_type == typeid(short) || _type == typeid(short*))
		{
			_value.si = new short[_arraySize];
			_byteSize = _arraySize * sizeof(short);
			memcpy(_value.si, rhs._value.si, _byteSize);
		}
		else if(_type == typeid(signed char) || _type == typeid(signed char*))
		{
			_value.sc = new signed char[_arraySize];
			_byteSize = _arraySize * sizeof(signed char);
			memcpy(_value.sc, rhs._value.sc, _byteSize);
		}
		else if(_type == typeid(unsigned int) || _type == typeid(unsigned int*))
		{
			_value.ui = new unsigned int[_arraySize];
			_byteSize = _arraySize * sizeof(unsigned int);
			memcpy(_value.ui, rhs._value.ui, _byteSize);
		}
		else if(_type == typeid(unsigned short) || _type == typeid(unsigned short*))
		{
			_value.usi = new unsigned short[_arraySize];
			_byteSize = _arraySize * sizeof(unsigned short);
			memcpy(_value.usi, rhs._value.usi, _byteSize);
		}
		else if(_type == typeid(unsigned char) || _type == typeid(unsigned char*))
		{
			_value.uc = new unsigned char[_arraySize];
			_byteSize = _arraySize * sizeof(unsigned char);
			memcpy(_value.uc, rhs._value.uc, _byteSize);
		}
		else if(_type == typeid(float) || _type == typeid(float*))
		{
			_value.f = new float[_arraySize];
			_byteSize = _arraySize * sizeof(float);
			memcpy(_value.f, rhs._value.f, _byteSize);
		}
		else if(_type == typeid(double) || _type == typeid(double*))
		{
			_value.d = new double[_arraySize];
			_byteSize = _arraySize * sizeof(double);
			memcpy(_value.d, rhs._value.d, _byteSize);
		}
		else if(_type == typeid(char) || _type == typeid(char*))
		{
			_value.s = new char[_arraySize];
			_byteSize = _arraySize * sizeof(char);
			memcpy(_value.s, rhs._value.s, _byteSize);
		}
	}

	PlcDataHelper::~PlcDataHelper()
	{
		if(_type == typeid(bool) || _type == typeid(bool*))
		{
			delete[] _value.b;
		}
		else if(_type == typeid(int) || _type == typeid(int*))
		{
			delete[] _value.i;
		}
		else if(_type == typeid(short) || _type == typeid(short*))
		{
			delete[] _value.si;
		}
		else if(_type == typeid(signed char) || _type == typeid(signed char*))
		{
			delete[] _value.sc;
		}
		else if(_type == typeid(unsigned int) || _type == typeid(unsigned int*))
		{
			delete[] _value.ui;
		}
		else if(_type == typeid(unsigned short) || _type == typeid(unsigned short*))
		{
			delete[] _value.usi;
		}
		else if(_type == typeid(unsigned char) || _type == typeid(unsigned char*))
		{
			delete[] _value.uc;
		}
		else if(_type == typeid(float) || _type == typeid(float*))
		{
			delete[] _value.f;
		}
		else if(_type == typeid(double) || _type == typeid(double*))
		{
			delete[] _value.d;
		}
		else if(_type == typeid(char) || _type == typeid(char*))
		{
			delete[] _value.s;
		}
	}

	PlcDataHelper PlcDataHelper::operator=( const PlcDataHelper& rhs )
	{
		return PlcDataHelper(rhs);
	}

	void PlcDataHelper::setValue( bool* b )
	{
		memcpy(_value.b, b, _byteSize);
	}

	void PlcDataHelper::setValue( bool b, int index /*= 0*/ )
	{
		if( index >= 0)
		{
			_value.b[index] = b;
		}
		else if( index == -1)
		{
			memset(_value.b, b, _byteSize);
		}
	}

	void PlcDataHelper::setValue( int* i )
	{
		memcpy(_value.i, i, _byteSize);
	}

	void PlcDataHelper::setValue( int i, int index /*= 0*/ )
	{
		if( index >= 0)
		{
			_value.i[index] = i;
		}
		else if( index == -1)
		{
			memset(_value.i, i, _byteSize);
		}
	}

	void PlcDataHelper::setValue( short* si )
	{
		memcpy(_value.si, si, _byteSize);
	}

	void PlcDataHelper::setValue( short si, int index /*= 0*/ )
	{
		if( index >= 0)
		{
			_value.si[index] = si;
		}
		else if( index == -1)
		{
			memset(_value.si, si, _byteSize);
		}
	}

	void PlcDataHelper::setValue( signed char* sc )
	{
		memcpy(_value.sc, sc, _byteSize);
	}

	void PlcDataHelper::setValue( signed char sc, int index /*= 0*/ )
	{
		if( index >= 0)
		{
			_value.sc[index] = sc;
		}
		else if( index == -1)
		{
			memset(_value.sc, sc, _byteSize);
		}
	}

	void PlcDataHelper::setValue( unsigned int* ui )
	{
		memcpy(_value.ui, ui, _byteSize);
	}

	void PlcDataHelper::setValue( unsigned int ui, int index /*= 0*/ )
	{
		if( index >= 0)
		{
			_value.ui[index] = ui;
		}
		else if( index == -1)
		{
			memset(_value.ui, ui, _byteSize);
		}
	}

	void PlcDataHelper::setValue( unsigned short* usi )
	{
		memcpy(_value.usi, usi, _byteSize);
	}

	void PlcDataHelper::setValue( unsigned short usi, int index /*= 0*/ )
	{
		if( index >= 0)
		{
			_value.usi[index] = usi;
		}
		else if( index == -1)
		{
			memset(_value.usi, usi, _byteSize);
		}
	}

	void PlcDataHelper::setValue( unsigned char* uc )
	{
		memcpy(_value.uc, uc, _byteSize);
	}

	void PlcDataHelper::setValue( unsigned char uc, int index /*= 0*/ )
	{
		if( index >= 0)
		{
			_value.uc[index] = uc;
		}
		else if( index == -1)
		{
			memset(_value.uc, uc, _byteSize);
		}
	}

	void PlcDataHelper::setValue( float* f )
	{
		memcpy(_value.f, f, _byteSize);
	}

	void PlcDataHelper::setValue( float f, int index /*= 0*/ )
	{
		if( index >= 0)
		{
			_value.f[index] = f;
		}
		else if( index == -1)
		{
			for(::size_t i = 0; i < _arraySize; i++)
			{
				_value.f[i] = f;
			}
		}
	}

	void PlcDataHelper::setValue( double* d )
	{
		memcpy(_value.d, d, _byteSize);
	}

	void PlcDataHelper::setValue( double d, int index /*= 0*/ )
	{
		if( index >= 0)
		{
			_value.d[index] = d;
		}
		else if( index == -1)
		{
			for(::size_t i = 0; i < _arraySize; i++)
			{
				_value.d[i] = d;
			}
		}
	}

	void PlcDataHelper::setValue( char* s )
	{
		memcpy(_value.s, s, _byteSize);
	}

	void PlcDataHelper::setValue( char s, int index /*= 0*/ )
	{
		if( index >= 0)
		{
			_value.s[index] = s;
		}
		else if( index == -1)
		{
			memset(_value.s, s, _byteSize);
		}
	}

	void PlcDataHelper::setValue( std::string s )
	{
		memcpy(_value.s, s.c_str(), _byteSize);
	}

	void PlcDataHelper::setValue( long* li )
	{
		memcpy(_value.li, li, _byteSize);
	}

	void PlcDataHelper::setValue( long li, int index /*= 0*/ )
	{
		if( index >= 0)
		{
			_value.li[index] = li;
		}
		else if( index == -1)
		{
			std::fill(_value.li, _value.li + _arraySize, li);
		}
	}

	void PlcDataHelper::setValue( long long* lli )
	{
		memcpy(_value.lli, lli, _byteSize);
	}

	void PlcDataHelper::setValue( long long lli, int index /*= 0*/ )
	{
		if( index >= 0)
		{
			_value.lli[index] = lli;
		}
		else if( index == -1)
		{
			std::fill(_value.lli, _value.lli + _arraySize, lli);
		}
	}

	void PlcDataHelper::setValue( unsigned long* uli )
	{
		memcpy(_value.uli, uli, _byteSize);
	}

	void PlcDataHelper::setValue( unsigned long uli, int index /*= 0*/ )
	{
		if( index >= 0)
		{
			_value.uli[index] = uli;
		}
		else if( index == -1)
		{
			std::fill(_value.uli, _value.uli + _arraySize, uli);
		}
	}

	void PlcDataHelper::setValue( unsigned long long* ulli )
	{
		memcpy(_value.ulli, ulli, _byteSize);
	}

	void PlcDataHelper::setValue( unsigned long long ulli, int index /*= 0*/ )
	{
		if( index >= 0)
		{
			_value.ulli[index] = ulli;
		}
		else if( index == -1)
		{
			std::fill(_value.ulli, _value.ulli + _arraySize, ulli);
		}
	}

	bool* PlcDataHelper::getBool() const
	{
		assert((_type == typeid(bool) || _type == typeid(bool*)));
		return _value.b;
	}

	bool PlcDataHelper::getBool( int index ) const
	{
		assert((_type == typeid(bool) || _type == typeid(bool*)));
		assert(index >= 0);
		if(index >= 0)
		{
			return _value.b[index];
		}
		else
		{
			return false;
		}
	}

	int* PlcDataHelper::getInteger() const
	{
		assert((_type == typeid(int) || _type == typeid(int*)));
		return _value.i;
	}

	int PlcDataHelper::getInteger( int index ) const
	{
		assert((_type == typeid(int) || _type == typeid(int*)));
		assert(index >= 0);
		if(index >= 0)
		{
			return _value.i[index];
		}
		else
		{
			return 0;
		}
	}

	short* PlcDataHelper::getShortInteger() const
	{
		assert((_type == typeid(short) || _type == typeid(short*)));
		return _value.si;
	}

	short PlcDataHelper::getShortInteger( int index ) const
	{
		assert((_type == typeid(short) || _type == typeid(short*)));
		assert(index >= 0);
		if(index >= 0)
		{
			return _value.si[index];
		}
		else
		{
			return 0;
		}
	}

	signed char* PlcDataHelper::getSignedChar() const
	{
		assert((_type == typeid(signed char) || _type == typeid(signed char*)));
		return _value.sc;
	}

	signed char PlcDataHelper::getSignedChar( int index ) const
	{
		assert((_type == typeid(signed char) || _type == typeid(signed char*)));
		assert(index >= 0);
		if(index >= 0)
		{
			return _value.sc[index];
		}
		else
		{
			return 0;
		}
	}

	unsigned int* PlcDataHelper::getUnsignedInteger() const
	{
		assert((_type == typeid(unsigned int) || _type == typeid(unsigned int*)));
		return _value.ui;
	}

	unsigned int PlcDataHelper::getUnsignedInteger( int index ) const
	{
		assert((_type == typeid(unsigned int) || _type == typeid(unsigned int*)));
		assert(index >= 0);
		if(index >= 0)
		{
			return _value.ui[index];
		}
		else
		{
			return 0;
		}
	}

	unsigned short* PlcDataHelper::getUnsignedShortInteger() const
	{
		assert((_type == typeid(unsigned short) || _type == typeid(unsigned short*)));
		return _value.usi;
	}

	unsigned short PlcDataHelper::getUnsignedShortInteger( int index ) const
	{
		assert((_type == typeid(unsigned short) || _type == typeid(unsigned short*)));
		assert(index >= 0);
		if(index >= 0)
		{
			return _value.usi[index];
		}
		else
		{
			return 0;
		}
	}

	unsigned char* PlcDataHelper::getUnsignedChar() const
	{
		assert((_type == typeid(unsigned char) || _type == typeid(unsigned char*)));
		return _value.uc;
	}

	unsigned char PlcDataHelper::getUnsignedChar( int index ) const
	{
		assert((_type == typeid(unsigned char) || _type == typeid(unsigned char*)));
		assert(index >= 0);
		if(index >= 0)
		{
			return _value.uc[index];
		}
		else
		{
			return 0;
		}
	}

	float* PlcDataHelper::getFloat() const
	{
		assert((_type == typeid(float) || _type == typeid(float*)));
		return _value.f;
	}

	float PlcDataHelper::getFloat( int index ) const
	{
		assert((_type == typeid(float) || _type == typeid(float*)));
		assert(index >= 0);
		if(index >= 0)
		{
			return _value.f[index];
		}
		else
		{
			return 0;
		}
	}

	double* PlcDataHelper::getDouble() const
	{
		assert((_type == typeid(double) || _type == typeid(double*)));
		return _value.d;
	}

	double PlcDataHelper::getDouble( int index ) const
	{
		assert((_type == typeid(double) || _type == typeid(double*)));
		assert(index >= 0);
		if(index >= 0)
		{
			return _value.d[index];
		}
		else
		{
			return 0;
		}
	}

	char* PlcDataHelper::getChar() const
	{
		assert((_type == typeid(char) || _type == typeid(char*)));
		return _value.s;
	}

	char PlcDataHelper::getChar( int index ) const
	{
		assert((_type == typeid(char) || _type == typeid(char*)));
		assert(index >= 0);
		if(index >= 0)
		{
			return _value.s[index];
		}
		else
		{
			return 0;
		}
	}

	std::string PlcDataHelper::getString()
	{
		assert((_type == typeid(char) || _type == typeid(char*)));
		return _value.s;
	}

	std::string PlcDataHelper::getName() const
	{
		return _name;
	}

	std::string PlcDataHelper::getPath() const
	{
		return _path;
	}

	std::string PlcDataHelper::getTaskName() const
	{
		return _task;
	}

	::size_t PlcDataHelper::getArraySize() const
	{
		return _arraySize;
	}

	::size_t PlcDataHelper::getByteSize() const
	{
		return _byteSize;
	}

	bool PlcDataHelper::checkType( const std::type_index& type ) const
	{
		return _type == type || std::type_index(getPlcTypeInfo()) == type;
	}

	const std::type_index& PlcDataHelper::getType() const
	{
		return _type;
	}

	const std::type_info& PlcDataHelper::getPlcTypeInfo() const
	{
		if(_type == typeid(bool) || _type == typeid(bool*))
		{
			return typeid(bool);
		}
		else if(_type == typeid(int) || _type == typeid(int*))
		{
			return typeid(int);
		}
		else if(_type == typeid(short) || _type == typeid(short*))
		{
			return typeid(short);
		}
		else if(_type == typeid(signed char) || _type == typeid(signed char*))
		{
			return typeid(signed char);
		}
		else if(_type == typeid(unsigned int) || _type == typeid(unsigned int*))
		{
			return typeid(unsigned int);
		}
		else if(_type == typeid(unsigned short) || _type == typeid(unsigned short*))
		{
			return typeid(unsigned short);
		}
		else if(_type == typeid(unsigned char) || _type == typeid(unsigned char*))
		{
			return typeid(unsigned char);
		}
		else if(_type == typeid(float) || _type == typeid(float*))
		{
			return typeid(float);
		}
		else if(_type == typeid(double) || _type == typeid(double*))
		{
			return typeid(double);
		}
		else if(_type == typeid(char) || _type == typeid(char*))
		{
			return typeid(char);
		}
		return typeid(void);
	}

	const type_info& PlcDataHelper::getTypeInfoFromString( std::string type )
	{
		if(type.compare(typeid(bool).name()) == 0)
		{
			return typeid(bool);
		}
		else if (type.compare(typeid(int).name()) == 0)
		{
			return typeid(int);
		}
		else if (type.compare(typeid(short).name()) == 0)
		{
			return typeid(short);
		}
		else if (type.compare(typeid(signed char).name()) == 0)
		{
			return typeid(signed char);
		}
		else if (type.compare(typeid(unsigned int).name()) == 0)
		{
			return typeid(unsigned int);
		}
		else if (type.compare(typeid(unsigned short).name()) == 0)
		{
			return typeid(unsigned short);
		}
		else if (type.compare(typeid(unsigned char).name()) == 0)
		{
			return typeid(unsigned char);
		}
		else if (type.compare(typeid(float).name()) == 0)
		{
			return typeid(float);
		}
		else if (type.compare(typeid(double).name()) == 0)
		{
			return typeid(double);
		}
		else if (type.compare(typeid(char).name()) == 0)
		{
			return typeid(char);
		}
		else if (type.compare(typeid(std::string).name()) == 0)
		{
			return typeid(std::string);
		}
		else if(type.compare("bool*") == 0)
		{
			return typeid(bool*);
		}
		else if (type.compare("int*") == 0)
		{
			return typeid(int*);
		}
		else if (type.compare("short*") == 0)
		{
			return typeid(short*);
		}
		else if (type.compare("signed char*") == 0)
		{
			return typeid(signed char*);
		}
		else if (type.compare("unsigned int*") == 0)
		{
			return typeid(unsigned int*);
		}
		else if (type.compare("unsigned short*") == 0)
		{
			return typeid(unsigned short*);
		}
		else if (type.compare("unsigned char*") == 0)
		{
			return typeid(unsigned char*);
		}
		else if (type.compare("float*") == 0)
		{
			return typeid(float*);
		}
		else if (type.compare("double*") == 0)
		{
			return typeid(double*);
		}
		else if (type.compare("char*") == 0)
		{
			return typeid(char*);
		}
		return typeid(int);
	}

	long* PlcDataHelper::getLongInteger() const
	{
		assert((_type == typeid(long) || _type == typeid(long*)));
		return _value.li;
	}

	long PlcDataHelper::getLongInteger( int index ) const
	{
		assert((_type == typeid(long) || _type == typeid(long*)));
		assert(index >= 0);
		if(index >= 0)
		{
			return _value.li[index];
		}
		else
		{
			return 0;
		}
	}

	long long* PlcDataHelper::getLongLongInteger() const
	{
		assert((_type == typeid(long long) || _type == typeid(long long*)));
		return _value.lli;
	}

	long long PlcDataHelper::getLongLongInteger( int index ) const
	{
		assert((_type == typeid(long long) || _type == typeid(long long*)));
		assert(index >= 0);
		if(index >= 0)
		{
			return _value.lli[index];
		}
		else
		{
			return 0;
		}
	}

	unsigned long* PlcDataHelper::getUnsignedLongInteger() const
	{
		assert((_type == typeid(unsigned long) || _type == typeid(unsigned long*)));
		return _value.uli;
	}

	unsigned long PlcDataHelper::getUnsignedLongInteger( int index ) const
	{
		assert((_type == typeid(unsigned long) || _type == typeid(unsigned long*)));
		assert(index >= 0);
		if(index >= 0)
		{
			return _value.uli[index];
		}
		else
		{
			return 0;
		}
	}

	unsigned long long* PlcDataHelper::getUnsignedLongLongInteger() const
	{
		assert((_type == typeid(unsigned long long) || _type == typeid(unsigned long long*)));
		return _value.ulli;
	}

	unsigned long long PlcDataHelper::getUnsignedLongLongInteger( int index ) const
	{
		assert((_type == typeid(unsigned long long) || _type == typeid(unsigned long long*)));
		assert(index >= 0);
		if(index >= 0)
		{
			return _value.ulli[index];
		}
		else
		{
			return 0;
		}
	}

}; //namespace Misc
}; //namespace Core
