// $Id$
/* -*- c++ -*- */
// ================================================================
//
// = LIBRARY
//    TAO IDL
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

#define NAMEBUFSIZE 1024
// maximum length of static buffers used to store names

class TAO_Visitor_Factory;
class be_visitor_context;

class TAO_CodeGen
{
  // = TITLE
  //   TAO_CodeGen
  //
  // = DESCRIPTION
  //   Holds global parameters for the Back End and generates the C++ mapping
  //
public:

  // define all the code generation states. The first letter C/S of the suffix stands
  // for client/server-side and the second letter H/I/S stands for
  // header/inline/impl file. These are used to denote the state or the context
  // of code generation we are in and serves to produce the right kind of
  // visitor for us

  enum CG_STATE
  {
    // initial state
    TAO_INITIAL,

    // rest of the state in alphabetical order of the types

    // emitting code for arguments of an operation. No distinction between
    // headers, inlines, stubs.
    TAO_ARGUMENT_ARGLIST_CH,                 // argument in op signature of
                                             // ... client header
    TAO_ARGUMENT_ARGLIST_OTHERS,             // ... in other cases
    TAO_ARGUMENT_PRE_INVOKE_CS,              // preprocessing of argument
                                             // variable before passing to
                                             // do_static_call  (in stub)
    TAO_ARGUMENT_INVOKE_CS,                  // passing argument variable to do_static_call
    TAO_ARGUMENT_POST_INVOKE_CS,             // postprocessing of argument
                                             // variable after do_static_call
    TAO_ARGUMENT_VARDECL_SS,                 // declaration of argument
                                             // variable in skeleton (server)
    TAO_ARGUMENT_MARSHAL_SS,                 // passing argument node to the
    TAO_ARGUMENT_DEMARSHAL_SS,               // demarshal and marshal
                                             // operations
    TAO_ARGUMENT_POST_MARSHAL_SS,
    TAO_ARGUMENT_PRE_UPCALL_SS,              // preprocessing of argument
                                             // variable before upcall
    TAO_ARGUMENT_UPCALL_SS,                  // passing argument variable to upcall
    TAO_ARGUMENT_COLLOCATED_UPCALL_SS,                  // passing argument
                                                        // variable to upcall
    TAO_ARGUMENT_POST_UPCALL_SS,             // postprocessing of argument
                                             // variable after upcall

    // emitting code for attributes.
    TAO_ATTRIBUTE_CH,                        // in client header
    TAO_ATTRIBUTE_CS,                        // in client stubs
    TAO_ATTRIBUTE_SH,                        // in server header
    TAO_ATTRIBUTE_IH,                        // in implementation header
    TAO_ATTRIBUTE_SS,                        // in server skeletons
    TAO_ATTRIBUTE_IS,                        // in implementation skeletons
    TAO_ATTRIBUTE_COLLOCATED_SH,             // in server header for collocated
    TAO_ATTRIBUTE_COLLOCATED_SS,             // in server skeletons for
                                             // collocated
    TAO_ATTRIBUTE_TIE_SH,
    TAO_ATTRIBUTE_TIE_SI,

    // emitting code for array defn
    TAO_ARRAY_CH,
    TAO_ARRAY_CI,
    TAO_ARRAY_CS,
    TAO_ARRAY_ANY_OP_CH,
    TAO_ARRAY_ANY_OP_CS,
    TAO_ARRAY_CDR_OP_CH,
    TAO_ARRAY_CDR_OP_CS,

    // emitting code for the constants
    TAO_CONSTANT_CH,                         // in client header
    TAO_CONSTANT_CI,                         // XXXASG rm?
    TAO_CONSTANT_CS,                         // in client stub

    // emitting code for enums
    TAO_ENUM_CH,                             // in client header
    TAO_ENUM_CS,                             // in client stubs
    TAO_ENUM_ANY_OP_CH,
    TAO_ENUM_ANY_OP_CS,
    TAO_ENUM_CDR_OP_CH,
    TAO_ENUM_CDR_OP_CS,

    // emitting code for exceptions
    TAO_EXCEPTION_CH,
    TAO_EXCEPTION_CTOR_CH,
    TAO_EXCEPTION_CI,
    TAO_EXCEPTION_CS,
    TAO_EXCEPTION_CTOR_CS,
    TAO_EXCEPTION_CTOR_ASSIGN_CS,
    TAO_EXCEPTION_ANY_OP_CH,
    TAO_EXCEPTION_ANY_OP_CS,
    TAO_EXCEPTION_CDR_OP_CH,
    TAO_EXCEPTION_CDR_OP_CS,

    // emitting fields i.e., struct members
    TAO_FIELD_CH,
    TAO_FIELD_CI,
    TAO_FIELD_CS,
    TAO_FIELD_CDR_OP_CH,
    TAO_FIELD_CDR_OP_CS,

