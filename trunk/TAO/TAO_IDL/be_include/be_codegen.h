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
  // header/inline/impl file.
  enum CG_STATE
  {
    // initial state
    TAO_INITIAL,

    // emitting code for root
    TAO_ROOT_CH,
    TAO_ROOT_CI,
    TAO_ROOT_CS,
    TAO_ROOT_SH,
    TAO_ROOT_SI,
    TAO_ROOT_SS,

    // emitting code for the module
    TAO_MODULE_CH,
    TAO_MODULE_CI,
    TAO_MODULE_CS,
    TAO_MODULE_SH,
    TAO_MODULE_SI,
    TAO_MODULE_SS,

    // emitting code for the interface
    TAO_INTERFACE_CH,
    TAO_INTERFACE_CI,
    TAO_INTERFACE_CS,
    TAO_INTERFACE_SH,
    TAO_INTERFACE_SI,
    TAO_INTERFACE_SS,

    // emitting code for the constants
    TAO_CONSTANT_CH,
    TAO_CONSTANT_CI,
    TAO_CONSTANT_CS,
    TAO_CONSTANT_SH,
    TAO_CONSTANT_SI,
    TAO_CONSTANT_SS,

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

    // emitting code for struct and its members
    TAO_STRUCT_CH,
    TAO_STRUCT_CI,
    TAO_STRUCT_CS,
    TAO_STRUCT_SH,
    TAO_STRUCT_SI,
    TAO_STRUCT_SS,

    // emitting code for exceptions
    TAO_EXCEPTION_CH,
    TAO_EXCEPTION_CI,
    TAO_EXCEPTION_CS,
    TAO_EXCEPTION_SH,
    TAO_EXCEPTION_SI,
    TAO_EXCEPTION_SS,

    // emitting code for enums
    TAO_ENUM_CH,
    TAO_ENUM_CI,
    TAO_ENUM_CS,
    TAO_ENUM_SH,
    TAO_ENUM_SI,
    TAO_ENUM_SS,

    // emitting code for array defn
    TAO_ARRAY_DEFN_CH,
    TAO_ARRAY_DEFN_CI,
    TAO_ARRAY_DEFN_CS,
    TAO_ARRAY_DEFN_SH,
    TAO_ARRAY_DEFN_SI,
    TAO_ARRAY_DEFN_SS,

    // emitting code for rest of the array decl
    TAO_ARRAY_OTHER_CH,
    TAO_ARRAY_OTHER_CI,
    TAO_ARRAY_OTHER_CS,
    TAO_ARRAY_OTHER_SH,
    TAO_ARRAY_OTHER_SI,
    TAO_ARRAY_OTHER_SS,

    // emitting code for strings
    TAO_STRING_CH,
    TAO_STRING_CI,
    TAO_STRING_CS,
    TAO_STRING_SH,
    TAO_STRING_SI,
    TAO_STRING_SS,

    // emitting code for an operation.
    TAO_OPERATION_CH,
    TAO_OPERATION_CI,
    TAO_OPERATION_CS,
    TAO_OPERATION_SH,
    TAO_OPERATION_SI,
    TAO_OPERATION_SS,

    // emitting code for arguments of an operation. No distinction between
    // headers, inclines, stubs.
    TAO_ARGUMENT_CH,
    TAO_ARGUMENT_CI,
    TAO_ARGUMENT_CS,
    TAO_ARGUMENT_SH,
    TAO_ARGUMENT_SI,
    TAO_ARGUMENT_SS,

    // emitting code for attributes
    TAO_ATTRIBUTE_CH,
    TAO_ATTRIBUTE_CI,
    TAO_ATTRIBUTE_CS,
    TAO_ATTRIBUTE_SH,
    TAO_ATTRIBUTE_SI,
    TAO_ATTRIBUTE_SS,

    // emitting code for typedefs
    TAO_TYPEDEF_CH,
    TAO_TYPEDEF_CI,
    TAO_TYPEDEF_CS,
    TAO_TYPEDEF_SH,
    TAO_TYPEDEF_SI,
    TAO_TYPEDEF_SS
  };

  TAO_CodeGen (void);
  // Constructor

  ~TAO_CodeGen (void);
  // destructor

  be_state *make_state (void);
  // factory method returning appropriate subclass of the be_state object
  // based on the current code generation state

  int gen_cplusplus_mapping (void);
  // generate the C++ mapping for CORBA IDL

  int client_header (const char *fname);
  // set the client header stream

  TAO_OutStream *client_header (void);
  // get the client header stream

  int client_stubs (const char *fname);
  // set the client stub stream

  TAO_OutStream *client_stubs (void);
  // get the client stubs stream

  int client_inline (const char *fname);
  // set the client inline stream

  TAO_OutStream *client_inline (void);
  // get the client inline stream

  int server_header (const char *fname);
  // set the server header stream

  TAO_OutStream *server_header (void);
  // get the server header stream

  int server_skeletons (const char *fname);
  // set the server skeletons stream

  TAO_OutStream *server_skeletons (void);
  // get the server skeletons stream

  int server_inline (const char *fname);
  // set the server inline stream

  TAO_OutStream *server_inline (void);
  // get the server inline stream

  void outstream (TAO_OutStream *os);
  // set current out stream

  TAO_OutStream *outstream (void);
  // retrieve current out stream being used

  int end_client_header (void);
  // put a last #endif in the client header

  int end_server_header (void);
  // put a last #endif in the server header

  void push (CG_STATE s);
  // set the code generation state

  void pop (void);
  // out of the current state

  void reset (void);
  // reset the stack to 1

  CG_STATE state (void);
  // return the current state

  void node (be_decl *n);
  // pass info

  be_decl *node (void);
  // retrieve passed info

private:
  TAO_OutStream *client_header_;
  // client header stream

  TAO_OutStream *client_stubs_;
  // client stub file stream

  TAO_OutStream *client_inline_;
  // client side inline definitions

  TAO_OutStream *server_header_;
  // server header stream

  TAO_OutStream *server_skeletons_;
  // server skeleton stream

  TAO_OutStream *server_inline_;
  // server side inline file

  TAO_OutStream *curr_os_;
  // currently used out stream

  CG_STATE *state_;
  // code generation state stack

  int top_;
  // top of state stack

  int size_;
  // size of allocated stack

  be_decl *node_;
  // save current node in this
};

typedef ACE_Singleton<TAO_CodeGen, ACE_SYNCH_MUTEX> TAO_CODEGEN;
// Singleton instance of the BE code generator


#endif // if !defined

