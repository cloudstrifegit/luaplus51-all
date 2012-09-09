///////////////////////////////////////////////////////////////////////////////
// This source file is part of the LuaPlus source distribution and is Copyright
// 2001-2012 by Joshua C. Jensen (jjensen@workspacewhiz.com).
//
// The latest version may be obtained from http://luaplus.org/.
//
// The code presented in this file may be used in any environment it is
// acceptable to use Lua.
///////////////////////////////////////////////////////////////////////////////
#ifndef LUAPLUS__LUAPLUSCD_H
#define LUAPLUS__LUAPLUSCD_H

LUA_EXTERN_C_BEGIN
#include "src/lua.h"
#include "src/lauxlib.h"
LUA_EXTERN_C_END

#include <stdlib.h>
#include <string.h>

// LuaPlus Call Dispatcher
namespace LPCD {

	struct LuaLightUserdata {
		LuaLightUserdata(const void* value) :
			m_value(value) {
		}

		const void* m_value;
	};


	struct LuaUserdata {
		LuaUserdata(const void* value) :
			m_value(value) {
		}

		const void* m_value;
	};

	struct LuaNil {
	};

	template<typename T> void Push(lua_State* L, T value);

	template<class T> struct TypeWrapper {};

	inline bool	Match(TypeWrapper<bool>, lua_State* L, int idx)
		{  return lua_type(L, idx) == LUA_TBOOLEAN;  }
	inline bool	Match(TypeWrapper<char>, lua_State* L, int idx)
		{  return lua_type(L, idx) == LUA_TNUMBER;  }
	inline bool	Match(TypeWrapper<unsigned char>, lua_State* L, int idx)
		{  return lua_type(L, idx) == LUA_TNUMBER;  }
	inline bool	Match(TypeWrapper<short>, lua_State* L, int idx)
		{  return lua_type(L, idx) == LUA_TNUMBER;  }
	inline bool	Match(TypeWrapper<unsigned short>, lua_State* L, int idx)
		{  return lua_type(L, idx) == LUA_TNUMBER;  }
	inline bool	Match(TypeWrapper<int>, lua_State* L, int idx)
		{  return lua_type(L, idx) == LUA_TNUMBER;  }
	inline bool	Match(TypeWrapper<unsigned int>, lua_State* L, int idx)
		{  return lua_type(L, idx) == LUA_TNUMBER;  }
	inline bool	Match(TypeWrapper<long>, lua_State* L, int idx)
		{  return lua_type(L, idx) == LUA_TNUMBER;  }
	inline bool	Match(TypeWrapper<unsigned long>, lua_State* L, int idx)
		{  return lua_type(L, idx) == LUA_TNUMBER;  }
	inline bool	Match(TypeWrapper<float>, lua_State* L, int idx)
		{  int type = lua_type(L, idx);  return type == LUA_TNUMBER;  }
	inline bool	Match(TypeWrapper<double>, lua_State* L, int idx)
		{  int type = lua_type(L, idx);  return type == LUA_TNUMBER;  }
	inline bool	Match(TypeWrapper<const char*>, lua_State* L, int idx)
		{  return lua_type(L, idx) == LUA_TSTRING;  }
	inline bool	Match(TypeWrapper<lua_State*>, lua_State* L, int idx)
		{  return lua_type(L, idx) == LUA_TNONE;  }
	inline bool	Match(TypeWrapper<void*>, lua_State* L, int idx)
		{  return lua_type(L, idx) == LUA_TLIGHTUSERDATA;  }

	inline void				Get(TypeWrapper<void>, lua_State*, int)
		{  }
	inline bool				Get(TypeWrapper<bool>, lua_State* L, int idx)
		{  return lua_toboolean(L, idx) != 0;  }
	inline char				Get(TypeWrapper<char>, lua_State* L, int idx)
		{  return static_cast<char>(lua_tonumber(L, idx));  }
	inline unsigned char	Get(TypeWrapper<unsigned char>, lua_State* L, int idx)
		{  return static_cast<unsigned char>(lua_tonumber(L, idx));  }
	inline short			Get(TypeWrapper<short>, lua_State* L, int idx)
		{  return static_cast<short>(lua_tonumber(L, idx));  }
	inline unsigned short	Get(TypeWrapper<unsigned short>, lua_State* L, int idx)
		{  return static_cast<unsigned short>(lua_tonumber(L, idx));  }
	inline int				Get(TypeWrapper<int>, lua_State* L, int idx)
		{  return static_cast<int>(lua_tonumber(L, idx));  }
	inline unsigned int		Get(TypeWrapper<unsigned int>, lua_State* L, int idx)
		{  return static_cast<unsigned int>(lua_tonumber(L, idx));  }
	inline long				Get(TypeWrapper<long>, lua_State* L, int idx)
		{  return static_cast<long>(lua_tonumber(L, idx));  }
	inline unsigned long	Get(TypeWrapper<unsigned long>, lua_State* L, int idx)
		{  return static_cast<unsigned long>(lua_tonumber(L, idx));  }
	inline float			Get(TypeWrapper<float>, lua_State* L, int idx)
		{  return static_cast<float>(lua_tonumber(L, idx));  }
	inline double			Get(TypeWrapper<double>, lua_State* L, int idx)
		{  return static_cast<double>(lua_tonumber(L, idx));  }
	inline const char*		Get(TypeWrapper<const char*>, lua_State* L, int idx)
		{  return static_cast<const char*>(lua_tostring(L, idx));  }
	inline LuaNil			Get(TypeWrapper<LuaNil>, lua_State* L, int idx)
		{  (void)L, (void)idx;  return LuaNil();  }
	inline lua_CFunction	Get(TypeWrapper<lua_CFunction>, lua_State* L, int idx)
		{  return static_cast<lua_CFunction>(lua_tocfunction(L, idx));  }
	inline void*			Get(TypeWrapper<void*>, lua_State* L, int idx)
		{  return static_cast<void*>(lua_touserdata(L, idx));  }
	inline lua_State*		Get(TypeWrapper<lua_State*>, lua_State* L, int /*idx*/)
		{  return L;  }

	//////////////////////////////////////////////////////////////////////////

	#define luaL_argassert(arg, _index_) if (!Match(TypeWrapper<P##arg>(), L, _index_)) \
				luaL_argerror(L, _index_, "bad argument")

	template<class RT>
	struct ReturnSpecialization {
		static int Call(RT (*func)(), lua_State* L, int /*index*/) {
			RT ret = func();
			Push(L, ret);
			return 1;
		}


