// $Id$
/* -*- c++ -*- */
// ================================================================
//
// = LIBRARY
//    TAO_IDL_BE
//
// = FILENAME
//    be_codegen.h
//
// = DESCRIPTION
//   The Code generator class
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ================================================================

#ifndef TAO_BE_CODEGEN_H
#define TAO_BE_CODEGEN_H

#include "ace/Singleton.h"
#include "ace/Synch.h"
#include "TAO_IDL_BE_Export.h"

class TAO_Visitor_Factory;
class TAO_OutStream;
class be_visitor_context;
class be_visitor;
class be_decl;

class TAO_IDL_BE_Export TAO_CodeGen
{
  // = TITLE
  //    TAO_CodeGen
  //
  // = DESCRIPTION
  //    Holds global parameters for the Back End and
  //    generates the C++ mapping.
  //
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
      TAO_ARGUMENT_INTERCEPTORS_ARGLIST_CS,     // ... client source
      TAO_ARGUMENT_INTERCEPTORS_INFO_ARGLIST_CS,    // ... client source
      TAO_ARGUMENT_INTERCEPTORS_ARGLIST_SS,     // ... server source
      TAO_ARGUMENT_INTERCEPTORS_INFO_ARGLIST_SS,    // ... server source
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
      TAO_INTERFACE_INTERCEPTORS_CH,
      TAO_INTERFACE_INTERCEPTORS_CS,
      TAO_INTERFACE_INTERCEPTORS_SH,
      TAO_INTERFACE_INTERCEPTORS_SS,

      TAO_INTERFACE_BASE_PROXY_IMPL_CH,
      TAO_INTERFACE_REMOTE_PROXY_IMPL_CH,
      TAO_INTERFACE_REMOTE_PROXY_IMPL_CS,
      TAO_INTERFACE_THRU_POA_PROXY_IMPL_SH,
      TAO_INTERFACE_THRU_POA_PROXY_IMPL_SS,
      TAO_INTERFACE_DIRECT_PROXY_IMPL_SH,
      TAO_INTERFACE_DIRECT_PROXY_IMPL_SS,

      // Emitting code for the AMH ResponseHandlers.
      TAO_INTERFACE_AMH_RH_SH,
      TAO_INTERFACE_AMH_RH_SS,

      // Emitting code for the valuetype.
      TAO_VALUETYPE_OBV_CH,                 // OBV_ class
      TAO_VALUETYPE_OBV_CI,
      TAO_VALUETYPE_OBV_CS,

      TAO_EVENTTYPE_OBV_CH,
      TAO_EVENTTYPE_OBV_CS,

      // Emitting code for the module,
      TAO_MODULE_OBV_CH,                    // for OBV_ (cmp. POA_ namespace)
      TAO_MODULE_OBV_CI,
      TAO_MODULE_OBV_CS,

      // Emitting code for an operation.
      TAO_OPERATION_CH,                       // in client header
      TAO_OPERATION_CS,                       // in client stubs

      TAO_OPERATION_ARGLIST_CH,               // parameter list in op signature
      TAO_OPERATION_ARGLIST_SH,               // ... for server header

      TAO_OPERATION_INTERCEPTORS_ARGLIST_CH,  // private member list list for request info
      TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_CH,   // private member list list for request info
      TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_CS,   // arglist for request info obj instantiation
      TAO_OPERATION_INTERCEPTORS_ARGLIST_CS,   // private member list list for request info                                                                         // ... for client source
      TAO_OPERATION_INTERCEPTORS_PARAMLIST,   // create the paramlist on demand
      TAO_OPERATION_INTERCEPTORS_EXCEPTLIST,  // create the exceptionlist on demand
      TAO_OPERATION_INTERCEPTORS_ARGLIST_SH,  // private member list list for request info
      TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_SH,   // private member list list for request info
      TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_SS,   // arglist for request info obj instantiation
      TAO_OPERATION_INTERCEPTORS_ARGLIST_SS,  // private member list list for request info                                                                  // ... for server source

      TAO_OPERATION_ARGLIST_PROXY_IMPL_XH,    // Proxy impl arg list generation
      // in client/server  header
      TAO_OPERATION_ARGLIST_PROXY_IMPL_XS,

      TAO_OPERATION_ARGLIST_BASE_PROXY_IMPL_CH,

