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
Microsystems, Inc. ("Sun").

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

#ifndef _IDL_IDL_GLOBAL_HH
#define _IDL_IDL_GLOBAL_HH

#pragma ident "%@(#)idl_global.hh	1.1% %92/06/23% Sun Microsystems"

// idl_global.hh
//
// Defines a class containing all global data for the compiler.
// Can be subclassed in BEs to store more data

class IDL_GlobalData
{
public:
  // Types

  // The ParseState type encodes the state of the Yacc parser so that
  // better error messages can be produced, and for use by BEs in situations
  // where context sensitive behavior is required
  enum ParseState {
      PS_NoState		// No state
    , PS_TypeDeclSeen		// Seen complete typedef declaration
    , PS_ConstDeclSeen		// Seen complete const declaration
    , PS_ExceptDeclSeen		// Seen complete exception declaration
    , PS_InterfaceDeclSeen	// Seen complete interface declaration
    , PS_ModuleDeclSeen		// Seen complete module declaration
    , PS_AttrDeclSeen		// Seen complete attribute declaration
    , PS_OpDeclSeen		// Seen complete operation declaration	
    , PS_ModuleSeen		// Seen a MODULE keyword
    , PS_ModuleIDSeen		// Seen the module ID
    , PS_ModuleSqSeen		// '{' seen for module
    , PS_ModuleQsSeen		// '}' seen for module
    , PS_ModuleBodySeen		// Seen complete module body
    , PS_InterfaceSeen		// Seen an INTERFACE keyword
    , PS_InterfaceIDSeen	// Seen the interface ID
    , PS_InheritSpecSeen	// Seen a complete inheritance spec
    , PS_ForwardDeclSeen	// Forward interface decl seen
    , PS_InterfaceSqSeen	// '{' seen for interface
    , PS_InterfaceQsSeen	// '}' seen for interface
    , PS_InterfaceBodySeen	// Seen an interface body
    , PS_InheritColonSeen	// Seen ':' in inheritance list
    , PS_SNListCommaSeen	// Seen ',' in list of scoped names
    , PS_ScopedNameSeen		// Seen a complete scoped name
    , PS_SN_IDSeen		// Seen an identifier as part of a scoped name
    , PS_ScopeDelimSeen		// Seen a scope delim as party of a scoped name
    , PS_ConstSeen		// Seen a CONST keyword
    , PS_ConstTypeSeen		// Parsed the type of a constant
    , PS_ConstIDSeen		// Seen the constant ID
    , PS_ConstAssignSeen	// Seen the '='
    , PS_ConstExprSeen		// Seen the constant value expression
    , PS_TypedefSeen		// Seen a TYPEDEF keyword
    , PS_TypeSpecSeen		// Seen a complete type specification
    , PS_DeclaratorsSeen	// Seen a complete list of declarators
    , PS_StructSeen		// Seen a STRUCT keyword
    , PS_StructIDSeen		// Seen the struct ID
    , PS_StructSqSeen		// '{' seen for struct
    , PS_StructQsSeen		// '}' seen for struct
    , PS_StructBodySeen		// Seen complete body of struct decl
    , PS_MemberTypeSeen		// Seen type of struct or except member
    , PS_MemberDeclsSeen	// Seen decls of struct or except members
    , PS_MemberDeclsCompleted	// Completed one struct or except member to ';'
    , PS_UnionSeen		// Seen a UNION keyword
    , PS_UnionIDSeen		// Seen the union ID
    , PS_SwitchSeen		// Seen the SWITCH keyword
    , PS_SwitchOpenParSeen	// Seen the switch open par.
    , PS_SwitchTypeSeen		// Seen the switch type spec
    , PS_SwitchCloseParSeen	// Seen the switch close par.
    , PS_UnionSqSeen		// '{' seen for union
    , PS_UnionQsSeen		// '}' seen for union
    , PS_DefaultSeen		// Seen DEFAULT keyword
    , PS_UnionLabelSeen		// Seen label of union element
    , PS_LabelColonSeen		// Seen ':' of union branch label
    , PS_LabelExprSeen		// Seen expression of union branch label
    , PS_UnionElemSeen		// Seen a union element
    , PS_UnionElemCompleted	// Completed one union member up to ';'
    , PS_CaseSeen		// Seen a CASE keyword
    , PS_UnionElemTypeSeen	// Seen type spec for union element
    , PS_UnionElemDeclSeen	// Seen declarator for union element
    , PS_UnionBodySeen		// Seen completed union body
    , PS_EnumSeen		// Seen an ENUM keyword
    , PS_EnumIDSeen		// Seen the enum ID
    , PS_EnumSqSeen		// Seen '{' for enum
    , PS_EnumQsSeen		// Seen '}' for enum
    , PS_EnumBodySeen		// Seen complete enum body
    , PS_EnumCommaSeen		// Seen ',' in list of enumerators
    , PS_SequenceSeen		// Seen a SEQUENCE keyword
    , PS_SequenceSqSeen		// Seen '<' for sequence
    , PS_SequenceQsSeen		// Seen '>' for sequence
    , PS_SequenceTypeSeen	// Seen type decl for sequence
    , PS_SequenceCommaSeen	// Seen comma for sequence
    , PS_SequenceExprSeen	// Seen size expression for sequence
    , PS_StringSeen		// Seen a STRING keyword
    , PS_StringSqSeen		// Seen '<' for string
    , PS_StringQsSeen		// Seen '>' for string
    , PS_StringExprSeen		// Seen size expression for string
    , PS_StringCompleted	// Seen a string decl without size spec
    , PS_ArrayIDSeen		// Seen array ID
    , PS_ArrayCompleted		// Seen completed array declaration
    , PS_DimSqSeen		// Seen '[' for array dimension
    , PS_DimQsSeen		// Seen ']' for array dimension
    , PS_DimExprSeen		// Seen size expression for array dimension
    , PS_AttrROSeen		// Seen READONLY keyword
    , PS_AttrSeen		// Seen ATTRIBUTE keyword
    , PS_AttrTypeSeen		// Seen type decl for attribute
    , PS_AttrDeclsSeen		// Seen declarators for attribute
    , PS_AttrCompleted		// Seen complete attribute declaration
    , PS_ExceptSeen		// Seen EXCEPTION keyword
    , PS_ExceptIDSeen		// Seen exception identifier
    , PS_ExceptSqSeen		// Seen '{' for exception
    , PS_ExceptQsSeen		// Seen '}' for exception
    , PS_ExceptBodySeen		// Seen complete exception body
    , PS_OpAttrSeen		// Seen operation attribute
    , PS_OpTypeSeen		// Seen operation return type
    , PS_OpIDSeen		// Seen operation ID
    , PS_OpParsCompleted	// Completed operation param list
    , PS_OpRaiseCompleted	// Completed operation except list
    , PS_OpContextCompleted	// Completed operation context spec
    , PS_OpCompleted		// Completed operation statement
    , PS_OpSqSeen		// Seen '(' for operation
    , PS_OpQsSeen		// Seen ')' for operation
    , PS_OpParCommaSeen		// Seen ',' in list of op params
    , PS_OpParDirSeen		// Seen parameter direction
    , PS_OpParTypeSeen		// Seen parameter type
    , PS_OpParDeclSeen		// Seen parameter declaration
    , PS_OpRaiseSeen		// Seen RAISES keyword
    , PS_OpRaiseSqSeen		// Seen '(' for RAISES
    , PS_OpRaiseQsSeen		// Seen ')' for RAISES
    , PS_OpContextSeen		// Seen CONTEXT keyword
    , PS_OpContextSqSeen	// Seen '(' for CONTEXT
    , PS_OpContextQsSeen	// Seen ')' for CONTEXT
    , PS_OpContextCommaSeen	// Seen ',' for CONTEXT
    , PS_DeclsCommaSeen		// Seen ',' in declarators list
    , PS_DeclsDeclSeen		// Seen complete decl in decls list
  };

