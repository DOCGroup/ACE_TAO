// -*- C++ -*-

//=============================================================================
/**
 *  @file    be_codegen.h
 *
 *  $Id$
 *
 * The Code generator class
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#ifndef TAO_BE_CODEGEN_H
#define TAO_BE_CODEGEN_H

#include "ace/Singleton.h"
#include "ace/Synch_Traits.h"
#include "ace/Unbounded_Queue.h"

#include "TAO_IDL_BE_Export.h"

class TAO_OutStream;
class be_visitor_context;
class be_visitor;
class be_decl;

/**
 * @class TAO_CodeGen
 *
 * @brief TAO_CodeGen
 *
 * Holds global parameters for the Back End and
 * generates the C++ mapping.
 */
class TAO_IDL_BE_Export TAO_CodeGen
{
public:

  // Define all the code generation states.
  // The first letter C/S of the suffix stands for client/server-side
  // The second letter H/I/S stands for header/inline/impl file.
  // These are used to denote the state or the context of code
  // generation we are in and serves to produce the right kind of
  // visitor for us.

  enum CG_STATE
    {
      // Initial state.
      TAO_INITIAL,

      // Rest of the state in alphabetical order of the types.

      // Emitting code for arguments of an operation. No distinction between
      // headers, inlines, stubs.
      TAO_ARGUMENT_COLLOCATED_UPCALL_SS,        // passing argument
      // variable to upcall

      // Emitting code for array defn.
      TAO_ARRAY_CH,

      // Emitting code for exceptions.
      TAO_EXCEPTION_CTOR_CH,
      TAO_EXCEPTION_CTOR_CS,

      // Emitting code for the interface.
      TAO_INTERFACE_CH,
      TAO_INTERFACE_SMART_PROXY_CH,
      TAO_INTERFACE_SMART_PROXY_CS,

      TAO_INTERFACE_DIRECT_PROXY_IMPL_SH,
      TAO_INTERFACE_DIRECT_PROXY_IMPL_SS,

      // Emitting code for the AMH ResponseHandlers.
      TAO_INTERFACE_AMH_RH_SH,
      TAO_INTERFACE_AMH_RH_SS,

      // Emitting code for the valuetype.
      TAO_VALUETYPE_OBV_CH,                 // OBV_ class

      // Emitting code for the module,
      TAO_MODULE_OBV_CH,                    // for OBV_ (cmp. POA_ namespace)
      TAO_MODULE_OBV_CI,
      TAO_MODULE_OBV_CS,

      // Emitting code for an operation.
      TAO_OPERATION_CH,                       // in client header
      TAO_OPERATION_CS,                       // in client stubs

      TAO_OPERATION_ARGLIST_CH,               // parameter list in op signature
      TAO_OPERATION_ARGLIST_SH,               // ... for server header
      TAO_TIE_OPERATION_ARGLIST_SH,           // ... for TIE class header

      TAO_OPERATION_ARGLIST_PROXY_IMPL_XS,

      TAO_OPERATION_ARGLIST_IH,               // ... for implementation header
      TAO_OPERATION_ARGLIST_IS,               // ... for implementation header
      TAO_OPERATION_ARGLIST_COLLOCATED_SH,    // ... for collocated server
                                              //   variable to do_static_call
      TAO_OPERATION_ARG_INVOKE_CS,            // passing argument variable to do_static_call
                                              //   after do_static_call
      TAO_OPERATION_ARG_DECL_SS,              // argument decl in skeleton
      TAO_OPERATION_ARG_DEMARSHAL_SS,         //   and argument variables to the
                                              // variable
      TAO_OPERATION_ARG_UPCALL_SS,            // variables to upcall
      TAO_OPERATION_COLLOCATED_ARG_UPCALL_SS, // variables to upcall for
                                              // collocated op

      TAO_OBV_OPERATION_ARGLIST_CH,           // parameter list in obv op signature
      TAO_OBV_OPERATION_ARGLIST_CS,           // used only for AMH exceptions
      TAO_OBV_OPERATION_ARGLIST_SH,           // ... for server header
      TAO_OBV_OPERATION_ARGLIST_IH,           // ... for implementation header
      TAO_OBV_OPERATION_ARGLIST_IS,           // ... for implementation header

