// -*- C++ -*-

// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_global.h
//
// = DESCRIPTION
//    Header file for class containing compiler back end global data.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef _BE_GLOBAL_H
#define _BE_GLOBAL_H

#include "TAO_IDL_BE_Export.h"

#include "ace/Unbounded_Queue.h"
#include "ace/SString.h"

class be_interface;
class be_valuetype;
class be_module;
class be_interface;
class be_interface_fwd;
class UTL_String;
class AST_PredefinedType;
class AST_Generator;

// Defines a class containing all back end global data.

class TAO_IDL_BE_Export BE_GlobalData
{
public:
  // = TITLE
  //    BE_GlobalData
  //
  // = DESCRIPTION
  //    Storage of global data specific to the compiler back end
  //
  enum LOOKUP_STRATEGY
  {
    // various lookup strategies
    TAO_LINEAR_SEARCH,
    TAO_DYNAMIC_HASH,
    TAO_PERFECT_HASH,
    TAO_BINARY_SEARCH
  };

  // Constructor
  BE_GlobalData (void);
  // Destructor
  virtual ~BE_GlobalData (void);

  // To switch between changing or non-changing standard include
  // files (e.g. tao/corba.h)  so that #include statements can be
  // generated with ""s or <>s respectively.
  virtual void changing_standard_include_files (size_t changing);
  virtual size_t changing_standard_include_files (void);

  // Helper functions that generate the file names for the C++ mapping
  // generated code.
  // The parameter <base_name_only> set to 0 (no base name, but full
  // name with output dir path, is useful, when I just want just the
  // base name to use in #include's etc.
  static const char *be_get_client_hdr_fname (
      bool base_name_only = 0
    );
  static const char *be_get_client_stub_fname (void);
  static const char *be_get_client_inline_fname (
      bool base_name_only = 0
    );
  static const char *be_get_server_hdr_fname (
      bool base_name_only = 0
    );
  static const char *be_get_implementation_hdr_fname (
      bool base_name_only = 0
    );
  static const char *be_get_implementation_skel_fname (
      bool base_name_only = 0
    );
  static const char *be_get_server_template_hdr_fname (
      bool base_name_only = 0
    );
  static const char *be_get_server_skeleton_fname (void);
  static const char *be_get_implementation_skeleton_fname (void);
  static const char *be_get_server_template_skeleton_fname (
      bool base_name_only = 0
    );
  static const char *be_get_server_inline_fname (
      bool base_name_only = 0
    );
  static const char *be_get_anyop_source_fname (
      bool base_name_only = 0
    );
  static const char *be_get_anyop_header_fname (
      bool base_name_only = 0
    );

  // Helper functions: obtain the names of each generated file given
  // the IDL file name.
  // The parameter <base_name_only> set to 0 (no base name, but full
  // name with output dir path, is useful, when I want just the
  // base name to use in #include's etc.
  static const char *be_get_client_hdr (UTL_String *idl_file_name,
                                        bool base_name_only = 0);
  static const char *be_get_client_stub (UTL_String *idl_file_name);
  static const char *be_get_client_inline (UTL_String *idl_file_name,
                                           bool base_name_only = 0);
  static const char *be_get_server_hdr (UTL_String *idl_file_name,
                                        bool base_name_only = 0);
  static const char *be_get_implementation_hdr (UTL_String *idl_file_name,
                                                bool base_name_only = 0);
  static const char *be_get_implementation_skel (UTL_String *idl_file_name,
                                                 bool base_name_only = 0);
  static const char *be_get_server_template_hdr (UTL_String *idl_file_name,
                                                 bool base_name_only = 0);
  static const char *be_get_server_skeleton (UTL_String *idl_file_name);
  static const char *be_get_server_template_skeleton (
      UTL_String *idl_file_name,
      bool base_name_only = 0
    );
  static const char *be_get_server_inline (UTL_String *idl_file_name,
                                           bool base_name_only = 0);
  static const char *be_get_anyop_header (UTL_String *idl_file_name,
                                          bool base_name_only = 0);
  static const char *be_get_anyop_source (UTL_String *idl_file_name,
                                          bool base_name_only = 0);