      TAO_OPERATION_ARGLIST_IH,               // ... for implementation header
      TAO_OPERATION_ARGLIST_IS,               // ... for implementation header
      TAO_OPERATION_ARGLIST_COLLOCATED_SH,    // ... for collocated server
      // before sending over the wire
      TAO_OPERATION_ARG_PRE_INVOKE_CS,        // preprocessing of arguments
      //   variable to do_static_call
      TAO_OPERATION_ARG_INVOKE_CS,            // passing argument variable to do_static_call
      //   after do_static_call
      TAO_OPERATION_ARG_POST_INVOKE_CS,       // processing of arg after do_static_call
      TAO_OPERATION_ARG_DECL_SS,              // argument decl in skeleton
      TAO_OPERATION_ARG_DEMARSHAL_SS,         //   and argument variables to the
      TAO_OPERATION_ARG_MARSHAL_SS,
      // variable
      TAO_OPERATION_ARG_UPCALL_SS,            // variables to upcall
      TAO_OPERATION_COLLOCATED_ARG_UPCALL_SS, // variables to upcall for
      // collocated op
      TAO_OPERATION_INTERCEPTORS_ARG_INFO_CS, // Interceptor args
      TAO_OPERATION_INTERCEPTORS_ARG_INFO_SS, // Interceptor args

      TAO_OBV_OPERATION_ARGLIST_CH,           // parameter list in obv op signature
      TAO_OBV_OPERATION_ARGLIST_CS,           // used only for AMH exceptions
      // ... for client header
      TAO_OBV_OPERATION_ARGLIST_OBV_CH,       // ... for OBV_ class
      TAO_OBV_OPERATION_ARGLIST_SH,           // ... for server header
      TAO_OBV_OPERATION_ARGLIST_IH,           // ... for implementation header
      TAO_OBV_OPERATION_ARGLIST_IS,           // ... for implementation header
      TAO_OBV_OPERATION_ARGLIST_OTHERS,          // ... for all other cases
      TAO_OBV_OPERATION_ARGLIST_IMPL_CH,      // for implementations, e.g. exception holders
      TAO_OBV_OPERATION_ARGLIST_IMPL_CS,      // for implementations, e.g. exception holders

      // AMI next generation states
      TAO_AMI_INTERFACE_CH,
      TAO_AMI_SENDC_OPERATION_CH,
      TAO_AMI_SENDC_OPERATION_CS,
      TAO_AMI_HANDLER_REPLY_STUB_OPERATION_CH,
      TAO_AMI_HANDLER_REPLY_STUB_OPERATION_CS,
      TAO_AMI_EXCEPTION_HOLDER_VALUETYPE_CH,
      TAO_AMI_EXCEPTION_HOLDER_VALUETYPE_CS,
      TAO_AMI_EXCEPTION_HOLDER_RAISE_OPERATION_CS,

      // Emitting code for root.
      TAO_ROOT_CH,
      TAO_ROOT_CI,
      TAO_ROOT_CS,
      TAO_ROOT_SH,
      TAO_ROOT_SI,
      TAO_ROOT_SS,
      TAO_ROOT_TIE_SH,
      TAO_ROOT_TIE_SI,
      TAO_ROOT_IH,
      TAO_ROOT_IS,
      TAO_ROOT_ANY_OP_CH,
      TAO_ROOT_ANY_OP_CS,
      TAO_ROOT_CDR_OP_CH,
      TAO_ROOT_CDR_OP_CI,
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
      TAO_BINARY_SEARCH,
      TAO_ACTIVE_DEMUX,
      TAO_USER_DEFINED
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

      // Means we are not generating the assignment operator.
      TAO_UNION_COPY_CONSTRUCTOR,

      // Generating the _var template parameter in sequence of arrays.
      TAO_ARRAY_SEQ_CH_TEMPLATE_VAR,

      // Generating an AMH response handler operation.
      TAO_AMH_RESPONSE_HANDLER_OPERATION,