      // Emitting code for root.
      TAO_ROOT_CH,
      TAO_ROOT_CI,
      TAO_ROOT_CS,
      TAO_ROOT_SH,
      TAO_ROOT_SS,
      TAO_ROOT_TIE_SH,
      TAO_ROOT_TIE_SS,
      TAO_ROOT_IH,
      TAO_ROOT_IS,
      TAO_ROOT_SVH,
      TAO_ROOT_SVS,
      TAO_ROOT_SVTH,
      TAO_ROOT_SVTS,
      TAO_ROOT_EXH,
      TAO_ROOT_EXS,
      TAO_ROOT_CNH,
      TAO_ROOT_CNS,
      TAO_ROOT_EX_IDL,
      TAO_ROOT_ANY_OP_CH,
      TAO_ROOT_ANY_OP_CS,
      TAO_ROOT_CDR_OP_CH,
      TAO_ROOT_CDR_OP_CS,

      // Emitting code for sequence base type.
      TAO_SEQUENCE_BASE_CH,

      // For special sequnce elements.
      TAO_SEQELEM_RETTYPE_CH,

      // For sequence buffer types.
      TAO_SEQUENCE_BUFFER_TYPE_CH,

      // Emitting code for the public members of the union.
      TAO_UNION_PUBLIC_CH,
      TAO_UNION_PUBLIC_CI,
      TAO_UNION_PUBLIC_CS,
      TAO_UNION_PUBLIC_ASSIGN_CS,
      TAO_UNION_PUBLIC_RESET_CS,

      // Emitting code for private members of the union.
      TAO_UNION_PRIVATE_CH
    };

  enum LOOKUP_STRATEGY
    {
      // Various lookup strategies.
      TAO_LINEAR_SEARCH,
      TAO_DYNAMIC_HASH,
      TAO_PERFECT_HASH,
      TAO_BINARY_SEARCH
    };

  enum CG_SUB_STATE
    {
      // Sub states.

      // These are for the CDR operators.
      TAO_CDR_INPUT,
      TAO_CDR_OUTPUT,
      TAO_CDR_SCOPE,

      // These are for typecode generation.
      TAO_TC_DEFN_TYPECODE,                   // top level typecode
      TAO_TC_DEFN_TYPECODE_NESTED,            // nested tc

      TAO_TC_DEFN_ENCAPSULATION,              // encapsulation

      TAO_TC_DEFN_SCOPE,                      // scope
      TAO_TC_DEFN_TC_SIZE,                    // tc size computation
      TAO_TC_DEFN_ENCAP_LEN,                  // encap size computation
      TAO_TC_DEFN_SCOPE_LEN,                  // scope size computation

      // Means we are generating the copy constructor
      TAO_UNION_COPY_CONSTRUCTOR,

      // Generating the _var template parameter in sequence of arrays.
      TAO_ARRAY_SEQ_CH_TEMPLATE_VAR,

      // Generating an AMH response handler operation.
      TAO_AMH_RESPONSE_HANDLER_OPERATION,

      // Used to denote either error or don't care.
      TAO_SUB_STATE_UNKNOWN
    };

  /// Constructor
  TAO_CodeGen (void);

  /// Destructor
  ~TAO_CodeGen (void);

  /// Set the client header stream.
  int start_client_header (const char *fname);

  /// Set the client inline stream.
  int start_client_inline (const char *fname);

  /// Set the client stub stream.
  int start_client_stubs (const char *fname);

  /// Set the server header stream.
  int start_server_header (const char *fname);

  /// Set the implementation header stream.
  int start_implementation_header (const char *fname);

  /// Set the implementation skeleton stream.
  int start_implementation_skeleton (const char *fname);

  /// Set the server template header stream.
  int start_server_template_header (const char *fname);

  /// Set the server skeletons stream.
  int start_server_skeletons (const char *fname);

  /// Set the server template skeletons stream.
  int start_server_template_skeletons (const char *fname);

  /// Set the anyop header stream.
  int start_anyop_header (const char *fname);

  /// Set the anyop source stream.
  int start_anyop_source (const char *fname);

  int start_ciao_svnt_header (const char *fname);
  int start_ciao_svnt_source (const char *fname);
  int start_ciao_svnt_template_header (const char *fname);
  int start_ciao_svnt_template_source (const char *fname);
  int start_ciao_exec_header (const char *fname);
  int start_ciao_exec_source (const char *fname);
  int start_ciao_exec_idl (const char *fname);
  int start_ciao_conn_header (const char *fname);
  int start_ciao_conn_source (const char *fname);
  int start_ciao_ami_conn_idl (const char *fname);