  const char* skel_export_macro (void) const;
  // returns the macro name for exporting server side classes in Win32
  // DLL.

  void skel_export_macro (const char* s);
  // set the macro name for export server side classes in Win32 DLL.

  const char* skel_export_include (void) const;
  // returns the name of the include file that contains the server
  // side export macro definition.

  void skel_export_include (const char* s);
  // set the name of the include file that contains the server side
  // export macro definition.

  const char* stub_export_macro (void) const;
  // returns the macro name for exporting client side classes in Win32
  // DLL.

  void stub_export_macro (const char* s);
  // set the macro name for export client side classes in Win32 DLL.

  const char* stub_export_include (void) const;
  // returns the name of the include file that contains the client
  // side export macro definition.

  void stub_export_include (const char* s);
  // set the name of the include file that contains the client side
  // export macro definition.

  const char* anyop_export_macro (void) const;
  // returns the macro name for exporting client side classes in Win32
  // DLL.

  void anyop_export_macro (const char* s);
  // set the macro name for export client side classes in Win32 DLL.

  const char* anyop_export_include (void) const;
  // returns the name of the include file that contains the client
  // side export macro definition.

  void anyop_export_include (const char* s);
  // set the name of the include file that contains the client side
  // export macro definition.

  const char* pch_include (void) const;
  // returns the name of the include file to be used for precompiled
  // header support.

  void pch_include (const char* s);
  // set the name of the include file to be used for precompiled
  // header support.

  const char* pre_include (void) const;
  // returns the name of the include file to be put at the top of
  // every header file.

  void pre_include (const char* s);
  // set the name of the include file to be put at the top of every
  // header file.

  const char* post_include (void) const;
  // returns the name of the include file to be put at the bottom of
  // every header file.

  void post_include (const char* s);
  // set the name of the include file to be put at the bottom of every
  // header file.

  const char* include_guard (void) const;
  // returns the guard that is placed in the client header file

  void include_guard (const char* s);
  // set the guard that is placed in the client header file

  const char* safe_include (void) const;
  // returns the name of the include file that is used instead of the own
  // generated client header file

  void safe_include (const char* s);
  // set the name of the include file that is used instead of the own
  // generated client header file

  const char* unique_include (void) const;
  // returns the name of the include file that should on get
  // generated in the client header file

  void unique_include (const char* s);
  // set the name of the include file that should on get
  // generated in the client header file

  /// Set text that opens a "versioned" namespace.
  void versioning_begin (const char* s);

  /// Get text that opens a "versioned" namespace.
  const char * versioning_begin (void) const;

  /// Get text that opens a "versioned" namespace for core TAO/orbsvcs
  /// related code.
  const char * core_versioning_begin (void) const;

  /// Set text that closes a "versioned" namespace.
  void versioning_end (const char* s);

  /// Get text that closes a "versioned" namespace.
  const char * versioning_end (void) const;

  /// Get text that closes a "versioned" namespace for core TAO/orbsvcs
  /// related code.
  const char * core_versioning_end (void) const;

  // = Set and get methods for different file name endings.

  void client_hdr_ending (const char* s);
  // Set the client_hdr_ending.

  const char* client_hdr_ending (void) const;
  // Get the client_hdr_ending.

  void client_inline_ending (const char* s);
  // Set the client_inline_ending.

  const char* client_inline_ending (void) const;
  // Get the client_inline_ending.

  void  client_stub_ending (const char* s);
  // Set the client_stub_ending.

  const char* client_stub_ending (void) const;
  // Get the client_stub_ending.

  void server_hdr_ending (const char* s);
  // Set the server_hdr_ending.

  const char* server_hdr_ending (void) const;
  // Get the server_hdr_ending.

  void implementation_hdr_ending (const char* s);
  // Set the implementation_hdr_ending.

  void impl_class_prefix (const char* s);
  // Set the implementation class prefix.

  void impl_class_suffix (const char* s);
  // Set the implementation class suffix.

  void implementation_skel_ending (const char* s);
  // Set the implementation_skel_ending.

