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

#include "ace/SString.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Containers_T.h"

// idl_global.hh
//
// Defines a class containing all front end global data.

class TAO_IDL_FE_Export IDL_GlobalData
{
public:
  // Types

  // The ParseState type encodes the state of the Yacc parser so that
  // better error messages can be produced, and for use by BEs in situations
  // where context sensitive behavior is required
  enum ParseState {
      PS_NoState                // No state
    , PS_TypeDeclSeen           // Seen complete typedef declaration
    , PS_ConstDeclSeen          // Seen complete const declaration
    , PS_ExceptDeclSeen         // Seen complete exception declaration
    , PS_InterfaceDeclSeen      // Seen complete interface declaration
    , PS_ModuleDeclSeen         // Seen complete module declaration
    , PS_ValuetypeDeclSeen      // Seen complete valuetype declaration
    , PS_AttrDeclSeen           // Seen complete attribute declaration
    , PS_OpDeclSeen             // Seen complete operation declaration
    , PS_ModuleSeen             // Seen a MODULE keyword
    , PS_ModuleIDSeen           // Seen the module ID
    , PS_ModuleSqSeen           // '{' seen for module
    , PS_ModuleQsSeen           // '}' seen for module
    , PS_ModuleBodySeen         // Seen complete module body
    , PS_InterfaceSeen          // Seen an INTERFACE keyword
    , PS_InterfaceIDSeen        // Seen the interface ID
    , PS_InheritSpecSeen        // Seen a complete inheritance spec
    , PS_ForwardDeclSeen        // Forward interface decl seen
    , PS_InterfaceSqSeen        // '{' seen for interface
    , PS_InterfaceQsSeen        // '}' seen for interface
    , PS_InterfaceBodySeen      // Seen an interface body
    , PS_InheritColonSeen       // Seen ':' in inheritance list
    , PS_ValuetypeSeen          // Seen a VALUETYPE keyword
    , PS_ValuetypeIDSeen        // Seen the valuetype ID
    , PS_SNListCommaSeen        // Seen ',' in list of scoped names
    , PS_ScopedNameSeen         // Seen a complete scoped name
    , PS_SN_IDSeen              // Seen an identifier as part of a scoped name
    , PS_ScopeDelimSeen         // Seen a scope delim as party of a scoped name
    , PS_ConstSeen              // Seen a CONST keyword
    , PS_ConstTypeSeen          // Parsed the type of a constant
    , PS_ConstIDSeen            // Seen the constant ID
    , PS_ConstAssignSeen        // Seen the '='
    , PS_ConstExprSeen          // Seen the constant value expression
    , PS_TypedefSeen            // Seen a TYPEDEF keyword
    , PS_TypeSpecSeen           // Seen a complete type specification
    , PS_DeclaratorsSeen        // Seen a complete list of declarators
    , PS_StructSeen             // Seen a STRUCT keyword
    , PS_StructIDSeen           // Seen the struct ID
    , PS_StructSqSeen           // '{' seen for struct
    , PS_StructQsSeen           // '}' seen for struct
    , PS_StructBodySeen         // Seen complete body of struct decl
    , PS_MemberTypeSeen         // Seen type of struct or except member
    , PS_MemberDeclsSeen        // Seen decls of struct or except members
    , PS_MemberDeclsCompleted   // Completed one struct or except member to ';'
    , PS_UnionSeen              // Seen a UNION keyword
    , PS_UnionIDSeen            // Seen the union ID
    , PS_SwitchSeen             // Seen the SWITCH keyword
    , PS_SwitchOpenParSeen      // Seen the switch open par.
    , PS_SwitchTypeSeen         // Seen the switch type spec
    , PS_SwitchCloseParSeen     // Seen the switch close par.
    , PS_UnionSqSeen            // '{' seen for union
    , PS_UnionQsSeen            // '}' seen for union
    , PS_DefaultSeen            // Seen DEFAULT keyword
    , PS_UnionLabelSeen         // Seen label of union element
    , PS_LabelColonSeen         // Seen ':' of union branch label
    , PS_LabelExprSeen          // Seen expression of union branch label
    , PS_UnionElemSeen          // Seen a union element
    , PS_UnionElemCompleted     // Completed one union member up to ';'
    , PS_CaseSeen               // Seen a CASE keyword
    , PS_UnionElemTypeSeen      // Seen type spec for union element
    , PS_UnionElemDeclSeen      // Seen declarator for union element
    , PS_UnionBodySeen          // Seen completed union body
    , PS_EnumSeen               // Seen an ENUM keyword
    , PS_EnumIDSeen             // Seen the enum ID
    , PS_EnumSqSeen             // Seen '{' for enum
    , PS_EnumQsSeen             // Seen '}' for enum
    , PS_EnumBodySeen           // Seen complete enum body
    , PS_EnumCommaSeen          // Seen ',' in list of enumerators
    , PS_SequenceSeen           // Seen a SEQUENCE keyword
    , PS_SequenceSqSeen         // Seen '<' for sequence
    , PS_SequenceQsSeen         // Seen '>' for sequence
    , PS_SequenceTypeSeen       // Seen type decl for sequence
    , PS_SequenceCommaSeen      // Seen comma for sequence
    , PS_SequenceExprSeen       // Seen size expression for sequence
    , PS_StringSeen             // Seen a STRING keyword
    , PS_StringSqSeen           // Seen '<' for string
    , PS_StringQsSeen           // Seen '>' for string
    , PS_StringExprSeen         // Seen size expression for string
    , PS_StringCompleted        // Seen a string decl without size spec
    , PS_ArrayIDSeen            // Seen array ID
    , PS_ArrayCompleted         // Seen completed array declaration
    , PS_DimSqSeen              // Seen '[' for array dimension
    , PS_DimQsSeen              // Seen ']' for array dimension
    , PS_DimExprSeen            // Seen size expression for array dimension
    , PS_AttrROSeen             // Seen READONLY keyword
    , PS_AttrSeen               // Seen ATTRIBUTE keyword
    , PS_AttrTypeSeen           // Seen type decl for attribute
    , PS_AttrDeclsSeen          // Seen declarators for attribute
    , PS_AttrCompleted          // Seen complete attribute declaration
    , PS_ExceptSeen             // Seen EXCEPTION keyword
    , PS_ExceptIDSeen           // Seen exception identifier
    , PS_ExceptSqSeen           // Seen '{' for exception
    , PS_ExceptQsSeen           // Seen '}' for exception
    , PS_ExceptBodySeen         // Seen complete exception body
    , PS_OpAttrSeen             // Seen operation attribute
    , PS_OpTypeSeen             // Seen operation return type
    , PS_OpIDSeen               // Seen operation ID
    , PS_OpParsCompleted        // Completed operation param list
    , PS_OpRaiseCompleted       // Completed operation except list
    , PS_OpContextCompleted     // Completed operation context spec
    , PS_OpCompleted            // Completed operation statement
    , PS_OpSqSeen               // Seen '(' for operation
    , PS_OpQsSeen               // Seen ')' for operation
    , PS_OpParCommaSeen         // Seen ',' in list of op params
    , PS_OpParDirSeen           // Seen parameter direction
    , PS_OpParTypeSeen          // Seen parameter type
    , PS_OpParDeclSeen          // Seen parameter declaration
    , PS_OpRaiseSeen            // Seen RAISES keyword
    , PS_OpRaiseSqSeen          // Seen '(' for RAISES
    , PS_OpRaiseQsSeen          // Seen ')' for RAISES
    , PS_OpContextSeen          // Seen CONTEXT keyword
    , PS_OpContextSqSeen        // Seen '(' for CONTEXT
    , PS_OpContextQsSeen        // Seen ')' for CONTEXT
    , PS_OpContextCommaSeen     // Seen ',' for CONTEXT
    , PS_DeclsCommaSeen         // Seen ',' in declarators list
    , PS_DeclsDeclSeen          // Seen complete decl in decls list
    , PS_NativeSeen             // Seen a native declaration
    , PS_PragmaPrefixSyntax     // Could not parse the #pragma prefix
  };

