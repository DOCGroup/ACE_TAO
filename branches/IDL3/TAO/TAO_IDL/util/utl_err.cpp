// $Id$

/*

COPYRIGHT

Copyright 1992, 1993, 1994 Sun Microsystems, Inc.  Printed in the United
States of America.  All Rights Reserved.

This product is protected by copyright and distributed under the following
license restricting its use.

The Interface Definition Language Compiler Front End (CFE) is made
available for your use provided that you include this license and copyright
notice on all media and documentation and the software program in which
this product is incorporated in whole or part. You may copy and extend
functionality (but may not remove functionality) of the Interface
Definition Language CFE without charge, but you are not authorized to
license or distribute it to anyone else except as part of a product or
program developed by you or with the express written consent of Sun
Microsystems, Inc. ("Sun".

The names of Sun Microsystems, Inc. and any of its subsidiaries or
affiliates may not be used in advertising or publicity pertaining to
distribution of Interface Definition Language CFE as permitted herein.

This license is effective until terminated by Sun for failure to comply
with this license.  Upon termination, you shall destroy or return all code
and documentation for the Interface Definition Language CFE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED AS IS WITH NO WARRANTIES OF
ANY KIND INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS
FOR A PARTICULAR PURPOSE, NONINFRINGEMENT, OR ARISING FROM A COURSE OF
DEALING, USAGE OR TRADE PRACTICE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED WITH NO SUPPORT AND WITHOUT
ANY OBLIGATION ON THE PART OF Sun OR ANY OF ITS SUBSIDIARIES OR AFFILIATES
TO ASSIST IN ITS USE, CORRECTION, MODIFICATION OR ENHANCEMENT.

SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES SHALL HAVE NO LIABILITY WITH
RESPECT TO THE INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY
INTERFACE DEFINITION LANGUAGE CFE OR ANY PART THEREOF.

IN NO EVENT WILL SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES BE LIABLE FOR
ANY LOST REVENUE OR PROFITS OR OTHER SPECIAL, INDIRECT AND CONSEQUENTIAL
DAMAGES, EVEN IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

Use, duplication, or disclosure by the government is subject to
restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
Technical Data and Computer Software clause at DFARS 252.227-7013 and FAR
52.227-19.

Sun, Sun Microsystems and the Sun logo are trademarks or registered
trademarks of Sun Microsystems, Inc.

SunSoft, Inc.
2550 Garcia Avenue
Mountain View, California  94043

NOTE:

SunOS, SunSoft, Sun, Solaris, Sun Microsystems or the Sun logo are
trademarks or registered trademarks of Sun Microsystems, Inc.

 */

// utl_err.cc - Implementation of error reporting object for IDL
//              compiler program

#include "utl_err.h"
#include "utl_identifier.h"
#include "utl_string.h"
#include "global_extern.h"
#include "nr_extern.h"
#include "ast_interface.h"
#include "ast_enum.h"
#include "ast_union.h"
#include "ast_union_label.h"

ACE_RCSID (util, 
           utl_err, 
           "$Id$")

#if defined (ACE_GCC_HONORS_STD_NAMESPACE) \
             && (ACE_GCC_HONORS_STD_NAMESPACE == 1)
using std::cerr;
#endif