  const char* implementation_hdr_ending (void) const;
  // Get the implementation_hdr_ending.

  const char* impl_class_prefix (void) const;
  //Get implementation class prefix

  const char* impl_class_suffix (void) const;
  //Get implementation class suffix

  const char* implementation_skel_ending (void) const;
  // Get the implementation_skel_ending.

  void server_template_hdr_ending (const char* s);
  // Set the server_template_hdr_ending.

  const char* server_template_hdr_ending (void) const;
  // Get the server_template_hdr_ending.

  void server_skeleton_ending (const char* s);
  // Set the server_skeleton_ending.

  const char* server_skeleton_ending (void) const;
  // Get the server_skeleton_ending.

  void server_template_skeleton_ending (const char* s);
  // Set the server_template_skeleton_ending.

  const char* server_template_skeleton_ending (void) const;
  // Get the server_template_skeleton_ending.

  void server_inline_ending (const char* s);
  // Set the server_inline_ending.

  const char* server_inline_ending (void) const;
  // Get the server_inline_ending.

  void anyop_header_ending (const char* s);
  // Set the anyop_header_ending.

  void use_clonable_in_args (bool clonable);
  // Set the clonable_in_args.

  bool use_clonable_in_args (void) const;
  // Get the clonable_in_args setting.

  bool gen_template_export (void) const;
  // Get the gen_seq_template_export_ member.

  void gen_template_export (bool val);
  // Set the gen_seq_template_export_ member.

  bool gen_ostream_operators (void) const;
  // Get the gen_ostream_operators_ member.

  void gen_ostream_operators (bool val);
  // Set the gen_ostream_operators_ member.

  const char* anyop_header_ending (void) const;
  // Get the anyop_header_ending.

  void anyop_source_ending (const char* s);
  // Set the anyop_source_ending.

  const char* anyop_source_ending (void) const;
  // Get the anyop_source_ending.

  void output_dir (const char* s);
  // Set the directory where all the IDL-Compiler-Generated files are
  // to be kept. Default is current directory from which the
  // <tao_idl> is called.

  const char* output_dir (void) const;
  // Get the directory where all the IDL-Compiler-Generated files are
  // to be kept. Default is current directory from which the
  // <tao_idl> is called.

  void skel_output_dir (const char* s);
  // Set the directory where all the *S.* files are
  // to be kept. Default is output_dir_.

  const char* skel_output_dir (void) const;
  // Get the directory where all the *S.* files are
  // to be kept. Default is output_dir_.

  void anyop_output_dir (const char* s);
  // Set the directory where all the *A.* files are
  // to be kept. Default is output_dir_.

  const char* anyop_output_dir (void) const;
  // Get the directory where all the *A.* files are
  // to be kept. Default is output_dir_.

  void any_support (bool);
  // Set any support.

  bool any_support (void) const;
  // Check Any support.

  void tc_support (bool);
  // Set TypeCode support.

  bool tc_support (void) const;
  // Check TypeCode support

  void obv_opt_accessor (bool);
  // Set optimized valuetype member accessor generation.

  bool obv_opt_accessor (void) const;
  // Check optimized valuetype member accessor generation.

  void gen_impl_files (bool);
  // Set generation of implementation files.

  bool gen_impl_files (void) const;
  // Check if we want to generate implementation files.

  void gen_impl_debug_info (bool);
  // Set generation of source file and line number for implementation files.

  bool gen_impl_debug_info (void) const;
  // Check generation of source file and line number implementation files.

  void gen_copy_ctor (bool);
  // Set generation of copy constructor.

  bool gen_copy_ctor (void) const;
  // Check if we want to generate the copy constructor.

  void gen_assign_op (bool);
  // Set the generation of the assignment operator.

  bool gen_assign_op (void) const;
  // Check if we want to generate the assignment operator.

  void gen_thru_poa_collocation (bool);
  // Set whether we want to generate Thru_POA collocation stubs.

  bool gen_thru_poa_collocation (void) const;
  // Check if we want to generate Thru_POA collocation stubs.

  void gen_direct_collocation (bool);
  // Set whether we want to generate Direct collocation stubs.

