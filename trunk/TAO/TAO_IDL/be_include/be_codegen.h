// $Id$
/* -*- c++ -*- */
// ============================================================================
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
// ============================================================================

#if !defined (TAO_BE_CODEGEN_H)
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
    TAO_ARGUMENT_CH,                         // in client header
    TAO_ARGUMENT_CI,                         // in client inline
    TAO_ARGUMENT_CS,                         // in client stubs
    TAO_ARGUMENT_SH,                         // in server header
    TAO_ARGUMENT_SI,                         // in server inline
    TAO_ARGUMENT_SS,                         // in server skeleton
    TAO_ARGUMENT_ARGLIST_CH,                 // argument in op signature of
                                             // ... client header
    TAO_ARGUMENT_ARGLIST_OTHERS,             // ... in other cases
    TAO_ARGUMENT_PRE_DOCALL_CS,              // preprocessing of argument
                                             // variable before passing to
                                             // do_static_call  (in stub)
    TAO_ARGUMENT_DOCALL_CS,                  // passing argument variable to do_static_call
    TAO_ARGUMENT_POST_DOCALL_CS,             // postprocessing of argument
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
    TAO_ATTRIBUTE_SS,                        // in server skeletons
    TAO_ATTRIBUTE_COLLOCATED_SH,             // in server header for collocated
    TAO_ATTRIBUTE_COLLOCATED_SS,             // in server skeletons for
                                             // collocated

    TAO_ATTRIBUTE_RETVAL_DECL_SS,
    TAO_ATTRIBUTE_RETVAL_ASSIGN_SS,
    TAO_ATTRIBUTE_RETURN_TYPE_CH,
    TAO_ATTRIBUTE_INPARAM_TYPE_CH,
    TAO_ATTRIBUTE_RETURN_TYPE_CS,
    TAO_ATTRIBUTE_RETVAL_DECL_CS,
    TAO_ATTRIBUTE_RETVAL_EXCEPTION_CS,
    TAO_ATTRIBUTE_RETVAL_RETURN_CS,
    TAO_ATTRIBUTE_INPARAM_TYPE_CS,
    TAO_ATTRIBUTE_PRE_DOCALL_CS,
    TAO_ATTRIBUTE_DOCALL_CS,
    TAO_ATTRIBUTE_POST_DOCALL_CS,
    TAO_ATTRIBUTE_RETURN_TYPE_SH,
    TAO_ATTRIBUTE_INPARAM_TYPE_SH,
    TAO_ATTRIBUTE_RESULT_SS,
    TAO_ATTRIBUTE_INPARAM_TYPE_SS,
    TAO_ATTRIBUTE_PRE_UPCALL_SS,
    TAO_ATTRIBUTE_UPCALL_SS,
    TAO_ATTRIBUTE_POST_UPCALL_SS,
    TAO_ATTRIBUTE_TIE_SH,
    TAO_ATTRIBUTE_TIE_SI,

    // emitting code for array defn
    TAO_ARRAY_CH,
    TAO_ARRAY_CI,
    TAO_ARRAY_CS,

    TAO_ARRAY_DEFN_CH,
    TAO_ARRAY_DEFN_CI,
    TAO_ARRAY_DEFN_CS,
    TAO_ARRAY_DEFN_SH,
    TAO_ARRAY_DEFN_SI,
    TAO_ARRAY_DEFN_SS,
    TAO_ARRAY_ANY_OP_CH,
    TAO_ARRAY_ANY_OP_CS,

    // emitting code for rest of the array decl
    TAO_ARRAY_OTHER_CH,
    TAO_ARRAY_OTHER_CI,
    TAO_ARRAY_OTHER_CS,
    TAO_ARRAY_OTHER_SH,
    TAO_ARRAY_OTHER_SI,
    TAO_ARRAY_OTHER_SS,

    // emitting code for the constants
    TAO_CONSTANT_CH,                         // in client header
    TAO_CONSTANT_CI,                         // XXXASG rm?
    TAO_CONSTANT_CS,                         // in client stub

    // emitting code for enums
    TAO_ENUM_CH,                             // in client header
    TAO_ENUM_CI,                             // XXXASG rm?
    TAO_ENUM_CS,                             // in client stubs
    TAO_ENUM_SH,                             // XXXASG rm? all 3
    TAO_ENUM_SI,
    TAO_ENUM_SS,
    TAO_ENUM_ANY_OP_CH,
    TAO_ENUM_ANY_OP_CS,

    // emitting code for exceptions
    TAO_EXCEPTION_CH,
    TAO_EXCEPTION_CTOR_CH,
    TAO_EXCEPTION_CI,
    TAO_EXCEPTION_CS,
    TAO_EXCEPTION_CTOR_CS,
    TAO_EXCEPTION_CTOR_ASSIGN_CS,
    TAO_EXCEPTION_SH,
    TAO_EXCEPTION_SI,
    TAO_EXCEPTION_SS,
    TAO_EXCEPTION_ANY_OP_CH,
    TAO_EXCEPTION_ANY_OP_CS,

    // emitting fields i.e., struct members
    TAO_FIELD_CH,
    TAO_FIELD_CI,
    TAO_FIELD_CS,

    // emitting code for the interface
    TAO_INTERFACE_CH,
    TAO_INTERFACE_CI,
    TAO_INTERFACE_CS,
    TAO_INTERFACE_SH,
    TAO_INTERFACE_SI,
    TAO_INTERFACE_SS,
    TAO_INTERFACE_COLLOCATED_SH,
    TAO_INTERFACE_COLLOCATED_SS,
    TAO_INTERFACE_ANY_OP_CH,
    TAO_INTERFACE_ANY_OP_CS,
    TAO_INTERFACE_TIE_SH,
    TAO_INTERFACE_TIE_SI,

    // emitting code for the interface forward declaration
    TAO_INTERFACE_FWD_CH,
    TAO_INTERFACE_FWD_CI,

    // emitting code for the module
    TAO_MODULE_CH,
    TAO_MODULE_CI,
    TAO_MODULE_CS,
    TAO_MODULE_SH,
    TAO_MODULE_SI,
    TAO_MODULE_SS,
    TAO_MODULE_ANY_OP_CH,
    TAO_MODULE_ANY_OP_CS,

    // emitting code for an operation.
    TAO_OPERATION_CH,                       // in client header
    TAO_OPERATION_CI,                       // XXXASG rm?
    TAO_OPERATION_CS,                       // in client stubs
    TAO_OPERATION_SH,                       // in server header
    TAO_OPERATION_SI,                       // XXXASG rm?
    TAO_OPERATION_SS,                       // in server skeletons
    TAO_OPERATION_COLLOCATED_SH,            // in collocated server header
    TAO_OPERATION_COLLOCATED_SS,            // in collocated server skel
    TAO_OPERATION_RETTYPE_CH,                // return type in client header op
                                             // signature
    TAO_OPERATION_RETTYPE_OTHERS,            // ... in other cases
    TAO_OPERATION_RETURN_TYPE_CS,            //
    TAO_OPERATION_ARGLIST_CH,                // parameter list in op signature
                                             // ... for client header
    TAO_OPERATION_ARGLIST_SH,                // ... for server header
    TAO_OPERATION_ARGLIST_COLLOCATED_SH,     // ... for collocated server
    TAO_OPERATION_ARGLIST_OTHERS,            // ... for all other cases
    TAO_OPERATION_RETVAL_DECL_CS,            // return value variable declaration
    TAO_OPERATION_RETVAL_EXCEPTION_CS,       // return value on exception (error)
    TAO_OPERATION_RETVAL_PRE_DOCALL_CS,      // preprocessing for return value
                                             // before sending over the wire
    TAO_OPERATION_ARG_PRE_DOCALL_CS,         // preprocessing of arguments
                                             //   before do_static_call
    TAO_OPERATION_RETVAL_DOCALL_CS,          // passing the return type
                                             //   variable to do_static_call
    TAO_OPERATION_ARG_DOCALL_CS,             // passing argument variable to do_static_call
    TAO_OPERATION_RETVAL_POST_DOCALL_CS,     // processing  of return type
                                             //   after do_static_call
    TAO_OPERATION_ARG_POST_DOCALL_CS,        // processing of arg after do_static_call
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
    TAO_OPERATION_RETVAL_UPCALL_SS,          // passing return type var and argument
    TAO_OPERATION_ARG_UPCALL_SS,             // variables to upcall
    TAO_OPERATION_COLLOCATED_ARG_UPCALL_SS,  // variables to upcall for
                                             // collocated op
    TAO_OPERATION_RETVAL_POST_UPCALL_SS,     // post upcall processing for
    TAO_OPERATION_ARG_POST_UPCALL_SS,        //   return and argument variables
    TAO_OPERATION_RESULT_SS,                 // XXXASG rm?
    TAO_OPERATION_TIE_SH,
    TAO_OPERATION_TIE_SI,

    // for predefined type
    TAO_PREDEFINED_TYPE_CH,
    TAO_PREDEFINED_TYPE_CI,
    TAO_PREDEFINED_TYPE_CS,

    // emitting code for root
    TAO_ROOT_CH,
    TAO_ROOT_CI,
    TAO_ROOT_CS,
    TAO_ROOT_SH,
    TAO_ROOT_SI,
    TAO_ROOT_SS,
    TAO_ROOT_ANY_OP_CH,
    TAO_ROOT_ANY_OP_CS,

    // emitting sequences
    TAO_SEQUENCE_CH,
    TAO_SEQUENCE_CI,
    TAO_SEQUENCE_CS,
    TAO_SEQUENCE_ANY_OP_CH,
    TAO_SEQUENCE_ANY_OP_CS,

    // emitting code for sequence base type
    TAO_SEQUENCE_BASE_CH,
    TAO_SEQUENCE_BASE_CI,
    TAO_SEQUENCE_BASE_CS,
    TAO_SEQUENCE_BASE_SH,
    TAO_SEQUENCE_BASE_SI,
    TAO_SEQUENCE_BASE_SS,

    // emitting code for sequence body
    TAO_SEQUENCE_BODY_CH,
    TAO_SEQUENCE_BODY_CI,
    TAO_SEQUENCE_BODY_CS,
    TAO_SEQUENCE_BODY_SH,
    TAO_SEQUENCE_BODY_SI,
    TAO_SEQUENCE_BODY_SS,

    // for special sequnce elements
    TAO_SEQELEM_RETTYPE_CH,
    TAO_SEQELEM_RETTYPE_CI,
    TAO_SEQELEM_RETTYPE_CS,

    // for sequence buffer types
    TAO_SEQUENCE_BUFFER_TYPE_CH,
    TAO_SEQUENCE_BUFFER_TYPE_CI,
    TAO_SEQUENCE_BUFFER_TYPE_CS,

    // emitting code for strings
    TAO_STRING_CH,
    TAO_STRING_CI,
    TAO_STRING_CS,
    TAO_STRING_SH,
    TAO_STRING_SI,
    TAO_STRING_SS,

    // emitting code for struct and its members
    TAO_STRUCT_CH,
    TAO_STRUCT_CI,
    TAO_STRUCT_CS,
    TAO_STRUCT_ANY_OP_CH,
    TAO_STRUCT_ANY_OP_CS,

    // emitting code for typedefs
    TAO_TYPEDEF_CH,
    TAO_TYPEDEF_CI,
    TAO_TYPEDEF_CS,
    TAO_TYPEDEF_SH,
    TAO_TYPEDEF_SI,
    TAO_TYPEDEF_SS,
    TAO_TYPEDEF_ANY_OP_CH,
    TAO_TYPEDEF_ANY_OP_CS,

    // emitting code for unions
    TAO_UNION_CH,
    TAO_UNION_CI,
    TAO_UNION_CS,
    TAO_UNION_ANY_OP_CH,
    TAO_UNION_ANY_OP_CS,

    // emitting code for the discriminant
    TAO_UNION_DISCTYPEDEFN_CH,
    TAO_UNION_DISCTYPEDEFN_CI,
    TAO_UNION_DISCTYPEDEFN_CS,
    TAO_UNION_DISCTYPEDEFN_SH,
    TAO_UNION_DISCTYPEDEFN_SI,
    TAO_UNION_DISCTYPEDEFN_SS,

    // emitting code for the public members of the union
    TAO_UNION_PUBLIC_CH,
    TAO_UNION_PUBLIC_CI,
    TAO_UNION_PUBLIC_CS,
    TAO_UNION_PUBLIC_ASSIGN_CS,
    TAO_UNION_PUBLIC_RESET_CS,
    TAO_UNION_PUBLIC_ACCESS_CS,
    TAO_UNION_PUBLIC_SH,
    TAO_UNION_PUBLIC_SI,
    TAO_UNION_PUBLIC_SS,

    // emitting code for private members of the union
    TAO_UNION_PRIVATE_CH,
    TAO_UNION_PRIVATE_CI,
    TAO_UNION_PRIVATE_CS,
    TAO_UNION_PRIVATE_SH,
    TAO_UNION_PRIVATE_SI,
    TAO_UNION_PRIVATE_SS,

    // emitting code for typecodes
    TAO_TYPECODE_DECL,
    TAO_TYPECODE_DEFN,

    // Always must be last
    TAO_UNKNOWN
  };

  enum LOOKUP_STRATEGY
  {
    // various lookup strategies
    TAO_LINEAR,
    TAO_DYNAMIC_HASH,
    TAO_PERFECT_HASH,
    TAO_ACTIVE_DEMUX,
    TAO_USER_DEFINED
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

  void visitor_factory (TAO_Visitor_Factory *);
  // set the visitor factory  object

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
  // currently used out stream

  be_decl *node_;
  // save current node in this

  TAO_Visitor_Factory *visitor_factory_;
  // visitor factory object.

  LOOKUP_STRATEGY strategy_;
  // The enumerated value indicating the lookup strategy.
};

typedef ACE_Singleton<TAO_CodeGen, ACE_SYNCH_RECURSIVE_MUTEX> TAO_CODEGEN;
// Singleton instance of the BE code generator

extern TAO_CodeGen *tao_cg; // code generator instance which is used everywhere

#endif /* if !defined */
