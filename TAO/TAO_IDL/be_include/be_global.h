// -*- C++ -*-

//=============================================================================
/**
 *  @file    be_global.h
 *
 *  $Id$
 *
 *  Header file for class containing compiler back end global data.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================

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

/*
 * Storage of global data specific to the compiler back end
 */
class TAO_IDL_BE_Export BE_GlobalData
{
public:
  /// various lookup strategies
  enum LOOKUP_STRATEGY
  {
    TAO_LINEAR_SEARCH,
    TAO_DYNAMIC_HASH,
    TAO_PERFECT_HASH,
    TAO_BINARY_SEARCH
  };

  /// To help with DDD portability in DDS4CCM
  /// connectors.
  enum DDS_IMPL
  {
    NONE,
    NDDS,
    OPENSPLICE,
    OPENDDS
  };

  BE_GlobalData (void);
  ~BE_GlobalData (void);

  // To switch between changing or non-changing standard include
  // files (e.g. tao/corba.h)  so that #include statements can be
  // generated with ""s or <>s respectively.
  void changing_standard_include_files (size_t changing);
  size_t changing_standard_include_files (void);

  // Helper functions that generate the file names for the C++ mapping
  // generated code.
  // The parameter <base_name_only> set to 0 (no base name, but full
  // name with output dir path, is useful, when I just want just the
  // base name to use in #include's etc.
  static const char *be_get_client_hdr_fname (
    bool base_name_only = false);

  static const char *be_get_client_stub_fname (void);

  static const char *be_get_client_inline_fname (
    bool base_name_only = false);

  static const char *be_get_server_hdr_fname (
    bool base_name_only = false);

  static const char *be_get_implementation_hdr_fname (
    bool base_name_only = false);

  static const char *be_get_implementation_skel_fname (
    bool base_name_only = false);

  static const char *be_get_server_template_hdr_fname (
    bool base_name_only = false);

  static const char *be_get_server_skeleton_fname (void);

  static const char *be_get_implementation_skeleton_fname (void);

  static const char *be_get_server_template_skeleton_fname (
    bool base_name_only = false);

  static const char *be_get_server_inline_fname (
    bool base_name_only = false);

  static const char *be_get_anyop_source_fname (
    bool base_name_only = false);

  static const char *be_get_anyop_header_fname (
    bool base_name_only = false);

  static const char *be_get_ciao_svnt_hdr_fname (
    bool base_name_only = false);

  static const char *be_get_ciao_svnt_src_fname (
    bool base_name_only = false);

  static const char *be_get_ciao_exec_hdr_fname (
    bool base_name_only = false);

  static const char *be_get_ciao_exec_src_fname (
    bool base_name_only = false);

  static const char *be_get_ciao_exec_stub_hdr_fname (
    bool base_name_only = false);

  static const char *be_get_ciao_exec_idl_fname (
    bool base_name_only = false);

  static const char *be_get_ciao_conn_hdr_fname (
    bool base_name_only = false);

  static const char *be_get_ciao_conn_src_fname (
    bool base_name_only = false);

  static const char *be_get_ciao_ami_conn_idl_fname (
    bool base_name_only = false);

  static const char *be_get_ciao_ami_conn_impl_hdr_fname (
    bool base_name_only = false);

  static const char *be_get_ciao_ami_conn_impl_src_fname (
    bool base_name_only = false);

  // Helper functions: obtain the names of each generated file given
  // the IDL file name.
  // The parameter <base_name_only> set to 0 (no base name, but full
  // name with output dir path, is useful, when I want just the
  // base name to use in #include's etc.
  static const char *be_get_client_hdr (
    UTL_String *idl_file_name,
    bool base_name_only = false);

  static const char *be_get_client_stub (
    UTL_String *idl_file_name);

  static const char *be_get_client_inline (
    UTL_String *idl_file_name,
    bool base_name_only = false);

  static const char *be_get_server_hdr (
    UTL_String *idl_file_name,
    bool base_name_only = false);

  static const char *be_get_implementation_hdr (
    UTL_String *idl_file_name,
    bool base_name_only = false);

  static const char *be_get_implementation_skel (
    UTL_String *idl_file_name,
    bool base_name_only = false);

  static const char *be_get_server_template_hdr (
    UTL_String *idl_file_name,
    bool base_name_only = false);