  bool gen_direct_collocation (void) const;
  // Check if we want to generate Direct collocation stubs.

  void gen_corba_e (bool);
  // Set whether we want to generate for CORBA/e

  bool gen_corba_e (void) const;
  // Check if we want to generate for CORBA/e

  void gen_minimum_corba (bool);
  // Set whether we want to generate for Minimum CORBA

  bool gen_minimum_corba (void) const;
  // Check if we want to generate for Minimum CORBA

  void opt_tc (bool);
  // Set optimized typecodes.

  bool opt_tc (void) const;
  // Check if TypeCodes are optimized.

  void ami_call_back (bool value);
  // To enable or disable AMI call back feature of the Messaging
  // specification in the generated code.

  bool ami_call_back (void) const;
  // Return the flag.

  void gen_amh_classes (bool value);
  // To enable or disable AMH in the generated code.

  bool gen_amh_classes (void) const;
  // Return the flag.

  void gen_tie_classes (bool value);
  // Set the generation of tie classes and files.

  bool gen_tie_classes (void) const;
  // Return the flag.

  void gen_smart_proxies (bool value);
  // To enable or disable AMI call back feature of the Messaging
  // specification in the generated code.

  bool gen_smart_proxies (void) const;
  // Return the flag.

  void gen_inline_constants (bool value);
  // Set the flag.

  bool gen_inline_constants (void) const;
  // Return the flag.

  void gen_dcps_type_support (bool value);
  // Set the flag.

  bool gen_dcps_type_support (void) const;
  // Return the flag.

  void gen_dcps_type_support_only (bool value);
  // Set the flag.

  bool gen_dcps_type_support_only (void) const;
  // Return the flag.

  void gen_orb_h_include (bool value);
  // Set the flag.

  bool gen_orb_h_include (void) const;
  // Return the flag.

  bool gen_empty_anyop_header (void) const;
  // Generate an empty anyop header (A.h)

  void gen_empty_anyop_header (bool value);
  // Generate an empty anyop header (A.h)

  void lookup_strategy (LOOKUP_STRATEGY s);
  // Set the lookup strategy.

  BE_GlobalData::LOOKUP_STRATEGY lookup_strategy (void) const;
  // Return the enumerated value for the lookup strategy. Default is
  // perfect hashing.

  void destroy (void);
  // Cleanup function.

  ACE_Unbounded_Queue<be_interface *> non_local_interfaces;
  // Used in the generation of declarations for
  // xxx_Proxy_Broker_Factory_function_pointer.

  ACE_Unbounded_Queue<be_interface_fwd *> non_defined_interfaces;
  // Used in the generation of extern function declarations so we
  // can use non-defined interfaces as members and parameters.

  ACE_Unbounded_Queue<be_interface *> mixed_parentage_interfaces;
  // Used in the generation of overrides for CORBA::release and
  // CORBA::is_nil, needed when the interface inherits versions from
  // both CORBA::Object and CORBA::AbstractBase.

  AST_PredefinedType *void_type (void);
  // Accessor for the member, sets it on the first call.

  be_interface *ccmobject (void);
  // Accessor for the member, sets it on the first call.

  be_module *messaging (void);
  // Accessor for the member, sets it on the first call.

  be_valuetype *messaging_exceptionholder (void);
  // Accessor for the member, sets it on the first call.

  be_valuetype *exceptionholder (void) const;
  void exceptionholder (be_valuetype *val);
  // Accessors for the member.

  be_interface *messaging_replyhandler (void);
  // Accessor for the member, sets it on the first call.

  bool gen_anyop_files (void) const;
  void gen_anyop_files (bool val);
  // Accessors for the member.

  bool gen_skel_files (void) const;
  void gen_skel_files (bool val);
  // Accessors for the member gen_skel_files_.

  bool gen_client_inline (void) const;
  void gen_client_inline (bool val);
  // Accessors for the member gen_client_inline_.

  bool gen_server_inline (void) const;
  void gen_server_inline (bool val);
  // Accessors for the member gen_server_inline_.

