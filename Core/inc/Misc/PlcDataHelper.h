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
#ifndef __PlcDataHelper_H__
#define __PlcDataHelper_H__

#include "CoreDLL.h"
#include <string>
#include <typeindex>

#pragma warning(push)
#pragma warning(disable: 4251)

namespace Core
{
	namespace Misc
	{
		/**
		 * \brief PortConnectionHelper struct.
		 *
		 */
		class CORE_API PlcDataHelper
		{
		public:
			/**
			 * \brief constructor
			 *
			 */
			PlcDataHelper(const std::string& name, const std::type_index& type, const size_t& size, const void* data, const std::string& task, const std::string& path = "");

			/**
			 * \brief copy constructor
			 *
			 */
			PlcDataHelper(const PlcDataHelper& rhs);

			/**
			 * \brief destructor
			 *
			 */
			~PlcDataHelper();

			/**
			 * \brief check if type is equal to the given one
			 *
			 *	@param const std::type_info* type. The type to compare to.
			 *	@return true if type is equal.
			 */
			bool checkType(const std::type_index& type)const;

			/**
			 * \brief get type info from std string.
			 *
			 *	\return std::type_info
			 */
			static const type_info& getTypeInfoFromString(std::string type);

		#pragma region Setter

			/**
				Set the value of the Variable
				@param b boolean value
			*/
			void setValue(bool* b);

			/**
				Set the value of the Variable at the given index
				@param b boolean value
			*/
			void setValue(bool b, int index = 0);

			/**
				Set the value of the Variable
				@param i int value
			*/
			void setValue(int* i);

			/**
				Set the value of the Variable at the given index
				@param i int value
			*/
			void setValue(int i, int index = 0);

			/**
				Set the value of the Variable
				@param si short value
			*/
			void setValue(short* si);

			/**
				Set the value of the Variable at the given index
				@param si short value
			*/
			void setValue(short si, int index = 0);

			/**
				Set the value of the Variable
				@param i long value
			*/
			void setValue(long* li);

			/**
				Set the value of the Variable at the given index
				@param i long value
			*/
			void setValue(long li, int index = 0);

			/**
				Set the value of the Variable
				@param i long long value
			*/
			void setValue(long long* lli);

			/**
				Set the value of the Variable at the given index
				@param i long long value
			*/
			void setValue(long long lli, int index = 0);

			/**
				Set the value of the Variable
				@param sc char value (8 bit integer)
			*/
			void setValue(signed char* sc);

			/**
				Set the value of the Variable at the given index
				@param sc char value (8 bit integer)
			*/
			void setValue(signed char sc, int index = 0);
	
			/**
				Set the value of the Variable
				@param ui unsigned int value
			*/
			void setValue(unsigned int* ui);

			/**
				Set the value of the Variable at the given index
				@param ui unsigned int value
			*/
			void setValue(unsigned int ui, int index = 0);

			/**
				Set the value of the Variable
				@param ui unsigned int value
			*/
			void setValue(unsigned short* usi);

			/**
				Set the value of the Variable at the given index
				@param ui unsigned int value
			*/
			void setValue(unsigned short usi, int index = 0);

			/**
				Set the value of the Variable
				@param i unsigned long value
			*/
			void setValue(unsigned long* uli);

			/**
				Set the value of the Variable at the given index
				@param i unsigned long value
			*/
			void setValue(unsigned long uli, int index = 0);

			/**
				Set the value of the Variable
				@param i unsigned long long value
			*/
			void setValue(unsigned long long* ulli);

			/**
				Set the value of the Variable at the given index
				@param i unsigned long long value
			*/
			void setValue(unsigned long long ulli, int index = 0);

			/**
				Set the value of the Variable
				@param uc unsigned char value (8 bit unsigned integer)
			*/
			void setValue(unsigned char* uc);

			/**
				Set the value of the Variable at the given index
				@param uc unsigned char value (8 bit unsigned integer)
			*/
			void setValue(unsigned char uc, int index = 0);
	
			/**
				Set the value of the Variable
				@param f float value
			*/
			void setValue(float* f);