  static const char *be_get_server_skeleton (
    UTL_String *idl_file_name);

  static const char *be_get_server_template_skeleton (
      UTL_String *idl_file_name,
      bool base_name_only = false);

  static const char *be_get_server_inline (
    UTL_String *idl_file_name,
    bool base_name_only = false);

  static const char *be_get_anyop_header (
    UTL_String *idl_file_name,
    bool base_name_only = false);

  static const char *be_get_anyop_source (
    UTL_String *idl_file_name,
    bool base_name_only = false);

  static const char *be_get_ciao_svnt_header (
    UTL_String *idl_file_name,
    bool base_name_only = false);

  static const char *be_get_ciao_svnt_source (
    UTL_String *idl_file_name,
    bool base_name_only = false);

  static const char *be_get_ciao_exec_header (
    UTL_String *idl_file_name,
    bool base_name_only = false);

  static const char *be_get_ciao_exec_source (
    UTL_String *idl_file_name,
    bool base_name_only = false);

  static const char *be_get_ciao_exec_stub_header (
    UTL_String *idl_file_name,
    bool base_name_only = false);

  static const char *be_get_ciao_exec_idl (
    UTL_String *idl_file_name,
    bool base_name_only = false);

  static const char *be_get_ciao_conn_header (
    UTL_String *idl_file_name,
    bool base_name_only = false);

  static const char *be_get_ciao_conn_source (
    UTL_String *idl_file_name,
    bool base_name_only = false);

  static const char *be_get_dds_typesupport_header (
    UTL_String *idl_file_name,
    bool base_name_only = false);

  static const char *be_get_ciao_ami_conn_idl (
    UTL_String *idl_file_name,
    bool base_name_only = false);

  static const char *be_get_ciao_ami_conn_impl_hdr (
    UTL_String *idl_file_name,
    bool base_name_only = false);

  static const char *be_get_ciao_ami_conn_impl_src (
    UTL_String *idl_file_name,
    bool base_name_only = false);

  //// Returns the macro name for exporting server side classes in Win32
  //// DLL.
  const char* skel_export_macro (void) const;

  //// Set the macro name for export server side classes in Win32 DLL.
  void skel_export_macro (const char* s);

  //// Returns the name of the include file that contains the server
  //// side export macro definition.
  const char* skel_export_include (void) const;

  //// Set the name of the include file that contains the server side
  //// export macro definition.
  void skel_export_include (const char* s);

  //// Returns the macro name for exporting client side classes in Win32
  //// DLL.
  const char* stub_export_macro (void) const;

  //// Set the macro name for export client side classes in Win32 DLL.
  void stub_export_macro (const char* s);

  //// Returns the name of the include file that contains the client
  //// side export macro definition.
  const char* stub_export_include (void) const;

  //// Set the name of the include file that contains the client side
  //// export macro definition.
  void stub_export_include (const char* s);

  //// Returns the macro name for exporting *A.h file classes in Win32
  //// DLL.
  const char* anyop_export_macro (void) const;

  //// Set the macro name for export *A.h file classes in Win32 DLL.
  void anyop_export_macro (const char* s);

  //// Returns the name of the include file that contains the *A.h
  //// file export macro definition.
  const char* anyop_export_include (void) const;

  //// Set the name of the include file that contains the *A.h file
  //// export macro definition.
  void anyop_export_include (const char* s);

  //// Returns the macro name for exporting exec impl classes in Win32
  //// DLL.
  const char* exec_export_macro (void) const;

  //// Set the macro name for export exec impl classes in Win32 DLL.
  void exec_export_macro (const char* s);

  //// Returns the name of the include file that contains the exec
  //// impl export macro definition.
  const char* exec_export_include (void) const;

  //// Set the name of the include file that contains the exec impl
  //// export macro definition.
  void exec_export_include (const char* s);

  //// Returns the macro name for exporting CIAO svnt classes in Win32
  //// DLL.
  const char* svnt_export_macro (void) const;

  //// Set the macro name for export CIAO svnt classes in Win32 DLL.
  void svnt_export_macro (const char* s);

  //// Returns the name of the include file that contains the CIAO
  //// svnt export macro definition.
  const char* svnt_export_include (void) const;

  //// Set the name of the include file that contains the CIAO svnt
  //// export macro definition.
  void svnt_export_include (const char* s);

