// $Id$
typedef union {
  CC_Command *command;
  CosConcurrencyControl::lock_mode lock_mode;
  char *id;
  int num;
} YYSTYPE;
#define	T_START_CMD	258
#define	T_CREATE_CMD	259
#define	T_LOCK_CMD	260
#define	T_UNLOCK_CMD	261
#define	T_TRYLOCK_CMD	262
#define	T_CHANGEMODE_CMD	263
#define	T_SLEEP_CMD	264
#define	T_WAIT_CMD	265
#define	T_REPEAT_CMD	266
#define	T_EXCEP_CMD	267
#define	T_PRINT_CMD	268
#define	T_LOOKUP_CMD	269
#define	T_TERM	270
#define	T_READ	271
#define	T_IREAD	272
#define	T_UPGRADE	273
#define	T_WRITE	274
#define	T_IWRITE	275
#define	T_IDENT	276
#define	Q_STRING	277
#define	T_NUM	278


extern YYSTYPE CC_commandlval;
