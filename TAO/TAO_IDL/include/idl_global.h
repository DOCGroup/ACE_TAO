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

// May of them could be forward declared..
#include "ace/SString.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Containers_T.h"
#include "ace/Synch_Traits.h"
#include "ace/Null_Mutex.h"

#include "ast_expression.h"
#include "ast_predefined_type.h"
#include "utl_stack.h"
#include "fe_utils.h"

class AST_Root;
class AST_Module;
class AST_Interface;
class AST_ValueType;
class AST_Component;
class AST_Uses;
class AST_Generator;
class UTL_Error;
class UTL_String;
class UTL_Indenter;

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
    , PS_TypeDeclSeen           // Seen complete type declaration
    , PS_TypeIdDeclSeen         // Seen complete typeId declaration
    , PS_TypePrefixDeclSeen     // Seen complete type_prefix declaration
    , PS_ConstDeclSeen          // Seen complete const declaration
    , PS_ExceptDeclSeen         // Seen complete exception declaration
    , PS_InterfaceDeclSeen      // Seen complete interface declaration
    , PS_ModuleDeclSeen         // Seen complete module declaration
    , PS_ValueTypeDeclSeen      // Seen complete valuetype declaration
    , PS_ComponentDeclSeen      // Seen complete component declaration
    , PS_HomeDeclSeen           // Seen complete home declaration
    , PS_EventDeclSeen          // Seen complete eventtype declartion
    , PS_PorttypeDeclSeen       // Seen complete porttype declaration
    , PS_ConnectorDeclSeen      // Seen complete connector declaration
    , PS_AttrDeclSeen           // Seen complete attribute declaration
    , PS_OpDeclSeen             // Seen complete operation declaration
    , PS_ProvidesDeclSeen       // Seen complete privides declaration
    , PS_ExtProvidesDeclSeen    // Seen complete extended provides declaration
    , PS_UsesDeclSeen           // Seen complete uses declaration
    , PS_ExtUsesDeclSeen        // Seen complete extended uses declaration
    , PS_EmitsDeclSeen          // Seen complete emits declaration
    , PS_PublishesDeclSeen      // Seen complete publishes declaration
    , PS_FactoryDeclSeen        // Seen complete factory declaration
    , PS_FinderDeclSeen         // Seen complete finder declaration
    , PS_ConsumesDeclSeen       // Seen complete subscribes declaration
    , PS_ExtendedPortDeclSeen   // Seen extended port
    , PS_MirrorPortDeclSeen     // Seen mirror port
    , PS_ModuleSeen             // Seen a MODULE keyword
    , PS_ModuleIDSeen           // Seen the module ID
    , PS_ModuleSqSeen           // '{' seen for module
    , PS_ModuleQsSeen           // '}' seen for module
    , PS_ModuleBodySeen         // Seen complete module body
    , PS_InheritColonSeen       // Seen ':' in inheritance list
    , PS_InheritSpecSeen        // Seen a complete inheritance spec
    , PS_SupportSpecSeen        // Seen a complete supports spec
    , PS_ManagesSeen            // Seen a MANAGES keyword
    , PS_ManagesIDSeen          // Seen the scoped name referred to by MANAGES
    , PS_PrimaryKeySpecSeen     // Seen a complete primary key spec
    , PS_InterfaceSeen          // Seen an INTERFACE keyword
    , PS_InterfaceIDSeen        // Seen the interface ID
    , PS_InterfaceForwardSeen   // Forward interface decl seen
    , PS_InterfaceSqSeen        // '{' seen for interface
    , PS_InterfaceQsSeen        // '}' seen for interface
    , PS_InterfaceBodySeen      // Seen an interface body
    , PS_TmplModuleIDSeen       // Seen the template module ID
    , PS_TmplModuleParamsSeen   // Seen template module params
    , PS_TmplModuleSqSeen       // '{' seen for template module
    , PS_TmplModuleQsSeen       // '}' seen for template module
    , PS_TmplModuleBodySeen     // Seen a template module body
    , PS_InstModuleSeen         // Seen MODULE keyword + reference
    , PS_InstModuleArgsSeen     // Seen template args
    , PS_InstModuleIDSeen       // Seen instantiated module ID
    , PS_ModuleRefSeen          // Seen ALIAS keyword + reference
    , PS_ModuleRefParamsSeen    // Seen the subset of params used
    , PS_ModuleRefIDSeen        // Seen referenced module ID
    , PS_ValueTypeSeen          // Seen a VALUETYPE keyword
    , PS_ValueTypeForwardSeen   // Forward valuetype decl seen
    , PS_ValueTypeIDSeen        // Seen the valuetype ID
    , PS_ValueTypeSqSeen        // '{' seen for value type
    , PS_ValueTypeQsSeen        // '}' seen for value type
    , PS_ValueTypeBodySeen      // Seen a value type body
    , PS_EventTypeSeen          // Seen a EVENTTYPE keyword
    , PS_EventTypeForwardSeen   // Forward eventtype decl seen
    , PS_EventTypeIDSeen        // Seen the eventtype ID
    , PS_EventTypeSqSeen        // '{' seen for event type
    , PS_EventTypeQsSeen        // '}' seen for event type
    , PS_EventTypeBodySeen      // Seen a event type body
    , PS_ComponentSeen          // Seen a component declaration
    , PS_ComponentIDSeen        // Seen the component ID
    , PS_ComponentForwardSeen   // Seen a forward declaration of a component
    , PS_ComponentSqSeen        // '{' seen for component
    , PS_ComponentQsSeen        // '}' seen for component
    , PS_ComponentBodySeen      // Seen a component body
    , PS_HomeSeen               // Seen a home declaration
    , PS_HomeIDSeen             // Seen the home ID
    , PS_HomeSqSeen             // '{' seen for home
    , PS_HomeQsSeen             // '}' seen for home
    , PS_HomeBodySeen           // Seen a home body
    , PS_ConnectorSeen          // Seen an CONNECTOR keyword
    , PS_ConnectorIDSeen        // Seen the connector ID
    , PS_ConnectorSqSeen        // '{' seen for connector
    , PS_ConnectorQsSeen        // '}' seen for connector
    , PS_ConnectorBodySeen      // Seen a connector body
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
    , PS_StructForwardSeen      // Forward struct decl seen
    , PS_MemberTypeSeen         // Seen type of struct or except member
    , PS_MemberDeclsSeen        // Seen decls of struct or except members
    , PS_MemberDeclsCompleted   // Completed one struct or except member to ';'
    , PS_UnionSeen              // Seen a UNION keyword
    , PS_UnionIDSeen            // Seen the union ID
    , PS_UnionForwardSeen       // Forward union decl seen
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
    , PS_OpGetRaiseCompleted    // Completed attribute get except list
    , PS_OpSetRaiseCompleted    // Completed attribute set except list
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
    , PS_OpGetRaiseSeen         // Seen GETRAISES keyword
    , PS_OpGetRaiseSqSeen       // Seen '(' for GETRAISES
    , PS_OpGetRaiseQsSeen       // Seen ')' for GETRAISES
    , PS_OpSetRaiseSeen         // Seen SETRAISES keyword
    , PS_OpSetRaiseSqSeen       // Seen '(' for SETRAISES
    , PS_OpSetRaiseQsSeen       // Seen ')' for SETRAISES
    , PS_OpContextSeen          // Seen CONTEXT keyword
    , PS_OpContextSqSeen        // Seen '(' for CONTEXT
    , PS_OpContextQsSeen        // Seen ')' for CONTEXT
    , PS_OpContextCommaSeen     // Seen ',' for CONTEXT
    , PS_DeclsCommaSeen         // Seen ',' in declarators list
    , PS_DeclsDeclSeen          // Seen complete decl in decls list
    , PS_NativeSeen             // Seen a native declaration
    , PS_PragmaPrefixSyntax     // Could not parse the #pragma prefix
    , PS_ValueBoxDeclSeen       // Seen complete valuebox declaration
    , PS_PorttypeSeen           // Seen PORTTYPE keyword
    , PS_PorttypeIDSeen         // Seen porttype indentifier
    , PS_PorttypeSqSeen         // Seen '{' for porttype
    , PS_PorttypeQsSeen         // Seen '}' for porttype
    , PS_PorttypeBodySeen       // Seen complete porttype body
  };

  // flags for types of declarations seen while parsing.
  bool abstract_iface_seen_;
  bool abstractbase_seen_;
  bool aggregate_seen_;
  bool ambiguous_type_seen_;
  bool any_arg_seen_;
  bool any_seen_;
  bool any_seq_seen_;
  bool array_seen_;
  bool array_seq_seen_;
  bool base_object_seen_;
  bool basic_arg_seen_;
  bool basic_type_seen_;
  bool bd_string_arg_seen_;
  bool boolean_seq_seen_;
  bool char_seq_seen_;
  bool component_seen_;
  bool connector_seen_;
  bool double_seq_seen_;
  bool enum_seen_;
  bool exception_seen_;
  bool fixed_array_arg_seen_;
  bool fixed_size_arg_seen_;
  bool float_seq_seen_;
  bool fwd_iface_seen_;
  bool fwd_valuetype_seen_;
  bool iface_seq_seen_;
  bool interface_seen_;
  bool local_iface_seen_;
  bool long_seq_seen_;
  bool longdouble_seq_seen_;
  bool longlong_seq_seen_;
  bool non_local_iface_seen_;
  bool non_local_op_seen_;
  bool object_arg_seen_;
  bool octet_seq_seen_;
  bool operation_seen_;
  bool pseudo_seq_seen_;
  bool recursive_type_seen_;
  bool seq_seen_;
  bool short_seq_seen_;
  bool special_basic_arg_seen_;
  bool string_seen_;
  bool string_member_seen_;
  bool string_seq_seen_;
  bool typecode_seen_;
  bool ub_string_arg_seen_;
  bool ulong_seq_seen_;
  bool ulonglong_seq_seen_;
  bool union_seen_;
  bool ushort_seq_seen_;
  bool valuebase_seen_;
  bool valuefactory_seen_;
  bool valuetype_seen_;
  bool var_array_arg_seen_;
  bool var_size_arg_seen_;
  bool vt_seq_seen_;
  bool wchar_seq_seen_;
  bool wstring_seq_seen_;
  bool dds_connector_seen_;
  bool ami_connector_seen_;

  // flag to force generation of skeleton includes (see bug #2419).
  bool need_skeleton_includes_;

  IDL_GlobalData (void);
  ~IDL_GlobalData (void);

  // Operations
  UTL_ScopeStack   &scopes (void);              // Scopes stack

  AST_Root         *root (void);                // Root of AST
  void             set_root (AST_Root *);       // Set it

  AST_Generator    *gen (void);                 // Generator
  void             set_gen (AST_Generator *);   // Set it

  AST_ValueType    *primary_key_base (void);    // PrimaryKeyBase
  void             primary_key_base (AST_ValueType *);   // Set it

  UTL_Error        *err (void);                 // Error reporter
  void             set_err (UTL_Error *);       // Set it

  int              err_count (void);            // How many errors?
  void             set_err_count (int);         // Set it

  long             lineno (void);               // Where in file?
  void             set_lineno (long);           // Set it

  UTL_String       *filename (void);            // What file?
  void             set_filename (UTL_String *); // Set it

  UTL_String       *main_filename (void);       // What's the main
                                                // file name?
  void             set_main_filename (UTL_String *);
                                                // Set it

  UTL_String       *real_filename (void);       // What's the real
                                                // file name?
  void             set_real_filename (UTL_String *);
                                                // Set it

  UTL_String       *stripped_filename (void);   // Stripped filename
  void             set_stripped_filename (UTL_String *);  // Set it

  bool             imported (void);             // Are we imported?
  bool             import (void);               // Is import on?
  void             set_import (bool);           // Set it

  bool             in_main_file (void);         // Are we?
  void             set_in_main_file (bool);     // Set it

  const char       *prog_name (void);           // Invoked as..
  void             set_prog_name (const char *);  // Set it

  const char       *cpp_location (void);        // Where's CPP?
  void             set_cpp_location (const char *);// Set it

  long             compile_flags (void);        // What flags are on?
  void             set_compile_flags (long);    // Turn some on or off

  char             *local_escapes (void);       // Get local escapes
  void             set_local_escapes (const char *);// Set it

  UTL_Indenter     *indent (void);              // Get indenter
  void             set_indent (UTL_Indenter *); // Set it

  void             store_include_file_name (UTL_String *);

  UTL_String       **include_file_names (void); // Array of file names
  void             set_include_file_names (UTL_String **); // Set it

  unsigned long    n_include_file_names (void); // How many
  void             set_n_include_file_names (unsigned long n);

  void             reset_flag_seen (void);

  // = Types & methods supporting DDS DCPS data type/key definition (from #pragma)
  typedef ACE_Unbounded_Queue<ACE_TString> DCPS_Key_List;

  struct DCPS_Data_Type_Info
  {
    UTL_ScopedName *name_;
    DCPS_Key_List   key_list_;
  };

  typedef ACE_Unbounded_Queue_Iterator<ACE_TString> DCPS_Data_Type_Info_Iter;

  typedef ACE_Hash_Map_Manager_Ex< const char*,
                                   DCPS_Data_Type_Info*,
                                   ACE_Hash<char*>,
                                   ACE_Equal_To<char*>,
                                   ACE_Null_Mutex>
    DCPS_Type_Info_Map ;

  // FE calls when #pragma DCPS_DATA_TYPE is processed
  void add_dcps_data_type (const char* id);

  // FE calls when #pragma DCPS_DATA_KEY is processed
  bool add_dcps_data_key (const char* id, const char* key);

  // returns null if not matching; otherwise pointer to the info
  DCPS_Data_Type_Info* is_dcps_type (UTL_ScopedName* target);

  // FE calls when #pragma DCPS_SUPPORT_ZERO_COPY_READ is processed
  void dcps_support_zero_copy_read (bool value);

  // BE calls to check the status of zero-copy read support
  bool dcps_support_zero_copy_read (void) const;

  // FE calls when #pragma DCPS_GEN_ZERO_COPY_READ is processed
  void dcps_gen_zero_copy_read (bool value);

  // BE calls to check the status of zero-copy read support
  bool dcps_gen_zero_copy_read (void) const;

  // = Access methods to deal with other IDL files included in the main
  //   IDL file. These IDL files are exactly the same strings that are
  //   "#include"d in the main IDL file, not the ones after CC
  //   preprocessor parsed the file.

  // Just storing the pointer. No memory will be allocated.
  void add_to_included_idl_files (const char *file_name);

  // Get all the files.
  char** included_idl_files (void);

  // The number of currently availabe include files.
  size_t n_included_idl_files (void);

  // Set the number of included_idl_files. Use this carefully. This
  // method is used when we validate all the #included idl files,
  // against the ones that we get after preprocessing.
  void n_included_idl_files (size_t n);

  ParseState parse_state (void);       // What state we're in
  void set_parse_state (ParseState s); // Set it

  UTL_String *idl_src_file (void);
  // Returns the IDL source file being compiled.

  void idl_src_file (UTL_String *);
  // Set the source IDL file that is being parsed.

  void temp_dir (const char *s);
  // Set the directory where the IDL compiler can keep all its temp
  // files. By default, IDL compiler looks for TEMP env variable and
  // if it is not set,  "/tmp/" is assigned.

  const char *temp_dir (void) const;
  // Get the directory where the IDL compiler can keep all its temp
  // files. By default, IDL compiler looks for TEMP env variable and
  // if it is not set,  "/tmp/" is assigned.

  void tao_root (const char *s);
  // Set the path of TAO_ROOT.

  const char *tao_root (void) const;
  // Get the path for TAO_ROOT.

  void gperf_path (const char *s);
  // Set the path for the perfect hashing program (GPERF).

  const char *gperf_path (void) const;
  // Get the path for the perfect hashing program (GPERF).

  void ident_string (const char *s);
  // Set the value of the #ident string.

  const char *ident_string (void) const;
  // Get the value of the #ident string.

  void case_diff_error (bool);
  // report an error (1) for indentifiers in the same scope
  // that differ only by case, or report a warning (0).

  bool case_diff_error (void);
  // are we strict about case-only differences or not?

  void nest_orb (bool);
  // Set on or off whether we are using the NEST ORB.

  bool nest_orb (void);
  // are we beIng used with the NEST ORB?

  void destroy (void);
  // Cleanup function.

  void append_idl_flag (const char *s);
  // Save each flag passed to the IDL compiler.

  const char *idl_flags (void) const;
  // Get a string representation of the flags
  // passed to the idl compiler.

  ACE_Hash_Map_Manager<ACE_CString, int, ACE_Null_Mutex> &
  idl_keywords (void);
  // Accessor for the IDL keyword container.

  ACE_Unbounded_Stack<char *> & pragma_prefixes (void);
  // Accessor for the pragma prefix container.

  void update_prefix (char *filename);
  // Do repo id prefix bookkeeping when the file changes.

  long seen_include_file_before (char *);
  // Seen this include before?

  long last_seen_index (void) const;
  void last_seen_index (long val);
  // Accessors for last_seen_index_ member.

  bool repeat_include (void) const;
  void repeat_include (bool val);
  // Accessors for repeat_include_ member.

  bool preserve_cpp_keywords (void);
  // Whether we should not mung idl element names that are
  // C++ keywords e.g. delete, operator etc. with _cxx_ prefix.
  // Should be true when being used by the IFR Service

  void preserve_cpp_keywords (bool);
  // Set whether we should not mung idl element names that are C++
  // keywords e.g. delete, operator etc. with _cxx_ prefix.
  // Is set by the IFR Service.

  void add_include_path (const char *s, bool is_system);
  // Add another path to 'include_paths_'. is_system is true for TAO/CIAO
  // specific paths and false for paths provided by user.

  void add_rel_include_path (const char *s);
  ACE_Unbounded_Queue<char *> const & rel_include_paths (void) const;
  // Accessor/mutator for the rel_include_paths_ member.

  void add_ciao_lem_file_names (const char *s);
  ACE_Unbounded_Queue<char *> & ciao_lem_file_names (void);
  // Accessor/mutator for the ciao_lem_file_names_ member.

  void add_ciao_rti_ts_file_names (const char *s);
  ACE_Unbounded_Queue<char *> & ciao_rti_ts_file_names (void);
  // Accessor/mutator for the ciao_rti_ts_file_names_ member.

  void add_ciao_spl_ts_file_names (const char *s);
  ACE_Unbounded_Queue<char *> & ciao_spl_ts_file_names (void);
  // Accessor/mutator for the ciao_spl_ts_file_names_ member.

  void add_ciao_oci_ts_file_names (const char *s);
  ACE_Unbounded_Queue<char *> & ciao_oci_ts_file_names (void);
  // Accessor/mutator for the ciao_oci_ts_file_names_ member.

  void add_ciao_ami_iface_names (const char *s);
  ACE_Unbounded_Queue<char *> & ciao_ami_iface_names (void);
  // Accessor/mutator for the ciao_ami_iface_names_ member.

  void add_ciao_ami_recep_names (const char *s);
  ACE_Unbounded_Queue<char *> & ciao_ami_recep_names (void);
  // Accessor/mutator for the ciao_ami_recep_names_ member.

  void add_included_ami_recep_names (const char *s);
  ACE_Unbounded_Queue<char *> & included_ami_recep_names (void);
  // Accessor/mutator for the included_ami_recep_names_ member.

  bool included_ami_receps_done (void) const;
  void included_ami_receps_done (bool val);

  void add_ciao_ami_idl_fnames (const char *s);
  ACE_Unbounded_Queue<char *> & ciao_ami_idl_fnames (void);
  // Accessor/mutator for the included_ami_idl_fnames_ member.

  void add_dds4ccm_impl_fnames (const char *s);
  ACE_Unbounded_Queue<char *> & dds4ccm_impl_fnames (void);
  // Accessor mutator for the dds4ccm_impl_fnames_ member.

  ACE_Unbounded_Queue<AST_Decl *> & masking_scopes (void);
  // Accessor for the member

  ACE_Unbounded_Queue<AST_Interface *> & mixed_parentage_interfaces (void);
  // Accessor for the member

  ACE_Hash_Map_Manager<char *, char *, ACE_Null_Mutex> &
  file_prefixes (void);
  // Accessor for the IDL file prefix container.

  bool pass_orb_idl (void) const;
  void pass_orb_idl (bool val);
  // Accessor for the pass_orb_idl_ member.

  bool using_ifr_backend (void) const;
  void using_ifr_backend (bool val);
  // Accessor for the using_ifr_backend_ member.

  bool ignore_idl3 (void) const;
  void ignore_idl3 (bool val);
  // Accessor for the ignore_idl3_ member.

  int check_gperf (void);
  // Currently called only from IDL backend, but could be useful elsewhere.

  void fini (void);
  // Do final cleanup just before process exits.

  ACE_Unbounded_Queue<AST_ValueType *> &primary_keys (void);
  // Accessor for the member.

  void check_primary_keys (void);
  // Called affer yy_parse() returns - iterates over our list
  // of primary keys. Must be called this late so that we can
  // be sure that all forward declared stucts or unions that
  // might be used in such a valuetype are fully defined.

  const char *recursion_start (void) const;
  void recursion_start (const char *val);
  // Accessors for the member.

  bool multi_file_input (void) const;
  void multi_file_input (bool val);
  // Accessors for the member.

  const char *big_file_name (void) const;
  // Just get the const string.

  FE_Utils::T_PARAMLIST_INFO const *current_params (void) const;
  void current_params (FE_Utils::T_PARAMLIST_INFO *params);
  // Accessors for the member. If UTL_Scope::lookup_by_name()
  // has a 0 result, it will check this param list (if it is not
  // 0, meaning we are in the scope of a template module) for a
  // match before returning, in case it is processing a
  // referenced template parameter of the eclosing template
  // module.