  //// Returns the macro name for exporting CIAO connector
  //// classes in Win32 DLL.
  const char* conn_export_macro (void) const;

  //// Set the macro name for export CIAO connector
  //// classes in Win32 DLL.
  void conn_export_macro (const char* s);

  //// Returns the name of the include file that contains the CIAO
  //// connector export macro definition.
  const char* conn_export_include (void) const;

  //// Set the name of the include file that contains the CIAO
  //// connector export macro definition.
  void conn_export_include (const char* s);

  //// Returns the name of the include file to be used for precompiled
  //// header support.
  const char* pch_include (void) const;

  //// Set the name of the include file to be used for precompiled
  //// header support.
  void pch_include (const char* s);

  //// Returns the name of the include file to be put at the top of
  //// every header file.
  const char* pre_include (void) const;

  //// Set the name of the include file to be put at the top of every
  //// header file.
  void pre_include (const char* s);

  //// Returns the name of the include file to be put at the bottom of
  //// every header file.
  const char* post_include (void) const;

  //// Set the name of the include file to be put at the bottom of every
  //// header file.
  void post_include (const char* s);

  //// Returns the guard that is placed in the client header file
  const char* include_guard (void) const;

  //// Set the guard that is placed in the client header file
  void include_guard (const char* s);

  //// Returns the name of the include file that is used instead of the own
  //// generated client header file
  const char* safe_include (void) const;

  //// set the name of the include file that is used instead of the own
  //// generated client header file
  void safe_include (const char* s);

  //// Returns the name of the include file that should on get
  //// generated in the client header file
  const char* unique_include (void) const;

  /// Set the name of the include file that should on get
  /// generated in the client header file
  void unique_include (const char* s);

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

  /// Set the client_hdr_ending.
  void client_hdr_ending (const char* s);

  /// Get the client_hdr_ending.
  const char* client_hdr_ending (void) const;

  /// Set the client_inline_ending.
  void client_inline_ending (const char* s);

  /// Get the client_inline_ending.
  const char* client_inline_ending (void) const;

  /// Set the client_stub_ending.
  void  client_stub_ending (const char* s);

  /// Get the client_stub_ending.
  const char* client_stub_ending (void) const;

  /// Set the server_hdr_ending.
  void server_hdr_ending (const char* s);

  /// Get the server_hdr_ending.
  const char* server_hdr_ending (void) const;

  /// Set the implementation_hdr_ending.
  void implementation_hdr_ending (const char* s);

  /// Set the implementation class prefix.
  void impl_class_prefix (const char* s);

  /// Set the implementation class suffix.
  void impl_class_suffix (const char* s);

  /// Set the implementation_skel_ending.
  void implementation_skel_ending (const char* s);

  /// Get the implementation_hdr_ending.
  const char* implementation_hdr_ending (void) const;

  ///Get implementation class prefix
  const char* impl_class_prefix (void) const;

  ///Get implementation class suffix
  const char* impl_class_suffix (void) const;

  /// Get the implementation_skel_ending.
  const char* implementation_skel_ending (void) const;

  /// Set the server_template_hdr_ending.
  void server_template_hdr_ending (const char* s);

  /// Get the server_template_hdr_ending.
  const char* server_template_hdr_ending (void) const;

  /// Set the server_skeleton_ending.
  void server_skeleton_ending (const char* s);

  /// Get the server_skeleton_ending.
  const char* server_skeleton_ending (void) const;

  /// Set the server_template_skeleton_ending.
  void server_template_skeleton_ending (const char* s);

  /// Get the server_template_skeleton_ending.
  const char* server_template_skeleton_ending (void) const;

  /// Set the server_inline_ending.
  void server_inline_ending (const char* s);

  /// Get the server_inline_ending.
  const char* server_inline_ending (void) const;

  /// Set the anyop_header_ending.
  void anyop_header_ending (const char* s);

  /// Get the anyop_header_ending.
  const char* anyop_header_ending (void) const;

  /// Set the anyop_source_ending.
  void anyop_source_ending (const char* s);

  /// Get the anyop_source_ending.
  const char* anyop_source_ending (void) const;

  /// Similar to above, but for CIAO servant and executor
  /// impl and executor IDL files, if generated.
  void ciao_svnt_header_ending (const char* s);
  const char* ciao_svnt_header_ending (void) const;

