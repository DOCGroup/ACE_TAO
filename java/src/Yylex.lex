package JACE.ServiceConfigurator;

import java.io.*;
import JACE.OS.*;
import java_cup.runtime.*;

// This was written for JLex version 1.2

%%

// Return a java_cup.runtime.token instead of a Yytoken from yylex()
%type java_cup.runtime.token
%{
  // Used to assemble the parameter string for a service
  private String params;  
%}

%eofval{ 
  return new java_cup.runtime.token (sym.EOF);
%eofval}

%line

%state COMMENT
%state PARAMS

ALPHA=[A-Za-z_]
DIGIT=[0-9]
WHITE_SPACE=[\ \t\b\012]
PATHNAME=[\.\\\/A-Za-z_\-0-9]
NEWLINE=\n
OTHER=.

%%

<YYINITIAL> dynamic {return new java_cup.runtime.token (sym.ACE_DYNAMIC); }
<YYINITIAL> static { return new java_cup.runtime.token (sym.ACE_STATIC); }
<YYINITIAL> suspend { return new java_cup.runtime.token (sym.ACE_SUSPEND); }
<YYINITIAL> resume { return new java_cup.runtime.token (sym.ACE_RESUME); }
<YYINITIAL> remove { return new java_cup.runtime.token (sym.ACE_REMOVE); }
<YYINITIAL> stream { return new java_cup.runtime.token (sym.ACE_USTREAM); }
<YYINITIAL> Module { return new java_cup.runtime.token (sym.ACE_MODULE_T); }
<YYINITIAL> Service_Object { return new java_cup.runtime.token (sym.ACE_SVC_OBJ_T); }
<YYINITIAL> STREAM { return new java_cup.runtime.token (sym.ACE_STREAM_T); }
<YYINITIAL> active { return new java_cup.runtime.token (sym.ACE_ACTIVE); }
<YYINITIAL> inactive { return new java_cup.runtime.token (sym.ACE_INACTIVE); }
<YYINITIAL> ":" { return new java_cup.runtime.token (sym.ACE_COLON); }
<YYINITIAL> \" {
  yybegin(PARAMS);
  params = new String();
}
<YYINITIAL> "#" { yybegin(COMMENT); }
<YYINITIAL> "*" { return new java_cup.runtime.token (sym.ACE_STAR); }
<YYINITIAL> "(" { return new java_cup.runtime.token (sym.ACE_LPAREN); }
<YYINITIAL> ")" { return new java_cup.runtime.token (sym.ACE_RPAREN); }
<YYINITIAL> "{" { return new java_cup.runtime.token (sym.ACE_LBRACE); }
<YYINITIAL> "}" { return new java_cup.runtime.token (sym.ACE_RBRACE); }
<YYINITIAL> {WHITE_SPACE}* { /* Skip all white space */ }
<YYINITIAL> {ALPHA}({ALPHA}|{DIGIT}|_)* { 
  return new java_cup.runtime.str_token (sym.ACE_IDENT, yytext()); 
}
<YYINITIAL> {PATHNAME}* { 
  return new java_cup.runtime.str_token (sym.ACE_PATHNAME, yytext()); 
}
<YYINITIAL> {NEWLINE} { /* Empty */ }
<YYINITIAL> {OTHER} { 
  ACE.ERROR ("Unknown text, line " + (yyline + 1) + ": \"" + yytext() + "\""); 
  return new java_cup.runtime.str_token (sym.error); 
}
<PARAMS> [\"\n] {
  yybegin(YYINITIAL);
  return new java_cup.runtime.str_token (sym.ACE_STRING, params);
}
<PARAMS> . {
  params = params + yytext();
}
<COMMENT> {NEWLINE} { yybegin(YYINITIAL); }
<COMMENT> {OTHER} { /* Skip everything on a comment line */ }