    // emitting code for the interface
    TAO_INTERFACE_CH,
    TAO_INTERFACE_CI,
    TAO_INTERFACE_CS,
    TAO_INTERFACE_SH,
    TAO_INTERFACE_IH,
    TAO_INTERFACE_SI,
    TAO_INTERFACE_SS,
    TAO_INTERFACE_IS,
    TAO_INTERFACE_COLLOCATED_SH,
    TAO_INTERFACE_COLLOCATED_SS,
    TAO_INTERFACE_ANY_OP_CH,
    TAO_INTERFACE_ANY_OP_CS,
    TAO_INTERFACE_CDR_OP_CH,
    TAO_INTERFACE_CDR_OP_CS,
    TAO_INTERFACE_TIE_SH,
    TAO_INTERFACE_TIE_SI,

    // emitting code for the interface forward declaration
    TAO_INTERFACE_FWD_CH,
    TAO_INTERFACE_FWD_CI,
    TAO_INTERFACE_FWD_CDR_OP_CI,

    // emitting code for the module
    TAO_MODULE_CH,
    TAO_MODULE_CI,
    TAO_MODULE_CS,
    TAO_MODULE_SH,
    TAO_MODULE_SI,
    TAO_MODULE_IH,
    TAO_MODULE_SS,
    TAO_MODULE_IS,
    TAO_MODULE_ANY_OP_CH,
    TAO_MODULE_ANY_OP_CS,
    TAO_MODULE_CDR_OP_CH,
    TAO_MODULE_CDR_OP_CS,

    // emitting code for an operation.
    TAO_OPERATION_CH,                       // in client header
    TAO_OPERATION_CS,                       // in client stubs
    TAO_OPERATION_SH,                       // in server header
    TAO_OPERATION_IH,                       // in implementation header
    TAO_OPERATION_SS,                       // in server skeletons
    TAO_OPERATION_IS,                       // in server skeletons
    TAO_OPERATION_COLLOCATED_SH,            // in collocated server header
    TAO_OPERATION_COLLOCATED_SS,            // in collocated server skel
    TAO_OPERATION_RETTYPE_CH,                // return type in client header op
                                             // signature
    TAO_OPERATION_RETTYPE_IS,                // return type in client header op
    TAO_OPERATION_RETTYPE_OTHERS,            // ... in other cases
    TAO_OPERATION_ARGLIST_CH,                // parameter list in op signature
                                             // ... for client header
    TAO_OPERATION_ARGLIST_SH,                // ... for server header
    TAO_OPERATION_ARGLIST_IH,                // ... for implementation header
    TAO_OPERATION_ARGLIST_IS,                // ... for implementation header
    TAO_OPERATION_ARGLIST_COLLOCATED_SH,     // ... for collocated server
    TAO_OPERATION_ARGLIST_OTHERS,            // ... for all other cases
    TAO_OPERATION_RETVAL_DECL_CS,            // return value variable declaration
    TAO_OPERATION_RETVAL_PRE_INVOKE_CS,      // preprocessing for return value
                                             // before sending over the wire
    TAO_OPERATION_ARG_PRE_INVOKE_CS,         // preprocessing of arguments
                                             //   before do_static_call
    TAO_OPERATION_RETVAL_INVOKE_CS,          // passing the return type
                                             //   variable to do_static_call
    TAO_OPERATION_ARG_INVOKE_CS,             // passing argument variable to do_static_call
    TAO_OPERATION_RETVAL_POST_INVOKE_CS,     // processing  of return type
                                             //   after do_static_call
    TAO_OPERATION_ARG_POST_INVOKE_CS,        // processing of arg after do_static_call
    TAO_OPERATION_RETVAL_RETURN_CS,          // returning the return type
                                             // variable
    TAO_OPERATION_EXCEPTLIST_CS,             // generating the exception list
    TAO_OPERATION_RETVAL_DECL_SS,            // return type decl in skeleton
    TAO_OPERATION_ARG_DECL_SS,               // argument decl in skeleton
    TAO_OPERATION_RETVAL_DEMARSHAL_SS,       // passing return type variable
    TAO_OPERATION_ARG_DEMARSHAL_SS,          //   and argument variables to the
    TAO_OPERATION_RETVAL_MARSHAL_SS,         //   marshal and demarshal operations
    TAO_OPERATION_ARG_MARSHAL_SS,
    TAO_OPERATION_ARG_POST_MARSHAL_SS,
    TAO_OPERATION_RETVAL_ASSIGN_SS,          // assigning to return type
                                             // variable
    TAO_OPERATION_ARG_PRE_UPCALL_SS,         // pre upcall processing
    TAO_OPERATION_ARG_UPCALL_SS,             // variables to upcall
    TAO_OPERATION_COLLOCATED_ARG_UPCALL_SS,  // variables to upcall for
                                             // collocated op
    TAO_OPERATION_RETVAL_POST_UPCALL_SS,     // post upcall processing for
    TAO_OPERATION_ARG_POST_UPCALL_SS,        //   return and argument variables
    TAO_OPERATION_RESULT_SS,                 // XXXASG rm?
    TAO_OPERATION_TIE_SH,
    TAO_OPERATION_TIE_SI,