			/**
				Set the value of the Variable at the given index
				@param f float value
			*/
			void setValue(float f, int index = 0);

			/**
				Set the value of the Variable
				@param d double value
			*/
			void setValue(double* d);

			/**
				Set the value of the Variable at the given index
				@param d double value
			*/
			void setValue(double d, int index = 0);

			/**
				Set the value of the Variable
				@param s string value
			*/
			void setValue(char* s);

			/**
				Set the value of the Variable at the given index
				@param s string value
			*/
			void setValue(char s, int index = 0);

			/**
				Set the value of the Variable at the given index
				@param s string value
			*/
			void setValue( std::string s );

		#pragma endregion Setter

		#pragma region Getter

			/**
				Get the values of the Variable
				@return boolean array
			*/
			bool* getBool()const;

			/**
				Get the value of the Variable at the given index
				@return boolean value
			*/
			bool getBool(int index)const;

			/**
				Get the values of the Variable
				@return integer array
			*/
			int* getInteger()const;

			/**
				Get the value of the Variable at the given index
				@return integer value
			*/
			int getInteger(int index)const;
	
			/**
				Get the values of the Variable
				@return short array
			*/
			short* getShortInteger()const;

			/**
				Get the value of the Variable at the given index
				@return short value
			*/
			short getShortInteger(int index)const;

			/**
				Get the values of the Variable
				@return signed char array (8 bit integer)
			*/
			signed char* getSignedChar()const;

			/**
				Get the value of the Variable at the given index
				@return signed char value (8 bit integer)
			*/
			signed char getSignedChar(int index)const;

			/**
				Get the values of the Variable
				@return unsigned int array
			*/
			unsigned int* getUnsignedInteger()const;
	
			/**
				Get the value of the Variable at the given index
				@return unsigned int value
			*/
			unsigned int getUnsignedInteger(int index)const;

			/**
				Get the values of the Variable
				@return unsigned short array
			*/
			unsigned short* getUnsignedShortInteger()const;

			/**
				Get the value of the Variable at the given index
				@return unsigned short value
			*/
			unsigned short getUnsignedShortInteger(int index)const;

			/**
				Get the values of the Variable
				@return unsigned char array (8 bit unsigned int)
			*/
			unsigned char* getUnsignedChar()const;

			/**
				Get the value of the Variable at the given index
				@return unsigned char value (8 bit unsigned int)
			*/
			unsigned char getUnsignedChar(int index)const;

			/**
				Get the values of the Variable
				@return float array
			*/
			float* getFloat()const;

			/**
				Get the value of the Variable at the given index
				@return float value
			*/
			float getFloat(int index)const;

			/**
				Get the values of the Variable
				@return double array
			*/
			double* getDouble()const;

			/**
				Get the value of the Variable at the given index
				@return double value
			*/
			double getDouble(int index)const;

			/**
				Get the values of the Variable
				@return char array
			*/
			char* getChar()const;

			/**
				Get the value of the Variable at the given index
				@return char value
			*/
			char getChar(int index)const;

			/**
				Get the values of the Variable
				@return long array
			*/
			long* getLongInteger()const;

			/**
				Get the value of the Variable at the given index
				@return long value
			*/
			long getLongInteger(int index)const;

			/**
				Get the values of the Variable
				@return long long array
			*/
			long long* getLongLongInteger()const;

			/**
				Get the value of the Variable at the given index
				@return long long value
			*/
			long long getLongLongInteger(int index)const;

			/**
				Get the values of the Variable
				@return unsinged long array
			*/
			unsigned long* getUnsignedLongInteger()const;

			/**
				Get the value of the Variable at the given index
				@return unsigned long value
			*/
			unsigned long getUnsignedLongInteger(int index)const;

			/**
				Get the values of the Variable
				@return unsigned long long array
			*/
			unsigned long long* getUnsignedLongLongInteger()const;

			/**
				Get the value of the Variable at the given index
				@return unsigned long long value
			*/
			unsigned long long getUnsignedLongLongInteger(int index)const;

			/**
				Get the values of the Variable
				@return std::string
			*/
			std::string getString();