		template <typename P1>
		static int Call(RT (*func)(P1), lua_State* L, int index) {
			luaL_argassert(1, index + 0);

			RT ret = func(
				Get(TypeWrapper<P1>(), L, index + 0)
			);
			Push(L, ret);
			return 1;
		}


		template <typename P1, typename P2>
		static int Call(RT (*func)(P1, P2), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);

			RT ret = func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1)
			);
			Push(L, ret);
			return 1;
		}


		template <typename P1, typename P2, typename P3>
		static int Call(RT (*func)(P1, P2, P3), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);

			RT ret = func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2)
			);
			Push(L, ret);
			return 1;
		}


		template <typename P1, typename P2, typename P3, typename P4>
		static int Call(RT (*func)(P1, P2, P3, P4), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);

			RT ret = func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3)
			);
			Push(L, ret);
			return 1;
		}


		template <typename P1, typename P2, typename P3, typename P4,
					typename P5>
		static int Call(RT (*func)(P1, P2, P3, P4, P5), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);

			RT ret = func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4)
			);
			Push(L, ret);
			return 1;
		}


		template <typename P1, typename P2, typename P3, typename P4,
					typename P5, typename P6>
		static int Call(RT (*func)(P1, P2, P3, P4, P5, P6), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);

			RT ret = func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5)
			);
			Push(L, ret);
			return 1;
		}


		template <typename P1, typename P2, typename P3, typename P4,
					typename P5, typename P6, typename P7>
		static int Call(RT (*func)(P1, P2, P3, P4, P5, P6, P7), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);

			RT ret = func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6)
			);
			Push(L, ret);
			return 1;
		}


		template <typename P1, typename P2, typename P3, typename P4,
					typename P5, typename P6, typename P7, typename P8>
		static int Call(RT (*func)(P1, P2, P3, P4, P5, P6, P7, P8), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);

			RT ret = func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6),
				Get(TypeWrapper<P8>(), L, index + 7)
			);
			Push(L, ret);
			return 1;
		}


		template <typename P1, typename P2, typename P3, typename P4,
					typename P5, typename P6, typename P7, typename P8,
					typename P9>
		static int Call(RT (*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);
			luaL_argassert(9, index + 8);

			RT ret = func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6),
				Get(TypeWrapper<P8>(), L, index + 7),
				Get(TypeWrapper<P9>(), L, index + 8)
			);
			Push(L, ret);
			return 1;
		}


		template <typename P1, typename P2, typename P3, typename P4,
					typename P5, typename P6, typename P7, typename P8,
					typename P9, typename P10>
		static int Call(RT (*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);
			luaL_argassert(9, index + 8);
			luaL_argassert(10, index + 9);

			RT ret = func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6),
				Get(TypeWrapper<P8>(), L, index + 7),
				Get(TypeWrapper<P9>(), L, index + 8),
				Get(TypeWrapper<P10>(), L, index + 9)
			);
			Push(L, ret);
			return 1;
		}


		//////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////
		template <typename Callee>
		static int Call(Callee& callee, RT (Callee::*func)(), lua_State* L, int /*index*/) {
			RT ret = (callee.*func)();
			Push(L, ret);
			return 1;
		}


		template <typename Callee>
		static int Call(Callee& callee, RT (Callee::*func)() const, lua_State* L, int /*index*/) {
			RT ret = (callee.*func)();
			Push(L, ret);
			return 1;
		}


		template <typename Callee, typename P1>
		static int Call(Callee& callee, RT (Callee::*func)(P1), lua_State* L, int index) {
			luaL_argassert(1, index + 0);

			RT ret = (callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0)
			);
			Push(L, ret);
			return 1;
		}


		template <typename Callee, typename P1>
		static int Call(Callee& callee, RT (Callee::*func)(P1) const, lua_State* L, int index) {
			luaL_argassert(1, index + 0);

			RT ret = (callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0)
			);
			Push(L, ret);
			return 1;
		}


		template <typename Callee, typename P1, typename P2>
		static int Call(Callee& callee, RT (Callee::*func)(P1, P2), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);

			RT ret = (callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1)
			);
			Push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2>
		static int Call(Callee& callee, RT (Callee::*func)(P1, P2) const, lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);

			RT ret = (callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1)
			);
			Push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3>
		static int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);

			RT ret = (callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2)
			);
			Push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3>
		static int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3) const, lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);

			RT ret = (callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2)
			);
			Push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4>
		static int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);

			RT ret = (callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3)
			);
			Push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4>
		static int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4) const, lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);

			RT ret = (callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3)
			);
			Push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5>
		static int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);

			RT ret = (callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4)
			);
			Push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5>
		static int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5) const, lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);

			RT ret = (callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4)
			);
			Push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6>
		static int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);

			RT ret = (callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5)
			);
			Push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6>
		static int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6) const, lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);

			RT ret = (callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5)
			);
			Push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7>
		static int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);

			RT ret = (callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6)
			);
			Push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7>
		static int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7) const, lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);

			RT ret = (callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6)
			);
			Push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8>
		static int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);

			RT ret = (callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6),
				Get(TypeWrapper<P8>(), L, index + 7)
			);
			Push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8>
		static int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8) const, lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);

			RT ret = (callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6),
				Get(TypeWrapper<P8>(), L, index + 7)
			);
			Push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8,
					typename P9>
		static int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);
			luaL_argassert(9, index + 8);

			RT ret = (callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6),
				Get(TypeWrapper<P8>(), L, index + 7),
				Get(TypeWrapper<P9>(), L, index + 8)
			);
			Push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8,
					typename P9>
		static int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9) const, lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);
			luaL_argassert(9, index + 8);

			RT ret = (callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6),
				Get(TypeWrapper<P8>(), L, index + 7),
				Get(TypeWrapper<P9>(), L, index + 8)
			);
			Push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8,
					typename P9, typename P10>
		static int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);
			luaL_argassert(9, index + 8);
			luaL_argassert(10, index + 9);

			RT ret = (callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6),
				Get(TypeWrapper<P8>(), L, index + 7),
				Get(TypeWrapper<P9>(), L, index + 8),
				Get(TypeWrapper<P10>(), L, index + 9)
			);
			Push(L, ret);
			return 1;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8,
					typename P9, typename P10>
		static int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) const, lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);
			luaL_argassert(9, index + 8);
			luaL_argassert(10, index + 9);

			RT ret = (callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6),
				Get(TypeWrapper<P8>(), L, index + 7),
				Get(TypeWrapper<P9>(), L, index + 8),
				Get(TypeWrapper<P10>(), L, index + 9)
			);
			Push(L, ret);
			return 1;
		}
	};


	template<>
	struct ReturnSpecialization<void> {
		static int Call(void (*func)(), lua_State* L, int /*index*/) {
			(void)L;
			func();
			return 0;
		}


		template <typename P1>
		static int Call(void (*func)(P1), lua_State* L, int index) {
			luaL_argassert(1, index + 0);

			func(
				Get(TypeWrapper<P1>(), L, index + 0)
			);
			return 0;
		}


		template <typename P1, typename P2>
		static int Call(void (*func)(P1, P2), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);

			func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1)
			);
			return 0;
		}


		template <typename P1, typename P2, typename P3>
		static int Call(void (*func)(P1, P2, P3), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);

			func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2)
			);
			return 0;
		}

		template <typename P1, typename P2, typename P3, typename P4>
		static int Call(void (*func)(P1, P2, P3, P4), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);

			func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3)
			);
			return 0;
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5>
		static int Call(void (*func)(P1, P2, P3, P4, P5), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);

			func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4)
			);
			return 0;
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
		static int Call(void (*func)(P1, P2, P3, P4, P5, P6), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);

			func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5)
			);
			return 0;
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
		static int Call(void (*func)(P1, P2, P3, P4, P5, P6, P7), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);

			func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6)
			);
			return 0;
		}

		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
		static int Call(void (*func)(P1, P2, P3, P4, P5, P6, P7, P8), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);

			func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6),
				Get(TypeWrapper<P8>(), L, index + 7)
			);
			return 0;
		}


		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
		static int Call(void (*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);
			luaL_argassert(9, index + 8);

			func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6),
				Get(TypeWrapper<P8>(), L, index + 7),
				Get(TypeWrapper<P9>(), L, index + 8)
			);
			return 0;
		}


		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10>
		static int Call(void (*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);
			luaL_argassert(9, index + 8);
			luaL_argassert(10, index + 9);

			func(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6),
				Get(TypeWrapper<P8>(), L, index + 7),
				Get(TypeWrapper<P9>(), L, index + 8),
				Get(TypeWrapper<P10>(), L, index + 9)
			);
			return 0;
		}

		//////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////
		template <typename Callee>
		static int Call(Callee& callee, void (Callee::*func)(), lua_State* /*L*/, int /*index*/) {
			(callee.*func)();
			return 0;
		}


		template <typename Callee>
		static int Call(Callee& callee, void (Callee::*func)() const, lua_State* /*L*/, int /*index*/) {
			(callee.*func)();
			return 0;
		}


		template <typename Callee, typename P1>
		static int Call(Callee& callee, void (Callee::*func)(P1), lua_State* L, int index) {
			luaL_argassert(1, index + 0);

			(callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0)
			);
			return 0;
		}


		template <typename Callee, typename P1>
		static int Call(Callee& callee, void (Callee::*func)(P1) const, lua_State* L, int index) {
			luaL_argassert(1, index + 0);

			(callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0)
			);
			return 0;
		}


		template <typename Callee, typename P1, typename P2>
		static int Call(Callee& callee, void (Callee::*func)(P1, P2), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);

			(callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2>
		static int Call(Callee& callee, void (Callee::*func)(P1, P2) const, lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);

			(callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3>
		static int Call(Callee& callee, void (Callee::*func)(P1, P2, P3), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);

			(callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3>
		static int Call(Callee& callee, void (Callee::*func)(P1, P2, P3) const, lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);

			(callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4>
		static int Call(Callee& callee, void (Callee::*func)(P1, P2, P3, P4), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);

			(callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4>
		static int Call(Callee& callee, void (Callee::*func)(P1, P2, P3, P4) const, lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);

			(callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5>
		static int Call(Callee& callee, void (Callee::*func)(P1, P2, P3, P4, P5), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);

			(callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5>
		static int Call(Callee& callee, void (Callee::*func)(P1, P2, P3, P4, P5) const, lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);

			(callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6>
		static int Call(Callee& callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);

			(callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6>
		static int Call(Callee& callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6) const, lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);

			(callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7>
		static int Call(Callee& callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6, P7), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);

			(callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7>
		static int Call(Callee& callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6, P7) const, lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);

			(callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8>
		static int Call(Callee& callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);

			(callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6),
				Get(TypeWrapper<P8>(), L, index + 7)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8>
		static int Call(Callee& callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8) const, lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);

			(callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6),
				Get(TypeWrapper<P8>(), L, index + 7)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8,
					typename P9>
		static int Call(Callee& callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);
			luaL_argassert(9, index + 8);

			(callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6),
				Get(TypeWrapper<P8>(), L, index + 7),
				Get(TypeWrapper<P9>(), L, index + 8)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8,
					typename P9>
		static int Call(Callee& callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9) const, lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);
			luaL_argassert(9, index + 8);

			(callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6),
				Get(TypeWrapper<P8>(), L, index + 7),
				Get(TypeWrapper<P9>(), L, index + 8)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8,
					typename P9, typename P10>
		static int Call(Callee& callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10), lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);
			luaL_argassert(9, index + 8);
			luaL_argassert(10, index + 9);

			(callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6),
				Get(TypeWrapper<P8>(), L, index + 7),
				Get(TypeWrapper<P9>(), L, index + 8),
				Get(TypeWrapper<P10>(), L, index + 9)
			);
			return 0;
		}

		template <typename Callee, typename P1, typename P2, typename P3,
					typename P4, typename P5, typename P6, typename P7, typename P8,
					typename P9, typename P10>
		static int Call(Callee& callee, void (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) const, lua_State* L, int index) {
			luaL_argassert(1, index + 0);
			luaL_argassert(2, index + 1);
			luaL_argassert(3, index + 2);
			luaL_argassert(4, index + 3);
			luaL_argassert(5, index + 4);
			luaL_argassert(6, index + 5);
			luaL_argassert(7, index + 6);
			luaL_argassert(8, index + 7);
			luaL_argassert(9, index + 8);
			luaL_argassert(10, index + 9);

			(callee.*func)(
				Get(TypeWrapper<P1>(), L, index + 0),
				Get(TypeWrapper<P2>(), L, index + 1),
				Get(TypeWrapper<P3>(), L, index + 2),
				Get(TypeWrapper<P4>(), L, index + 3),
				Get(TypeWrapper<P5>(), L, index + 4),
				Get(TypeWrapper<P6>(), L, index + 5),
				Get(TypeWrapper<P7>(), L, index + 6),
				Get(TypeWrapper<P8>(), L, index + 7),
				Get(TypeWrapper<P9>(), L, index + 8),
				Get(TypeWrapper<P10>(), L, index + 9)
			);
			return 0;
		}
	};


	template <typename RT>
	int Call(RT (*func)(), lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(func, L, index);
	}


	template <typename RT, typename P1>
	int Call(RT (*func)(P1), lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(func, L, index);
	}


	template <typename RT, typename P1, typename P2>
	int Call(RT (*func)(P1, P2), lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(func, L, index);
	}


	template <typename RT, typename P1, typename P2, typename P3>
	int Call(RT (*func)(P1, P2, P3), lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(func, L, index);
	}


	template <typename RT, typename P1, typename P2, typename P3, typename P4>
	int Call(RT (*func)(P1, P2, P3, P4), lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(func, L, index);
	}


	template <typename RT, typename P1, typename P2, typename P3, typename P4,
				typename P5>
	int Call(RT (*func)(P1, P2, P3, P4, P5), lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(func, L, index);
	}


	template <typename RT, typename P1, typename P2, typename P3, typename P4,
				typename P5, typename P6>
	int Call(RT (*func)(P1, P2, P3, P4, P5, P6), lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(func, L, index);
	}


	template <typename RT, typename P1, typename P2, typename P3, typename P4,
				typename P5, typename P6, typename P7>
	int Call(RT (*func)(P1, P2, P3, P4, P5, P6, P7), lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(func, L, index);
	}


	template <typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
	int Call(RT (*func)(P1, P2, P3, P4, P5, P6, P7, P8), lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(func, L, index);
	}


	template <typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
	int Call(RT (*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9), lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(func, L, index);
	}


	template <typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10>
	int Call(RT (*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10), lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(func, L, index);
	}


	template <typename Callee, typename RT>
	int Call(Callee& callee, RT (Callee::*func)(), lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT>
	int Call(Callee& callee, RT (Callee::*func)() const, lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1>
	int Call(Callee& callee, RT (Callee::*func)(P1), lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1>
	int Call(Callee& callee, RT (Callee::*func)(P1) const, lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2>
	int Call(Callee& callee, RT (Callee::*func)(P1, P2), lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2>
	int Call(Callee& callee, RT (Callee::*func)(P1, P2) const, lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3>
	int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3), lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3>
	int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3) const, lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4>
	int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4), lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4>
	int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4) const, lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4, typename P5>
	int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5), lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4, typename P5>
	int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5) const, lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4, typename P5, typename P6>
	int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6), lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4, typename P5, typename P6>
	int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6) const, lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4, typename P5, typename P6, typename P7>
	int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7), lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3,
				typename P4, typename P5, typename P6, typename P7>
	int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7) const, lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
	int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8), lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
	int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8) const, lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
	int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9), lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
	int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9) const, lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10>
	int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10), lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	template <typename Callee, typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9, typename P10>
	int Call(Callee& callee, RT (Callee::*func)(P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) const, lua_State* L, int index) {
		return ReturnSpecialization<RT>::Call(callee, func, L, index);
	}


	inline unsigned char* GetFirstUpValueAsUserdata(lua_State* L) {
		void* buffer;

#ifndef FAST_DISPATCH
		buffer = lua_touserdata(L, lua_upvalueindex(1));
#else // FAST_DISPATCH
#endif // FAST_DISPATCH

		return (unsigned char*)buffer;
	}


	// The Helper class is needed for less fortunate compiler template implementations.
	template <typename Func>
	class DirectCallFunctionDispatchHelper {
	public:
		static inline int DirectCallFunctionDispatcher(lua_State* L) {
 			unsigned char* buffer = GetFirstUpValueAsUserdata(L);
			return Call(*(Func*)(buffer), L, 1);
		}
	};


	template <typename Callee, typename Func>
	class DirectCallMemberDispatcherHelper {
	public:
		static inline int DirectCallMemberDispatcher(lua_State* L) {
 			unsigned char* buffer = GetFirstUpValueAsUserdata(L);
			return Call(**(Callee**)buffer, *(Func*)(buffer + sizeof(Callee*)), L, 1);
		}
	};

	inline int lua_StateFunctionDispatcher(lua_State* L) {
		typedef int (*Functor)(lua_State*);
 		unsigned char* buffer = GetFirstUpValueAsUserdata(L);
		Functor& func = *(Functor*)(buffer);
		return (*func)(L);
	}


	template <typename Callee>
	class lua_StateMemberDispatcherHelper {
	public:
		static inline int lua_StateMemberDispatcher(lua_State* L) {
			typedef int (Callee::*Functor)(lua_State*);
 			unsigned char* buffer = GetFirstUpValueAsUserdata(L);
			Callee& callee = **(Callee**)buffer;
			Functor& func = *(Functor*)(buffer + sizeof(Callee*));
			return (callee.*func)(L);
		}
	};

	template<> inline void Push<bool>(lua_State* L, bool value)						{  lua_pushboolean(L, value);  }
	template<> inline void Push<char>(lua_State* L, char value)						{  lua_pushnumber(L, (lua_Number)value);  }
	template<> inline void Push<unsigned char>(lua_State* L, unsigned char value)	{  lua_pushnumber(L, (lua_Number)value);  }
	template<> inline void Push<short>(lua_State* L, short value)					{  lua_pushnumber(L, (lua_Number)value);  }
	template<> inline void Push<unsigned short>(lua_State* L, unsigned short value)	{  lua_pushnumber(L, (lua_Number)value);  }
	template<> inline void Push<int>(lua_State* L, int value)						{  lua_pushnumber(L, (lua_Number)value);  }
	template<> inline void Push<unsigned int>(lua_State* L, unsigned int value)		{  lua_pushnumber(L, (lua_Number)value);  }
	template<> inline void Push<long>(lua_State* L, long value)						{  lua_pushnumber(L, (lua_Number)value);  }
	template<> inline void Push<unsigned long>(lua_State* L, unsigned long value)	{  lua_pushnumber(L, (lua_Number)value);  }
	template<> inline void Push<double>(lua_State* L, double value)					{  lua_pushnumber(L, (lua_Number)value);  }
	template<> inline void Push<float>(lua_State* L, float value)					{  lua_pushnumber(L, (lua_Number)value);  }
	template<> inline void Push<char*>(lua_State* L, char* value)					{  lua_pushstring(L, value);  }
	template<> inline void Push<const char*>(lua_State* L, const char* value)		{  lua_pushstring(L, value);  }
	inline void Push(lua_State* L, const char* value, int len)						{  lua_pushlstring(L, value, len);  }
	template<> inline void Push<const LuaNil&>(lua_State* L, const LuaNil&)			{  lua_pushnil(L);  }
	template<> inline void Push<lua_CFunction>(lua_State* L, lua_CFunction value)	{  lua_pushcclosure(L, value, 0);  }
	template<> inline void Push<const void*>(lua_State* L, const void* value)		{  lua_pushlightuserdata(L, (void*)value);  }
	template<> inline void Push<void*>(lua_State* L, void* value)					{  lua_pushlightuserdata(L, value);  }
	template<> inline void Push<const LuaLightUserdata&>(lua_State* L, const LuaLightUserdata& value)	{  lua_pushlightuserdata(L, (void*)value.m_value);  }
    template<> inline void Push<const LuaUserdata&>(lua_State* L, const LuaUserdata& value)				{  *(void **)(lua_newuserdata(L, sizeof(void *))) = (void*)value.m_value;  }
    template<> inline void Push<LuaUserdata&>(lua_State* L, LuaUserdata& value)		{  *(void **)(lua_newuserdata(L, sizeof(void *))) = (void*)value.m_value;  }
    template<> inline void Push<LuaUserdata>(lua_State* L, LuaUserdata value)		{  *(void **)(lua_newuserdata(L, sizeof(void *))) = (void*)value.m_value;  }
} // namespace LPCD