// Convert an error code into a const char *
static const char *
error_string (UTL_Error::ErrorCode c)
{
  switch (c)
  {
    case UTL_Error::EIDL_OK:
      return "all is fine ";
    case UTL_Error::EIDL_SYNTAX_ERROR:
      return "";
    case UTL_Error::EIDL_REDEF:
      return "illegal redefinition ";
    case UTL_Error::EIDL_DEF_USE:
      return "redefinition after use, ";
    case UTL_Error::EIDL_MULTIPLE_BRANCH:
      return "union with duplicate branch label ";
    case UTL_Error::EIDL_COERCION_FAILURE:
      return "coercion failure ";
    case UTL_Error::EIDL_SCOPE_CONFLICT:
      return "definition scope is different than fwd declare scope, ";
    case UTL_Error::EIDL_ONEWAY_CONFLICT:
      return "oneway operation with OUT or INOUT parameters, ";
    case UTL_Error::EIDL_PREFIX_CONFLICT:
      return "prefix at declaration differs from prefix at definition or use, ";
    case UTL_Error::EIDL_ILLEGAL_VERSION:
      return "illegal version number, ";
    case UTL_Error::EIDL_VERSION_RESET:
      return "version already set by #pragma version or #pragma id, ";
    case UTL_Error::EIDL_ID_RESET:
      return "cannot reset id to a different string, ";
    case UTL_Error::EIDL_TYPEID_RESET:
      return "repo id already set by previous call to 'typeid', ";
    case UTL_Error::EIDL_INVALID_TYPEID:
      return "'typeid' may not be applied to this type, ";
    case UTL_Error::EIDL_INVALID_TYPEPREFIX:
      return "'typeprefix' may not be applied to this type, ";
    case UTL_Error::EIDL_DISC_TYPE:
      return "union with illegal discriminator type, ";
    case UTL_Error::EIDL_LABEL_TYPE:
      return "label type incompatible with union discriminator type, ";
    case UTL_Error::EIDL_ILLEGAL_ADD:
      return "forward declared type may be used only as a sequence element, ";
    case UTL_Error::EIDL_ILLEGAL_USE:
      return "illegal type used in expression, ";
    case UTL_Error::EIDL_ILLEGAL_RAISES:
      return "error in or illegal use of raises(..) clause, ";
    case UTL_Error::EIDL_ILLEGAL_CONTEXT:
      return "error in context(..) clause, ";
    case UTL_Error::EIDL_CANT_INHERIT:
      // More intelligible message printed by error routine.
      return "";
    case UTL_Error::EIDL_CANT_SUPPORT:
      // More intelligible message printed by error routine.
      return "";
    case UTL_Error::EIDL_LOOKUP_ERROR:
      return "error in lookup of symbol: ";
    case UTL_Error::EIDL_INHERIT_FWD_ERROR:
      // More intelligible message printed by error routine.
      return "";
    case UTL_Error::EIDL_SUPPORTS_FWD_ERROR:
      // More intelligible message printed by error routine.
      return "";
    case UTL_Error::EIDL_CONSTANT_EXPECTED:
      return "constant expected: ";
    case UTL_Error::EIDL_EVAL_ERROR:
      return "expression evaluation error: ";
    case UTL_Error::EIDL_NAME_CASE_ERROR:
      return "identifier spellings differ only in case: ";
    case UTL_Error::EIDL_NAME_CASE_WARNING:
      return "Warning - identifier spellings differ only in case: ";
    case UTL_Error::EIDL_KEYWORD_ERROR:
      return "spelling differs from IDL keyword only in case: ";
    case UTL_Error::EIDL_KEYWORD_WARNING:
      return "Warning - spelling differs from IDL keyword only in case: ";
    case UTL_Error::EIDL_ENUM_VAL_EXPECTED:
      return "enumerator expected: ";
    case UTL_Error::EIDL_ENUM_VAL_NOT_FOUND:
      return "enumerator by this name not defined: ";
    case UTL_Error::EIDL_AMBIGUOUS:
      return "ambiguous definition: ";
    case UTL_Error::EIDL_DECL_NOT_DEFINED:
      return "forward declared but never defined: ";
    case UTL_Error::EIDL_FWD_DECL_LOOKUP:
      /* More intelligible message printed by error routine */
      return "";
    case UTL_Error::EIDL_RECURSIVE_TYPE:
      return "illegal recursive use of type: ";
    case UTL_Error::EIDL_NONVOID_ONEWAY:
      return "non-void return type in oneway operation: ";
    case UTL_Error::EIDL_REDEF_SCOPE:
      return "redefinition inside defining scope: ";
    case UTL_Error::EIDL_NOT_A_TYPE:
      return "specified symbol is not a type: ";
    case UTL_Error::EIDL_UNDERSCORE:
      return "identifier has more than one leading underscore: ";
    case UTL_Error::EIDL_EMPTY_MODULE:
      return "module must contain at least one declaration: ";
    case UTL_Error::EIDL_BACK_END:
      return "back end: ";
  }

  return 0;
}

// Print out an error message header on cerr
static void
idl_error_header (UTL_Error::ErrorCode c,
                  long lineno,
                  UTL_String *s)
{
  ACE_ERROR ((LM_ERROR,
              "%s: \"%s\", line %d: %s",
              idl_global->prog_name (),
              idl_global->read_from_stdin () ? "standard input"
                                             : s->get_string (),
              lineno == -1 ? idl_global->lineno () : lineno,
              error_string (c)));
}

// Convert the type of an AST_Expression to a char *.
static const char *
exprtype_to_string (AST_Expression::ExprType t)
{
  switch (t) {
  case AST_Expression::EV_short:
    return "short";
  case AST_Expression::EV_ushort:
    return "unsigned short";
  case AST_Expression::EV_long:
    return "long";
  case AST_Expression::EV_ulong:
    return "unsigned long";
  case AST_Expression::EV_float:
    return "float";
  case AST_Expression::EV_double:
    return "double";
  case AST_Expression::EV_char:
    return "char";
  case AST_Expression::EV_octet:
    return "octet";
  case AST_Expression::EV_bool:
    return "boolean";
  case AST_Expression::EV_string:
    return "string";
  case AST_Expression::EV_enum:
    return "enum";
  case AST_Expression::EV_void:
    return "void";
  case AST_Expression::EV_none:
    return "none";
  case AST_Expression::EV_wchar:
    return "wchar";
  case AST_Expression::EV_longlong:
  case AST_Expression::EV_ulonglong:
  case AST_Expression::EV_longdouble:
  case AST_Expression::EV_wstring:
    return 0;
  }
  return 0;
}