  void ciao_svnt_source_ending (const char* s);
  const char* ciao_svnt_source_ending (void) const;

  void ciao_exec_header_ending (const char* s);
  const char* ciao_exec_header_ending (void) const;

  void ciao_exec_source_ending (const char* s);
  const char* ciao_exec_source_ending (void) const;

  void ciao_exec_stub_header_ending (const char* s);
  const char* ciao_exec_stub_header_ending (void) const;

  void ciao_exec_idl_ending (const char* s);
  const char* ciao_exec_idl_ending (void) const;

  void ciao_conn_header_ending (const char* s);
  const char* ciao_conn_header_ending (void) const;

  void ciao_conn_source_ending (const char* s);
  const char* ciao_conn_source_ending (void) const;

  /// For generating TypeSupport header file includes.
  void dds_typesupport_hdr_ending (const char* s);
  const char* dds_typesupport_hdr_ending (void) const;

  /// For optionally generating CIAO AMI connector IDL file.
  void ciao_ami_conn_idl_ending (const char* s);
  const char* ciao_ami_conn_idl_ending (void) const;

  /// For optionally generating CIAO AMI reply handler
  /// impl header file.
  void ciao_ami_conn_impl_hdr_ending (const char* s);
  const char* ciao_ami_conn_impl_hdr_ending (void) const;

  /// For optionally generating CIAO AMI reply handler
  /// impl source file.
  void ciao_ami_conn_impl_src_ending (const char* s);
  const char* ciao_ami_conn_impl_src_ending (void) const;

  /// For optionally controlling the container type, default is Session
  void ciao_container_type (const char* s);
  const char* ciao_container_type (void) const;

  /// Set the clonable_in_args.
  void use_clonable_in_args (bool clonable);

  /// Get the clonable_in_args setting.
  bool use_clonable_in_args (void) const;

  /// Get the gen_seq_template_export_ member.
  bool gen_template_export (void) const;

  /// Set the gen_seq_template_export_ member.
  void gen_template_export (bool val);

  /// Get the gen_ostream_operators_ member.
  bool gen_ostream_operators (void) const;

  /// Set the gen_ostream_operators_ member.
  void gen_ostream_operators (bool val);

  /**
   * Set the directory where all the IDL-Compiler-Generated files are
   * to be kept. Default is current directory from which the
   * <tao_idl> is called.
   */
  void output_dir (const char* s);

  /**
   * Get the directory where all the IDL-Compiler-Generated files are
   * to be kept. Default is current directory from which the
   * <tao_idl> is called.
   */
  const char* output_dir (void) const;

  /// Set the path for all *C.* file includes.
  /// Default is local directory or $TAO_ROOT/tao.
  void stub_include_dir (const char* s);

  /// Get the path for all *C.* file includes.
  /// Default is local directory or $TAO_ROOT/tao.
  const char* stub_include_dir (void) const;

  /// Set the directory where all the *S.* files are
  /// to be kept. Default is output_dir_.
  void skel_output_dir (const char* s);

  /// Get the directory where all the *S.* files are
  /// to be kept. Default is output_dir_.
  const char* skel_output_dir (void) const;

  /// Set the directory where all the *A.* files are
  /// to be kept. Default is output_dir_.
  void anyop_output_dir (const char* s);

  /// Get the directory where all the *A.* files are
  /// to be kept. Default is output_dir_.
  const char* anyop_output_dir (void) const;

  /// Set any support.
  void any_support (bool);

  /// Check Any support.
  bool any_support (void) const;

  /// Set TypeCode support.
  void tc_support (bool);

  /// Check TypeCode support
  bool tc_support (void) const;

  /// Set optimized valuetype member accessor generation.
  void obv_opt_accessor (bool);

  /// Check optimized valuetype member accessor generation.
  bool obv_opt_accessor (void) const;

  /// Set generation of implementation files.
  void gen_impl_files (bool);

  /// Check if we want to generate implementation files.
  bool gen_impl_files (void) const;

  /// Set generation of source file and line number for implementation files.
  void gen_impl_debug_info (bool);

  /// Check generation of source file and line number implementation files.
  bool gen_impl_debug_info (void) const;

  /// Set generation of copy constructor.
  void gen_copy_ctor (bool);

  /// Check if we want to generate the copy constructor.
  bool gen_copy_ctor (void) const;

