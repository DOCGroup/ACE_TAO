/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
// 
// = FILENAME
//    be_root.h
//
// = DESCRIPTION
//    Extension of class AST_Root that provides the backend
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and 
//    Aniruddha Gokhale
// 
// ============================================================================

#if !defined (TAO_BE_ROOT_H)
#define TAO_BE_ROOT_H

class be_root : public virtual AST_Root, 
                public virtual be_scope,
                public virtual be_decl
{
  // = TITLE
  //   The back end extension of the AST_Root class
  //
  // = DESCRIPTION
  //
public:
  // Operations
  be_root (void);
  // Default constructor

  be_root (UTL_ScopedName *n, UTL_StrList *p);
  // Constructor that sets its scoped name <n>, and any pragmas <p>

  int gen_idl2cplusplus_mapping (void);
  // generates the IDL to C++ mapping

  virtual int gen_client_header (void);
  // Generates the client-side header information for the root 

  virtual int gen_client_stubs (void);
  // Generates the client-side stubs for the root

  virtual int gen_server_header (void);
  // Generates the server-side header information for the root 

  virtual int gen_server_skeletons (void);
  // Generates the server-side skeletons for the root

  virtual int gen_client_inline (void);
  // Generates the client-side inline for the root

  virtual int gen_server_inline (void);
  // Generates the server-side inlines for the root

  // =Scope management functions that override the AST scope management methods
  // since the AST methods set the names of the sequences, strings, and arrays
  // to a fixed value which interferes with our back-end code generation
  AST_Sequence *fe_add_sequence (AST_Sequence *);
  AST_String   *fe_add_string (AST_String *);
  AST_Array    *fe_add_array (AST_Array *);

  // Narrowing
  DEF_NARROW_METHODS3 (be_root, AST_Root, be_scope, be_decl);
  DEF_NARROW_FROM_DECL (be_root);
  DEF_NARROW_FROM_SCOPE (be_root);
};

#endif
