/* $Id$
 * ============================================================================
 *
 * = LIBRARY
 *    TAO/orbsvcs/tests
 *
 * = FILENAME
 *    CC_command.y
 *
 * = DESCRIPTION
 *   This is the yacc file for the concurrency service script language.
 *
 * = AUTHORS
 *	Torben Worm <tworm@cs.wustl.edu>
 *
 * ============================================================================
 */
%{
#include "CC_command.h"
#include "ace/ACE.h"
#include "orbsvcs/CosConcurrencyControlC.h"

void yyerror (const char* s);
int yylex(void);

extern int line_no;
extern char line_buf[500];
extern char *yytext;
extern CC_CommandList *cmdlist;
%}

%union {
  CC_Command *command;
  CosConcurrencyControl::lock_mode lock_mode;
  char *id;
  int num;
}

%token T_START_CMD T_CREATE_CMD T_LOCK_CMD T_UNLOCK_CMD T_TRYLOCK_CMD
%token T_CHANGEMODE_CMD T_SLEEP_CMD T_WAIT_CMD T_REPEAT_CMD T_EXCEP_CMD
%token T_PRINT_CMD T_LOOKUP_CMD T_TERM
%token T_READ T_IREAD T_UPGRADE T_WRITE T_IWRITE
%token <id> T_IDENT Q_STRING
%token <num> T_NUM

%type <command> cmd
%type <id> config_file_name lock_set_name exception_name
%type <lock_mode> lock_mode

%%
start:		/* empty */
		| cmd_list { cmdlist->add(new CC_Sleep_Cmd(0));
			   /* dummy to check exeption in the last command */
		  if(cmdlist->execute()!=0) //CC_FAIL
                    { printf(" ** Test succeded!!\n"); }
                  else
                    { printf(" ** Test FAILED!!\n"); } }
		;

cmd_list:	cmd_list cmd { cmdlist->add($2); }
		| cmd { cmdlist->add($1); }
		;

cmd:		  T_START_CMD config_file_name T_TERM
			{ $$ = new CC_Start_Cmd($2); }
		| T_CREATE_CMD lock_set_name T_TERM
			{ $$ = new CC_CreateLockSet_Cmd($2); }
		| T_CREATE_CMD T_TERM
                        { $$ = new CC_CreateLockSet_Cmd(""); }
		| T_LOCK_CMD lock_set_name lock_mode T_TERM
			{ $$ = new CC_Lock_Cmd($2, $3); }
		| T_LOCK_CMD lock_mode T_TERM
			{ $$ = new CC_Lock_Cmd("", $2); }
		| T_UNLOCK_CMD lock_set_name lock_mode T_TERM
			{ $$ = new CC_UnLock_Cmd($2, $3); }
		| T_UNLOCK_CMD lock_mode T_TERM
			{ $$ = new CC_UnLock_Cmd("", $2); }
		| T_TRYLOCK_CMD lock_set_name lock_mode T_TERM
			{ $$ = new CC_TryLock_Cmd($2, $3); }
		| T_TRYLOCK_CMD lock_mode T_TERM
			{ $$ = new CC_TryLock_Cmd("", $2); }
		| T_CHANGEMODE_CMD lock_set_name lock_mode lock_mode T_TERM
		        { $$ = new CC_ChangeMode_Cmd($2, $3, $4); }
		| T_CHANGEMODE_CMD lock_mode lock_mode T_TERM
		        { $$ = new CC_ChangeMode_Cmd("", $2, $3); }
		| T_SLEEP_CMD T_NUM T_TERM
			{ $$ = new CC_Sleep_Cmd($2); }
		| T_WAIT_CMD T_TERM
			{ $$ = new CC_Wait_Cmd(""); }
		| T_WAIT_CMD Q_STRING T_TERM
			{ $$ = new CC_Wait_Cmd($2); }
		| T_PRINT_CMD Q_STRING T_TERM
			{ $$ = new CC_Print_Cmd($2); }
		| T_LOOKUP_CMD lock_set_name T_TERM
			{ $$ = new CC_Lookup_Cmd($2); }
		| T_REPEAT_CMD T_NUM T_TERM
                        { cmdlist->setrepeat($2);
                          $$ = new CC_Dummy_Cmd();}
		| T_EXCEP_CMD exception_name T_TERM
			{ $$ = new CC_Excep_Cmd($2); }
		;

config_file_name: T_IDENT {$$ = $1; }

lock_set_name:	T_IDENT {$$ = $1; }

exception_name:	T_IDENT {$$ = $1; }

lock_mode:	T_READ { $$ = CosConcurrencyControl::read; }
		| T_IREAD { $$ = CosConcurrencyControl::intention_read; }
		| T_UPGRADE { $$ = CosConcurrencyControl::upgrade; }
		| T_WRITE { $$ = CosConcurrencyControl::write; }
		| T_IWRITE { $$ = CosConcurrencyControl::intention_write; }

%%

void
yyerror (const char *s)
{
  ACE_OS::printf ("%d: %s at %s in:\n%s\n",
                  line_no,
                  s,
                  yytext,
                  line_buf);
  ACE_OS::exit (-1);
}