  // Constructor
  IDL_GlobalData();
  virtual ~IDL_GlobalData() {}

  // Operations
  virtual UTL_ScopeStack	*scopes();		// Scopes stack
  virtual void			set_scopes(UTL_ScopeStack *);
							// Set it

  virtual AST_Root		*root();		// Root of AST
  virtual void			set_root(AST_Root *);	// Set it

  virtual AST_Generator		*gen();			// Generator
  virtual void			set_gen(AST_Generator *);// Set it

  virtual UTL_Error		*err();			// Error reporter
  virtual void			set_err(UTL_Error *);	// Set it

  virtual long			err_count();		// How many errors?
  virtual void			set_err_count(long);	// Set it

  virtual long			lineno();		// Where in file?
  virtual void			set_lineno(long);	// Set it

  virtual String		*filename();		// What file?
  virtual void			set_filename(String *);	// Set it

  virtual String		*main_filename();	// What's the main
							// file name?
  virtual void			set_main_filename(String *);
							// Set it

  virtual String		*real_filename();	// What's the real
							// file name?
  virtual void			set_real_filename(String *);
			                                // Set it
  
  virtual String		*stripped_filename();	// Stripped filename
  virtual void			set_stripped_filename(String *);
							// Set it

  virtual idl_bool		imported();		// Are we imported?
  virtual idl_bool		import();		// Is import on?
  virtual void			set_import(idl_bool);	// Set it