  // Constructor
  IDL_GlobalData (void);
  // Destructor
  virtual ~IDL_GlobalData (void);

  // Operations
  virtual UTL_ScopeStack   *scopes (void);              // Scopes stack
  virtual void             set_scopes (UTL_ScopeStack *);
                                                        // Set it

  virtual AST_Root         *root (void);                // Root of AST
  virtual void             set_root (AST_Root *);       // Set it

  virtual AST_Generator    *gen (void);                 // Generator
  virtual void             set_gen (AST_Generator *);   // Set it

  virtual UTL_Error        *err (void);                 // Error reporter
  virtual void             set_err (UTL_Error *);       // Set it

  virtual long             err_count (void);            // How many errors?
  virtual void             set_err_count (long);        // Set it

  virtual long             lineno (void);               // Where in file?
  virtual void             set_lineno (long);           // Set it

  virtual UTL_String       *filename (void);            // What file?
  virtual void             set_filename (UTL_String *); // Set it

  virtual UTL_String       *main_filename (void);       // What's the main
                                                        // file name?
  virtual void             set_main_filename (UTL_String *);
                                                        // Set it

  virtual UTL_String       *real_filename (void);       // What's the real
                                                        // file name?
  virtual void             set_real_filename (UTL_String *);
                                                        // Set it

