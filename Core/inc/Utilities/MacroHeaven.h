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

//MSVC's __VA_ARGS__ expansion seems to behave differently from C99. Therefor use EXPAND macro to call __VA_ARGS__
#define EXPAND(x) x
#define VARARG_PRINT_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, ...) _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32
#define VARARG_PRINT(...) EXPAND(__VA_ARGS__)
/**
 * \brief count macro arguments
 *
 */
#define VA_NARGS_IMPL(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, N, ...) N
#define VA_NARGS(...) EXPAND(VA_NARGS_IMPL(X, __VA_ARGS__, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

/**
 * \brief macro to call base0 to base32 macro
 *
 */
#define VARARG_IMPL2(base, count, ...) EXPAND(base##count(__VA_ARGS__))
#define VARARG_IMPL(base, count, ...) EXPAND(VARARG_IMPL2(base, count, __VA_ARGS__))
#define VARARG(base, ...) VARARG_IMPL(base, EXPAND(VA_NARGS(__VA_ARGS__)), __VA_ARGS__)

/**
* \brief macro to call base1 for each argument.
*		to execute the macro an empty macro base0 needs to be defined too.
*/
#define VARARG_FOREACH_CALL0(base)
#define VARARG_FOREACH_CALL1(base, _1) base(_1)
#define VARARG_FOREACH_CALL2(base, _1, _2)				\
	VARARG_FOREACH_CALL1(base, _1)						\
	VARARG_FOREACH_CALL1(base, _2)
#define VARARG_FOREACH_CALL3(base, _1, ...)				\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL2(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL4(base, _1, ...)				\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL3(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL5(base, _1, ...)				\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL4(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL6(base, _1, ...)				\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL5(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL7(base, _1, ...)				\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL6(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL8(base, _1, ...)				\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL7(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL9(base, _1, ...)				\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL8(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL10(base, _1, ...)			\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL9(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL11(base, _1, ...)			\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL10(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL12(base, _1, ...)			\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL11(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL13(base, _1, ...)			\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL12(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL14(base, _1, ...)			\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL13(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL15(base, _1, ...)			\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL14(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL16(base, _1, ...)			\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL15(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL17(base, _1, ...)			\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL16(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL18(base, _1, ...)			\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL17(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL19(base, _1, ...)			\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL18(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL20(base, _1, ...)			\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL19(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL21(base, _1, ...)			\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL20(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL22(base, _1, ...)			\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL21(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL23(base, _1, ...)			\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL22(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL24(base, _1, ...)			\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL23(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL25(base, _1, ...)			\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL24(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL26(base, _1, ...)			\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL25(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL27(base, _1, ...)			\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL26(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL28(base, _1, ...)			\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL27(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL29(base, _1, ...)			\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL28(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL30(base, _1, ...)			\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL29(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL31(base, _1, ...)			\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL30(base, __VA_ARGS__))
#define VARARG_FOREACH_CALL32(base, _1, ...)			\
	VARARG_FOREACH_CALL1(base, _1)						\
	EXPAND(VARARG_FOREACH_CALL31(base, __VA_ARGS__))

#define VARARG_FOREACH_IMPL2(base, count, ...) EXPAND(VARARG_FOREACH_CALL##count(base, ##__VA_ARGS__))
#define VARARG_FOREACH_IMPL(base, count, ...) EXPAND(VARARG_FOREACH_IMPL2(base, count, __VA_ARGS__))
#define VARARG_FOREACH(base, ...) EXPAND(VARARG_FOREACH_IMPL(base, VA_NARGS(__VA_ARGS__), __VA_ARGS__))