			/**
				Get the Name of the Variable
				@return Name of the Variable as std::string
			*/
			std::string getName()const;

			/**
				Get array size of the Variable
				@return size of the array.
			*/
			::size_t getArraySize()const;

			/**
				Get size in bytes of the Variable
				@return size in bytes
			*/
			::size_t getByteSize() const;

			/**
			 * \brief get the type index of the stored type.
			 *
			 *	\return std::type_index&. The type index of the stored type.
			 */
			const std::type_index& getType() const;

			/**
			 * \brief get the type info of the stored type.
			 *
			 *	\return std::type_info&. The type index of the stored type.
			 */
			const std::type_info& getPlcTypeInfo() const;

			/**
			 *  \brief Get the path of the Variable
			 *	
			 *	\return path of the Variable as std::string
			 */
			std::string getPath()const;

			/**
			 *  \brief Get the task name of the Variable
			 *	
			 *	\return task name of the Variable as std::string
			 */
			std::string getTaskName()const;
	
		#pragma endregion Getter

		#pragma region Operators

			operator bool(void) const
			{
				return getBool(0);
			}

			operator int(void) const
			{
				return getInteger(0);
			}

			operator short(void) const
			{
				return getShortInteger(0);
			}

			operator long(void) const
			{
				return getLongInteger(0);
			}

			operator long long(void) const
			{
				return getLongLongInteger(0);
			}

			operator signed char(void) const
			{
				return getSignedChar(0);
			}

			operator unsigned int(void) const
			{
				return getUnsignedInteger(0);
			}

			operator unsigned short(void) const
			{
				return getUnsignedShortInteger(0);
			}

			operator unsigned char(void) const
			{
				return getUnsignedChar(0);
			}

			operator unsigned long(void) const
			{
				return getUnsignedLongInteger(0);
			}

			operator unsigned long long(void) const
			{
				return getUnsignedLongLongInteger(0);
			}

			operator float(void) const
			{
				return getFloat(0);
			}

			operator double(void) const
			{
				return getDouble(0);
			}

			operator char(void) const
			{
				return getChar(0);
			}

			//array operators

			operator bool*(void) const
			{
				return getBool();
			}

			operator int*(void) const
			{
				return getInteger();
			}

			operator short*(void) const
			{
				return getShortInteger();
			}

			operator long*(void) const
			{
				return getLongInteger();
			}

			operator long long*(void) const
			{
				return getLongLongInteger();
			}

			operator signed char*(void) const
			{
				return getSignedChar();
			}

			operator unsigned int*(void) const
			{
				return getUnsignedInteger();
			}

			operator unsigned short*(void) const
			{
				return getUnsignedShortInteger();
			}

			operator unsigned char*(void) const
			{
				return getUnsignedChar();
			}

			operator unsigned long*(void) const
			{
				return getUnsignedLongInteger();
			}

			operator unsigned long long*(void) const
			{
				return getUnsignedLongLongInteger();
			}

			operator float*(void) const
			{
				return getFloat();
			}

			operator double*(void) const
			{
				return getDouble();
			}

			operator char*(void) const
			{
				return getChar();
			}

			PlcDataHelper operator= (const PlcDataHelper& rhs);

#pragma endregion Operators

		private:
			/**< Variant definition of the variable */
			union Variant
			{
				bool* b;
				int* i;
				short* si;
				long* li;
				long long* lli;
				signed char* sc;
				unsigned int* ui;
				unsigned short* usi;
				unsigned char* uc;
				unsigned long* uli;
				unsigned long long* ulli;
				float* f;
				double* d;
				char* s;
			};

			Variant						_value;				//<! Data storage of the variable.
			std::string					_name;				//<! Variable name.
			std::string					_path;				//<! The path.
			std::string					_task;				//<! The parent object name.
			const std::type_index		_type;				//<! The variable type.
			const size_t				_arraySize;			//<! The array size.
			size_t						_byteSize;			//<! The size in bytes.
		};
	};
};

#pragma warning(pop)
#endif //__PlcDataHelper_H__