  bool gen_client_stub (void) const;
  void gen_client_stub (bool val);
  // Accessors for the member gen_client_stub_.

  bool gen_server_skeleton (void) const;
  void gen_server_skeleton (bool val);
  // Accessors for the member gen_server_skeleton_.

  bool gen_local_iface_anyops (void) const;
  void gen_local_iface_anyops (bool val);
  // Accessors for the member gen_local_iface_anyops_.

  bool gen_custom_ending (void) const;
  void gen_custom_ending (bool val);
  // Accessors for the member gen_custom_ending_.

  ACE_CString spawn_options (void);
  // Command line passed to ACE_Process::spawn. Different
  // implementations in IDL and IFR backends.

  void parse_args (long &i, char **av);
  // Parse args that affect the backend.

  void prep_be_arg (char *s);
  // Special BE arg call factored out of DRV_args.

  void arg_post_proc (void);
  // Checks made after parsing args.

  void usage (void) const;
  // Display usage of BE-specific options.

  AST_Generator *generator_init (void);
  // Create an AST node generator.

private:
  size_t changing_standard_include_files_;
  // To switch between changing or non-changing standard include
  // files (e.g. tao/corba.h)  so that #include statements can be
  // generated with ""s or <>s respectively.

  // Macros and includes used to export classes from generated code.
  char* skel_export_macro_;
  char* skel_export_include_;
  char* stub_export_macro_;
  char* stub_export_include_;

  // Macro and include used on ORB .pidl files generating to the
  // AnyTypeCode library.
  char* anyop_export_macro_;
  char* anyop_export_include_;

  char* pch_include_;
  char* pre_include_;
  char* post_include_;
  char* include_guard_;
  char* safe_include_;
  char* unique_include_;

  /// Text that opens a "versioned" namespace for core TAO and orbsvcs
  /// related code.
  ACE_CString core_versioning_begin_;

  /// Text that closes a "versioned" namespace for core TAO and
  /// orbsvcs related code.
  ACE_CString core_versioning_end_;

  /// Text that opens a "versioned" namepace.
  ACE_CString versioning_begin_;

  /// Text that closes a "versioned" namepace.
  ACE_CString versioning_end_;

  // Client's header file name ending. Default is "C.h".
  char* client_hdr_ending_;

  // Client's stub's file name ending. Default is "C.cpp".
  char* client_stub_ending_;

  // Client's inline file name ending. Default is "C.i".
  char* client_inline_ending_;

  // Server's hdr file name ending. Default is "S.h".
  char* server_hdr_ending_;

  // Implementation's hdr file name ending. Default is "I.h".
  char* implementation_hdr_ending_;

  // Implementation's skeleton file name ending. Default is "I.cpp".
  char* implementation_skel_ending_;

  //Implementaion class prefix
  char* impl_class_prefix_;

  //Implementation class suffix
  char* impl_class_suffix_;

  // Server's template hdr file name ending. Default is "S_T.h".
  char* server_template_hdr_ending_;

  // Server's skeleton file name ending. Default is "S.cpp".
  char* server_skeleton_ending_;

  // Server's template skeleton file name ending. Default is
  // "S_T.cpp".
  char* server_template_skeleton_ending_;

  // Server's inline file name ending. Default is "S.i".
  char* server_inline_ending_;

  // Anyop header file name ending. Default is "A.h".
  char* anyop_hdr_ending_;

  // Anyop source file name ending. Default is "A.cpp".
  char* anyop_src_ending_;

  char* output_dir_;
  // Directory where all the IDL-Compiler-Generated files are to be
  // kept. Default value is 0 for this string which means the current
  // directory from which the <tao_idl> is called.

  char* skel_output_dir_;
  // Directory where all the *S.* files are to be
  // kept. Default value is 0 for this string which means the
  // value for output_dir_ is used.

  char* anyop_output_dir_;
  // Directory where all the *A.* files are to be
  // kept. Default value is 0 for this string which means the
  // value for output_dir_ is used.

  bool any_support_;
  // do we support Any operators?

  bool tc_support_;
  // do we support typecodes?