  virtual idl_bool		in_main_file();		// Are we?
  virtual void			set_in_main_file(idl_bool);	// Set it

  virtual char			*prog_name();		// Invoked as..
  virtual void			set_prog_name(char *);	// Set it

  virtual char			*cpp_location();	// Where's CPP?
  virtual void			set_cpp_location(char *);// Set it

  virtual long			compile_flags();	// What flags are on?
  virtual void			set_compile_flags(long);// Turn some on or off

  virtual char			*be();			// Get BE to use
  virtual void			set_be(char *);		// Set it

  virtual char			*local_escapes();	// Get local escapes
  virtual void			set_local_escapes(char *);// Set it

  virtual UTL_Indenter		*indent();		// Get indenter
  virtual void			set_indent(UTL_Indenter *);
							// Set it

  virtual UTL_StrList		*pragmas();		// Get pragmas
  virtual void			set_pragmas(UTL_StrList *);// Set it

  virtual idl_bool		read_from_stdin();	// Reading from stdin?
  virtual void			set_read_from_stdin(idl_bool); // Set it

  virtual void			store_include_file_name(String *);

  virtual String	      **include_file_names();	// Array of file names
  virtual void			set_include_file_names(String **); // Set it

  virtual unsigned long		n_include_file_names();	// How many
  virtual void			set_n_include_file_names(unsigned long n);

  virtual ParseState		parse_state();		// What state we're in
  virtual void			set_parse_state(ParseState s); // Set it

  // Convert from a predefined type to an expression type
  virtual AST_Expression::ExprType
				PredefinedTypeToExprType(
				  AST_PredefinedType::PredefinedType
				);

private:
  // Data
  UTL_ScopeStack		*pd_scopes;		// Store scopes stack
  AST_Root			*pd_root;		// Store AST root
  AST_Generator			*pd_gen;		// Store generator
  UTL_Error			*pd_err;		// Error object
  long				pd_err_count;		// Count of errors
  long				pd_lineno;		// What line #
  String			*pd_filename;		// What file
  String			*pd_main_filename;	// What main filename
  String			*pd_real_filename;	// What real filename
  String			*pd_stripped_filename;	// Stripped filename
  idl_bool			pd_import;		// Is import on?
  idl_bool			pd_in_main_file;	// Are we in it?
  char				*pd_prog_name;		// Argv[0]
  char				*pd_cpp_location;	// Where to find CPP
  long				pd_compile_flags;	// Compile flags
  char				*pd_be;			// BE name to use
  char				*pd_local_escapes;	// Trapdoor argument
  UTL_Indenter			*pd_indent;		// Indent object
  UTL_StrList			*pd_pragmas;		// List of pragmas
							// as its being built
  idl_bool			pd_read_from_stdin;	// Reading from stdin?
  String			**pd_include_file_names;// Array of file names
  unsigned long			pd_n_include_file_names;// How many
  unsigned long			pd_n_alloced_file_names;// How many alloced

  ParseState			pd_parse_state;		// Parse state we're in

  // Operations
  long				seen_include_file_before(String *);
							// Seen this include
							// before?
};

#endif	//_IDL_IDL_GLOBAL_HH