template <typename Func>
inline void lpcd_pushdirectclosure(lua_State* L, Func func, unsigned int nupvalues = 0) {
	unsigned char* buffer = (unsigned char*)lua_newuserdata(L, sizeof(func));
	memcpy(buffer, &func, sizeof(func));
	lua_insert(L, -1 - (int)nupvalues);
	lua_pushcclosure(L, LPCD::DirectCallFunctionDispatchHelper<Func>::DirectCallFunctionDispatcher, nupvalues + 1);
}


template <typename Callee, typename Func>
inline void lpcd_pushdirectclosure(lua_State* L, const Callee& callee, Func func, unsigned int nupvalues = 0) {
	unsigned char* buffer = (unsigned char*)lua_newuserdata(L, sizeof(Callee*) + sizeof(func));
	const void* pCallee = &callee;
	memcpy(buffer, &pCallee, sizeof(Callee*));
	memcpy(buffer + sizeof(Callee*), &func, sizeof(func));
	lua_insert(L, -1 - (int)nupvalues);
	lua_pushcclosure(L, LPCD::DirectCallMemberDispatcherHelper<Callee, Func>::DirectCallMemberDispatcher, nupvalues + 1);
}


inline void lpcd_pushfunctorclosure(lua_State* L, int (*func)(lua_State*), unsigned int nupvalues = 0) {
	unsigned char* buffer = (unsigned char*)lua_newuserdata(L, sizeof(func));
	memcpy(buffer, &func, sizeof(func));
	lua_insert(L, -1 - (int)nupvalues);
	lua_pushcclosure(L, LPCD::lua_StateFunctionDispatcher, nupvalues + 1);
}