// Convert a parse state into a possible error message
static const char *
parse_state_to_error_message (IDL_GlobalData::ParseState ps)
{
  switch (ps) {
  case IDL_GlobalData::PS_NoState:
    return "Statement cannot be parsed";
  case IDL_GlobalData::PS_TypeDeclSeen:
    return "Malformed typedef declaration";
  case IDL_GlobalData::PS_ConstDeclSeen:
    return "Malformed const declaration";
  case IDL_GlobalData::PS_ExceptDeclSeen:
    return "Malformed exception declaration";
  case IDL_GlobalData::PS_InterfaceDeclSeen:
    return "Malformed interface declaration";
  case IDL_GlobalData::PS_ValuetypeDeclSeen:
    return "Malformed value type declaration";
  case IDL_GlobalData::PS_ModuleDeclSeen:
    return "Malformed module declaration";
  case IDL_GlobalData::PS_AttrDeclSeen:
    return "Malformed attribute declaration";
  case IDL_GlobalData::PS_OpDeclSeen:
    return "Malformed operation declaration";
  case IDL_GlobalData::PS_ModuleSeen:
    return "Missing module identifier following MODULE keyword";
  case IDL_GlobalData::PS_ModuleIDSeen:
    return "Missing '{' or illegal syntax following module identifier";
  case IDL_GlobalData::PS_ModuleSqSeen:
    return "Illegal syntax following module '{' opener";
  case IDL_GlobalData::PS_ModuleQsSeen:
    return "Illegal syntax following module '}' closer";
  case IDL_GlobalData::PS_ModuleBodySeen:
    return "Illegal syntax following module body statement(s)";
  case IDL_GlobalData::PS_InterfaceSeen:
    return "Missing interface identifier following INTERFACE keyword";
  case IDL_GlobalData::PS_ValuetypeSeen:
    return "Missing interface identifier following VALUETYPE keyword";
  case IDL_GlobalData::PS_InterfaceIDSeen:
    return "Illegal syntax following interface identifier";
  case IDL_GlobalData::PS_ValuetypeIDSeen:
    return "Illegal syntax following value type identifier";
  case IDL_GlobalData::PS_InheritSpecSeen:
    return "Missing '{' or illegal syntax following inheritance spec";
  case IDL_GlobalData::PS_InterfaceForwardSeen:
    return "Missing ';' following forward interface declaration";
  case IDL_GlobalData::PS_ValuetypeForwardSeen:
    return "Missing ';' following forward value type declaration";
  case IDL_GlobalData::PS_StructForwardSeen:
    return "Missing ';' following forward struct declaration";
  case IDL_GlobalData::PS_UnionForwardSeen:
    return "Missing ';' following forward union declaration";
  case IDL_GlobalData::PS_InterfaceSqSeen:
    return "Illegal syntax following interface '{' opener";
  case IDL_GlobalData::PS_InterfaceQsSeen:
    return "Illegal syntax following interface '}' closer";
  case IDL_GlobalData::PS_InterfaceBodySeen:
    return "Illegal syntax following interface body statement(s)";
  case IDL_GlobalData::PS_InheritColonSeen:
    return "Illegal syntax following ':' starting inheritance list";
  case IDL_GlobalData::PS_SNListCommaSeen:
    return "Found illegal scoped name in scoped name list";
  case IDL_GlobalData::PS_ScopedNameSeen:
    return "Missing ',' following scoped name in scoped name list";
  case IDL_GlobalData::PS_SN_IDSeen:
    return "Illegal component in scoped name";
  case IDL_GlobalData::PS_ScopeDelimSeen:
    return "Illegal component in scoped name following '::'";
  case IDL_GlobalData::PS_ConstSeen:
    return "Missing type or illegal syntax following CONST keyword";
  case IDL_GlobalData::PS_ConstTypeSeen:
    return "Missing identifier or illegal syntax following const type";
  case IDL_GlobalData::PS_ConstIDSeen:
    return "Missing '=' or illegal syntax after const identifier";
  case IDL_GlobalData::PS_ConstAssignSeen:
    return "Missing value expr or illegal syntax following '='";
  case IDL_GlobalData::PS_ConstExprSeen:
    return "Missing ';' or illegal syntax following value expr in const";
  case IDL_GlobalData::PS_TypedefSeen:
    return "Missing type or illegal syntax following TYPEDEF keyword";
  case IDL_GlobalData::PS_TypeSpecSeen:
    return "Missing declarators or illegal syntax following type spec";
  case IDL_GlobalData::PS_DeclaratorsSeen:
    return "Illegal syntax following declarators in TYPEDEF declaration";
  case IDL_GlobalData::PS_StructSeen:
    return "Missing struct identifier following STRUCT keyword";
  case IDL_GlobalData::PS_StructIDSeen:
    return "Missing '{' or illegal syntax following struct identifier";
  case IDL_GlobalData::PS_StructSqSeen:
    return "Illegal syntax following struct '{' opener";
  case IDL_GlobalData::PS_StructQsSeen:
    return "Illegal syntax following struct '}' closer";
  case IDL_GlobalData::PS_StructBodySeen:
    return "Illegal syntax following struct body statement(s)";
  case IDL_GlobalData::PS_MemberTypeSeen:
    return "Illegal syntax or missing identifier following member type";
  case IDL_GlobalData::PS_MemberDeclsSeen:
    return "Illegal syntax following member declarator(s)";
  case IDL_GlobalData::PS_MemberDeclsCompleted:
    return "Missing ',' between member decls of same type(?)";
  case IDL_GlobalData::PS_UnionSeen:
    return "Missing identifier following UNION keyword";
  case IDL_GlobalData::PS_UnionIDSeen:
    return "Illegal syntax following union identifier";
  case IDL_GlobalData::PS_SwitchSeen:
    return "Illegal syntax following SWITCH keyword";
  case IDL_GlobalData::PS_SwitchOpenParSeen:
    return "Illegal syntax following '(' in switch in union";
  case IDL_GlobalData::PS_SwitchTypeSeen:
    return "Illegal syntax following type decl in switch in union";
  case IDL_GlobalData::PS_SwitchCloseParSeen:
    return "Missing union '{' opener";
  case IDL_GlobalData::PS_UnionSqSeen:
    return "Illegal syntax following union '{' opener";
  case IDL_GlobalData::PS_UnionQsSeen:
    return "Illegal syntax following union '}' closer";
  case IDL_GlobalData::PS_DefaultSeen:
    return "Illegal syntax or missing ':' following DEFAULT keyword";
  case IDL_GlobalData::PS_UnionLabelSeen:
    return "Illegal syntax following branch label in union";
  case IDL_GlobalData::PS_LabelColonSeen:
    return "Illegal syntax following ':' in branch label in union";
  case IDL_GlobalData::PS_LabelExprSeen:
    return "Illegal syntax following label expression in union";
  case IDL_GlobalData::PS_UnionElemSeen:
  case IDL_GlobalData::PS_UnionElemCompleted:
    return "Illegal syntax following union element";
  case IDL_GlobalData::PS_CaseSeen:
    return "Illegal syntax following CASE keyword in union";
  case IDL_GlobalData::PS_UnionElemTypeSeen:
    return "Illegal syntax following type decl in union element";
  case IDL_GlobalData::PS_UnionElemDeclSeen:
    return "Illegal syntax following declarator in union element";
  case IDL_GlobalData::PS_UnionBodySeen:
    return "Illegal syntax following union body statement(s)";
  case IDL_GlobalData::PS_EnumSeen:
    return "Illegal syntax or missing identifier following ENUM keyword";
  case IDL_GlobalData::PS_EnumIDSeen:
    return "Illegal syntax or missing '{' following enum identifier";
  case IDL_GlobalData::PS_EnumSqSeen:
    return "Illegal syntax following enum '{' opener";
  case IDL_GlobalData::PS_EnumQsSeen:
    return "Illegal syntax following enum '}' closer";
  case IDL_GlobalData::PS_EnumBodySeen:
    return "Illegal syntax following enum body statement(s)";
  case IDL_GlobalData::PS_EnumCommaSeen:
    return "Illegal syntax or missing identifier following ',' in enum";
  case IDL_GlobalData::PS_SequenceSeen:
    return "Illegal syntax or missing '<' following SEQUENCE keyword";
  case IDL_GlobalData::PS_SequenceSqSeen:
    return "Illegal syntax or missing type following '<' in sequence";
  case IDL_GlobalData::PS_SequenceQsSeen:
    return "Illegal syntax following '>' in sequence";
  case IDL_GlobalData::PS_SequenceTypeSeen:
    return "Illegal syntax following sequence type declaration";
  case IDL_GlobalData::PS_SequenceCommaSeen:
    return "Illegal syntax or missing size expr following ',' in sequence";
  case IDL_GlobalData::PS_SequenceExprSeen:
    return "Illegal syntax or missing '>' following size expr in sequence";
  case IDL_GlobalData::PS_StringSeen:
    return "Illegal syntax or missing '<' following STRING keyword";
  case IDL_GlobalData::PS_StringSqSeen:
    return "Illegal syntax or missing size expr following '<' in string";
  case IDL_GlobalData::PS_StringQsSeen:
    return "Illegal syntax following '>' in string";
  case IDL_GlobalData::PS_StringExprSeen:
    return "Illegal syntax or missing '>' after size expr in string";
  case IDL_GlobalData::PS_StringCompleted:
    return "Illegal syntax after string declaration";
  case IDL_GlobalData::PS_ArrayIDSeen:
    return "Illegal syntax or missing dimensions after array identifier";
  case IDL_GlobalData::PS_ArrayCompleted:
    return "Illegal syntax after array declaration";
  case IDL_GlobalData::PS_DimSqSeen:
    return "Illegal syntax or missing size expr after '[' in array declaration";
  case IDL_GlobalData::PS_DimQsSeen:
    return "Illegal syntax after ']' in array declaration";
  case IDL_GlobalData::PS_DimExprSeen:
    return "Illegal syntax or missing ']' after size expr in array declaration";
  case IDL_GlobalData::PS_AttrROSeen:
    return "Illegal syntax after READONLY keyword";
  case IDL_GlobalData::PS_AttrSeen:
    return "Illegal syntax after ATTRIBUTE keyword";
  case IDL_GlobalData::PS_AttrTypeSeen:
    return "Illegal syntax after type in attribute declaration";
  case IDL_GlobalData::PS_AttrDeclsSeen:
    return "Illegal syntax after attribute declarators";
  case IDL_GlobalData::PS_AttrCompleted:
    return "Illegal syntax after attribute declaration";
  case IDL_GlobalData::PS_ExceptSeen:
    return "Illegal syntax or missing identifier after EXCEPTION keyword";
  case IDL_GlobalData::PS_ExceptIDSeen:
    return "Illegal syntax or missing '{' after exception identifier";
  case IDL_GlobalData::PS_ExceptSqSeen:
    return "Illegal syntax after exception '{' opener";
  case IDL_GlobalData::PS_ExceptQsSeen:
    return "Illegal syntax after exception '}' closer";
  case IDL_GlobalData::PS_ExceptBodySeen:
    return "Illegal syntax after exception body statement(s)";
  case IDL_GlobalData::PS_OpAttrSeen:
    return "Illegal syntax or missing type after operation attribute";
  case IDL_GlobalData::PS_OpTypeSeen:
    return "Illegal syntax or missing identifier after operation type";
  case IDL_GlobalData::PS_OpIDSeen:
    return "Illegal syntax or missing '(' after operation identifier";
  case IDL_GlobalData::PS_OpParsCompleted:
    return "Illegal syntax after operation parameter list";
  case IDL_GlobalData::PS_OpRaiseCompleted:
    return "Illegal syntax after optional RAISES in operation declaration";
  case IDL_GlobalData::PS_OpContextCompleted:
    return "Illegal syntax after optional CONTEXT in operation declaration";
  case IDL_GlobalData::PS_OpCompleted:
    return "Illegal syntax after operation declaration";
  case IDL_GlobalData::PS_OpSqSeen:
    return "Illegal syntax after operation parameter list '(' opener";
  case IDL_GlobalData::PS_OpQsSeen:
    return "Illegal syntax after operation parameter list ')' closer";
  case IDL_GlobalData::PS_OpParCommaSeen:
    return "Illegal syntax or missing direction in parameter declaration";
  case IDL_GlobalData::PS_OpParDirSeen:
    return "Illegal syntax or missing type in parameter declaration";
  case IDL_GlobalData::PS_OpParTypeSeen:
    return "Illegal syntax or missing declarator in parameter declaration";
  case IDL_GlobalData::PS_OpParDeclSeen:
    return "Illegal syntax following parameter declarator";
  case IDL_GlobalData::PS_OpRaiseSeen:
    return "Illegal syntax or missing '(' after RAISES keyword";
  case IDL_GlobalData::PS_OpRaiseSqSeen:
    return "Illegal syntax after RAISES '(' opener";
  case IDL_GlobalData::PS_OpRaiseQsSeen:
    return "Illegal syntax after RAISES ')' closer";
  case IDL_GlobalData::PS_OpContextSeen:
    return "Illegal syntax or missing '(' after CONTEXT keyword";
  case IDL_GlobalData::PS_OpContextSqSeen:
    return "Illegal syntax after CONTEXT '(' opener";
  case IDL_GlobalData::PS_OpContextQsSeen:
    return "Illegal syntax after CONTEXT ')' closer";
  case IDL_GlobalData::PS_OpContextCommaSeen:
    return "Illegal syntax after ',' in CONTEXT in operation declaration";
  case IDL_GlobalData::PS_DeclsCommaSeen:
    return "Illegal syntax after ',' in declarators list";
  case IDL_GlobalData::PS_DeclsDeclSeen:
    return "Illegal syntax after declarator in declarators list";
  case IDL_GlobalData::PS_PragmaPrefixSyntax:
    return "Illegal syntax for #pragma prefix";
  default:
    return "Some syntax error";
  }
}

