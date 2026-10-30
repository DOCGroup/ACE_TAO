// -*- C++ -*-

//=============================================================================
/**
 *  @file    be_global.h
 *
 *  Header file for class containing compiler back end global data.
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================

#ifndef _BE_GLOBAL_H
#define _BE_GLOBAL_H

#include "TAO_IDL_BE_Export.h"

#include <deque>
#include <optional>
#include <string>

class be_interface;
class be_valuetype;
class be_module;
class be_interface;
class be_interface_fwd;
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
    DDS_NONE,
    NDDS,
    OPENSPLICE,
    OPENDDS,
    COREDX
  };

  inline static std::string const core_versioned_ns_begin =
    "\nTAO_BEGIN_VERSIONED_NAMESPACE_DECL\n";
  inline static std::string const core_versioned_ns_end =
    "\nTAO_END_VERSIONED_NAMESPACE_DECL\n";

  BE_GlobalData ();
  ~BE_GlobalData ();
  BE_GlobalData (BE_GlobalData const &) = delete;
  BE_GlobalData (BE_GlobalData &&) = delete;
  BE_GlobalData &operator= (BE_GlobalData const &) = delete;
  BE_GlobalData &operator= (BE_GlobalData &&) = delete;

  // To switch between changing or non-changing standard include
  // files (e.g. tao/corba.h)  so that #include statements can be
  // generated with ""s or <>s respectively.
  void changing_standard_include_files (size_t changing);
  size_t changing_standard_include_files ();

  // Helper functions that generate the file names for the C++ mapping
  // generated code.
  // The parameter <base_name_only> set to 0 (no base name, but full
  // name with output dir path, is useful, when I just want just the
  // base name to use in #include's etc.
  static std::string be_get_client_hdr_fname (
    bool base_name_only = false);

  static std::string be_get_client_stub_fname ();

  static std::string be_get_client_inline_fname (
    bool base_name_only = false);

  static std::string be_get_server_hdr_fname (
    bool base_name_only = false);

  static std::string be_get_implementation_hdr_fname (
    bool base_name_only = false);

  static std::string be_get_implementation_skel_fname (
    bool base_name_only = false);

  static std::string be_get_server_template_hdr_fname (
    bool base_name_only = false);

  static std::string be_get_server_skeleton_fname ();

  static std::string be_get_implementation_skeleton_fname ();

  static std::string be_get_server_template_skeleton_fname (
    bool base_name_only = false);

  static std::string be_get_anyop_source_fname (
    bool base_name_only = false);

  static std::string be_get_anyop_header_fname (
    bool base_name_only = false);

  static std::string be_get_ciao_svnt_hdr_fname (
    bool base_name_only = false);

  static std::string be_get_ciao_tmpl_svnt_hdr_fname (
    bool base_name_only=false);

  static std::string be_get_ciao_svnt_src_fname (
    bool base_name_only = false);

  static std::string be_get_ciao_tmpl_svnt_src_fname (
    bool base_name_only=false);

  static std::string be_get_ciao_exec_hdr_fname (
    bool base_name_only = false);

  static std::string be_get_ciao_exec_src_fname (
    bool base_name_only = false);

  static std::string be_get_ciao_exec_stub_hdr_fname (
    bool base_name_only = false);

  static std::string be_get_ciao_exec_idl_fname (
    bool base_name_only = false);

  static std::string be_get_ciao_conn_hdr_fname (
    bool base_name_only = false);

  static std::string be_get_ciao_conn_src_fname (
    bool base_name_only = false);

  static std::string be_get_ciao_ami_conn_idl_fname (
    bool base_name_only = false);

  static std::string be_get_ciao_ami_conn_impl_hdr_fname (
    bool base_name_only = false);

  static std::string be_get_ciao_ami_conn_impl_src_fname (
    bool base_name_only = false);

  // Helper functions: obtain the names of each generated file given
  // the IDL file name.
  // The parameter <base_name_only> set to 0 (no base name, but full
  // name with output dir path, is useful, when I want just the
  // base name to use in #include's etc.
  static std::string be_get_client_hdr (
    std::string const& idl_file_name,
    bool base_name_only = false);

  static std::string be_get_client_stub (
    std::string const& idl_file_name);

  static std::string be_get_client_inline (
    std::string const& idl_file_name,
    bool base_name_only = false);

  static std::string be_get_server_hdr (
    std::string const& idl_file_name,
    bool base_name_only = false);

  static std::string be_get_svnt_template_hdr (
    std::string const& idl_file_name,
    bool base_name_only = false);

  static std::string be_get_implementation_hdr (
    std::string const& idl_file_name,
    bool base_name_only = false);

  static std::string be_get_implementation_skel (
    std::string const& idl_file_name,
    bool base_name_only = false);

  static std::string be_get_server_template_hdr (
    std::string const& idl_file_name,
    bool base_name_only = false);

  static std::string be_get_server_skeleton (
    std::string const& idl_file_name);

  static std::string be_get_server_template_skeleton (
      std::string const& idl_file_name,
      bool base_name_only = false);

  static std::string be_get_anyop_header (
    std::string const& idl_file_name,
    bool base_name_only = false);

  static std::string be_get_anyop_source (
      std::string const& idl_file_name,
    bool base_name_only = false);

  static std::string be_get_ciao_svnt_header (
    std::string const& idl_file_name,
    bool base_name_only = false);

  static std::string be_get_ciao_svnt_template_header (
    std::string const& idl_file_name,
    bool base_name_only = false);

  static std::string be_get_ciao_svnt_source (
    std::string const& idl_file_name,
    bool base_name_only = false);

  static std::string be_get_ciao_svnt_template_source (
    std::string const& idl_file_name,
    bool base_name_only = false);

  static std::string be_get_ciao_exec_header (
    std::string const& idl_file_name,
    bool base_name_only = false);

  static std::string be_get_ciao_exec_source (
    std::string const& idl_file_name,
    bool base_name_only = false);

  static std::string be_get_ciao_exec_stub_header (
    std::string const& idl_file_name,
    bool base_name_only = false);

  static std::string be_get_ciao_exec_idl (
    std::string const& idl_file_name,
    bool base_name_only = false);

  static std::string be_get_ciao_conn_header (
    std::string const& idl_file_name,
    bool base_name_only = false);

  static std::string be_get_ciao_conn_source (
    std::string const& idl_file_name,
    bool base_name_only = false);

  static std::string be_get_dds_typesupport_header (
    std::string const& idl_file_name,
    bool base_name_only = false);

  static std::string be_get_ciao_ami_conn_idl (
    std::string const& idl_file_name,
    bool base_name_only = false);

  static std::string be_get_ciao_ami_conn_impl_hdr (
    std::string const& idl_file_name,
    bool base_name_only = false);

  static std::string be_get_ciao_ami_conn_impl_src (
    std::string const& idl_file_name,
    bool base_name_only = false);

  /// Returns the macro name for exporting server side classes in Win32
  /// DLL.
  std::string const& skel_export_macro () const;

  /// Set the macro name for export server side classes in Win32 DLL.
  void skel_export_macro (std::string const& s);

  /// Returns the name of the include file that contains the server
  /// side export macro definition.
  std::string const& skel_export_include () const;

  /// Set the name of the include file that contains the server side
  /// export macro definition.
  void skel_export_include (std::string const& s);

  /// Returns the name of the include file that will be generated for the
  /// server side export macro definition.
  std::string const& skel_export_file () const;

  /// Set the name of the include file that the will be generated for server
  /// side export macro definition.
  void skel_export_file (std::string const& s);

  /// Returns the macro name for exporting client side classes in Win32
  /// DLL.
  std::string const& stub_export_macro () const;

  /// Set the macro name for export client side classes in Win32 DLL.
  void stub_export_macro (std::string const& s);

  /// Returns the name of the include file that contains the client
  /// side export macro definition.
  std::string const& stub_export_include () const;

  /// Set the name of the include file that contains the client side
  /// export macro definition.
  void stub_export_include (std::string const& s);

  /// Returns the name of the include file will be generated for the client
  /// side export macro definition.
  std::string const& stub_export_file () const;

  /// Set the name of the include file that will be generated for the client
  /// side export macro definition.
  void stub_export_file (std::string const& s);

  /// Returns the macro name for exporting *A.h file classes in Win32
  /// DLL.
  std::string const& anyop_export_macro () const;

  /// Set the macro name for export *A.h file classes in Win32 DLL.
  void anyop_export_macro (std::string const& s);

  /// Returns the name of the include file that contains the *A.h
  /// file export macro definition.
  std::string const& anyop_export_include () const;

  /// Set the name of the include file that contains the *A.h file
  /// export macro definition.
  void anyop_export_include (std::string const& s);

  /// Returns the macro name for exporting exec impl classes in Win32
  /// DLL.
  std::string const& exec_export_macro () const;

  /// Set the macro name for export exec impl classes in Win32 DLL.
  void exec_export_macro (std::string const& s);

  /// Returns the name of the include file that contains the exec
  /// impl export macro definition.
  std::string const& exec_export_include () const;

  /// Set the name of the include file that contains the exec impl
  /// export macro definition.
  void exec_export_include (std::string const& s);

  /// Returns the macro name for exporting CIAO svnt classes in Win32
  /// DLL.
  std::string const& svnt_export_macro () const;

  /// Set the macro name for export CIAO svnt classes in Win32 DLL.
  void svnt_export_macro (std::string const& s);

  /// Returns the name of the include file that contains the CIAO
  /// svnt export macro definition.
  std::string const& svnt_export_include () const;

  /// Set the name of the include file that contains the CIAO svnt
  /// export macro definition.
  void svnt_export_include (std::string const& s);

  /// Returns the macro name for exporting CIAO connector
  /// classes in Win32 DLL.
  std::string const& conn_export_macro () const;

  /// Set the macro name for export CIAO connector
  /// classes in Win32 DLL.
  void conn_export_macro (std::string const& s);

  /// Returns the name of the include file that contains the CIAO
  /// connector export macro definition.
  std::string const& conn_export_include () const;

  /// Set the name of the include file that contains the CIAO
  /// connector export macro definition.
  void conn_export_include (std::string const& s);

  /// Returns the name of the include file to be used for precompiled
  /// header support.
  std::string const& pch_include () const;

  /// Set the name of the include file to be used for precompiled
  /// header support.
  void pch_include (std::string const& s);

  /// Returns the name of the include file to be put at the top of
  /// every header file.
  std::string const& pre_include () const;

  /// Set the name of the include file to be put at the top of every
  /// header file.
  void pre_include (std::string const& s);

  /// Returns the name of the include file to be put at the bottom of
  /// every header file.
  std::string const& post_include () const;

  /// Set the name of the include file to be put at the bottom of every
  /// header file.
  void post_include (std::string const& s);

  /// Returns the guard that is placed in the client header file
  std::string const& include_guard () const;

  /// Set the guard that is placed in the client header file
  void include_guard (std::string const& s);

  /// Returns the name of the include file that is used instead of the own
  /// generated client header file
  std::string const& safe_include () const;

  /// set the name of the include file that is used instead of the own
  /// generated client header file
  void safe_include (std::string const& s);

  /// Returns the name of the include file that should on get
  /// generated in the client header file
  std::string const& unique_include () const;

  /// Set the name of the include file that should on get
  /// generated in the client header file
  void unique_include (std::string const& s);

  /// Returns the name of the include file that should be used as
  /// stripped_filename
  std::string const& stripped_filename () const;

  /// Set the name of the include file that should be used as stripped_filename
  void stripped_filename (std::string const& s);

  /// Set text that opens a "versioned" namespace.
  void versioning_begin (std::string const& s);

  /// Get text that opens a "versioned" namespace.
  std::string const& versioning_begin () const;

  /// Set include file for "versioned" namespace.
  void versioning_include (std::string const& s);

  /// Get include file for "versioned" namespace.
  std::string const& versioning_include () const;

  /// Get text that opens a "versioned" namespace for core TAO/orbsvcs
  /// related code.
  std::string const& core_versioning_begin () const;

  /// Set text that closes a "versioned" namespace.
  void versioning_end (std::string const& s);

  /// Get text that closes a "versioned" namespace.
  std::string const& versioning_end () const;

  /// Get text that closes a "versioned" namespace for core TAO/orbsvcs
  /// related code.
  std::string const& core_versioning_end () const;

  /// Get text that opens a "versioned" namespace for CORBA::Any operators
  std::string const& anyops_versioning_begin () const;

  /// Get text that closes a "versioned" namespace for CORBA::Any operators
  std::string const& anyops_versioning_end () const;

  // = Set and get methods for different file name endings.

  /// Set the client_hdr_ending.
  void client_hdr_ending (std::string const& s);

  /// Get the client_hdr_ending.
  std::string const& client_hdr_ending () const;

  /// Set the client_inline_ending.
  void client_inline_ending (std::string const& s);

  /// Get the client_inline_ending.
  std::string const& client_inline_ending () const;

  /// Set the client_stub_ending.
  void  client_stub_ending (std::string const& s);

  /// Get the client_stub_ending.
  std::string const& client_stub_ending () const;

  /// Set the server_hdr_ending.
  void server_hdr_ending (std::string const& s);

  /// Get the server_hdr_ending.
  std::string const& server_hdr_ending () const;

  /// Set the implementation_hdr_ending.
  void implementation_hdr_ending (std::string const& s);

  /// Set the implementation class prefix.
  void impl_class_prefix (std::string const& s);

  /// Set the implementation class suffix.
  void impl_class_suffix (std::string const& s);

  /// Set the implementation_skel_ending.
  void implementation_skel_ending (std::string const& s);

  /// Get the implementation_hdr_ending.
  std::string const& implementation_hdr_ending () const;

  ///Get implementation class prefix
  std::string const& impl_class_prefix () const;

  ///Get implementation class suffix
  std::string const& impl_class_suffix () const;

  /// Get the implementation_skel_ending.
  std::string const& implementation_skel_ending () const;

  /// Set the server_template_hdr_ending.
  void server_template_hdr_ending (std::string const& s);

  /// Get the server_template_hdr_ending.
  std::string const& server_template_hdr_ending () const;

  /// Set the server_skeleton_ending.
  void server_skeleton_ending (std::string const& s);

  /// Get the server_skeleton_ending.
  std::string const& server_skeleton_ending () const;

  /// Set the server_template_skeleton_ending.
  void server_template_skeleton_ending (std::string const& s);

  /// Get the server_template_skeleton_ending.
  std::string const& server_template_skeleton_ending () const;

  /// Set the anyop_header_ending.
  void anyop_header_ending (std::string const& s);

  /// Get the anyop_header_ending.
  std::string const& anyop_header_ending () const;

  /// Set the anyop_source_ending.
  void anyop_source_ending (std::string const& s);

  /// Get the anyop_source_ending.
  std::string const& anyop_source_ending () const;

  /// Similar to above, but for CIAO servant and executor
  /// impl and executor IDL files, if generated.
  void ciao_svnt_header_template_ending (std::string const& s);
  std::string const& ciao_svnt_header_template_ending () const;

  void ciao_svnt_source_template_ending (std::string const& s);
  std::string const& ciao_svnt_source_template_ending () const;

  void ciao_svnt_header_ending (std::string const& s);
  std::string const& ciao_svnt_header_ending () const;

  void ciao_svnt_source_ending (std::string const& s);
  std::string const& ciao_svnt_source_ending () const;

  void ciao_exec_header_ending (std::string const& s);
  std::string const& ciao_exec_header_ending () const;

  void ciao_exec_source_ending (std::string const& s);
  std::string const& ciao_exec_source_ending () const;

  void ciao_exec_stub_header_ending (std::string const& s);
  std::string const& ciao_exec_stub_header_ending () const;

  void ciao_exec_idl_ending (std::string const& s);
  std::string const& ciao_exec_idl_ending () const;

  void ciao_conn_header_ending (std::string const& s);
  std::string const& ciao_conn_header_ending () const;

  void ciao_conn_source_ending (std::string const& s);
  std::string const& ciao_conn_source_ending () const;

  /// For generating TypeSupport header file includes.
  void dds_typesupport_hdr_ending (std::string const& s);
  std::string const& dds_typesupport_hdr_ending () const;

  /// For optionally generating CIAO AMI connector IDL file.
  void ciao_ami_conn_idl_ending (std::string const& s);
  std::string const& ciao_ami_conn_idl_ending () const;

  /// For optionally generating CIAO AMI reply handler
  /// impl header file.
  void ciao_ami_conn_impl_hdr_ending (std::string const& s);
  std::string const& ciao_ami_conn_impl_hdr_ending () const;

  /// For optionally generating CIAO AMI reply handler
  /// impl source file.
  void ciao_ami_conn_impl_src_ending (std::string const& s);
  std::string const& ciao_ami_conn_impl_src_ending () const;

  /// For optionally controlling the container type, default is Session
  void ciao_container_type (std::string const& s);
  std::string const& ciao_container_type () const;

  /// Set the clonable_in_args.
  void use_clonable_in_args (bool clonable);

  /// Get the clonable_in_args setting.
  bool use_clonable_in_args () const;

  /// Get the gen_seq_template_export_ member.
  bool gen_template_export () const;

  /// Set the gen_seq_template_export_ member.
  void gen_template_export (bool val);

  /// Get the gen_ostream_operators_ member.
  bool gen_ostream_operators () const;

  /// Set the gen_ostream_operators_ member.
  void gen_ostream_operators (bool val);

  /// Get the gen_static_desc_operations_ member.
  bool gen_static_desc_operations () const;

  /// Set the gen_static_desc_operations_ member.
  void gen_static_desc_operations (bool val);


  /**
   * Set the directory where all the IDL-Compiler-Generated files are
   * to be kept. Default is current directory from which the
   * <tao_idl> is called.
   */
  void output_dir (std::string const& s);

  /**
   * Get the directory where all the IDL-Compiler-Generated files are
   * to be kept. Default is current directory from which the
   * <tao_idl> is called.
   */
  std::string const& output_dir () const;

  /// Set the path for all *C.* file includes.
  /// Default is local directory or $TAO_ROOT/tao.
  void stub_include_dir (std::string const& s);

  /// Get the path for all *C.* file includes.
  /// Default is local directory or $TAO_ROOT/tao.
  std::string const& stub_include_dir () const;

  /// Set the directory where all the *S.* files are
  /// to be kept. Default is output_dir_.
  void skel_output_dir (std::string const& s);

  /// Get the directory where all the *S.* files are
  /// to be kept. Default is output_dir_.
  std::string const& skel_output_dir () const;

  /// Set the directory where all the *A.* files are
  /// to be kept. Default is output_dir_.
  void anyop_output_dir (std::string const& s);

  /// Get the directory where all the *A.* files are
  /// to be kept. Default is output_dir_.
  std::string const& anyop_output_dir () const;

  /// Set the directory where all the *exec.* files are
  /// to be kept. Default is output_dir_.
  void exec_output_dir (std::string const& s);

  /// Get the directory where all the *exec.* files are
  /// to be kept. Default is output_dir_.
  std::string const& exec_output_dir () const;

  ///Get the flag for not overwriting already existing exec files.
  bool overwrite_not_exec () const;

  ///Set the flag for not overwriting already existing exec files.
  void overwrite_not_exec (bool val);

  /// Set any support.
  void any_support (bool);

  /// Check Any support.
  bool any_support () const;

  /// Set cdr support.
  void cdr_support (bool);

  /// Check cdr support.
  bool cdr_support () const;

  /// Set TypeCode support.
  void tc_support (bool);

  /// Check TypeCode support
  bool tc_support () const;

  /// Set optimized valuetype member accessor generation.
  void obv_opt_accessor (bool);

  /// Check optimized valuetype member accessor generation.
  bool obv_opt_accessor () const;

  /// Set generation of implementation files.
  void gen_impl_files (bool);

  /// Check if we want to generate implementation files.
  bool gen_impl_files () const;

  /// Set generation of source file and line number for implementation files.
  void gen_impl_debug_info (bool);

  /// Check generation of source file and line number implementation files.
  bool gen_impl_debug_info () const;

  /// Set generation of copy constructor.
  void gen_copy_ctor (bool);

  /// Check if we want to generate the copy constructor.
  bool gen_copy_ctor () const;

  /// Set the generation of the assignment operator.
  void gen_assign_op (bool);

  /// Check if we want to generate the assignment operator.
  bool gen_assign_op () const;

  /// Set whether we want to generate Thru_POA collocation stubs.
  void gen_thru_poa_collocation (bool);

  /// Check if we want to generate Thru_POA collocation stubs.
  bool gen_thru_poa_collocation () const;

  /// Set whether we want to generate Direct collocation stubs.
  void gen_direct_collocation (bool);

  /// Check if we want to generate Direct collocation stubs.
  bool gen_direct_collocation () const;

  /// Set whether we want to generate for CORBA/e
  void gen_corba_e (bool);

  /// Check if we want to generate for CORBA/e
  bool gen_corba_e () const;

  /// Set whether we want to generate for Minimum CORBA
  void gen_minimum_corba (bool);

  /// Check if we want to generate for Minimum CORBA
  bool gen_minimum_corba () const;

  /// Set whether we want to generate for noeventCCM
  void gen_noeventccm (bool);

  /// Check if we want to generate for noeventCCM
  bool gen_noeventccm () const;

  /// Set whether we want to generate for LwCCM
  void gen_lwccm (bool);

  /// Check if we want to generate for LwCCM
  bool gen_lwccm () const;

  /// Set optimized typecodes.
  void opt_tc (bool);

  /// Check if TypeCodes are optimized.
  bool opt_tc () const;

  /// To enable or disable AMI4CCM call back feature
  void ami4ccm_call_back (bool value);

  /// Return the flag.
  bool ami4ccm_call_back () const;

  /// To enable or disable AMI call back feature of the Messaging
  /// specification in the generated code.
  void ami_call_back (bool value);

  /// Return the flag.
  bool ami_call_back () const;

  /// To enable or disable AMH in the generated code.
  void gen_amh_classes (bool value);

  /// Return the flag.
  bool gen_amh_classes () const;

  /// Set the generation of tie classes and files.
  void gen_tie_classes (bool value);

  /// Return the flag.
  bool gen_tie_classes () const;

  /// To enable or disable AMI call back feature of the Messaging
  /// specification in the generated code.
  void gen_smart_proxies (bool value);

  /// Return the flag.
  bool gen_smart_proxies () const;

  /// Set the flag.
  void gen_inline_constants (bool value);

  /// Return the flag.
  bool gen_inline_constants () const;

  /// Set the flag.
  void gen_orb_h_include (bool value);

  /// Return the flag.
  bool gen_orb_h_include () const;

  /// Generate an empty anyop header (A.h)
  bool gen_empty_anyop_header () const;

  /// Generate an empty anyop header (A.h)
  void gen_empty_anyop_header (bool value);

  /// Set the lookup strategy.
  void lookup_strategy (LOOKUP_STRATEGY s);

  /// Return the enumerated value for the lookup strategy. Default is
  /// perfect hashing.
  LOOKUP_STRATEGY lookup_strategy () const;

  /// Set the DDS implementation.
  void dds_impl (std::string const& val);

  /// Return the enumerated value for the DDS implementation.
  /// Default is NDDS.
  DDS_IMPL dds_impl () const;

  /// Cleanup function.
  void destroy ();

  /// Used in the generation of extern function declarations so we
  /// can use non-defined interfaces as members and parameters.
  std::deque<be_interface_fwd *> non_defined_interfaces;

  /// Accessor for the member, sets it on the first call.
  AST_PredefinedType *void_type ();

  /// Accessor for the member, sets it on the first call.
  be_interface *ccmobject ();

  /// Accessor for the member, sets it on the first call.
  be_module *messaging ();

  /// Accessor for the member, sets it on the first call.
  be_valuetype *messaging_exceptionholder ();

  /// Accessor for the member, sets it on the first call.
  be_interface *messaging_replyhandler ();

  /// Accessors for the member.
  bool gen_anyop_files () const;
  void gen_anyop_files (bool val);

  /// Accessors for the member gen_skel_files_.
  bool gen_skel_files () const;
  void gen_skel_files (bool val);

  /// Accessors for the member gen_svnt_cpp_files_.
  bool gen_svnt_cpp_files () const;
  void gen_svnt_cpp_files (bool val);

  /// Accessors for the member gen_svnt_t_files_.
  bool gen_svnt_t_files () const;
  void gen_svnt_t_files (bool val);

  /// Accessors for the member gen_client_inline_.
  bool gen_client_inline () const;
  void gen_client_inline (bool val);

  /// Accessors for the member gen_client_stub_.
  bool gen_client_stub () const;
  void gen_client_stub (bool val);

  /// Accessors for the member gen_client_header_.
  bool gen_client_header () const;
  void gen_client_header (bool val);

  /// Accessors for the member gen_server_skeleton_.
  bool gen_server_skeleton () const;
  void gen_server_skeleton (bool val);

  /// Accessors for the member gen_server_header_.
  bool gen_server_header () const;
  void gen_server_header (bool val);

  /// Accessors for the member gen_local_iface_anyops_.
  bool gen_local_iface_anyops () const;
  void gen_local_iface_anyops (bool val);

  /// Accessors for the member gen_custom_ending_.
  bool gen_custom_ending () const;
  void gen_custom_ending (bool val);

  /// Accessors for the member gen_unique_guards_.
  bool gen_unique_guards () const;
  void gen_unique_guards (bool val);

  /// Accessors for the corresponding members.

  bool gen_ciao_svnt () const;
  void gen_ciao_svnt (bool val);

  bool gen_ciao_exec_idl () const;
  void gen_ciao_exec_idl (bool val);

  bool gen_ciao_exec_impl () const;
  void gen_ciao_exec_impl (bool val);

  bool gen_ciao_exec_reactor_impl () const;
  void gen_ciao_exec_reactor_impl (bool val);

  bool gen_ciao_conn_impl () const;
  void gen_ciao_conn_impl (bool val);

  bool gen_dds_typesupport_idl () const;
  void gen_dds_typesupport_idl (bool val);

  bool gen_ciao_valuefactory_reg () const;
  void gen_ciao_valuefactory_reg (bool val);

  bool gen_stub_export_hdr_file () const;
  void gen_stub_export_hdr_file (bool val);

  bool gen_skel_export_hdr_file () const;
  void gen_skel_export_hdr_file (bool val);

  bool gen_svnt_export_hdr_file () const;
  void gen_svnt_export_hdr_file (bool val);

  bool gen_exec_export_hdr_file () const;
  void gen_exec_export_hdr_file (bool val);

  bool gen_conn_export_hdr_file () const;
  void gen_conn_export_hdr_file (bool val);

  bool alt_mapping () const;
  void alt_mapping (bool val);

  bool in_facet_servant () const;
  void in_facet_servant (bool val);

  bool gen_arg_traits () const;
  void gen_arg_traits (bool val);

  bool gen_anytypecode_adapter () const;
  void gen_anytypecode_adapter (bool val);

  bool no_fixed_err () const;
  void no_fixed_err (bool val);

  unsigned long tab_size () const;
  void tab_size (unsigned long val);

  /// Command line passed to ACE_Process::spawn. Different
  /// implementations in IDL and IFR backends.
  std::string spawn_options () const;

  /**
   * Parse an argument that the frontend did not recognize.
   */
  void parse_args (long &i, char **av);