#if defined (ACE_OPENVMS)
  static char* translateName (const char* name, char *name_buf);
#endif

  AST_Module *corba_module (void) const;
  void corba_module (AST_Module *m);
  // Accessors for the member.

  struct Include_Path_Info
  {
    char *path_;
    bool is_system_;
  };

  typedef ACE_Unbounded_Queue<Include_Path_Info>
    Unbounded_Paths_Queue;
  typedef ACE_Unbounded_Queue_Iterator<Include_Path_Info>
    Unbounded_Paths_Queue_Iterator;

  Unbounded_Paths_Queue &include_paths (void);
  // Accessor for the member.

  enum ANON_TYPE_DIAGNOSTIC
  {
    ANON_TYPE_ERROR,
    ANON_TYPE_WARNING,
    ANON_TYPE_SILENT
  };
  // Determines which type of diagnostic to emit, if any, upon
  // detecting the presence of an anonymous IDL construct.

  void anon_type_diagnostic (ANON_TYPE_DIAGNOSTIC val);
  // Mutator for the member.

  bool anon_error (void) const;
  bool anon_warning (void) const;
  bool anon_silent (void) const;
  // Simple checks for the value of anon_type_diagnostic_.

  bool in_typedef (void) const;
  void in_typedef (bool val);
  // Accessors for the member.