// Public methods.

// Report error messages with various numbers of arguments.

void
UTL_Error::syntax_error (IDL_GlobalData::ParseState ps)
{
  idl_error_header (EIDL_SYNTAX_ERROR,
                    idl_global->lineno (),
                    idl_global->filename ());
  ACE_ERROR ((LM_ERROR,
              "%s\n",
              parse_state_to_error_message (ps)));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

void
UTL_Error::error0 (UTL_Error::ErrorCode c)
{
  idl_error_header (c,
                    idl_global->lineno (),
                    idl_global->filename ());
  ACE_ERROR ((LM_ERROR,
              "\n"));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

void
UTL_Error::error1 (UTL_Error::ErrorCode c,
                   AST_Decl *d)
{
  idl_error_header (c,
                    idl_global->lineno (),
                    idl_global->filename ());
  d->name ()->dump (*ACE_DEFAULT_LOG_STREAM);;
  ACE_ERROR ((LM_ERROR,
              "\n"));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

void
UTL_Error::error2 (UTL_Error::ErrorCode c,
                   AST_Decl *d1,
                   AST_Decl *d2)
{
  idl_error_header (c,
                    idl_global->lineno (),
                    idl_global->filename ());
  d1->name ()->dump (*ACE_DEFAULT_LOG_STREAM);;
  ACE_ERROR ((LM_ERROR,
              ", "));
  d2->name ()->dump (*ACE_DEFAULT_LOG_STREAM);;
  ACE_ERROR ((LM_ERROR,
              "\n"));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

void
UTL_Error::error3 (UTL_Error::ErrorCode c,
                   AST_Decl *d1,
                   AST_Decl *d2,
                   AST_Decl *d3)
{
  idl_error_header (c,
                    idl_global->lineno (),
                    idl_global->filename ());
  d1->name ()->dump (*ACE_DEFAULT_LOG_STREAM);;
  ACE_ERROR ((LM_ERROR,
              ", "));
  d2->name ()->dump (*ACE_DEFAULT_LOG_STREAM);;
  ACE_ERROR ((LM_ERROR,
              ", "));
  d3->name ()->dump (*ACE_DEFAULT_LOG_STREAM);;
  ACE_ERROR ((LM_ERROR,
              "\n"));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

void
UTL_Error::warning0 (UTL_Error::ErrorCode c)
{
  idl_error_header (c,
                    idl_global->lineno (),
                    idl_global->filename ());
  ACE_ERROR ((LM_ERROR,
              "\n"));
}

void
UTL_Error::warning1 (UTL_Error::ErrorCode c,
                     AST_Decl *d)
{
  idl_error_header (c,
                    idl_global->lineno (),
                    idl_global->filename ());
  d->name ()->dump (*ACE_DEFAULT_LOG_STREAM);;
  ACE_ERROR ((LM_ERROR,
              "\n"));
}

void
UTL_Error::warning2 (UTL_Error::ErrorCode c,
                     AST_Decl *d1,
                     AST_Decl *d2)
{
  idl_error_header (c,
                    idl_global->lineno (),
                    idl_global->filename ());
  d1->name ()->dump (*ACE_DEFAULT_LOG_STREAM);;
  ACE_ERROR ((LM_ERROR,
              ", "));
  d2->name ()->dump (*ACE_DEFAULT_LOG_STREAM);;
  ACE_ERROR ((LM_ERROR,
              "\n"));
}

void
UTL_Error::warning3 (UTL_Error::ErrorCode c,
                     AST_Decl *d1,
                     AST_Decl *d2,
                     AST_Decl *d3)
{
  idl_error_header (c,
                    idl_global->lineno (),
                    idl_global->filename ());
  d1->name ()->dump (*ACE_DEFAULT_LOG_STREAM);;
  ACE_ERROR ((LM_ERROR,
              ", "));
  d2->name ()->dump (*ACE_DEFAULT_LOG_STREAM);;
  ACE_ERROR ((LM_ERROR,
              ", "));
  d3->name ()->dump (*ACE_DEFAULT_LOG_STREAM);;
  ACE_ERROR ((LM_ERROR,
              "\n"));
}

// Report a failed coercion attempt.
void
UTL_Error::coercion_error (AST_Expression *v,
                           AST_Expression::ExprType t)
{
  idl_error_header (EIDL_COERCION_FAILURE,
                    v->line (),
                    v->file_name ());
  v->dump (*ACE_DEFAULT_LOG_STREAM);
  ACE_ERROR ((LM_ERROR,
              " to %s\n",
              exprtype_to_string (t)));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

// Report a failed name lookup attempt.
void
UTL_Error::lookup_error (UTL_ScopedName *n)
{
  idl_error_header (EIDL_LOOKUP_ERROR,
                    idl_global->lineno (),
                    idl_global->filename ());
  n->dump (*ACE_DEFAULT_LOG_STREAM);;
  ACE_ERROR ((LM_ERROR,
              "\n"));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

// Report an illegal version number assignment.
void
UTL_Error::version_number_error (char *n)
{
  idl_error_header (EIDL_ILLEGAL_VERSION,
                    idl_global->lineno (),
                    idl_global->filename ());
  ACE_ERROR ((LM_ERROR,
              "%s\n",
              n));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

// Report an attempt to set the version a second time.
void
UTL_Error::version_reset_error (void)
{
  idl_error_header (EIDL_VERSION_RESET,
                    idl_global->lineno (),
                    idl_global->filename ());
  ACE_ERROR ((LM_ERROR,
              "\n"));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

// Report a reset of the id a second time to a different string.
void
UTL_Error::id_reset_error (char *o,
                           char *n)
{
  idl_error_header (EIDL_ID_RESET,
                    idl_global->lineno (),
                    idl_global->filename ());
  ACE_ERROR ((LM_ERROR,
              "%s, %s\n",
              o,
              n));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

// Report an attempt to inherit from an interface which was only
// declared forward but not yet defined.
void
UTL_Error::inheritance_fwd_error (UTL_ScopedName *n,
                                  AST_Interface *f)
{
  idl_error_header (EIDL_INHERIT_FWD_ERROR,
                    f->line (),
                    f->file_name ());
  ACE_ERROR ((LM_ERROR,
              "interface "));
  n->dump (*ACE_DEFAULT_LOG_STREAM);
  ACE_ERROR ((LM_ERROR,
              " cannot inherit from forward declared interface "));
  f->local_name ()->dump (*ACE_DEFAULT_LOG_STREAM);
  ACE_ERROR ((LM_ERROR,
              "\n"));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

// Report an attempt to inherit from something other than an interface.
void
UTL_Error::inheritance_error (UTL_ScopedName *n,
                              AST_Decl *d)
{
  idl_error_header (EIDL_CANT_INHERIT,
                    idl_global->lineno (),
                    idl_global->filename ());
  n->dump (*ACE_DEFAULT_LOG_STREAM);;
  ACE_ERROR ((LM_ERROR,
              " attempts to inherit from "));
  d->name ()->dump (*ACE_DEFAULT_LOG_STREAM);
  ACE_ERROR ((LM_ERROR,
              "\n"));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

// Report an attempt to support an interface which was only
// declared forward but not yet defined.
void
UTL_Error::supports_fwd_error (UTL_ScopedName *n,
                               AST_Interface *f)
{
  idl_error_header (EIDL_SUPPORTS_FWD_ERROR,
                    f->line (),
                    f->file_name ());
  ACE_ERROR ((LM_ERROR,
              "interface "));
  n->dump (*ACE_DEFAULT_LOG_STREAM);
  ACE_ERROR ((LM_ERROR,
              " cannot support forward declared interface "));
  f->local_name ()->dump (*ACE_DEFAULT_LOG_STREAM);
  ACE_ERROR ((LM_ERROR,
              "\n"));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

// Report an attempt to support something other than an interface.
void
UTL_Error::supports_error (UTL_ScopedName *n,
                           AST_Decl *d)
{
  idl_error_header (EIDL_CANT_SUPPORT,
                    idl_global->lineno (),
                    idl_global->filename ());
  n->dump (*ACE_DEFAULT_LOG_STREAM);
  ACE_ERROR ((LM_ERROR,
              " attempts to support "));
  d->name ()->dump (*ACE_DEFAULT_LOG_STREAM);;
  ACE_ERROR ((LM_ERROR,
              "\n"));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

// Report illegal inheritance from non-abstract valuetype or interface.
void
UTL_Error::abstract_inheritance_error (UTL_ScopedName *v,
                                       UTL_ScopedName *i)
{
  idl_error_header (EIDL_CANT_INHERIT,
                    idl_global->lineno (),
                    idl_global->filename ());
  ACE_ERROR ((LM_ERROR,
              " abstract valuetype "));
  v->dump (*ACE_DEFAULT_LOG_STREAM);
  ACE_ERROR ((LM_ERROR,
              " attempts to inherit from nonabstract type: "));
  i->dump (*ACE_DEFAULT_LOG_STREAM);
  ACE_ERROR ((LM_ERROR,
              "\n"));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

// Report illegal support of non-abstract interface.
void
UTL_Error::abstract_support_error (UTL_ScopedName *v,
                                   UTL_ScopedName *i)
{
  idl_error_header (EIDL_CANT_SUPPORT,
                    idl_global->lineno (),
                    idl_global->filename ());
  ACE_ERROR ((LM_ERROR,
              " valuetype "));
  v->dump (*ACE_DEFAULT_LOG_STREAM);
  ACE_ERROR ((LM_ERROR,
              " attempts to support more than one concrete type: "));
  i->dump (*ACE_DEFAULT_LOG_STREAM);
  ACE_ERROR ((LM_ERROR,
              "\n"));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

void
UTL_Error::concrete_supported_inheritance_error (UTL_ScopedName *v,
                                                 UTL_ScopedName *i)
{
  idl_error_header (EIDL_CANT_SUPPORT,
                    idl_global->lineno (),
                    idl_global->filename ());
  ACE_ERROR ((LM_ERROR,
              " valuetype "));
  v->dump (*ACE_DEFAULT_LOG_STREAM);
  ACE_ERROR ((LM_ERROR,
              " supports concrete interface that does not inherit from"
              " all ancestors of valuetype's ancestor's concrete supported"
              " interface: "));
  i->dump (*ACE_DEFAULT_LOG_STREAM);
  ACE_ERROR ((LM_ERROR,
              "\n"));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

// Report an error while evaluating an expression.
void
UTL_Error::eval_error (AST_Expression *v)
{
  idl_error_header (EIDL_EVAL_ERROR,
                    v->line (),
                    v->file_name ());
  v->dump (*ACE_DEFAULT_LOG_STREAM);
  ACE_ERROR ((LM_ERROR,
              "\n"));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

// Report a situation where a constant was expected but we
// got something else. This may occur in a union when a label
// evaluates to a non-constant.
void
UTL_Error::constant_expected (UTL_ScopedName *n,
                              AST_Decl *d)
{
  idl_error_header (EIDL_CONSTANT_EXPECTED,
                    d->line (),
                    d->file_name ());
  n->dump (*ACE_DEFAULT_LOG_STREAM);;
  ACE_ERROR ((LM_ERROR,
              " bound to "));
  d->dump (*ACE_DEFAULT_LOG_STREAM);
  ACE_ERROR ((LM_ERROR,
              "\n"));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

// Report a situation where an enumerator was expected but we
// got something else. This may occur in a union with an enum
// discriminator where a label did not evaluate to a constant
// enumerator.
void
UTL_Error::enum_val_expected (AST_Union *u,
                              AST_UnionLabel *l)
{
  idl_error_header (EIDL_ENUM_VAL_EXPECTED,
                    u->line (),
                    u->file_name ());
  ACE_ERROR ((LM_ERROR,
              " union %s, ",
              u->local_name ()->get_string ()));
  l->dump (*ACE_DEFAULT_LOG_STREAM);
  ACE_ERROR ((LM_ERROR,
              "\n"));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

// Report a situation where an enumerator was received but we
// could not find it (by name) in an enum. This occurs when
// a union with an enum discriminator is being parsed, and a
// label evaluates to an enumerator which is not a member of
// the enum discriminator type.
void
UTL_Error::enum_val_lookup_failure (AST_Union *u,
                                    AST_Enum *e,
                                    UTL_ScopedName *n)
{
  idl_error_header (EIDL_ENUM_VAL_NOT_FOUND,
                    u->line (),
                    u->file_name ());
  ACE_ERROR ((LM_ERROR,
              " union %s,  enum %s,  enumerator ",
              u->local_name ()->get_string (),
              e->local_name ()->get_string ()));
  n->dump (*ACE_DEFAULT_LOG_STREAM);
  ACE_ERROR ((LM_ERROR,
              "\n"));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

// Report clash between declared and referenced indentifiers.
void
UTL_Error::redef_error (char *b,
                        char *n)
{
  idl_error_header (EIDL_REDEF,
                    idl_global->lineno (),
                    idl_global->filename ());
  ACE_ERROR ((LM_ERROR,
              "\"%s\" and \"%s\"\n",
              b,
              n));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

// Report two or more spellings for an identifier.
void
UTL_Error::name_case_error (char *b,
                            char *n)
{
  idl_error_header (EIDL_NAME_CASE_ERROR,
                    idl_global->lineno (),
                    idl_global->filename ());
  ACE_ERROR ((LM_ERROR,
              "\"%s\" and \"%s\"\n",
              b,
              n));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

void
UTL_Error::name_case_warning (char *b,
                              char *n)
{
  idl_error_header (EIDL_NAME_CASE_WARNING,
                    idl_global->lineno (),
                    idl_global->filename ());
  ACE_ERROR ((LM_ERROR,
              "\"%s\" and \"%s\"\n",
              b,
              n));
}

void
UTL_Error::idl_keyword_error (char *n)
{
  idl_error_header (EIDL_KEYWORD_ERROR,
                    idl_global->lineno (),
                    idl_global->filename ());
  ACE_ERROR ((LM_ERROR,
              "\"%s\"\n",
              n));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

void
UTL_Error::idl_keyword_warning (char *n)
{
  idl_error_header (EIDL_KEYWORD_WARNING,
                    idl_global->lineno (),
                    idl_global->filename ());
  ACE_ERROR ((LM_ERROR,
              "\"%s\"\n",
              n));
}

// Report an ambiguous definition of a name.
void
UTL_Error::ambiguous (UTL_Scope *s,
                      AST_Decl *l,
                      AST_Decl *d)
{
  idl_error_header (EIDL_AMBIGUOUS,
                    d->line (),
                    d->file_name ());
  ACE_ERROR ((LM_ERROR,
              " scope: %s,  collision: ",
              (ScopeAsDecl (s))->local_name ()->get_string ()));
  d->name ()->dump (*ACE_DEFAULT_LOG_STREAM);;
  ACE_ERROR ((LM_ERROR,
              " vs. "));
  l->name ()->dump (*ACE_DEFAULT_LOG_STREAM);;
  ACE_ERROR ((LM_ERROR,
              "\n"));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

// Report a forward declared interface which was never defined.
void
UTL_Error::fwd_decl_not_defined (AST_Type *d)
{
  idl_error_header (EIDL_DECL_NOT_DEFINED,
                    d->line (),
                    d->file_name ());
  ACE_ERROR ((LM_ERROR,
              "interface "));
  d->local_name ()->dump (*ACE_DEFAULT_LOG_STREAM);
  ACE_ERROR ((LM_ERROR,
              "\n"));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

// Report lookup in undefined forward declared but undefined interface.
void
UTL_Error::fwd_decl_lookup (AST_Interface *d,
                            UTL_ScopedName *n)
{
  idl_error_header (EIDL_FWD_DECL_LOOKUP,
                    idl_global->lineno (),
                    idl_global->filename ());
  ACE_ERROR ((LM_ERROR,
              "trying to look up "));
  n->dump (*ACE_DEFAULT_LOG_STREAM);;
  ACE_ERROR ((LM_ERROR,
              " in undefined forward declared interface "));
  d->local_name ()->dump (*ACE_DEFAULT_LOG_STREAM);
  ACE_ERROR ((LM_ERROR,
              "\n"));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

// Report a redefinition inside its own scope.
void
UTL_Error::redefinition_in_scope (AST_Decl *d,
                                  AST_Decl *s)
{
  idl_error_header (EIDL_REDEF_SCOPE,
                    d->line (),
                    d->file_name ());
  d->name ()->dump (*ACE_DEFAULT_LOG_STREAM);;
  ACE_ERROR ((LM_ERROR,
              ", "));
  s->name ()->dump (*ACE_DEFAULT_LOG_STREAM);;
  ACE_ERROR ((LM_ERROR,
              "\n"));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

// Report not a type error.
void
UTL_Error::not_a_type (AST_Decl *d)
{
  idl_error_header (EIDL_NOT_A_TYPE,
                    idl_global->lineno (),
                    idl_global->filename ());
  if (d == 0 || d->name () == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "unknown symbol"));
    }
  else
    {
      d->name ()->dump (*ACE_DEFAULT_LOG_STREAM);;
    }

  ACE_ERROR ((LM_ERROR,
              "\n"));
  idl_global->set_err_count (idl_global->err_count () + 1);
}

void
UTL_Error::back_end (long lineno,
                     UTL_String *s)
{
  idl_error_header (EIDL_BACK_END,
                    lineno,
                    s);
  idl_global->set_err_count (idl_global->err_count () + 1);
}

