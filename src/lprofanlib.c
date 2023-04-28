/*
** $Id: lprofanlib.c $
** unofficial profanOS library
*/

#define lprofanlib_c
#define LUA_LIB

#include "lprefix.h"


#include <syscall.h>
#include <stdlib.h>

#include "lua.h"

#include "lauxlib.h"
#include "lualib.h"


static int pf_setpixel (lua_State *L) {
  int x = luaL_checkinteger(L, 1);
  int y = luaL_checkinteger(L, 2);
  int color = luaL_checkinteger(L, 3);
  c_vesa_set_pixel(x, y, color);
  return 1;
}

static int pf_ticks (lua_State *L) {
  lua_pushinteger(L, c_timer_get_ms());
  return 1;
}

static int pf_call_c (lua_State *L) {
  // lua: profan.call_c(function_pointer, arg1_size, arg1, arg2_size, arg2, ...)
  // all arguments are integers
  int function_pointer = luaL_checkinteger(L, 1);
  int arg_count = lua_gettop(L) - 1;
  if (arg_count % 2 != 0) {
    luaL_error(L, "invalid argument count");
    return 0;
  }
  int *args_sizes = malloc(arg_count * sizeof(int) / 2);
  int *args = malloc(arg_count * sizeof(int) / 2);
  int i;
  for (i = 0; i < arg_count; i += 2) {
    args_sizes[i / 2] = luaL_checkinteger(L, i + 2);
    args[i / 2] = luaL_checkinteger(L, i + 3);
  }

  // push arguments in reverse order
  for (i = arg_count / 2 - 1; i >= 0; i--) {
    if (args_sizes[i] == 4) {
      asm volatile("pushl %0" : : "r"(args[i]));
    } else if (args_sizes[i] == 2) {
      // move the 2 bytes to the lower 2 bytes of a 4 byte integer
      asm volatile("pushl %0" : : "r"(args[i] << 16));
    } else if (args_sizes[i] == 1) {
      // move the byte to the lower byte of a 4 byte integer
      asm volatile("pushl %0" : : "r"(args[i] << 24));
    } else {
      luaL_error(L, "invalid argument size");
      return 0;
    }
  }

  // call the function
  int return_value;
  asm volatile("call *%1" : "=a"(return_value) : "r"(function_pointer));

  free(args_sizes);
  free(args);

  lua_pushinteger(L, return_value);
  return 1;
}

static int pf_memval (lua_State *L) {
  // lua: profan.memval(address, size)
  // size is 1, 2 or 4
  int address = luaL_checkinteger(L, 1);
  int size = luaL_checkinteger(L, 2);
  if (size == 1) {
    lua_pushinteger(L, *((char *)address));
  } else if (size == 2) {
    lua_pushinteger(L, *((short *)address));
  } else if (size == 4) {
    lua_pushinteger(L, *((int *)address));
  } else {
    luaL_error(L, "invalid size");
  }
  return 1;
}

static int pf_memset (lua_State *L) {
  // lua: profan.memset(address, size, value)
  // size is 1, 2 or 4
  int address = luaL_checkinteger(L, 1);
  int size = luaL_checkinteger(L, 2);
  int value = luaL_checkinteger(L, 3);
  if (size == 1) {
    *((char *)address) = value;
  } else if (size == 2) {
    *((short *)address) = value;
  } else if (size == 4) {
    *((int *)address) = value;
  } else {
    luaL_error(L, "invalid size");
  }
  return 1;
}

static int pf_pin (lua_State *L) {
  // lua: profan.pin(port, size)
  // size is 1, 2 or 4
  int port = luaL_checkinteger(L, 1);
  int size = luaL_checkinteger(L, 2);
  int result;
  if (size == 1) {
    asm volatile("in %%dx, %%al" : "=a" (result) : "d" (port));
  } else if (size == 2) {
    asm volatile("in %%dx, %%ax" : "=a" (result) : "d" (port));
  } else if (size == 4) {
    asm volatile("in %%dx, %%eax" : "=a" (result) : "d" (port));
  } else {
    luaL_error(L, "invalid size");
  }
  lua_pushinteger(L, result);
  return 1;
}

static int pf_pout (lua_State *L) {
  // lua: profan.pout(port, size, value)
  // size is 1, 2 or 4
  int port = luaL_checkinteger(L, 1);
  int size = luaL_checkinteger(L, 2);
  int value = luaL_checkinteger(L, 3);
  if (size == 1) {
    asm volatile("out %%al, %%dx" : : "a" (value), "d" (port));
  } else if (size == 2) {
    asm volatile("out %%ax, %%dx" : : "a" (value), "d" (port));
  } else if (size == 4) {
    asm volatile("out %%eax, %%dx" : : "a" (value), "d" (port));
  } else {
    luaL_error(L, "invalid size");
  }
  return 1;
}

static const luaL_Reg profanlib[] = {
  {"setpixel",  pf_setpixel},
  {"ticks",     pf_ticks},
  {"call_c",    pf_call_c},
  {"memval",    pf_memval},
  {"memset",    pf_memset},
  {"pin",       pf_pin},
  {"pout",      pf_pout},
  {NULL, NULL}
};


/* }====================================================== */


LUAMOD_API int luaopen_profan (lua_State *L) {
  luaL_newlib(L, profanlib);
  return 1;
}