private:
  // Data
  UTL_ScopeStack             pd_scopes;              // Store scopes stack
  AST_Root                   *pd_root;               // Store AST root
  AST_Generator              *pd_gen;                // Store generator
  AST_ValueType              *pd_primary_key_base;   // Store PrimaryKeyBase
  UTL_Error                  *pd_err;                // Error object
  int                        pd_err_count;           // Count of errors
  long                       pd_lineno;              // What line #
  UTL_String                 *pd_filename;           // What file
  UTL_String                 *pd_main_filename;      // What main filename
  UTL_String                 *pd_real_filename;      // What real filename
  UTL_String                 *pd_stripped_filename;  // Stripped filename
  bool                       pd_import;              // Is import on?
  bool                       pd_in_main_file;        // Are we in it?
  const char                 *pd_prog_name;          // Argv[0]
  const char                 *pd_cpp_location;       // Where to find CPP
  long                       pd_compile_flags;       // Compile flags
  char                       *pd_local_escapes;      // Trapdoor argument
  UTL_Indenter               *pd_indent;             // Indent object
                                                     // as it's being built
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

  char *tao_root_;
  // Needed if orb.idl is included.

  char *gperf_path_;
  // Path for the perfect hash generator(gperf) program. Default
  // is $ACE_ROOT/bin/gperf.

  char *temp_dir_;
  // Temp directory where which we can rewsolve in drv_preproc.cpp by
  // checking for  TEMP env variable otherwise we assign to /tmp/.

  char *ident_string_;
  // Holds a string that begins with #ident, to be passed from the IDL
  // file to the generated files.

  bool case_diff_error_;
  // Do we report an error for indentifiers in the same scope that differ
  // only by case? or just a warning?

  bool nest_orb_;
  // Is this front end being used for the NEST ORB?

  ACE_CString idl_flags_;
  // Concatenation of all the command line options.

  ACE_Hash_Map_Manager<ACE_CString, int, ACE_Null_Mutex> idl_keywords_;
  // Container for all the IDL keywords so local names can be checked.

  ACE_Unbounded_Stack<char *> pragma_prefixes_;
  // Container for all the #pragma prefix declarations.

  bool repeat_include_;
  // Has this IDL file been included before?

  bool preserve_cpp_keywords_;
  // Do we allow C++ keywords as identifiers in the idl to stay as they are ?

  Unbounded_Paths_Queue include_paths_;
  // List of -I options passed to us.

  ACE_Unbounded_Queue<char *> rel_include_paths_;
  // Used by backends with the -r option.

  ACE_Hash_Map_Manager<char *, char *, ACE_Null_Mutex> file_prefixes_;
  // Associates a prefix with a file.

  bool pass_orb_idl_;
  // Treat orb.idl like any other included IDL file.

  bool using_ifr_backend_;
  // Set by the IFR backend itself.

  bool ignore_idl3_;
  // Need this for eventtypes left over after running idl3_to_idl2,
  // we don't want to try to generate another event consumer.
  DCPS_Type_Info_Map dcps_type_info_map_ ;
  // Map of #pragma DCPS_DATA_TYPE and DCPS_DATA_KEY infomation.

  bool dcps_support_zero_copy_read_;
  // Are we enabled to support DCPS zero-copy read.
  // Need this flag to avoid generating wrong code for pre v0.12 DDS
  // with new TAO_IDL compiler.

  bool dcps_gen_zero_copy_read_;
  // Are we generating code insupport of DCPS zero-copy read data sequences?

  ACE_Unbounded_Queue<AST_ValueType *>primary_keys_;
  // List of valuetypes used as a primary key.

  char *recursion_start_;
  // Path to directory subtree we are iterating/recursing over.
  // Not used by all backends.

  bool multi_file_input_;
  // Some backends process multiple files to a single output.

  const char *big_file_name_;
  // Used if the above flag is set.

  FE_Utils::T_PARAMLIST_INFO *current_params_;
  // Stored if we are parsing the scope of a template module,
  // 0 otherwise.

  ACE_Unbounded_Queue<char *> ciao_lem_file_names_;
  // Files parsed with ciao lem pragma

  ACE_Unbounded_Queue<char *> ciao_rti_ts_file_names_;
  // Files parsed with RTI typesupport pragma

  ACE_Unbounded_Queue<char *> ciao_spl_ts_file_names_;
  // Files parsed with OpenSplice typesupport pragma

  ACE_Unbounded_Queue<char *> ciao_oci_ts_file_names_;
  // Files parsed with OpenDDS typesupport pragma

  ACE_Unbounded_Queue<char *> ciao_ami_iface_names_;
  // Interfaces that get a reply handler generated for AMI4CCM.

  ACE_Unbounded_Queue<char *> ciao_ami_recep_names_;
  // Receptacles that get a sendc_ version added for AMI4CCM.

  ACE_Unbounded_Queue<char *> included_ami_recep_names_;
  // We need to do something different with these...

  bool included_ami_receps_done_;
  // ...but we need to do it only once.

  ACE_Unbounded_Queue<char *> ciao_ami_idl_fnames_;
  // Stores directives from #pragma ciao ami4ccm idl "xxx".

  ACE_Unbounded_Queue<char *> dds4ccm_impl_fnames_;
  // Stores directives from #pragma dds4ccm impl "xxx".

  ACE_Unbounded_Queue<AST_Decl *> masking_scopes_;
  // Used to check for an incorrect lookup success that should
  // have been masked by an inner scope of the same name.

  /**
   * Used in the generation of overrides for CORBA::release and
   * CORBA::is_nil, needed when the interface inherits versions from
   * both CORBA::Object and CORBA::AbstractBase.
   */
  ACE_Unbounded_Queue<AST_Interface *> mixed_parentage_interfaces_;

  /// For quick access.
  AST_Module *corba_module_;

  ANON_TYPE_DIAGNOSTIC anon_type_diagnostic_;

  /// Flag set in parser so we can decide whether to emit
  /// an anonymous type diagnostic.
  bool in_typedef_;
};

#endif  //_IDL_IDL_GLOBAL_HH
