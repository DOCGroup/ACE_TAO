package JACE.ServiceConfigurator;

import java.io.*;
import java_cup.runtime.*;
import JACE.OS.*;

parser code {: 
  // Lexical Analyzer
  private Yylex lexer_;

  public void setLexer(Yylex lexer)
    {
      this.lexer_ = lexer;
    }
:};

init with {: 
      try {

	FileInputStream fs = new FileInputStream(new String(ServiceConfig.serviceConfigFile_));

        this.setLexer(new Yylex(fs));

      } catch (FileNotFoundException fnf) {

	ACE.ERROR("File not found: " + fnf);

      } catch (SecurityException se) {

	ACE.ERROR("Security: " + se);
      }
:};

scan with {: return this.lexer_.yylex(); :};

terminal token ACE_DYNAMIC, ACE_STATIC, ACE_SUSPEND, ACE_RESUME, ACE_REMOVE, ACE_STREAM;
terminal token ACE_MODULE_T, ACE_STREAM_T, ACE_SVC_OBJ_T, ACE_ACTIVE, ACE_INACTIVE;
terminal str_token ACE_PATHNAME, ACE_IDENT, ACE_STRING;
terminal token ACE_LPAREN, ACE_RPAREN, ACE_LBRACE, ACE_RBRACE, ACE_STAR, ACE_COLON;
terminal token ACE_USTREAM; 

non terminal AddServiceObjectNode dynamic, svc_location; /* AddServiceObjectNode */
non terminal SuspendNode suspend;  /* SuspendNode */
non terminal ResumeNode resume;  /* ResumeNode */
non terminal RemoveNode remove;  /* RemoveNode */
non terminal ParseNode module_list, stream, svc_config_entry;
non terminal ParseNode svc_config_entries, static;
non terminal java_cup.runtime.str_token stream_modules, module;
non terminal java_cup.runtime.int_token status;
non terminal java_cup.runtime.str_token svc_initializer;
non terminal java_cup.runtime.str_token pathname, parameters_opt;
non terminal java_cup.runtime.str_token stream_ops, type;

start with svc_config_entries;

svc_config_entries ::= svc_config_entry:e1 svc_config_entries
                       {: 
			 if (e1 != null)
			   e1.apply();
                       :}
                       |
                       svc_config_entry:e1
                       {:  
			 if (e1 != null)
			   e1.apply();
		       :}
                       ;

svc_config_entry ::= dynamic
                     {: 
		       /* Empty -- result auto set to dynamic */
		       /* CUP$result = (ParseNode)CUP$stack.elementAt(CUP$top-0); */
		     :}
                     | 
                     static  
                     {: 
		       /* More graceful error system needed here */
		       ACE.ERROR("Not implemented: static service loading"); :}
                     | 
                     suspend
                     {: 
		       /* Empty -- result auto set to suspend */
		       /* CUP$result = (ParseNode)CUP$stack.elementAt(CUP$top-0); */
		     :}
                     | 
                     resume
                     {:  /* Empty -- result auto set to resume */ 
		     :}
                     | 
                     remove
                     {: 

		     :}
                     | 
                     stream
		     {: ACE.ERROR("Not implemented: stream loading"); :}
                     ;

dynamic ::= ACE_DYNAMIC svc_location:e1 parameters_opt:e2
{:
  RESULT.init(e1.name(), e1.locator(), e1.suspended());

  RESULT.params(e2.str_val);
:}
     ;

static ::= ACE_STATIC ACE_IDENT parameters_opt
     ;

suspend ::= ACE_SUSPEND ACE_IDENT:e1
{:
  RESULT.init(e1.str_val);
:}
     ;

resume ::= ACE_RESUME ACE_IDENT:e1
{:
  RESULT.init(e1.str_val);
:}
     ;

remove ::= ACE_REMOVE ACE_IDENT:e1
{:
  RESULT.init(e1.str_val);
:}
     ;

stream ::= ACE_USTREAM stream_ops stream_modules
           |
           ACE_USTREAM ACE_IDENT stream_modules
           ;

stream_ops ::= dynamic
               |
               static
               ;

stream_modules ::= ACE_LBRACE 
                   |
                   module_list ACE_RBRACE
                   ;

module_list ::= module_list module
                {: ACE.ERROR("Not implemented: module manipulation"); :}
                | 
                {: ACE.ERROR("Not implemented: module manipulation"); :}
                ;

module ::= dynamic
           |
           static
           |
           suspend
           | 
           resume
           | 
           remove
           ;

svc_location ::= ACE_IDENT:e1 type:e2 svc_initializer:e3 status:e4
{:
  boolean suspended = false;
  if (e4.int_val == 1)
    suspended = true;

  RESULT.init(e1.str_val, e3.str_val, suspended);
:}
;

status ::= ACE_ACTIVE
           {: RESULT.int_val = 0; :}
           |
           ACE_INACTIVE
           {: RESULT.int_val = 1; :}
           |
           {: // Default case
              RESULT.int_val = 0; :} 
           ;

svc_initializer ::= pathname:e1 ACE_COLON ACE_IDENT:e2
                    {: RESULT.str_val = new String(e1.str_val + ":" + e2.str_val); :}
                    |
                    pathname:e1 ACE_COLON ACE_IDENT:e2 ACE_LPAREN ACE_RPAREN
                    {: RESULT.str_val = new String(e1.str_val + ":" + e2.str_val); :}
                    ;

type ::= ACE_MODULE_T ACE_STAR
         |
         ACE_SVC_OBJ_T ACE_STAR
         {: RESULT.str_val = new String("Service Object"); :}
         |
         ACE_STREAM_T ACE_STAR
         ;

parameters_opt ::= ACE_STRING:e
                   {: RESULT.str_val = new String(e.str_val); :}
                   |
                   ;

pathname ::= ACE_PATHNAME:e
             {: RESULT.str_val = new String(e.str_val); :}
             | 
             ACE_IDENT:e
             {: RESULT.str_val = new String(e.str_val); :}
             ;