template <typename Callee>
inline void lpcd_pushfunctorclosureex(lua_State* L, const Callee& callee, int (Callee::*func)(lua_State*), unsigned int nupvalues = 0) {
	unsigned char* buffer = (unsigned char*)lua_newuserdata(L, sizeof(Callee*) + sizeof(func));
	const void* pCallee = &callee;
	memcpy(buffer, &pCallee, sizeof(Callee*));
	memcpy(buffer + sizeof(Callee*), &func, sizeof(func));
	lua_insert(L, -1 - (int)nupvalues);
	lua_pushcclosure(L, LPCD::lua_StateMemberDispatcherHelper<Callee>::lua_StateMemberDispatcher, nupvalues + 1);
}


///////////////////////////////////////////////////////////////////////////////

namespace LPCD {
	inline void* GetObjectUserdata(lua_State* L) {
		int type = lua_type(L, 1);
		if (type == LUA_TUSERDATA)
			return *(void **)(lua_touserdata(L, 1));
		else if (type == LUA_TTABLE) {
			lua_pushstring(L, "__object");
			lua_rawget(L, 1);

			void* ret;
			int subType = lua_type(L, -1);
			if (subType == LUA_TLIGHTUSERDATA) {
				ret = lua_touserdata(L, -1);
			} else if (subType == LUA_TUSERDATA) {
				ret = *(void **)(lua_touserdata(L, -1));
			} else {
				luaL_error(L, "The table does not have a userdata member called __object.");
			}
			lua_pop(L, 1);
			return ret;
		} else {
			luaL_argerror(L, 1, "must be userdata or a table with a userdata member called __object");
		}

		return NULL;
	}

