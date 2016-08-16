/* lualvm, a Lua wrapper for the LLVM-C API
 * Copyright (C) 2016 gilzoide
 * 
 * This program is free software: you can redistribute it and/or modify
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
 *
 * Any bugs should be reported to <gilzoide@gmail.com>
 */

/** @file lualvm.hpp
 * Some common stuff, for internal lualvm use only
 */
#pragma once

// Lua includes
#include <lua.hpp>

// other stuff
#include <type_traits>
#include <iostream>
using namespace std;


/**
 * Auxiliary function that register types in Lua    [+1, -0, -]
 *
 * This function register a new Metatable, setting it's "__metatable" field to
 * `tname` and it's "__index" field to the functions, so methods can be used
 *
 * @param L Lua state
 * @param tname Metatable name
 * @param funcs Functions to be registered
 * @param nup Number of upvalues
 */
inline void registerLuaMetatable (lua_State *L, const char *tname
		, const luaL_Reg *funcs , int nup = 0) {
	// create the metatable
	luaL_newmetatable (L, tname);
	luaL_setfuncs (L, funcs, nup);
	// let "__index" point to the methods, so that we can use them.
	// duplicate the table, as it'll be consumed by `setfield`
	lua_pushvalue (L, -1);
	lua_setfield (L, -2, "__index");
	// and give it a nice name
	lua_pushstring (L, tname);
	lua_setfield (L, -2, "__metatable");
}


/**
 * Require module (from Lua), pushing it to the stack    [+1, -0, e]
 *
 * @param L Lua state
 * @param module Module name
 */
inline void callRequire (lua_State *L, const char *module) {
	lua_getglobal (L, "require");
	lua_pushstring (L, module);
	lua_call (L, 1, 1);
}


/**
 * Template for getting types from Lua, checking it's metatable    [+0, -0, e]
 *
 * @note This is made to work with both light and full userdata
 *
 * @param L Lua state
 * @param index Lua stack index
 * @param metatable Metatable name to be checked
 *
 * @return Whatever T type userdata there was
 */
template<typename T>
T lualvm_check (lua_State *L, int index, const char *metatable) {
	auto pointer = luaL_checkudata (L, index, metatable);
	if (lua_islightuserdata (L, index)) {
		return static_cast<T> (pointer);
	}
	else {
		auto value = static_cast<T> (* static_cast<T *> (pointer));
		if (!value) {
			lua_pushfstring (L, "null %s", metatable);
			lua_error (L);
		}

		return value;
	}
}


/**
 * Push a managed object into Lua, associating `metatable` to it    [+1, -0, -]
 *
 * @warning Objects pushed like this __will__ be garbage collected by Lua, so if
 * using pointers as the object, push each object only once, and use
 * non-managed pointers for references, or double free might happen
 *
 * @param L Lua state
 * @param value Value of managed object to be pushed
 * @param metatable Metatable to be associated to `value`
 */
template<typename T>
void lualvm_pushManaged (lua_State *L, T value, const char *metatable) {
	auto pointer = (T *) lua_newuserdata (L, sizeof (T));
	*pointer = value;
	luaL_getmetatable (L, metatable);
	lua_setmetatable (L, -2);
}


/**
 * Push a non-managed object into Lua, associating `metatable` to it (if not `nil`)    [+1, -0, -]
 *
 * @warning Lua light userdata __must__ be pointers, so compilation is restricted
 * to pointer types only (through a `static_assert`)
 *
 * @warning Objects pushed like this __will never__ be garbage collected by Lua, so
 * if using pointers as the object, either push one managed instance of it, or
 * deallocate it on C or with some method in Lua, or memory will leak
 *
 * @param L Lua state
 * @param ptr Pointer to be pushed
 * @param metatable Metatable to be associated to `value`
 */
template<typename T>
void lualvm_push (lua_State *L, T ptr, const char *metatable) {
	static_assert (std::is_pointer<T>::value, "Can't push Lua light userdata if it isn't a pointer!");
	if (ptr) {
		lua_pushlightuserdata (L, ptr);
		luaL_getmetatable (L, metatable);
		lua_setmetatable (L, -2);
	}
	else {
		lua_pushnil (L);
	}
}
