/* -*- c++ -*- */
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

class be_interface;
class be_interface_fwd;

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
    TAO_BINARY_SEARCH,
    TAO_ACTIVE_DEMUX,
    TAO_USER_DEFINED
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
  static const char  *be_get_client_hdr_fname (int base_name_only = 0);
  static const char  *be_get_client_stub_fname (void);
  static const char  *be_get_client_inline_fname (int base_name_only = 0);
  static const char  *be_get_server_hdr_fname (int base_name_only = 0);
  static const char  *be_get_implementation_hdr_fname (int base_name_only = 0);
  static const char  *be_get_implementation_skel_fname (int base_name_only = 0);
  static const char  *be_get_server_template_hdr_fname (int base_name_only = 0);
  static const char  *be_get_server_skeleton_fname (void);
  static const char  *be_get_implementation_skeleton_fname (void);
  //  static const char  *be_get_implementation_hdr_fname (void);
  static const char  *be_get_server_template_skeleton_fname (int base_name_only = 0);
  static const char  *be_get_server_inline_fname (int base_name_only = 0);
  static const char  *be_get_server_template_inline_fname (int base_name_only = 0);

  // Helper functions: obtain the names of each generated file given
  // the IDL file name.
  // The parameter <base_name_only> set to 0 (no base name, but full
  // name with output dir path, is useful, when I want just the
  // base name to use in #include's etc.
  static const char *be_get_client_hdr (UTL_String *idl_file_name,
                                        int base_name_only = 0);
  static const char *be_get_client_stub (UTL_String *idl_file_name);
  static const char *be_get_client_inline (UTL_String *idl_file_name,
                                           int base_name_only = 0);
  static const char *be_get_server_hdr (UTL_String *idl_file_name,
                                        int base_name_only = 0);
  static const char *be_get_implementation_hdr (UTL_String *idl_file_name,
                                                int base_name_only = 0);
  static const char *be_get_implementation_skel (UTL_String *idl_file_name,
                                                 int base_name_only = 0);
  static const char *be_get_server_template_hdr (UTL_String *idl_file_name,
                                                 int base_name_only = 0);
  static const char *be_get_server_skeleton (UTL_String *idl_file_name);
  static const char *be_get_server_template_skeleton (UTL_String *idl_file_name,
                                                      int base_name_only = 0);
  static const char *be_get_server_inline (UTL_String *idl_file_name,
                                           int base_name_only = 0);
  static const char *be_get_server_template_inline (UTL_String *idl_file_name,
                                                    int base_name_only = 0);

  virtual const char* skel_export_macro (void) const;
  // returns the macro name for exporting server side classes in Win32
  // DLL.

  virtual void skel_export_macro (const char* s);
  // set the macro name for export server side classes in Win32 DLL.

  virtual const char* skel_export_include (void) const;
  // returns the name of the include file that contains the server
  // side export macro definition.

  virtual void skel_export_include (const char* s);
  // set the name of the include file that contains the server side
  // export macro definition.

  virtual const char* stub_export_macro (void) const;
  // returns the macro name for exporting client side classes in Win32
  // DLL.

  virtual void stub_export_macro (const char* s);
  // set the macro name for export client side classes in Win32 DLL.

  virtual const char* stub_export_include (void) const;
  // returns the name of the include file that contains the client
  // side export macro definition.

  virtual void stub_export_include (const char* s);
  // set the name of the include file that contains the client side
  // export macro definition.

  virtual const char* pch_include (void) const;
  // returns the name of the include file to be used for precompiled
  // header support.

  virtual void pch_include (const char* s);
  // set the name of the include file to be used for precompiled
  // header support.

  virtual const char* pre_include (void) const;
  // returns the name of the include file to be put at the top of
  // every header file.

  virtual void pre_include (const char* s);
  // set the name of the include file to be put at the top of every
  // header file.

  virtual const char* post_include (void) const;
  // returns the name of the include file to be put at the bottom of
  // every header file.

  virtual void post_include (const char* s);
  // set the name of the include file to be put at the bottom of every
  // header file.

  // = Set and get methods for different file name endings.

  virtual void client_hdr_ending (const char* s);
  // Set the client_hdr_ending.

  virtual const char* client_hdr_ending (void) const;
  // Get the client_hdr_ending.

  virtual void client_inline_ending (const char* s);
  // Set the client_inline_ending.

  virtual const char* client_inline_ending (void) const;
  // Get the client_inline_ending.

  virtual void  client_stub_ending (const char* s);
  // Set the client_stub_ending.

  virtual const char* client_stub_ending (void) const;
  // Get the client_stub_ending.

  virtual void server_hdr_ending (const char* s);
  // Set the server_hdr_ending.

  virtual const char* server_hdr_ending (void) const;
  // Get the server_hdr_ending.

  virtual void implementation_hdr_ending (const char* s);
  // Set the implementation_hdr_ending.

  virtual void impl_class_prefix (const char* s);
  // Set the implementation class prefix.

  virtual void impl_class_suffix (const char* s);
  // Set the implementation class suffix.

  virtual void implementation_skel_ending (const char* s);
  // Set the implementation_skel_ending.

  virtual const char* implementation_hdr_ending (void) const;
  // Get the implementation_hdr_ending.

  virtual const char* impl_class_prefix (void) const;
  //Get implementation class prefix

  virtual const char* impl_class_suffix (void) const;
  //Get implementation class suffix

  virtual const char* implementation_skel_ending (void) const;
  // Get the implementation_skel_ending.

  virtual void server_template_hdr_ending (const char* s);
  // Set the server_template_hdr_ending.

  virtual const char* server_template_hdr_ending (void) const;
  // Get the server_template_hdr_ending.

  virtual void server_skeleton_ending (const char* s);
  // Set the server_skeleton_ending.

  virtual const char* server_skeleton_ending (void) const;
  // Get the server_skeleton_ending.

  virtual void server_template_skeleton_ending (const char* s);
  // Set the server_template_skeleton_ending.

  virtual const char* server_template_skeleton_ending (void) const;
  // Get the server_template_skeleton_ending.

  virtual void server_inline_ending (const char* s);
  // Set the server_inline_ending.

  virtual const char* server_inline_ending (void) const;
  // Get the server_inline_ending.

  virtual void server_template_inline_ending (const char* s);
  //  Set the server_template_inline_ending.

  virtual const char* server_template_inline_ending (void) const;
  // Get the server_template_inline_ending.

  virtual void output_dir (const char* s);
  // Set the directory where all the IDL-Compiler-Generated files are
  // to be kept. Default  is current directory from which the
  // <tao_idl> is called.

  virtual const char* output_dir (void) const;
  // Get the directory where all the IDL-Compiler-Generated files are
  // to be kept. Default  is current directory from which the
  // <tao_idl> is called.

  virtual void any_support (idl_bool);
  // enable suppressing any support    ?T.Kuepper: suppressed?

  virtual idl_bool any_support (void);
  // check if Any support is suppressed  ? enabled ?

  virtual void tc_support (idl_bool);
  // enable suppressing TypeCode support

  virtual idl_bool tc_support (void);
  // check if TypeCode support is suppressed