  /// Set the generation of the assignment operator.
  void gen_assign_op (bool);

  /// Check if we want to generate the assignment operator.
  bool gen_assign_op (void) const;

  /// Set whether we want to generate Thru_POA collocation stubs.
  void gen_thru_poa_collocation (bool);

  /// Check if we want to generate Thru_POA collocation stubs.
  bool gen_thru_poa_collocation (void) const;

  /// Set whether we want to generate Direct collocation stubs.
  void gen_direct_collocation (bool);

  /// Check if we want to generate Direct collocation stubs.
  bool gen_direct_collocation (void) const;

  /// Set whether we want to generate for CORBA/e
  void gen_corba_e (bool);

  /// Check if we want to generate for CORBA/e
  bool gen_corba_e (void) const;

  /// Set whether we want to generate for Minimum CORBA
  void gen_minimum_corba (bool);

  /// Check if we want to generate for Minimum CORBA
  bool gen_minimum_corba (void) const;

  /// Set whether we want to generate for LwCCM
  void gen_lwccm (bool);

  /// Check if we want to generate for LwCCM
  bool gen_lwccm (void) const;

  /// Set optimized typecodes.
  void opt_tc (bool);

  /// Check if TypeCodes are optimized.
  bool opt_tc (void) const;

  /// To enable or disable AMI4CCM call back feature
  void ami4ccm_call_back (bool value);

  /// Return the flag.
  bool ami4ccm_call_back (void) const;

  /// To enable or disable AMI call back feature of the Messaging
  /// specification in the generated code.
  void ami_call_back (bool value);

  /// Return the flag.
  bool ami_call_back (void) const;

  /// To enable or disable AMH in the generated code.
  void gen_amh_classes (bool value);

  /// Return the flag.
  bool gen_amh_classes (void) const;

  /// Set the generation of tie classes and files.
  void gen_tie_classes (bool value);

  /// Return the flag.
  bool gen_tie_classes (void) const;

  /// To enable or disable AMI call back feature of the Messaging
  /// specification in the generated code.
  void gen_smart_proxies (bool value);

  /// Return the flag.
  bool gen_smart_proxies (void) const;

  /// Set the flag.
  void gen_inline_constants (bool value);

  /// Return the flag.
  bool gen_inline_constants (void) const;

  /// Set the flag.
  void gen_dcps_type_support (bool value);

  /// Return the flag.
  bool gen_dcps_type_support (void) const;

  /// Set the flag.
  void gen_dcps_type_support_only (bool value);

  /// Return the flag.
  bool gen_dcps_type_support_only (void) const;

  /// Set the flag.
  void gen_orb_h_include (bool value);

  /// Return the flag.
  bool gen_orb_h_include (void) const;

  /// Generate an empty anyop header (A.h)
  bool gen_empty_anyop_header (void) const;

  /// Generate an empty anyop header (A.h)
  void gen_empty_anyop_header (bool value);

  /// Set the lookup strategy.
  void lookup_strategy (LOOKUP_STRATEGY s);

  /// Return the enumerated value for the lookup strategy. Default is
  /// perfect hashing.
  LOOKUP_STRATEGY lookup_strategy (void) const;

  /// Set the DDS implementation.
  void dds_impl (char const * const val);

  /// Return the enumerated value for the DDS implementation.
  /// Default is NDDS.
  DDS_IMPL dds_impl (void) const;

  /// Cleanup function.
  void destroy (void);

  /// Used in the generation of declarations for
  /// xxx_Proxy_Broker_Factory_function_pointer.
  ACE_Unbounded_Queue<be_interface *> non_local_interfaces;

  /// Used in the generation of extern function declarations so we
  /// can use non-defined interfaces as members and parameters.
  ACE_Unbounded_Queue<be_interface_fwd *> non_defined_interfaces;

  /// Accessor for the member, sets it on the first call.
  AST_PredefinedType *void_type (void);

  /// Accessor for the member, sets it on the first call.
  be_interface *ccmobject (void);

  /// Accessor for the member, sets it on the first call.
  be_module *messaging (void);

  /// Accessor for the member, sets it on the first call.
  be_valuetype *messaging_exceptionholder (void);

  /// Accessor for the member, sets it on the first call.
  be_interface *messaging_replyhandler (void);