	inline void* GetInPlaceObjectUserdata(lua_State* L) {
		int type = lua_type(L, 1);
		if (type == LUA_TUSERDATA)
			return lua_touserdata(L, 1);
		else if (type == LUA_TTABLE) {
			lua_pushstring(L, "__object");
			lua_rawget(L, 1);

			void* ret;
			int subType = lua_type(L, -1);
			if (subType == LUA_TLIGHTUSERDATA) {
				ret = lua_touserdata(L, -1);
			} else if (subType == LUA_TUSERDATA) {
				ret = *(void **)(lua_touserdata(L, -1));
			} else {
				luaL_error(L, "The table does not have a userdata member called __object.");
			}
			lua_pop(L, 1);
			return ret;
		} else {
			luaL_argerror(L, 1, "must be userdata or a table with a userdata member called __object");
		}

		return NULL;
	}

	template <typename Callee>
	class Object_MemberDispatcherHelper {
	public:
		static inline int Object_MemberDispatcher(lua_State* L) {
			typedef int (Callee::*Functor)(lua_State*);
 			unsigned char* buffer = GetFirstUpValueAsUserdata(L);
			Functor& func = *(Functor*)(buffer);
			Callee& callee = *(Callee*)GetObjectUserdata(L);
			return (callee.*func)(L);
		}
	};

	template <typename Callee, typename Func, int startIndex>
	class DirectCallObjectMemberDispatcherHelper {
	public:
		static inline int DirectCallMemberDispatcher(lua_State* L) {
 			unsigned char* buffer = GetFirstUpValueAsUserdata(L);
			Callee& callee = *(Callee*)GetObjectUserdata(L);
			return Call(callee, *(Func*)buffer, L, startIndex);
		}
	};

	template <typename Callee, typename Func, int startIndex>
	class DirectCallInPlaceObjectMemberDispatcherHelper {
	public:
		static inline int DirectCallMemberDispatcher(lua_State* L) {
 			unsigned char* buffer = GetFirstUpValueAsUserdata(L);
			Callee& callee = *(Callee*)GetInPlaceObjectUserdata(L);
			return Call(callee, *(Func*)buffer, L, startIndex);
		}
	};