  virtual UTL_String       *stripped_filename (void);   // Stripped filename
  virtual void             set_stripped_filename (UTL_String *);  // Set it

  virtual idl_bool         imported (void);             // Are we imported?
  virtual idl_bool         import (void);               // Is import on?
  virtual void             set_import (idl_bool);       // Set it

  virtual idl_bool         in_main_file (void);         // Are we?
  virtual void             set_in_main_file (idl_bool); // Set it

  virtual const char       *prog_name (void);           // Invoked as..
  virtual void             set_prog_name (const char *);  // Set it

  virtual const char       *cpp_location (void);        // Where's CPP?
  virtual void             set_cpp_location (const char *);// Set it

  virtual long             compile_flags (void);        // What flags are on?
  virtual void             set_compile_flags (long);    // Turn some on or off

  virtual const char       *be (void);                  // Get BE to use
  virtual void             set_be (const char *);       // Set it

  virtual char             *local_escapes (void);       // Get local escapes
  virtual void             set_local_escapes (const char *);// Set it

  virtual UTL_Indenter     *indent (void);              // Get indenter
  virtual void             set_indent (UTL_Indenter *); // Set it

  virtual idl_bool         read_from_stdin (void);      // Reading from stdin?
  virtual void             set_read_from_stdin (idl_bool); // Set it

  virtual void             store_include_file_name (UTL_String *);

  virtual UTL_String       **include_file_names (void); // Array of file names
  virtual void             set_include_file_names (UTL_String **); // Set it

  virtual unsigned long    n_include_file_names (void); // How many
  virtual void             set_n_include_file_names (unsigned long n);

  // = Access methods to deal with other IDL files included in the main
  //   IDL file. These IDL files are exactly the same strings that are
  //   "#include"d in the main IDL file, not the ones after CC
  //   preprocessor parsed the file.

  // Just storing the pointer. No memory will be allocated.
  virtual void add_to_included_idl_files (char* file_name);

  // Get all the files.
  virtual char** included_idl_files (void);

  // The number of currently availabe include files.
  virtual size_t n_included_idl_files (void);

  // Set the number of included_idl_files. Use this carefully. This
  // method is used when we validate all the #included idl files,
  // against the ones that we get after preprocessing.
  virtual void n_included_idl_files (size_t n);

  // Validate the included idl files, somefiles might have been
  // ignored by the preprocessor.
  virtual void validate_included_idl_files (void);

  virtual ParseState parse_state (void);    // What state we're in
  virtual void set_parse_state (ParseState s); // Set it

  // Convert from a predefined type to an expression type
  virtual AST_Expression::ExprType
  PredefinedTypeToExprType (AST_PredefinedType::PredefinedType);

  virtual UTL_String *idl_src_file (void);
  // Returns the IDL source file being compiled.

  virtual void idl_src_file (UTL_String *);
  // Set the source IDL file that is being parsed.

  virtual void temp_dir (const char *s);
  // Set the directory where the IDL compiler can keep all its temp
  // files. By default, IDL compiler looks for TEMP env variable and
  // if it is not set,  "/tmp/" is assigned.

  virtual const char *temp_dir (void) const;
  // Get the directory where the IDL compiler can keep all its temp
  // files. By default, IDL compiler looks for TEMP env variable and
  // if it is not set,  "/tmp/" is assigned.

  virtual void gperf_path (const char *s);
  // Set the path for the perfect hashing program (GPERF).

  virtual const char *gperf_path (void) const;
  // Get the path for the perfect hashing program (GPERF).

  virtual void ident_string (const char *s);
  // Set the value of the #ident string.