    // emitting code for root
    TAO_ROOT_CH,
    TAO_ROOT_CI,
    TAO_ROOT_CS,
    TAO_ROOT_SH,
    TAO_ROOT_SI,
    TAO_ROOT_SS,
    TAO_ROOT_IH,
    TAO_ROOT_IS,
    TAO_ROOT_ANY_OP_CH,
    TAO_ROOT_ANY_OP_CS,
    TAO_ROOT_CDR_OP_CH,
    TAO_ROOT_CDR_OP_CI,
    TAO_ROOT_CDR_OP_CS,

    // emitting sequences
    TAO_SEQUENCE_CH,
    TAO_SEQUENCE_CI,
    TAO_SEQUENCE_CS,
    TAO_SEQUENCE_ANY_OP_CH,
    TAO_SEQUENCE_ANY_OP_CS,
    TAO_SEQUENCE_CDR_OP_CH,
    TAO_SEQUENCE_CDR_OP_CS,

    // emitting code for sequence base type
    TAO_SEQUENCE_BASE_CH,
    TAO_SEQUENCE_BASE_CI,
    TAO_SEQUENCE_BASE_CS,

    // for special sequnce elements
    TAO_SEQELEM_RETTYPE_CH,
    TAO_SEQELEM_RETTYPE_CI,
    TAO_SEQELEM_RETTYPE_CS,

    // for sequence buffer types
    TAO_SEQUENCE_BUFFER_TYPE_CH,
    TAO_SEQUENCE_BUFFER_TYPE_CI,
    TAO_SEQUENCE_BUFFER_TYPE_CS,

    // emitting code for struct and its members
    TAO_STRUCT_CH,
    TAO_STRUCT_CI,
    TAO_STRUCT_CS,
    TAO_STRUCT_ANY_OP_CH,
    TAO_STRUCT_ANY_OP_CS,
    TAO_STRUCT_CDR_OP_CH,
    TAO_STRUCT_CDR_OP_CS,

    // emitting code for typedefs
    TAO_TYPEDEF_CH,
    TAO_TYPEDEF_CI,
    TAO_TYPEDEF_CS,
    TAO_TYPEDEF_ANY_OP_CH,
    TAO_TYPEDEF_ANY_OP_CS,
    TAO_TYPEDEF_CDR_OP_CH,
    TAO_TYPEDEF_CDR_OP_CS,

    // emitting code for unions
    TAO_UNION_CH,
    TAO_UNION_CI,
    TAO_UNION_CS,
    TAO_UNION_ANY_OP_CH,
    TAO_UNION_ANY_OP_CS,
    TAO_UNION_CDR_OP_CH,
    TAO_UNION_CDR_OP_CS,

    // emitting code for the discriminant
    TAO_UNION_DISCTYPEDEFN_CH,
    TAO_UNION_DISCTYPEDEFN_CI,
    TAO_UNION_DISCTYPEDEFN_CS,

    // emitting code for the public members of the union
    TAO_UNION_PUBLIC_CH,
    TAO_UNION_PUBLIC_CI,
    TAO_UNION_PUBLIC_CS,
    TAO_UNION_PUBLIC_ASSIGN_CS,
    TAO_UNION_PUBLIC_RESET_CS,
    TAO_UNION_PUBLIC_ACCESS_CS,

    // emitting code for private members of the union
    TAO_UNION_PRIVATE_CH,

    // emitting code for CDR operators for types defined inside unions
    TAO_UNION_BRANCH_CDR_OP_CH,
    TAO_UNION_BRANCH_CDR_OP_CS,

    // emitting code for typecodes
    TAO_TYPECODE_DECL,
    TAO_TYPECODE_DEFN,

    // Always must be last
    TAO_UNKNOWN
  };

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

  enum CG_SUB_STATE
  {
    // sub states
    TAO_CDR_INPUT,
    TAO_CDR_OUTPUT,
    TAO_CDR_SCOPE,

    TAO_SUB_STATE_UNKNOWN
  };

  TAO_CodeGen (void);
  // Constructor

  ~TAO_CodeGen (void);
  // destructor

