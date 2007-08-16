/* $id$
 * ============================================================================
 *
 * = LIBRARY
 *    TAO/orbsvcs/tests
 *
 * = FILENAME
 *    CC_command.l
 *
 * = DESCRIPTION
 *   This is the lex file for the concurrency service script language.
 *
 * = AUTHORS
 *	Torben Worm <tworm@cs.wustl.edu>
 *
 * ============================================================================
 */
%option noyywrap nounput noinput always-interactive
%{
#include "CC_command.h"
#include "CC_command.tab.h"
#include "ace/ACE.h"

extern int line_no;
extern char line_buf[500];
%}

%%

;		       return T_TERM;
start                  return T_START_CMD;
create                 return T_CREATE_CMD;
lock                   return T_LOCK_CMD;
unlock                 return T_UNLOCK_CMD;
try_lock               return T_TRYLOCK_CMD;
change_mode	       return T_CHANGEMODE_CMD;
sleep                  return T_SLEEP_CMD;
wait                   return T_WAIT_CMD;
print                  return T_PRINT_CMD;
repeat                 return T_REPEAT_CMD;
exception	       return T_EXCEP_CMD;
lookup		       return T_LOOKUP_CMD;
read		       return T_READ;
intention_read         return T_IREAD;
upgrade		       return T_UPGRADE;
write		       return T_WRITE;
intention_write	       return T_IWRITE;
\".*\"		       { yylval.id = ACE_OS::strdup(&yytext[1]);
			 yylval.id[yyleng-2]='\0';
			 return Q_STRING; }
[0-9]+                 { yylval.num = atoi(yytext); return T_NUM; }
[A-Za-z][A-Za-z_0-9\.:/]*  { yylval.id = ACE_OS::strdup(yytext);
                           return T_IDENT; }
^\/\/.*\n			{ }
[ ]+			{ }
\n.*                     { line_no ++;
                           ACE_OS::strcpy(line_buf, yytext+1);
                           yyless(1); }