  virtual const char *ident_string (void) const;
  // Get the value of the #ident string.

  virtual void obv_support (idl_bool);
  // set enable/disable OBV (Valuetype) support

  virtual idl_bool obv_support (void);
  // check if OBV (Valuetype) support is enabled

  virtual void case_diff_error (idl_bool);
  // report an error (1) for indentifiers in the same scope
  // that differ only by case, or report a warning (0).

  virtual idl_bool case_diff_error (void);
  // are we strict about case-only differences or not?

  virtual void destroy (void);
  // Cleanup function.

  virtual void append_idl_flag (const char *s);
  // Save each flag passed to the IDL compiler.

  virtual const char *idl_flags (void) const;
  // Get a string representation of the flags passed to the idl compiler.

  ACE_Hash_Map_Manager<ACE_CString, int, ACE_Null_Mutex> &
  idl_keywords (void);
  // Accessor for the IDL keyword container.

  ACE_Unbounded_Stack<char *> & pragma_prefixes (void);
  // Accessor for the pragma prefix container.

  UTL_ScopedName *string_to_scoped_name (char *s);
  // Parses a string with double colons.

  long seen_include_file_before(UTL_String *);
  // Seen this include before?

  long last_seen_index (void) const;
  void last_seen_index (long val);
  // Accessors for last_seen_index_ member.

  idl_bool repeat_include (void) const;
  void repeat_include (idl_bool val);
  // Accessors for repeat_include_ member.

private:
  // Data
  UTL_ScopeStack             *pd_scopes;             // Store scopes stack
  AST_Root                   *pd_root;               // Store AST root
  AST_Generator              *pd_gen;                // Store generator
  UTL_Error                  *pd_err;                // Error object
  long                       pd_err_count;           // Count of errors
  long                       pd_lineno;              // What line #
  UTL_String                 *pd_filename;           // What file
  UTL_String                 *pd_main_filename;      // What main filename
  UTL_String                 *pd_real_filename;      // What real filename
  UTL_String                 *pd_stripped_filename;  // Stripped filename
  idl_bool                   pd_import;              // Is import on?
  idl_bool                   pd_in_main_file;        // Are we in it?
  const char                 *pd_prog_name;          // Argv[0]
  const char                 *pd_cpp_location;       // Where to find CPP
  long                       pd_compile_flags;       // Compile flags
  const char                 *pd_be;                 // BE name to use
  char                       *pd_local_escapes;      // Trapdoor argument
  UTL_Indenter               *pd_indent;             // Indent object
                                                     // as its being built
  idl_bool                   pd_read_from_stdin;     // Reading from stdin?
  UTL_String                 **pd_include_file_names;// Array of file names.
  unsigned long              pd_n_include_file_names;// How many.
  unsigned long              pd_n_alloced_file_names;// How many alloced.

  char **included_idl_files_;
  // IDL files that are "#include'd.

  size_t n_included_idl_files_;
  // The number of such idl files that are currently stored.

  size_t n_allocated_idl_files_;
  // For char*'s have been allocated for this.

  ParseState                pd_parse_state;         // Parse state we're in.

  UTL_String                *pd_idl_src_file;       // IDL source file.

  char *gperf_path_;
  // Path for the perfect hash generator(gperf) program. Default
  // is $ACE_ROOT/bin/gperf.

  char *temp_dir_;
  // Temp directory where which we can rewsolve in drv_preproc.cpp by
  // checking for  TEMP env variable otherwise we assign to /tmp/.

  char *ident_string_;
  // Holds a string that begins with #ident, to be passed from the IDL
  // file to the generated files.

  idl_bool obv_support_;
  // Do we support OBV (Valuetype)?

  idl_bool case_diff_error_;
  // Do we report an error for indentifiers in the same scope that differ
  // only by case? or just a warning?

  ACE_CString idl_flags_;
  // Concatenation of all the command line options.

  ACE_Hash_Map_Manager<ACE_CString, int, ACE_Null_Mutex> idl_keywords_;
  // Container for all the IDL keywords so local names can be checked.

  ACE_Unbounded_Stack<char *> pragma_prefixes_;
  // Container for all the #pragma prefix declarations.

  long last_seen_index_;
  // The index (not zero-based!) of the last seen included file.

  idl_bool repeat_include_;
  // Has this IDL file been included before?
};


#endif  //_IDL_IDL_GLOBAL_HH
