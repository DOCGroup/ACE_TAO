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
  // define all the code generation states.

  enum CG_STATE
  {
    TAO_INITIAL,
    TAO_ROOT,
    TAO_MODULE,
    TAO_INTERFACE,
    TAO_CONSTANT,
    TAO_SEQUENCE_BASE,
    TAO_SEQUENCE_BODY,
    TAO_UNION_DISCTYPEDEFN, // union has a different kind of mapping
    TAO_UNION_PUBLIC,  
    TAO_UNION_PRIVATE,
    TAO_STRUCT,
    TAO_FIELD,
    TAO_EXCEPTION,
    TAO_ENUM,
    TAO_ARRAY_DEFN,
    TAO_ARRAY_OTHER,
    TAO_STRING,
    TAO_OPERATION,
    TAO_ARGUMENT,
    TAO_ATTRIBUTE,
    TAO_TYPEDEF
  };

  TAO_CodeGen (void);
  // Constructor

  ~TAO_CodeGen (void);
  // destructor

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

  void node (AST_Decl *n);
  // pass info

  AST_Decl *node (void);
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

  AST_Decl *node_;
};

typedef ACE_Singleton<TAO_CodeGen, ACE_SYNCH_MUTEX> TAO_CODEGEN;
// Singleton instance of the BE code generator


#endif // if !defined