	inline int PropertyMetatable_newindex(lua_State* L) {
													// table key value
		lua_pushvalue(L, 2);						// table key value key
		lua_rawget(L, lua_upvalueindex(1));			// table key value property
		if (lua_isfunction(L, -1)) {
			if (lua_getupvalue(L, -1, 2)) {			// table key value property userdataTest
				if (lua_touserdata(L, -1) == (void*)-2) {
					lua_pop(L, 1);								// table key value property
					lua_CFunction f = lua_tocfunction(L, -1);	// table key value propertyfunc
					lua_getupvalue(L, -1, 1);					// table key value propertyfunc offset
					lua_replace(L, 4);							// table key value offset
					return f(L);
				}
				lua_pop(L, 1);						// table key value
			}
			lua_rawset(L, -3);						// table
			return 0;
		} else if (!lua_isnil(L, -1)) {
			lua_pop(L, 1);							// table key value
			lua_rawset(L, -3);						// table
			return 0;
		}

		lua_pop(L, 1);								// table key value

		if (!lua_getmetatable(L, -3))				// table key value metatable
			return 0;

		do {
			lua_getfield(L, -1, "__newindex");				// table key value metatable __newindex
			if (lua_isnil(L, -1)) {
				lua_pop(L, 2);								// table key value
				lua_rawset(L, -3);							// table
				return 0;
			}

			lua_getupvalue(L, -1, 1);						// table key value metatable __newindex functions
			lua_pushvalue(L, 2);							// table key value metatable __newindex functions key
			lua_rawget(L, -2);								// table key value metatable __newindex functions property
			if (lua_isfunction(L, -1)) {
				if (lua_getupvalue(L, -1, 2)) {				// table key value metatable __newindex functions userdataTest
					if (lua_touserdata(L, -1) == (void*)-2) {
						lua_pop(L, 1);						// table key value metatable __newindex functions
						lua_CFunction f = lua_tocfunction(L, -1);	// table key value metatable __newindex functions
						lua_getupvalue(L, -1, 1);			// table key value metatable __newindex functions offset
						lua_replace(L, 4);					// table key value offset __newindex functions
						return f(L);
					}
					lua_pop(L, 2);							// table key value metatable __newindex
				}
				lua_rawset(L, -3);							// table key value
				return 1;
			} else if (!lua_isnil(L, -1)) {
				lua_pop(L, 4);
				lua_rawset(L, -3);
				return 0;
			}

			lua_pop(L, 3);									// table key value metatable
			if (!lua_getmetatable(L, 4)) {					// table key value metatable metatable2
				lua_pop(L, 1);
				break;
			}
			lua_remove(L, -2);								// table key value metatable2
		} while (true);

		lua_rawset(L, -3);
		return 0;
	}

	// function gettable_event (table, key)
	// upvalues:
	//     1 - function__index
	//     2 - properties__index
	inline int PropertyMetatable_index(lua_State* L) {
		lua_pushvalue(L, 2);							// table key key
		lua_rawget(L, lua_upvalueindex(1));				// table key property
		if (lua_isfunction(L, -1)) {
			if (lua_getupvalue(L, -1, 2)) {
				if (lua_touserdata(L, -1) == (void*)-2) {
					lua_pop(L, 1);
					lua_CFunction f = lua_tocfunction(L, -1);	// table key propertyfunc
					lua_getupvalue(L, -1, 1);					// table key propertyfunc offset
					lua_replace(L, 3);							// table key offset
					return f(L);
				}
				lua_pop(L, 1);
			}
			return 1;
		} else if (!lua_isnil(L, -1))
			return 1;

		lua_pop(L, 1);									// table key

		if (!lua_getmetatable(L, -2))					// table key metatable
			return 0;

		do {
			lua_getfield(L, -1, "__index");					// table key metatable __index
			if (lua_isnil(L, -1))
				return 0;

			lua_getupvalue(L, -1, 1);						// table key metatable __index functions
			lua_pushvalue(L, 2);							// table key metatable __index functions key
			lua_rawget(L, -2);								// table key metatable __index functions property
			if (lua_isfunction(L, -1)) {
				if (lua_getupvalue(L, -1, 2)) {				// table key metatable __index functions userdataTest
					if (lua_touserdata(L, -1) == (void*)-2) {
						lua_pop(L, 1);						// table key metatable __index functions
						lua_CFunction f = lua_tocfunction(L, -1);	// table key metatable __index functions
						lua_getupvalue(L, -1, 1);			// table key metatable __index functions offset
						lua_replace(L, 3);					// table key offset __index functions
						return f(L);
					}
					lua_pop(L, 1);							// table key metatable __index functions
				}
				return 1;
			} else if (!lua_isnil(L, -1))
				return 1;

			lua_pop(L, 3);									// table key metatable
			if (!lua_getmetatable(L, 3))					// table key metatable metatable2
				return 0;
			lua_remove(L, -2);								// table key metatable2
		} while (true);
	}

	template <typename Object, typename VarType>
	class PropertyMemberHelper {
	public:
		static int PropertyGet(lua_State* L) {
			Object* obj = (Object*)LPCD::GetObjectUserdata(L);
			void* offset = lua_touserdata(L, 3);
			LPCD::Push(L, *(VarType*)((unsigned char*)obj + (unsigned int)offset));
			return 1;
		}

		static int PropertySet(lua_State* L) {
			Object* obj = (Object*)LPCD::GetObjectUserdata(L);
			void* offset = lua_touserdata(L, 4);

			if (!Match(TypeWrapper<VarType>(), L, 3))
				luaL_argerror(L, 3, "bad argument");

			*(VarType*)((unsigned char*)obj + (unsigned int)offset) = Get(TypeWrapper<VarType>(), L, 3);
			return 0;
		}
	};

	template <typename Object, typename VarType>
	class InPlacePropertyMemberHelper {
	public:
		static int PropertyGet(lua_State* L) {
			Object* obj = (Object*)LPCD::GetInPlaceObjectUserdata(L);
			void* offset = lua_touserdata(L, 3);
			LPCD::Push(L, *(VarType*)((unsigned char*)obj + (unsigned int)offset));
			return 1;
		}

		static int PropertySet(lua_State* L) {
			Object* obj = (Object*)LPCD::GetInPlaceObjectUserdata(L);
			void* offset = lua_touserdata(L, 4);

			if (!Match(TypeWrapper<VarType>(), L, 3))
				luaL_argerror(L, 3, "bad argument");

			*(VarType*)((unsigned char*)obj + (unsigned int)offset) = Get(TypeWrapper<VarType>(), L, 3);
			return 0;
		}
	};

	template <typename VarType>
	class PropertyGlobalHelper {
	public:
		static int PropertyGet(lua_State* L) {
			void* offset = lua_touserdata(L, lua_upvalueindex(1));

			LPCD::Push(L, *(VarType*)offset);

			return 1;
		}