private:
  /**
   * To switch between changing or non-changing standard include
   * files (e.g. tao/corba.h)  so that #include statements can be
   * generated with ""s or <>s respectively.
   */
  size_t changing_standard_include_files_ {1};

  // Macros and includes used to export classes from generated code.
  std::string skel_export_macro_;
  std::string skel_export_include_;
  std::string skel_export_file_;
  std::string stub_export_macro_;
  std::string stub_export_include_;
  std::string stub_export_file_;

  // Macro and include used on ORB .pidl files generating to the
  // AnyTypeCode library.
  std::string anyop_export_macro_;
  std::string anyop_export_include_;

  std::string exec_export_macro_;
  std::string exec_export_include_;
  std::string svnt_export_macro_;
  std::string svnt_export_include_;
  std::string conn_export_macro_;
  std::string conn_export_include_;

  std::string pch_include_;
  std::string pre_include_;
  std::string post_include_;
  std::string include_guard_;
  std::string safe_include_;
  std::string unique_include_;
  std::string stripped_filename_;

  /// Text that opens a "versioned" namespace for core TAO and orbsvcs
  /// related code.
  std::string core_versioning_begin_;

  /// Text that closes a "versioned" namespace for core TAO and
  /// orbsvcs related code.
  std::string core_versioning_end_;

  /// Text that opens a "versioned" namespace for CORBA::Any operators
  std::string anyops_versioning_begin_;

  /// Text that closes a "versioned" namespace for CORBA::Any operators
  std::string anyops_versioning_end_;

  /// Text that opens a "versioned" namespace.
  std::string versioning_begin_;

  /// Text that closes a "versioned" namespace.
  std::string versioning_end_;

  /// Include for versioned namespace include
  std::string versioning_include_;

  /// Client's header file name ending. Default is "C.h".
  std::string client_hdr_ending_ {"C.h"};

  /// Client's stub's file name ending. Default is "C.cpp".
  std::string client_stub_ending_ {"C.cpp"};

  /// Client's inline file name ending. Default is "C.i".
  std::string client_inline_ending_ {"C.inl"};

  /// Server's hdr file name ending. Default is "S.h".
  std::string server_hdr_ending_ {"S.h"};

  /// Implementation's hdr file name ending. Default is "I.h".
  std::string implementation_hdr_ending_ {"I.h"};

  // Implementation's skeleton file name ending. Default is "I.cpp".
  std::string implementation_skel_ending_ {"I.cpp"};

  /// Implementaion class prefix
  std::string impl_class_prefix_;

  /// Implementation class suffix
  std::string impl_class_suffix_ {"_i"};

  /// Server's template hdr file name ending. Default is "S_T.h".
  std::string server_template_hdr_ending_ {"S_T.h"};

  /// Server's skeleton file name ending. Default is "S.cpp".
  std::string server_skeleton_ending_ {"S.cpp"};

  /// Server's template skeleton file name ending. Default is
  /// "S_T.cpp".
  std::string server_template_skeleton_ending_ {"S_T.cpp"};

  /// Anyop header file name ending. Default is "A.h".
  std::string anyop_hdr_ending_ {"A.h"};

  /// Anyop source file name ending. Default is "A.cpp".
  std::string anyop_src_ending_ {"A.cpp"};

  /// CIAO servant template header file name ending. Default is "_svnt_T.h".
  std::string ciao_svnt_hdr_template_ending_ {"_svnt_T.h"};

  /// CIAO servant template source file name ending. Default is "_svnt_T.cpp".
  std::string ciao_svnt_src_template_ending_ {"_svnt_T.cpp"};

  /// CIAO servant header file name ending. Default is "_svnt.h".
  std::string ciao_svnt_hdr_ending_ {"_svnt.h"};

  /// CIAO servant source file name ending. Default is "_svnt.cpp".
  std::string ciao_svnt_src_ending_ {"_svnt.cpp"};

  /// CIAO executor impl header file name ending. Default is "_exec.h".
  std::string ciao_exec_hdr_ending_ {"_exec.h"};

  /// CIAO executor impl source file name ending. Default is "_exec.cpp".
  std::string ciao_exec_src_ending_ {"_exec.cpp"};

  /// CIAO executor stub header file name ending. Default is "EC.h".
  std::string ciao_exec_stub_hdr_ending_ {"EC.h"};

  /// CIAO executor IDL file name ending. Default is "E.idl".
  std::string ciao_exec_idl_ending_ {"E.idl"};

  /// CIAO connector impl header file name ending. Default is "_conn.h".
  std::string ciao_conn_hdr_ending_ {"_conn.h"};

  /// CIAO connector impl source file name ending. Default is "_conn.cpp".
  std::string ciao_conn_src_ending_ {"_conn.cpp"};

  /// DDS TypeSupport header file name ending. Default is "Support.h".
  std::string dds_typesupport_hdr_ending_ {"Support.h"};

  /// CIAO AMI connector IDL file name ending. Default is "A.idl".
  std::string ciao_ami_conn_idl_ending_ {"A.idl"};

  /// CIAO AMI reply handler impl header file name ending.
  /// Default is "A_impl.h".
  std::string ciao_ami_conn_impl_hdr_ending_ {"_conn_i.h"};

  /// CIAO AMI reply handler impl source file name ending.
  /// Default is "A_impl.cpp".
  std::string ciao_ami_conn_impl_src_ending_ {"_conn_i.cpp"};

  /// CIAO container type
  /// Default is "Session".
  std::string ciao_container_type_ {"Session"};

  /**
   * Directory where all the IDL-Compiler-Generated files are to be
   * kept. Default value is 0 for this string which means the current
   * directory from which the <tao_idl> is called.
   */
  std::string output_dir_;

  /**
   * Path for all *C.* includes. Default value is 0, in which case
   * the local directory or $TAO_ROOT/tao is used.
   */
  std::string stub_include_dir_;

  /**
   * Directory where all the *S.* files are to be
   * kept. Default value is 0 for this string which means the
   * value for output_dir_ is used.
   */
  std::string skel_output_dir_;

  /**
   * Directory where all the *A.* files are to be
   * kept. Default value is 0 for this string which means the
   * value for output_dir_ is used.
   */
  std::string anyop_output_dir_;

  /**
    * Directory where all the *exec.* files are to be
    * kept. Default value is 0 for this string which means the
    * value for output_dir_ is used.
    */
  std::string exec_output_dir_;


  /// do we support Any operators?
  bool any_support_ {true};

  /// do we support cdr?
  bool cdr_support_ {true};

  /// do we support typecodes?
  bool tc_support_ {true};

  /// do we optimize valuetype accessors?
  bool obv_opt_accessor_ {};

  /// are we generating implementation files?
  bool gen_impl_files_ {};

  /// are we generating source file and line numbers for implementation files?
  bool gen_impl_debug_info_ {};

  /// are we generating the copy constructor?
  bool gen_copy_ctor_ {};

  /// are we generating the assignment operator?
  bool gen_assign_op_ {};

  /// are we generating Thru_POA collocated stubs?
  bool gen_thru_poa_collocation_ {true};

  /// are we generating Direct collocated stubs?
  bool gen_direct_collocation_ {};

  /// are we generating for CORBA/e
  bool gen_corba_e_ {};

  /// are we generating for Minimum CORBA
  bool gen_minimum_corba_ {};

  /// are we generating for LwCCM
  bool gen_lwccm_ {};

  /// are we generating for noeventCCM
  bool gen_noeventccm_ {};

  /// do we generate optimized typecodes?
  bool opt_tc_ {};

  /// Flag to indicate whether the AMI4ccm Call back feature should be enabled
  /// for the generated files or not.
  bool ami4ccm_call_back_ {};

  /**
   * Flag to indicate whether the AMI Call back feature of the
   * Messaging specification should be enabled for the generated files
   * or not.
   */
  bool ami_call_back_ {};

  /// Flag for generating AMH classes.
  bool gen_amh_classes_ {};

  /// Flag to indicate whether we generate the tie classes and
  /// files or not.
  bool gen_tie_classes_ {};

  /// Flag to indicate whether smart proxies classes will be generated
  /// or not.
  bool gen_smart_proxies_ {};

  /// Flag to indicate whether we are using an inline form of constant
  /// generation that pleases the C++ compiler better on some platforms.
  bool gen_inline_constants_ {true};

  /// Flag to indicate whether ORB.h include should be generated, needed for
  /// regenerating the pidl files.
  bool gen_orb_h_include_ {true};

  /// Flag to indicate whether an empty A.h should be generated
  bool gen_empty_anyop_header_ {};

  /// The enumerated value indicating the lookup strategy.
  LOOKUP_STRATEGY lookup_strategy_ {TAO_PERFECT_HASH};

  /// The enumerated value indicating the DDS implementation.
  DDS_IMPL dds_impl_ {DDS_NONE};

  /// Used for void operation return types.
  AST_PredefinedType *void_type_ {};

  /// Reference holder for component skeleton visitors.
  be_interface *ccmobject_ {};

  /// Storage for the messaging exceptions holder's virtual scope.
  be_module *messaging_ {};

  /// Storage for exception holders' base class node.
  be_valuetype *messaging_exceptionholder_ {};

  /// Storage for the reply handlers' base class node.
  be_interface *messaging_replyhandler_ {};

  /// Separate files for generated Any operators?
  bool gen_anyop_files_ {};

  /// True by default, but a command line option can turn this off so that
  /// we don't generate a S.cpp file
  bool gen_skel_files_ {true};

  /// True by default, but a command line option can turn this of so that
  /// we don't generate a svnt.cpp file
  bool gen_svnt_cpp_files_ {true};

  /// True by default, but a command line option can turn this of so that
  /// we don't generate a svnt_T.cpp file
  bool gen_svnt_t_files_ {true};

  /// True by default, but a command line option can turn this off so
  /// that we don't generate a client inline file
  bool gen_client_inline_ {true};

  /// True by default, but a command line option can turn this off so
  /// that we don't generate a client stub file
  bool gen_client_stub_ {true};

  /// True by default, but a command line option can turn this off so
  /// that we don't generate a client header file
  bool gen_client_header_ {true};

  /// True by default, but a command line option can turn this off so
  /// that we don't generate a server skeleton file
  bool gen_server_skeleton_ {true};

  /// True by default, but a command line option can turn this off so
  /// that we don't generate a server header file
  bool gen_server_header_ {true};

  /// True by default, can be turned off by a command line option
  /// for regenerating internal ORB files.
  bool gen_local_iface_anyops_ {true};

  /// Use in_clonable_arg_val instead of in_arg_val?
  bool use_clonable_in_args_ {};

  /// Generate explicit export for Visual Studio bug workaround,
  /// needed only in TAO basic sequence *C.h files.
  bool gen_template_export_ {};

  /// Generate ostream operators for each type declaration, for
  /// debugging or logging.
  bool gen_ostream_operators_ {};

  /// Generate static description operations for each interface
  bool gen_static_desc_operations_ {};

  /**
   * True by default, but a command line option can turn this off so
   * custom ending will not be applied to files in $TAO_ROOT/,
   * $TAO_ROOT/tao, $TAO_ROOT/orbsvcs, $TAO_ROOT/CIAO, $TAO_ROOT/CIAO/ciao.
   */
  bool gen_custom_ending_ {true};

 /**
   * True by default, but a command line option can turn this off so
   * the generated code (header) files will not be proteced by unique guards.
   */
  bool gen_unique_guards_ {true};

  /// False by default, these flags trigger code generation
  bool gen_ciao_svnt_ {};
  bool gen_ciao_exec_idl_ {};
  bool gen_ciao_exec_impl_ {};
  bool gen_ciao_exec_reactor_impl_ {};

  ///Flag to indicate whether generated exec files should
   ///overwritten already existing exe files
   bool overwrite_not_exec_ {};

  /// False by default, this flag triggers code generation
  /// for CCM connector implementations.
  bool gen_ciao_conn_impl_ {};

  /// Used for DDS implementations other than OpenDDS.
  bool gen_dds_typesupport_idl_ {};

  /// Generate automatic valuetype factory registration in
  /// CIAO servants.
  bool gen_ciao_valuefactory_reg_ {true};

  /// Flags toggling export header file generation.
  bool gen_stub_export_hdr_file_ {};
  bool gen_skel_export_hdr_file_ {};
  bool gen_svnt_export_hdr_file_ {};
  bool gen_exec_export_hdr_file_ {};
  bool gen_conn_export_hdr_file_ {};

  /// 2 spaces by default, can be set from the command line.
  unsigned int tab_size_ {2};

  /// Are we generating STL types?
  bool alt_mapping_ {};

  /// Are we in the act of generating a facet servant?
  bool in_facet_servant_ {};

  /// Are we generating arg traits template instantiations?
  bool gen_arg_traits_ {true};

  /// Generating arg traits in the ORB for the basic type
  /// sequences requires the AnyTypeCode_Adapter class.
  bool gen_anytypecode_adapter_ {};

  /// Don't generate an error for usage of the fixed data type
  bool no_fixed_err_ {};
};

#endif /* _BE_GLOBAL_H */
