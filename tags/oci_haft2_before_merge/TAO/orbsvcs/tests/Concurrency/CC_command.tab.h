// $Id$

typedef union {
  CC_Command *command;
  CosConcurrencyControl::lock_mode lock_mode;
  char *id;
  int num;
} ACE_CC_YYSTYPE;
#define	T_START_CMD	257
#define	T_CREATE_CMD	258
#define	T_LOCK_CMD	259
#define	T_UNLOCK_CMD	260
#define	T_TRYLOCK_CMD	261
#define	T_CHANGEMODE_CMD	262
#define	T_SLEEP_CMD	263
#define	T_WAIT_CMD	264
#define	T_REPEAT_CMD	265
#define	T_EXCEP_CMD	266
#define	T_PRINT_CMD	267
#define	T_LOOKUP_CMD	268
#define	T_TERM	269
#define	T_READ	270
#define	T_IREAD	271
#define	T_UPGRADE	272
#define	T_WRITE	273
#define	T_IWRITE	274
#define	T_IDENT	275
#define	Q_STRING	276
#define	T_NUM	277


extern ACE_CC_YYSTYPE ace_cc_yylval;