		static int PropertySet(lua_State* L) {
			void* offset = lua_touserdata(L, lua_upvalueindex(1));

			if (!Match(TypeWrapper<VarType>(), L, 1))
				luaL_argerror(L, 2, "bad argument");

			*(VarType*)offset = LPCD::Get(LPCD::TypeWrapper<VarType>(), L, 1);

			return 1;
		}
	};
} // namespace LPCD


template <typename Callee>
inline void lpcd_pushobjectfunctorclosure(lua_State* L, int (Callee::*func)(lua_State*), unsigned int nupvalues = 0) {
	unsigned char* buffer = (unsigned char*)lua_newuserdata(L, sizeof(func));
	memcpy(buffer, &func, sizeof(func));
	lua_pushcclosure(L, LPCD::Object_MemberDispatcherHelper<Callee>::Object_MemberDispatcher, nupvalues + 1);
}


template <typename Callee, typename Func>
inline void lpcd_pushobjectdirectclosure(lua_State* L, const Callee* callee, Func func, unsigned int nupvalues = 0) {
	unsigned char* buffer = (unsigned char*)lua_newuserdata(L, sizeof(func));
	memcpy(buffer, &func, sizeof(func));
	lua_pushcclosure(L, LPCD::DirectCallObjectMemberDispatcherHelper<Callee, Func, 2>::DirectCallMemberDispatcher, nupvalues + 1);
}


/*template <typename Callee>
inline void lpcd_pushinplaceobjectfunctorclosure(lua_State* L, int (Callee::*func)(lua_State*), unsigned int nupvalues = 0) {
	unsigned char* buffer = (unsigned char*)lua_newuserdata(L, sizeof(func));
	memcpy(buffer, &func, sizeof(func));
	//lua_pushcclosure(L, LPCD::Object_InPlaceMemberDispatcherHelper<Callee>::Object_MemberDispatcher, nupvalues + 1);
	lua_pushcclosure(L, LPCD::InPlaceMemberDispatcherHelper<Callee>::Object_MemberDispatcher, nupvalues + 1);
}
*/


template <typename Callee, typename Func>
inline void lpcd_pushinplaceobjectdirectclosure(lua_State* L, const Callee* callee, Func func, unsigned int nupvalues = 0) {
	unsigned char* buffer = (unsigned char*)lua_newuserdata(L, sizeof(func));
	memcpy(buffer, &func, sizeof(func));
	lua_pushcclosure(L, LPCD::DirectCallInPlaceObjectMemberDispatcherHelper<Callee, Func, 2>::DirectCallMemberDispatcher, nupvalues + 1);
}


template <typename Object, typename VarType>
inline void lpcd_pushmemberpropertygetclosure(lua_State* L, VarType Object::* var) {
	lua_pushlightuserdata(L, (void*)&(((Object*)0)->*var));
	lua_pushlightuserdata(L, (void*)-2);
	lua_pushcclosure(L, &LPCD::PropertyMemberHelper<Object, VarType>::PropertyGet, 2);
}


template <typename Object, typename VarType>
inline void lpcd_pushmemberpropertysetclosure(lua_State* L, VarType Object::* var) {
	lua_pushlightuserdata(L, (void*)&(((Object*)0)->*var));
	lua_pushlightuserdata(L, (void*)-2);
	lua_pushcclosure(L, &LPCD::PropertyMemberHelper<Object, VarType>::PropertySet, 2);
}


template <typename Object, typename VarType>
inline void lpcd_pushmemberinplacepropertygetclosure(lua_State* L, VarType Object::* var) {
	lua_pushlightuserdata(L, (void*)&(((Object*)0)->*var));
	lua_pushlightuserdata(L, (void*)-2);
	lua_pushcclosure(L, &LPCD::InPlacePropertyMemberHelper<Object, VarType>::PropertyGet, 2);
}


template <typename Object, typename VarType>
inline void lpcd_pushmemberinplacepropertysetclosure(lua_State* L, VarType Object::* var) {
	lua_pushlightuserdata(L, (void*)&(((Object*)0)->*var));
	lua_pushlightuserdata(L, (void*)-2);
	lua_pushcclosure(L, &LPCD::InPlacePropertyMemberHelper<Object, VarType>::PropertySet, 2);
}


template <typename VarType>
inline void lpcd_pushglobalpropertygetclosure(lua_State* L, VarType* var) {
	lua_pushlightuserdata(L, (void*)var);
	lua_pushlightuserdata(L, (void*)-2);
	lua_pushcclosure(L, &LPCD::PropertyGlobalHelper<VarType>::PropertyGet, 2);
}


template <typename VarType>
inline void lpcd_pushglobalpropertysetclosure(lua_State* L, VarType* var) {
	lua_pushlightuserdata(L, (void*)var);
	lua_pushlightuserdata(L, (void*)-2);
	lua_pushcclosure(L, &LPCD::PropertyGlobalHelper<VarType>::PropertySet, 2);
}


/* convert a stack index to positive */
#define lpcd_abs_index(L, i)		((i) > 0 || (i) <= LUA_REGISTRYINDEX ? (i) : lua_gettop(L) + (i) + 1)

inline void lpcd_integratepropertysupport(lua_State* L, int metatableIndex, bool inPlace = false) {
	metatableIndex = lpcd_abs_index(L, metatableIndex);
	lua_pushstring(L, "__index");		// metatable "__index"
	lua_newtable(L);					// metatable "__index" __index
	lua_pushboolean(L, inPlace ? 1 : 0);// metatable "__index" __index inPlace
	lua_pushcclosure(L, LPCD::PropertyMetatable_index, 2);	// metatable "__index" PropertyMetatable_index
	lua_rawset(L, metatableIndex);		// metatable

	lua_pushstring(L, "__newindex");	// metatable "__newindex"
	lua_newtable(L);					// metatable "__newindex" __newindex
	lua_pushboolean(L, inPlace ? 1 : 0);// metatable "__newindex" __newindex inPlace
	lua_pushcclosure(L, LPCD::PropertyMetatable_newindex, 2);	// metatable "__newindex" PropertyMetatable_newindex
	lua_rawset(L, metatableIndex);		// metatable
}


inline void lpcd_propertymetatable_getfunctions(lua_State* L, int metatableIndex) {
	lua_getfield(L, metatableIndex, "__index");		// (metatable) __index
	lua_getupvalue(L, -1, 1);						// (metatable) __index props__functions
	lua_remove(L, -2);								// (metatable) props__functions
}


