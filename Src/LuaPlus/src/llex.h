/*
** $Id: llex.h,v 1.58.1.1 2007/12/27 13:02:25 roberto Exp $
** Lexical Analyzer
** See Copyright Notice in lua.h
*/

#ifndef llex_h
#define llex_h

#include "lobject.h"
#include "lzio.h"


#define FIRST_RESERVED	257

/* maximum length of a reserved word */
#define TOKEN_LEN	(sizeof("function")/sizeof(char))


/*
* WARNING: if you change the order of this enumeration,
* grep "ORDER RESERVED"
*/
enum RESERVED {
  /* terminal symbols denoted by reserved words */
#if LUA_EXT_CONTINUE
  TK_AND = FIRST_RESERVED, TK_BREAK, TK_CONTINUE,
#else
  TK_AND = FIRST_RESERVED, TK_BREAK,
#endif /* LUA_EXT_CONTINUE */
  TK_DO, TK_ELSE, TK_ELSEIF, TK_END, TK_FALSE, TK_FOR, TK_FUNCTION,
  TK_IF, TK_IN, TK_LOCAL, TK_NIL, TK_NOT, TK_OR, TK_REPEAT,
  TK_RETURN, TK_THEN, TK_TRUE, TK_UNTIL, TK_WHILE,
  /* other terminal symbols */
  TK_CONCAT, TK_DOTS, TK_EQ, TK_GE, TK_LE, TK_NE, TK_NUMBER,
#if LUA_BITFIELD_OPS || LUA_WIDESTRING
  TK_NAME, TK_STRING,
#if LUA_WIDESTRING
  TK_WSTRING,
#endif /* LUA_WIDESTRING */
#if LUA_BITFIELD_OPS
  TK_SHL, TK_SHR, TK_XOR,
#endif /* LUA_BITFIELD_OPS */
  TK_EOS
#if LNUM_PATCH
  , TK_INT
#ifdef LNUM_COMPLEX
  , TK_NUMBER2   /* imaginary constants: Ni */ 
#endif
#endif /* LNUM_PATCH */
#else
  TK_NAME, TK_STRING, TK_EOS
#if LNUM_PATCH
  , TK_INT
#ifdef LNUM_COMPLEX
  , TK_NUMBER2   /* imaginary constants: Ni */ 
#endif
#endif /* LNUM_PATCH */
#endif /* LUA_BITFIELD_OPS || LUA_WIDESTRING */
};

/* number of reserved words */
#define NUM_RESERVED	(cast(int, TK_WHILE-FIRST_RESERVED+1))


/* array with token `names' */
LUAI_DATA const char *const luaX_tokens [];

#if LNUM_PATCH
/* SemInfo is a local data structure of 'llex.c', used for carrying a string
 * or a number. A separate token (TK_*) will tell, how to interpret the data.
 */      
#endif /* LNUM_PATCH */
typedef union {
  lua_Number r;
#if LNUM_PATCH
  lua_Integer i;
#endif /* LNUM_PATCH */
  TString *ts;
} SemInfo;  /* semantics information */


typedef struct Token {
  int token;
  SemInfo seminfo;
} Token;


typedef struct LexState {
  int current;  /* current character (charint) */
  int linenumber;  /* input line counter */
  int lastline;  /* line of last token `consumed' */
  Token t;  /* current token */
  Token lookahead;  /* look ahead token */
  struct FuncState *fs;  /* `FuncState' is private to the parser */
  struct lua_State *L;
  ZIO *z;  /* input stream */
  Mbuffer *buff;  /* buffer for tokens */
  TString *source;  /* current source name */
  char decpoint;  /* locale decimal point */
} LexState;


LUAI_FUNC void luaX_init (lua_State *L);
LUAI_FUNC void luaX_setinput (lua_State *L, LexState *ls, ZIO *z,
                              TString *source);
LUAI_FUNC TString *luaX_newstring (LexState *ls, const char *str, size_t l);
LUAI_FUNC void luaX_next (LexState *ls);
LUAI_FUNC void luaX_lookahead (LexState *ls);
LUAI_FUNC void luaX_lexerror (LexState *ls, const char *msg, int token);
LUAI_FUNC void luaX_syntaxerror (LexState *ls, const char *s);
LUAI_FUNC const char *luaX_token2str (LexState *ls, int token);


#endif