  /// Generate code at the end such as the <<= and >>= operators along
  /// with the ending #endif statement.
  int end_client_header (void);

  /// Generate necessary code at end of client inline file.
  void end_client_inline (void);

  /// Generate necessary code at end of client stub file.
  void end_client_stubs (void);

  /// Put a last #endif in the server header.
  int end_server_header (void);

  /// Put a last #endif in the server header.
  int end_implementation_header (const char *fname);

  /// Put a last #endif in the server header.
  int end_implementation_skeleton (const char *fname);

  /// Put a last #endif in the server template header.
  int end_server_template_header (void);

  /// Put a last #endif in the server skeletons.
  int end_server_template_skeletons (void);

  /// Put a last #endif in the server skeletons.
  int end_server_skeletons (void);

  /// Put a last #endif in the anyop header.
  int end_anyop_header (void);

  /// Make sure we end with a newline.
  int end_anyop_source (void);

  int end_ciao_svnt_header (void);
  int end_ciao_svnt_source (void);
  int end_ciao_svnt_template_header (void);
  int end_ciao_svnt_template_source (void);
  int end_ciao_exec_header (void);
  int end_ciao_exec_source (void);
  int end_ciao_exec_idl (void);
  int end_ciao_conn_header (void);
  int end_ciao_conn_source (void);
  int end_ciao_ami_conn_idl (void);

  /// Get the client header stream.
  TAO_OutStream *client_header (void);

  /// Get the client stubs stream.
  TAO_OutStream *client_stubs (void);

  /// Get the client inline stream.
  TAO_OutStream *client_inline (void);

  /// get the server header stream.
  TAO_OutStream *server_header (void);

  /// Get the implementation header stream.
  TAO_OutStream *implementation_header (void);

  /// Get the implementation skeleton stream.
  TAO_OutStream *implementation_skeleton (void);

  /// Get the server header template stream.
  TAO_OutStream *server_template_header (void);

  /// Get the server skeletons stream.
  TAO_OutStream *server_skeletons (void);

  /// Get the server template skeletons stream.
  TAO_OutStream *server_template_skeletons (void);

  /// Get the server template inline stream.
  TAO_OutStream *server_template_inline (void);

  /// Get the anyop header stream.
  TAO_OutStream *anyop_header (void);

  /// Get the anyop source stream.
  TAO_OutStream *anyop_source (void);

  /// Get the CIAO servant header stream.
  TAO_OutStream *ciao_svnt_header (void);

  /// Get the CIAO servant source stream.
  TAO_OutStream *ciao_svnt_source (void);

  /// Get the CIAO template servant header stream.
  TAO_OutStream *ciao_svnt_template_header (void);

  /// Get the CIAO template servant source stream.
  TAO_OutStream *ciao_svnt_template_source (void);

  /// Get the CIAO executor impl header stream.
  TAO_OutStream *ciao_exec_header (void);

  /// Get the CIAO executor impl source stream.
  TAO_OutStream *ciao_exec_source (void);

  /// Get the CIAO executor IDL source stream.
  TAO_OutStream *ciao_exec_idl (void);

  /// Get the CIAO connector impl header stream.
  TAO_OutStream *ciao_conn_header (void);

  /// Get the CIAO connector impl source stream.
  TAO_OutStream *ciao_conn_source (void);

  /// Get the CIAO AMI connector IDL stream.
  TAO_OutStream *ciao_ami_conn_idl (void);

  /// Set the gperf input file stream.
  void gperf_input_stream (TAO_OutStream *gperf_input);

  /// Retrieve the gperf input stream being used.
  TAO_OutStream *gperf_input_stream (void);

  /// Set the gperf input file name.
  void gperf_input_filename (char *filename);

  /**
   * Retrieve the gperf input file name being used.
   * Name of the temp file used to collect the input for gperf
   * program. This is needed coz I do ACE_OS::open on this when I need
   * ACE_HANDLE for the file instead FILE*.
   */
  char *gperf_input_filename (void);

  /// Convert input string to all one case.
  const char *upcase (const char *str);
  const char *downcase (const char *str);

  /// Pass along the #ident string, if any, from the IDL file.
  void gen_ident_string (TAO_OutStream *stream) const;