inline void lpcd_newclassmetatable(lua_State* L, const char* className, const char* baseClassName, bool inPlace = false) {
	luaL_newmetatable(L, className);			// class_metatable
	lpcd_integratepropertysupport(L, -1, inPlace);

	if (baseClassName) {
		luaL_getmetatable(L, baseClassName);	// class_metatable baseClass_metatable
		lua_setmetatable(L, -2);				// class_metatable
	}
}


template <typename Object, typename VarType>
inline void lpcd_propertycreate(lua_State* L, int metatableIndex, const char* varName, VarType Object::* var, bool read = true, bool write = true) {
	lua_pushvalue(L, metatableIndex);				// metatable

	lua_pushstring(L, varName);						// metatable varName

	if (read) {
		lua_getfield(L, -2, "__index");				// metatable varName __index
		lua_getupvalue(L, -1, 1);					// metatable varName __index functions
		lua_getupvalue(L, -2, 2);					// metatable varName __index functions inPlace
		bool inPlace = lua_toboolean(L, -1) != 0;
		lua_pop(L, 1);								// metatable varName __index functions
		lua_pushvalue(L, -3);						// metatable varName __index functions varName
		inPlace ? lpcd_pushmemberinplacepropertygetclosure(L, var) : lpcd_pushmemberpropertygetclosure(L, var);
													// metatable varName __index functions varName closure
		lua_rawset(L, -3);							// metatable varName __index functions
		lua_pop(L, 2);								// metatable varName
	}

	if (write) {
		lua_getfield(L, -2, "__newindex");			// metatable varName __newindex
		lua_getupvalue(L, -1, 1);					// metatable varName __newindex functions
		lua_getupvalue(L, -2, 2);					// metatable varName __newindex functions inPlace
		bool inPlace = lua_toboolean(L, -1) != 0;
		lua_pop(L, 1);								// metatable varName __newindex functions
		lua_pushvalue(L, -3);						// metatable varName __newindex functions varName
		inPlace ? lpcd_pushmemberinplacepropertysetclosure(L, var) : lpcd_pushmemberpropertysetclosure(L, var);
													// metatable varName __newindex functions varName closure
		lua_rawset(L, -3);							// metatable varName __newindex functions
		lua_pop(L, 2);								// metatable varName
	}

	lua_pop(L, 2);
}


inline void* lpcd_checkobject(lua_State* L, int index, const char* tname, bool throwError = true) {
	int wrappingType = lua_type(L, index);
	bool isTable = lua_type(L, index) == LUA_TTABLE;
	if (isTable) {
		lua_getfield(L, index, "__object");
		index = lpcd_abs_index(L, -1);
	}
	void *p = lua_touserdata(L, index);
	if (p != NULL) {  /* value is a userdata? */
		if (lua_getmetatable(L, index)) {  /* does it have a metatable? */
			if (tname) {
				lua_getfield(L, LUA_REGISTRYINDEX, tname);  /* get correct metatable */
				if (lua_rawequal(L, -1, -2)) {  /* does it have the correct mt? */
					lua_pop(L, 2);  /* remove both metatables */
					if (isTable)
						lua_pop(L, 1);
					return p;
				}
			}
			if (throwError) {
				luaL_typerror(L, index, tname);  /* else error */
			}
			return NULL;
		} else {
			if (isTable)
				lua_pop(L, 1);
			return p;
		}
	} else {
		if (throwError) {
			luaL_typerror(L, index, tname);
		} else {
			if (isTable)
				lua_pop(L, 1);
			return NULL;
		}
	}

	if (isTable)
		lua_pop(L, 1);
	return NULL;
}


class Class {
public:
	Class(lua_State* L, const char* className, const char* baseClassName = NULL)
		: L(L)
	{
		lpcd_newclassmetatable(L, className, baseClassName);
		lpcd_propertymetatable_getfunctions(L, -1);
	}

	~Class() {
		lua_pop(L, 2);
	}

	void MetatableFunction(const char* name, lua_CFunction func, unsigned int nupvalues = 0) {
		lua_pushcclosure(L, func, nupvalues);
		lua_setfield(L, -3, name);
	}

	template <typename Object, typename VarType>
	Class& Property(const char* varName, VarType Object::* var, bool read = true, bool write = true) {
		lpcd_propertycreate(L, -2, varName, var, read, write);
		return *this;
	}

	void Function(const char* name, lua_CFunction func, unsigned int nupvalues = 0) {
		lua_pushcclosure(L, func, nupvalues);
		lua_setfield(L, -2, name);
	}

	template <typename Callee>
	inline Class& ObjectFunctor(const char* name, int (Callee::*func)(lua_State*), unsigned int nupvalues = 0) {
		lpcd_pushobjectfunctorclosure(L, func, nupvalues);
		lua_setfield(L, -2, name);
		return *this;
	}

	template <typename Callee, typename Func>
	inline Class& ObjectDirect(const char* name, const Callee* callee, Func func, unsigned int nupvalues = 0) {
		lpcd_pushobjectdirectclosure(L, callee, func, nupvalues);
		lua_setfield(L, -2, name);
		return *this;
	}

	lua_State* L;
};


class InPlaceClass {
public:
	InPlaceClass(lua_State* L, const char* className, const char* baseClassName = NULL)
		: L(L)
	{
		lpcd_newclassmetatable(L, className, baseClassName, true);
		lpcd_propertymetatable_getfunctions(L, -1);
	}

	~InPlaceClass() {
		lua_pop(L, 2);
	}

	void MetatableFunction(const char* name, lua_CFunction func, unsigned int nupvalues = 0) {
		lua_pushcclosure(L, func, nupvalues);
		lua_setfield(L, -3, name);
	}

	template <typename Object, typename VarType>
	InPlaceClass& Property(const char* varName, VarType Object::* var, bool read = true, bool write = true) {
		lpcd_propertycreate(L, -2, varName, var, read, write);
		return *this;
	}

	template <typename Callee>
	inline InPlaceClass& ObjectFunctor(const char* name, int (Callee::*func)(lua_State*), unsigned int nupvalues = 0) {
		lpcd_pushinplaceobjectfunctorclosure(L, func, nupvalues);
		lua_setfield(L, -2, name);
		return *this;
	}

	template <typename Callee, typename Func>
	inline InPlaceClass& ObjectDirect(const char* name, const Callee* callee, Func func, unsigned int nupvalues = 0) {
		lpcd_pushinplaceobjectdirectclosure(L, callee, func, nupvalues);
		lua_setfield(L, -2, name);
		return *this;
	}

	lua_State* L;
};


#endif // LUAPLUS__LUAPLUSCD_H