#ifdef IDL_HAS_VALUETYPE
  virtual void obv_opt_accessor (idl_bool);
  virtual idl_bool obv_opt_accessor (void);
#endif /* IDL_HAS_VALUETYPE */

  virtual void gen_impl_files (idl_bool);
  //enable generation of implementation files

  virtual idl_bool gen_impl_files (void);
  // check if we want to generate implementation files

  virtual void gen_copy_ctor (idl_bool);
  //enable generation of copy constructor

  virtual idl_bool gen_copy_ctor (void);
  //check if we want to generate the copy constructor

  virtual void gen_assign_op (idl_bool);
  //enable the generation of the assignment operator

  virtual idl_bool gen_assign_op (void);
  //check if we want to generate the assignment operator

  virtual void gen_thru_poa_collocation (idl_bool);
  // set whether we want to generate Thru_POA collocation stubs.

  virtual idl_bool gen_thru_poa_collocation (void);
  // check if we want to generate Thru_POA collocation stubs.

  virtual void gen_direct_collocation (idl_bool);
  // set whether we want to generate Direct collocation stubs.

  virtual idl_bool gen_direct_collocation (void);
  // check if we want to generate Direct collocation stubs.

  virtual void exception_support (idl_bool);
  // enable real C++ exceptions

  virtual idl_bool exception_support (void);
  // check if real C++ exception support is to be enabled

  virtual void use_raw_throw (idl_bool);
  // enable replacement of 'ACE_THROW_SPEC' with 'throw'.

  virtual idl_bool use_raw_throw (void);
  // check if raw 'throw' generation option is set.

  virtual void opt_tc (idl_bool);
  // enable optimized typecodes

  virtual idl_bool opt_tc (void);
  // check if TypeCodes need be optimized

  virtual void ami_call_back (idl_bool value);
  // To enable or disable AMI call back feature of the Messaging
  // specification in the generated code.

  virtual idl_bool ami_call_back (void);
  // Return the flag.

  virtual void gen_amh_classes (idl_bool value);
  // To enable or disable AMH in the generated code.

  virtual idl_bool gen_amh_classes (void);
  // Return the flag.

  virtual void gen_tie_classes (idl_bool value);
  // Toggle the generation of tie classes and files.

  virtual idl_bool gen_tie_classes (void);
  // Return the flag.

  virtual void gen_smart_proxies (idl_bool value);
  // To enable or disable AMI call back feature of the Messaging
  // specification in the generated code.

  virtual idl_bool gen_smart_proxies (void);
  // Return the flag.

  void lookup_strategy (LOOKUP_STRATEGY s);
  // Set the lookup strategy.

  BE_GlobalData::LOOKUP_STRATEGY lookup_strategy (void) const;
  // Return the enumerated value for the lookup strategy. Default is
  // perfect hashing.

  virtual void destroy (void);
  // Cleanup function.

  ACE_Unbounded_Queue<be_interface *> non_local_interfaces;
  // Used in the generation of declarations for
  // xxx_Proxy_Broker_Factory_function_pointer.

  ACE_Unbounded_Queue<be_interface_fwd *> non_defined_interfaces;
  // Used in the generation of extern function declarations so we
  // can use non-defined interfaces as members and parameters.

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

  char* pch_include_;
  char* pre_include_;
  char* post_include_;

  // Client's header file name ending. Default is "C.h".
  char*  client_hdr_ending_;

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

  // Server's template inline file name ending. Default is "S_T.i".
  char* server_template_inline_ending_;

  char* output_dir_;
  // Directory where all the IDL-Compiler-Generated files are to be
  // kept. Default value is 0 for this string which means the current
  // directory from which the <tao_idl> is called.

  idl_bool any_support_;
  // do we support Any operators?

  idl_bool tc_support_;
  // do we support typecodes?

  idl_bool obv_opt_accessor_;
  // do we optimize valuetype accessors?

  idl_bool gen_impl_files_;
  // are we generating implementation files?

  idl_bool gen_copy_ctor_;
  // are we generating the copy constructor?

  idl_bool gen_assign_op_;
  // are we generating the assignment operator?

  idl_bool gen_thru_poa_collocation_;
  // are we generating Thru_POA collocated stubs?

  idl_bool gen_direct_collocation_;
  // are we generating Direct collocated stubs?

  idl_bool exception_support_;
  // do we support real C++ exceptions (strict mapping) for stubs/skeletons?

  idl_bool use_raw_throw_;
  // Another possible option if the above is TRUE.

  idl_bool opt_tc_;
  // do we generate optimized typecodes?

  idl_bool ami_call_back_;
  // Flag to indicate whether the AMI Call back feature of the
  // Messaging specification should be enabled for the generated files
  // or not.

  idl_bool gen_amh_classes_;
  // Flag for generating AMH classes.

  idl_bool gen_tie_classes_;
  // Flag to indicate whether we generate the tie classes and
  // files or not.

  idl_bool gen_smart_proxies_;
  // Flag to indicate whether smart proxies classes will be generated
  // or not.

  LOOKUP_STRATEGY lookup_strategy_;
  // The enumerated value indicating the lookup strategy.
};

#endif /* _BE_GLOBAL_H */