  be_visitor *make_visitor (be_visitor_context *);
  // Factory that makes the right visitor based on the contex. This
  // delegates the task to its factory data member

  int gen_cplusplus_mapping (void);
  // generate the C++ mapping for CORBA IDL

  int start_client_header (const char *fname);
  // set the client header stream

  int start_client_inline (const char *fname);
  // set the client inline stream

  int start_client_stubs (const char *fname);
  // set the client stub stream

  int start_server_header (const char *fname);
  // set the server header stream

  int start_implementation_header (const char *fname);
  // set the implementation header stream

  int start_implementation_skeleton (const char *fname);
  // set the implementation skeleton stream

  int start_server_template_header (const char *fname);
  // set the server template header stream

  int start_server_inline (const char *fname);
  // set the server inline stream

  int start_server_template_inline (const char *fname);
  // set the server template inline stream

  int start_server_skeletons (const char *fname);
  // set the server skeletons stream

  int start_server_template_skeletons (const char *fname);
  // set the server template skeletons stream

  int end_client_header (void);
  // generate code at the end such as the <<= and >>= operators alongwith the
  // ending #endif statement

  int end_server_header (void);
  // put a last #endif in the server header

  int end_implementation_header (const char *fname);
  // put a last #endif in the server header

  int end_implementation_skeleton (const char *fname);
  // put a last #endif in the server header

  int end_server_template_header (void);
  // put a last #endif in the server template header

  int end_server_template_skeletons (void);
  // put a last #endif in the server skeletons

  TAO_OutStream *client_header (void);
  // get the client header stream

  TAO_OutStream *client_stubs (void);
  // get the client stubs stream

  TAO_OutStream *client_inline (void);
  // get the client inline stream

  TAO_OutStream *server_header (void);
  // get the server header stream

  TAO_OutStream *implementation_header (void);
  // get the implementation header stream

  TAO_OutStream *implementation_skeleton (void);
  // get the implementation skeleton stream

  TAO_OutStream *server_template_header (void);
  // get the server header template stream

  TAO_OutStream *server_skeletons (void);
  // Get the server skeletons stream.

  TAO_OutStream *server_template_skeletons (void);
  // get the server template skeletons stream

  TAO_OutStream *server_inline (void);
  // get the server inline stream

  TAO_OutStream *server_template_inline (void);
  // get the server template inline stream

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
  // set current out stream

  TAO_OutStream *outstream (void);
  // retrieve current out stream being used

  void config_visitor_factory (void);
  // set the visitor factory  object. In this respect, this behaves as the
  // "strategy" pattern in which the TAO_CodeGen object is the context and the
  // visitor_factory is the strategy object

  void node (be_decl *n);
  // pass info

  be_decl *node (void);
  // retrieve passed info

  const char *upcase (const char *str);
  // convert input string to all upcase

  void lookup_strategy (LOOKUP_STRATEGY s);
  // Set the lookup strategy.

  TAO_CodeGen::LOOKUP_STRATEGY lookup_strategy (void) const;
  // Return the enumerated value for the lookup strategy. Default is
  // Dynamic Hashing.

private:
  TAO_OutStream *client_header_;
  // client header stream

  TAO_OutStream *client_stubs_;
  // client stub file stream

  TAO_OutStream *client_inline_;
  // client side inline definitions

  TAO_OutStream *server_header_;
  // server header stream

  TAO_OutStream *implementation_header_;
  // implementation header stream

  TAO_OutStream *implementation_skeleton_;
  // implementation skeleton stream

  TAO_OutStream *server_template_header_;
  // server header template stream

  TAO_OutStream *server_skeletons_;
  // server skeleton stream.

  TAO_OutStream *server_template_skeletons_;
  // server skeleton template stream

  TAO_OutStream *server_inline_;
  // server side inline file

  TAO_OutStream *server_template_inline_;
  // server side template inline file.

  TAO_OutStream *gperf_input_stream_;
  // TAO_OutStream to collect the input for gperf program.

  char *gperf_input_filename_;
  // Name of the temp file used to collect the input for gperf
  // program. This is needed coz I do ACE_OS::open on this when I need
  // ACE_HANDLE for the file instead FILE*.

  TAO_OutStream *curr_os_;
  // currently used out stream.

  be_decl *node_;
  // save current node in this.

  TAO_Visitor_Factory *visitor_factory_;
  // visitor factory object.

  LOOKUP_STRATEGY strategy_;
  // The enumerated value indicating the lookup strategy.
};

typedef ACE_Singleton<TAO_CodeGen, ACE_SYNCH_RECURSIVE_MUTEX> TAO_CODEGEN;
// Singleton instance of the BE code generator

extern TAO_CodeGen *tao_cg; // code generator instance which is used everywhere

#endif /* if !defined */