      // Used to denote either error or don't care.
      TAO_SUB_STATE_UNKNOWN
    };

  TAO_CodeGen (void);
  // Constructor

  ~TAO_CodeGen (void);
  // Destructor

  be_visitor *make_visitor (be_visitor_context *);
  // Factory that makes the right visitor based on the contex. This
  // delegates the task to its factory data member.

  int gen_cplusplus_mapping (void);
  // Generate the C++ mapping for CORBA IDL.

  int start_client_header (const char *fname);
  // Set the client header stream.

  int start_client_inline (const char *fname);
  // Set the client inline stream.

  int start_client_stubs (const char *fname);
  // Set the client stub stream.

  int start_server_header (const char *fname);
  // Set the server header stream.

  int start_implementation_header (const char *fname);
  // Set the implementation header stream.

  int start_implementation_skeleton (const char *fname);
  // Set the implementation skeleton stream.

  int start_server_template_header (const char *fname);
  // Set the server template header stream.

  int start_server_inline (const char *fname);
  // Set the server inline stream.

  int start_server_template_inline (const char *fname);
  // Set the server template inline stream.

  int start_server_skeletons (const char *fname);
  // Set the server skeletons stream.

  int start_server_template_skeletons (const char *fname);
  // Set the server template skeletons stream.

  int end_client_header (void);
  // Generate code at the end such as the <<= and >>= operators alongwith the
  // ending #endif statement.

  int end_server_header (void);
  // Put a last #endif in the server header.

  int end_implementation_header (const char *fname);
  // Put a last #endif in the server header.

  int end_implementation_skeleton (const char *fname);
  // Put a last #endif in the server header.

  int end_server_template_header (void);
  // Put a last #endif in the server template header.

  int end_server_template_inline (void);
  // Put a last #endif in the server template header.

  int end_server_template_skeletons (void);
  // Put a last #endif in the server skeletons.

  int end_server_skeletons (void);
  // Put a last #endif in the server skeletons.

  TAO_OutStream *client_header (void);
  // Get the client header stream.

  TAO_OutStream *client_stubs (void);
  // Get the client stubs stream.

  TAO_OutStream *client_inline (void);
  // Get the client inline stream.

  TAO_OutStream *server_header (void);
  // get the server header stream.

  TAO_OutStream *implementation_header (void);
  // Get the implementation header stream.

  TAO_OutStream *implementation_skeleton (void);
  // Get the implementation skeleton stream.

  TAO_OutStream *server_template_header (void);
  // Get the server header template stream.

  TAO_OutStream *server_skeletons (void);
  // Get the server skeletons stream.

  TAO_OutStream *server_template_skeletons (void);
  // Get the server template skeletons stream.

  TAO_OutStream *server_inline (void);
  // Get the server inline stream.

  TAO_OutStream *server_template_inline (void);
  // Get the server template inline stream.

  void gperf_input_stream (TAO_OutStream *gperf_input);
  // Set the gperf input file stream.

  TAO_OutStream *gperf_input_stream (void);
  // Retrieve the gperf input stream being used.

  void gperf_input_filename (char *filename);
  // Set the gperf input file name.

  char *gperf_input_filename (void);
  // Retrieve the gperf input file name being used.
  // Name of the temp file used to collect the input for gperf
  // program. This is needed coz I do ACE_OS::open on this when I need
  // ACE_HANDLE for the file instead FILE*.

  void outstream (TAO_OutStream *os);
  // Set current out stream.

  TAO_OutStream *outstream (void);
  // Retrieve current out stream being used.

  void config_visitor_factory (void);
  // Set the visitor factory  object. In this respect, this behaves as the
  // "strategy" pattern in which the TAO_CodeGen object is the context and the
  // visitor_factory is the strategy object.

  void node (be_decl *n);
  // Pass info.

  be_decl *node (void);
  // Retrieve passed info.

  const char *upcase (const char *str);
  // Convert input string to all upcase.

  void gen_ident_string (TAO_OutStream *stream) const;
  // Pass along the #ident string, if any, from the IDL file.

private:
  void gen_ifndef_string (const char *fname,
                          TAO_OutStream *stream,
                          const char *prefix,
                          const char *suffix);

  void gen_standard_include (TAO_OutStream *stream,
                             const char *included_file);

private:
  TAO_OutStream *client_header_;
  // Client header stream.

  TAO_OutStream *client_stubs_;
  // Client stub file stream

  TAO_OutStream *client_inline_;
  // Client side inline definitions.

  TAO_OutStream *server_header_;
  // Server header stream.

  TAO_OutStream *implementation_header_;
  // Implementation header stream.

  TAO_OutStream *implementation_skeleton_;
  // Implementation skeleton stream.

  TAO_OutStream *server_template_header_;
  // Server header template stream.

  TAO_OutStream *server_skeletons_;
  // Server skeleton stream.

  TAO_OutStream *server_template_skeletons_;
  // Server skeleton template stream.

  TAO_OutStream *server_inline_;
  // Server side inline file.

  TAO_OutStream *server_template_inline_;
  // Server side template inline file.

  TAO_OutStream *gperf_input_stream_;
  // TAO_OutStream to collect the input for gperf program.

  char *gperf_input_filename_;
  // Name of the temp file used to collect the input for gperf
  // program. This is needed coz I do ACE_OS::open on this when I need
  // ACE_HANDLE for the file instead FILE*.

  TAO_OutStream *curr_os_;
  // Currently used out stream.

  be_decl *node_;
  // Save current node in this.

  TAO_Visitor_Factory *visitor_factory_;
  // Visitor factory object.

  LOOKUP_STRATEGY strategy_;
  // The enumerated value indicating the lookup strategy.
};

typedef ACE_Singleton<TAO_CodeGen, ACE_SYNCH_RECURSIVE_MUTEX> TAO_CODEGEN;
// Singleton instance of the BE code generator.

extern TAO_IDL_BE_Export TAO_CodeGen *tao_cg;
// Code generator instance which is used everywhere.

#endif /* if !defined */