  /// Accessors for the member.
  bool gen_anyop_files (void) const;
  void gen_anyop_files (bool val);

  /// Accessors for the member gen_skel_files_.
  bool gen_skel_files (void) const;
  void gen_skel_files (bool val);

  /// Accessors for the member gen_client_inline_.
  bool gen_client_inline (void) const;
  void gen_client_inline (bool val);

  /// Accessors for the member gen_server_inline_.
  bool gen_server_inline (void) const;
  void gen_server_inline (bool val);

  /// Accessors for the member gen_client_stub_.
  bool gen_client_stub (void) const;
  void gen_client_stub (bool val);

  /// Accessors for the member gen_server_skeleton_.
  bool gen_server_skeleton (void) const;
  void gen_server_skeleton (bool val);

  /// Accessors for the member gen_local_iface_anyops_.
  bool gen_local_iface_anyops (void) const;
  void gen_local_iface_anyops (bool val);

  /// Accessors for the member gen_custom_ending_.
  bool gen_custom_ending (void) const;
  void gen_custom_ending (bool val);

  /// Accessors for the member gen_unique_guards_.
  bool gen_unique_guards (void) const;
  void gen_unique_guards (bool val);

  /// Accessors for the corresponding members.

  bool gen_ciao_svnt (void) const;
  void gen_ciao_svnt (bool val);

  bool gen_ciao_exec_idl (void) const;
  void gen_ciao_exec_idl (bool val);

  bool gen_ciao_exec_impl (void) const;
  void gen_ciao_exec_impl (bool val);

  bool gen_ciao_conn_impl (void) const;
  void gen_ciao_conn_impl (bool val);

  bool gen_ciao_valuefactory_reg (void) const;
  void gen_ciao_valuefactory_reg (bool val);

  bool gen_stub_export_hdr_file (void) const;
  void gen_stub_export_hdr_file (bool val);

  bool gen_skel_export_hdr_file (void) const;
  void gen_skel_export_hdr_file (bool val);

  bool gen_svnt_export_hdr_file (void) const;
  void gen_svnt_export_hdr_file (bool val);

  bool gen_exec_export_hdr_file (void) const;
  void gen_exec_export_hdr_file (bool val);

  bool gen_conn_export_hdr_file (void) const;
  void gen_conn_export_hdr_file (bool val);

  bool gen_lem_force_all (void) const;
  void gen_lem_force_all (bool val);

  bool alt_mapping (void) const;
  void alt_mapping (bool val);

  unsigned long tab_size (void) const;
  void tab_size (unsigned long val);

  /// Command line passed to ACE_Process::spawn. Different
  /// implementations in IDL and IFR backends.
  ACE_CString spawn_options (void);

  /// Parse args that affect the backend.
  void parse_args (long &i, char **av);

private:
  /**
   * To switch between changing or non-changing standard include
   * files (e.g. tao/corba.h)  so that #include statements can be
   * generated with ""s or <>s respectively.
   */
  size_t changing_standard_include_files_;

  // Macros and includes used to export classes from generated code.
  char* skel_export_macro_;
  char* skel_export_include_;
  char* stub_export_macro_;
  char* stub_export_include_;

  // Macro and include used on ORB .pidl files generating to the
  // AnyTypeCode library.
  char* anyop_export_macro_;
  char* anyop_export_include_;

  char* exec_export_macro_;
  char* exec_export_include_;
  char* svnt_export_macro_;
  char* svnt_export_include_;
  char* conn_export_macro_;
  char* conn_export_include_;

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

  /// Client's header file name ending. Default is "C.h".
  char* client_hdr_ending_;

  /// Client's stub's file name ending. Default is "C.cpp".
  char* client_stub_ending_;

  /// Client's inline file name ending. Default is "C.i".
  char* client_inline_ending_;

  /// Server's hdr file name ending. Default is "S.h".
  char* server_hdr_ending_;

  /// Implementation's hdr file name ending. Default is "I.h".
  char* implementation_hdr_ending_;

  // Implementation's skeleton file name ending. Default is "I.cpp".
  char* implementation_skel_ending_;

  /// Implementaion class prefix
  char* impl_class_prefix_;

  /// Implementation class suffix
  char* impl_class_suffix_;

  /// Server's template hdr file name ending. Default is "S_T.h".
  char* server_template_hdr_ending_;