  bool obv_opt_accessor_;
  // do we optimize valuetype accessors?

  bool gen_impl_files_;
  // are we generating implementation files?

  bool gen_impl_debug_info_;
  // are we generating source file and line numbers for implementation files?

  bool gen_copy_ctor_;
  // are we generating the copy constructor?

  bool gen_assign_op_;
  // are we generating the assignment operator?

  bool gen_thru_poa_collocation_;
  // are we generating Thru_POA collocated stubs?

  bool gen_direct_collocation_;
  // are we generating Direct collocated stubs?

  bool gen_corba_e_;
  // are we generating for CORBA/e

  bool gen_minimum_corba_;
  // are we generating for Minimum CORBA

  bool opt_tc_;
  // do we generate optimized typecodes?

  bool ami_call_back_;
  // Flag to indicate whether the AMI Call back feature of the
  // Messaging specification should be enabled for the generated files
  // or not.

  bool gen_amh_classes_;
  // Flag for generating AMH classes.

  bool gen_tie_classes_;
  // Flag to indicate whether we generate the tie classes and
  // files or not.

  bool gen_smart_proxies_;
  // Flag to indicate whether smart proxies classes will be generated
  // or not.

  bool gen_inline_constants_;
  // Flag to indicate whether we are using an inline form of constant
  // generation that pleases the C++ compiler better on some platforms.

  bool gen_dcps_type_support_;
  // Flag to indicate whether we are supporting DDS DCPS type definitions.
  // Includes Serializer operators (like TAO_Input/OutuptCDR).

  bool gen_dcps_type_support_only_;
  // Flag to indicate whether we are supporting DDS DCPS type only definitions.
  // Only generate DDS Serializer operators for tao/*Seq.pidl.

  bool gen_orb_h_include_;
  // Flag to indicate whether ORB.h include should be generated, needed for
  // regenerating the pidl files.

  bool gen_empty_anyop_header_;
  // Flag to indicate whether an empty A.h should be generated

  LOOKUP_STRATEGY lookup_strategy_;
  // The enumerated value indicating the lookup strategy.

  AST_PredefinedType *void_type_;
  // Used for void operation return types.

  be_interface *ccmobject_;
  // Reference holder for component skeleton visitors.

  be_module *messaging_;
  // Storage for the messaging exceptions holder's virtual scope.

  be_valuetype *messaging_exceptionholder_;
  // Storage for exception holders' base class node.

  be_valuetype *exceptionholder_;
  // Reference holder for the exceptionholder.

  be_interface *messaging_replyhandler_;
  // Storage for the reply handlers' base class node.

  bool gen_anyop_files_;
  // Separate files for generated Any operators?

  bool gen_skel_files_;
  // True by default, but a command line option can turn this off so
  // we generate only an empty *S.h file on the skeleton side.

  bool gen_client_inline_;
  // True by default, but a command line option can turn this off so
  // that we don't generate a client inline file

  bool gen_server_inline_;
  // True by default, but a command line option can turn this off so
  // that we don't generate a server inline file

  bool gen_client_stub_;
  // True by default, but a command line option can turn this off so
  // that we don't generate a client stub file

  bool gen_server_skeleton_;
  // True by default, but a command line option can turn this off so
  // that we don't generate a server skeleton file

  bool gen_local_iface_anyops_;
  // True by default, can be turned off by a command line option
  // for regenerating internal ORB files.

  bool use_clonable_in_args_;
  // Use in_clonable_arg_val instead of in_arg_val?

  bool gen_template_export_;
  // Generate explicit export for Visual Studio bug workaround,
  // needed only in TAO basic sequence *C.h files.

  bool gen_ostream_operators_;
  // Generate ostream operators for each type declaration, for
  // debuggin or logging.

  bool gen_custom_ending_;
  // True by default, but a command line option can turn this off so
  // custom ending will not be applied to files in $TAO_ROOT/,
  // $TAO_ROOT/tao, $TAO_ROOT/orbsvcs, $TAO_ROOT/CIAO, $TAO_ROOT/CIAO/ciao.
};

#endif /* _BE_GLOBAL_H */