  /// Generates the export files selected on the command line.
  void gen_export_files (void);

  /// Generate file include, with optional empty comment to
  /// short-circuit DOxygen.
  void gen_standard_include (TAO_OutStream *stream,
                             const char *included_file,
                             bool add_comment = false);

  /// Cleanup.
  void destroy (void);

private:
  void gen_ifndef_string (const char *fname,
                          TAO_OutStream *stream,
                          const char *prefix,
                          const char *suffix);

  /// Utility methods for generating file includes.
  void gen_stub_hdr_includes (void);
  void gen_stub_src_includes (void);
  void gen_skel_hdr_includes (void);
  void gen_skel_src_includes (void);
  void gen_seq_file_includes (void);
  void gen_any_file_includes (TAO_OutStream * stream) ;
  void gen_var_file_includes (void);
  void gen_stub_arg_file_includes (TAO_OutStream * stream);
  void gen_skel_arg_file_includes (TAO_OutStream * stream);
  void gen_cond_file_include (bool condition_green,
                              const char *filepath,
                              TAO_OutStream *stream);
  void gen_typecode_includes (TAO_OutStream * stream);

  /// Used if one or both of the CIAO code gen flags are set.
  void gen_svnt_hdr_includes (TAO_OutStream *stream);
  void gen_svnt_src_includes (TAO_OutStream *stream);
  void gen_exec_hdr_includes (void);
  void gen_exec_src_includes (void);
  void gen_exec_idl_includes (void);
  void gen_conn_hdr_includes (void);
  void gen_conn_src_includes (void);
  void gen_ami_conn_idl_includes (void);

  void gen_export_file (const char *filename,
                        const char *macro,
                        const char *msg,
                        bool for_skel = false);

  void make_rand_extension (char * const t);

  void gen_conn_ts_includes (ACE_Unbounded_Queue<char *> &ts_files);

private:
  /// Client header stream.
  TAO_OutStream *client_header_;

  /// Client stub file stream
  TAO_OutStream *client_stubs_;

  /// Client side inline definitions.
  TAO_OutStream *client_inline_;

  /// Server header stream.
  TAO_OutStream *server_header_;

  /// Implementation header stream.
  TAO_OutStream *implementation_header_;

  /// Implementation skeleton stream.
  TAO_OutStream *implementation_skeleton_;

  /// Server header template stream.
  TAO_OutStream *server_template_header_;

  /// Server skeleton stream.
  TAO_OutStream *server_skeletons_;

  /// Server skeleton template stream.
  TAO_OutStream *server_template_skeletons_;

  /// Anyop header file.
  TAO_OutStream *anyop_header_;

  /// Anyop source file.
  TAO_OutStream *anyop_source_;

  /// TAO_OutStream to collect the input for gperf program.
  TAO_OutStream *gperf_input_stream_;

  /// Component servant header file.
  TAO_OutStream *ciao_svnt_header_;

  /// Component servant source file.
  TAO_OutStream *ciao_svnt_source_;

  /// Template component servant header file.
  TAO_OutStream *ciao_svnt_template_header_;

  /// Template component servant source file.
  TAO_OutStream *ciao_svnt_template_source_;

  /// Component executor impl header file.
  TAO_OutStream *ciao_exec_header_;

  /// Component executor impl source file.
  TAO_OutStream *ciao_exec_source_;

  /// Component executor impl source file.
  TAO_OutStream *ciao_exec_idl_;

  /// Component connector impl header file.
  TAO_OutStream *ciao_conn_header_;

  /// Component connector impl source file.
  TAO_OutStream *ciao_conn_source_;

  /// Component connector impl source file.
  TAO_OutStream *ciao_ami_conn_idl_;

  /**
   * Name of the temp file used to collect the input for gperf
   * program. This is needed coz I do ACE_OS::open on this when I need
   * ACE_HANDLE for the file instead FILE*.
   */
  char *gperf_input_filename_;

  /// The enumerated value indicating the lookup strategy.
  LOOKUP_STRATEGY strategy_;
};

typedef ACE_Singleton<TAO_CodeGen, ACE_SYNCH_RECURSIVE_MUTEX> TAO_CODEGEN;
// Singleton instance of the BE code generator.

extern TAO_IDL_BE_Export TAO_CodeGen *tao_cg;
// Code generator instance which is used everywhere.

#endif /* if !defined */