  /// Server's skeleton file name ending. Default is "S.cpp".
  char* server_skeleton_ending_;

  /// Server's template skeleton file name ending. Default is
  /// "S_T.cpp".
  char* server_template_skeleton_ending_;

  /// Server's inline file name ending. Default is "S.i".
  char* server_inline_ending_;

  /// Anyop header file name ending. Default is "A.h".
  char* anyop_hdr_ending_;

  /// Anyop source file name ending. Default is "A.cpp".
  char* anyop_src_ending_;

  /// CIAO servant header file name ending. Default is "_svnt.h".
  char* ciao_svnt_hdr_ending_;

  /// CIAO servant source file name ending. Default is "_svnt.cpp".
  char* ciao_svnt_src_ending_;

  /// CIAO executor impl header file name ending. Default is "_exec.h".
  char* ciao_exec_hdr_ending_;

  /// CIAO executor impl source file name ending. Default is "_exec.cpp".
  char* ciao_exec_src_ending_;

  /// CIAO executor stub header file name ending. Default is "EC.h".
  char* ciao_exec_stub_hdr_ending_;

  /// CIAO executor IDL file name ending. Default is "E.idl".
  char* ciao_exec_idl_ending_;

  /// CIAO connector impl header file name ending. Default is "_conn.h".
  char* ciao_conn_hdr_ending_;

  /// CIAO connector impl source file name ending. Default is "_conn.cpp".
  char* ciao_conn_src_ending_;

  /// DDS TypeSupport header file name ending. Default is "Support.h".
  char* dds_typesupport_hdr_ending_;

  /// CIAO AMI connector IDL file name ending. Default is "A.idl".
  char* ciao_ami_conn_idl_ending_;

  /// CIAO AMI reply handler impl header file name ending.
  /// Default is "A_impl.h".
  char* ciao_ami_conn_impl_hdr_ending_;

  /// CIAO AMI reply handler impl source file name ending.
  /// Default is "A_impl.cpp".
  char* ciao_ami_conn_impl_src_ending_;

  /// CIAO container type
  /// Default is "Session".
  char* ciao_container_type_;

  /**
   * Directory where all the IDL-Compiler-Generated files are to be
   * kept. Default value is 0 for this string which means the current
   * directory from which the <tao_idl> is called.
   */
  char* output_dir_;

  /**
   * Path for all *C.* includes. Default value is 0, in which case
   * the local directory or $TAO_ROOT/tao is used.
   */
  char* stub_include_dir_;

  /**
   * Directory where all the *S.* files are to be
   * kept. Default value is 0 for this string which means the
   * value for output_dir_ is used.
   */
  char* skel_output_dir_;

  /**
   * Directory where all the *A.* files are to be
   * kept. Default value is 0 for this string which means the
   * value for output_dir_ is used.
   */
  char* anyop_output_dir_;

  /// do we support Any operators?
  bool any_support_;

  /// do we support typecodes?
  bool tc_support_;

  /// do we optimize valuetype accessors?
  bool obv_opt_accessor_;

  /// are we generating implementation files?
  bool gen_impl_files_;

  /// are we generating source file and line numbers for implementation files?
  bool gen_impl_debug_info_;

  /// are we generating the copy constructor?
  bool gen_copy_ctor_;

  /// are we generating the assignment operator?
  bool gen_assign_op_;

  /// are we generating Thru_POA collocated stubs?
  bool gen_thru_poa_collocation_;

  /// are we generating Direct collocated stubs?
  bool gen_direct_collocation_;

  /// are we generating for CORBA/e
  bool gen_corba_e_;

  /// are we generating for Minimum CORBA
  bool gen_minimum_corba_;

  /// are we generating for LwCCM
  bool gen_lwccm_;

  /// do we generate optimized typecodes?
  bool opt_tc_;

  /// Flag to indicate whether the AMI4ccm Call back feature should be enabled
  /// for the generated files or not.
  bool ami4ccm_call_back_;

  /**
   * Flag to indicate whether the AMI Call back feature of the
   * Messaging specification should be enabled for the generated files
   * or not.
   */
  bool ami_call_back_;

  /// Flag for generating AMH classes.
  bool gen_amh_classes_;

  /// Flag to indicate whether we generate the tie classes and
  /// files or not.
  bool gen_tie_classes_;

  /// Flag to indicate whether smart proxies classes will be generated
  /// or not.
  bool gen_smart_proxies_;

  /// Flag to indicate whether we are using an inline form of constant
  /// generation that pleases the C++ compiler better on some platforms.
  bool gen_inline_constants_;

  /// Flag to indicate whether we are supporting DDS DCPS type definitions.
  /// Includes Serializer operators (like TAO_Input/OutuptCDR).
  bool gen_dcps_type_support_;

  /// Flag to indicate whether we are supporting DDS DCPS type only definitions.
  /// Only generate DDS Serializer operators for tao/*Seq.pidl.
  bool gen_dcps_type_support_only_;

  /// Flag to indicate whether ORB.h include should be generated, needed for
  /// regenerating the pidl files.
  bool gen_orb_h_include_;

  /// Flag to indicate whether an empty A.h should be generated
  bool gen_empty_anyop_header_;

  /// The enumerated value indicating the lookup strategy.
  LOOKUP_STRATEGY lookup_strategy_;

  /// The enumerated value indicating the DDS implementation.
  DDS_IMPL dds_impl_;

  /// Used for void operation return types.
  AST_PredefinedType *void_type_;

  /// Reference holder for component skeleton visitors.
  be_interface *ccmobject_;

  /// Storage for the messaging exceptions holder's virtual scope.
  be_module *messaging_;

  /// Storage for exception holders' base class node.
  be_valuetype *messaging_exceptionholder_;

  /// Storage for the reply handlers' base class node.
  be_interface *messaging_replyhandler_;

  /// Separate files for generated Any operators?
  bool gen_anyop_files_;

  /// True by default, but a command line option can turn this off so
  /// we generate only an empty *S.h file on the skeleton side.
  bool gen_skel_files_;

  /// True by default, but a command line option can turn this off so
  /// that we don't generate a client inline file
  bool gen_client_inline_;

  /// True by default, but a command line option can turn this off so
  /// that we don't generate a server inline file
  bool gen_server_inline_;

  /// True by default, but a command line option can turn this off so
  /// that we don't generate a client stub file
  bool gen_client_stub_;

  /// True by default, but a command line option can turn this off so
  /// that we don't generate a server skeleton file
  bool gen_server_skeleton_;

  /// True by default, can be turned off by a command line option
  /// for regenerating internal ORB files.
  bool gen_local_iface_anyops_;

  /// Use in_clonable_arg_val instead of in_arg_val?
  bool use_clonable_in_args_;

  /// Generate explicit export for Visual Studio bug workaround,
  /// needed only in TAO basic sequence *C.h files.
  bool gen_template_export_;

  /// Generate ostream operators for each type declaration, for
  /// debugging or logging.
  bool gen_ostream_operators_;

  /**
   * True by default, but a command line option can turn this off so
   * custom ending will not be applied to files in $TAO_ROOT/,
   * $TAO_ROOT/tao, $TAO_ROOT/orbsvcs, $TAO_ROOT/CIAO, $TAO_ROOT/CIAO/ciao.
   */
  bool gen_custom_ending_;

 /**
   * True by default, but a command line option can turn this off so
   * the generated code (header) files will not be proteced by unique guards.
   */
  bool gen_unique_guards_;

  /// False by default, these flags trigger code generation
  /// formerly done by the CIAO CIDL compiler.
  bool gen_ciao_svnt_;
  bool gen_ciao_exec_idl_;
  bool gen_ciao_exec_impl_;

  /// False by default, this flag triggers code generation
  /// for CCM connector implementations.
  bool gen_ciao_conn_impl_;

  /// Generate automatic valuetype factory registration in
  /// CIAO servants.
  bool gen_ciao_valuefactory_reg_;

  /// Flags toggling export header file generation.
  bool gen_stub_export_hdr_file_;
  bool gen_skel_export_hdr_file_;
  bool gen_svnt_export_hdr_file_;
  bool gen_exec_export_hdr_file_;
  bool gen_conn_export_hdr_file_;

  /// Toggles generation of exec IDL for facets with imported
  /// interface types (not in main IDL file).
  bool gen_lem_force_all_;

  /// 2 spaces by default, can be set from the command line.
  unsigned int tab_size_;

  bool alt_mapping_;
  // Are we generating STL types?
};

#endif /* _BE_GLOBAL_H */
