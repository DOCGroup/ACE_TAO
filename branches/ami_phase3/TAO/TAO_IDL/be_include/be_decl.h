/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_decl.h
//
// = DESCRIPTION
//    Extension of the AST_Decl class.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef TAO_BE_DECL_H
#define TAO_BE_DECL_H

class be_scope;
class be_visitor;

/*
 * BE_Decl
 */
class be_decl : public virtual AST_Decl
{
  // = TITLE
  //   be_decl
  // = DESCRIPTION
  //   The back end extension of the AST_Decl class. Provides an abstract
  //   interface
  //
public:
  enum SIZE_TYPE
  {
    SIZE_UNKNOWN,
    FIXED,
    VARIABLE
  };
  // indicates if we are fixed size or variable. Most useful for structs,
  // unions, and arrays.

  // =Operations

  be_decl (void);
  // Default constructor

  be_decl (AST_Decl::NodeType type, UTL_ScopedName *n, UTL_StrList *pragmas);
  // constructor that sets the node type

  ~be_decl (void);
  // destructor

  virtual void size_type (SIZE_TYPE);
  // set the size type

  virtual SIZE_TYPE size_type (void);
  // return our size type

  const char *fullname (void);
  // return the stringified full name

  const char *flatname (void);
  // return the flattened full scoped name


  const char *repoID (void);
  // retrieve the repository ID

  const char* prefix (void);
  // retrive the repository ID prefix

  virtual idl_bool is_nested (void);
  // determines if we are inside of a nested scope or not

  virtual be_scope *scope (void);
  // return the scope created by this node (if one exists)

  // Visiting
  virtual int accept (be_visitor *visitor);

  // boolean methods to test if code was already generated
  idl_bool cli_hdr_gen (void);
  idl_bool cli_stub_gen (void);
  idl_bool cli_inline_gen (void);
  idl_bool srv_hdr_gen (void);
  idl_bool impl_hdr_gen (void);
  idl_bool srv_skel_gen (void);
  idl_bool impl_skel_gen (void);
  idl_bool srv_inline_gen (void);
  idl_bool cli_hdr_any_op_gen (void);
  idl_bool cli_stub_any_op_gen (void);
  idl_bool cli_hdr_cdr_op_gen (void);
  idl_bool cli_stub_cdr_op_gen (void);
  idl_bool cli_inline_cdr_op_gen (void);

  // set the flag indicating that code generation is done
  void cli_hdr_gen (idl_bool);
  void cli_stub_gen (idl_bool);
  void cli_inline_gen (idl_bool);
  void srv_hdr_gen (idl_bool);
  void impl_hdr_gen (idl_bool);
  void srv_skel_gen (idl_bool);
  void impl_skel_gen (idl_bool);
  void srv_inline_gen (idl_bool);
  void cli_hdr_any_op_gen (idl_bool);
  void cli_stub_any_op_gen (idl_bool);
  void cli_hdr_cdr_op_gen (idl_bool);
  void cli_stub_cdr_op_gen (idl_bool);
  void cli_inline_cdr_op_gen (idl_bool);

  idl_bool is_child (be_decl *node);
  // am I a direct child of node?

  // Narrowing
  DEF_NARROW_METHODS1 (be_decl, AST_Decl);
  DEF_NARROW_FROM_DECL (be_decl);

protected:
  // =helper

  virtual int compute_size_type (void);
  // determine our size type and set it if it is unknown

  virtual void compute_fullname (void);
  // computes the fully scoped name

  virtual void compute_flatname (void);
  // compute the flattened fully scoped name


  virtual void compute_repoID (void);
  // computes the repoID

  virtual void compute_prefix (void);
  // computes the prefix for the repoID

  // variables that indicate if the code generation for that node is already
  // been done. This way we avoid regenerating same code.
  idl_bool cli_hdr_gen_;
  idl_bool cli_stub_gen_;
  idl_bool cli_inline_gen_;
  idl_bool srv_hdr_gen_;
  idl_bool impl_hdr_gen_;
  idl_bool srv_skel_gen_;
  idl_bool impl_skel_gen_;
  idl_bool srv_inline_gen_;
  idl_bool cli_hdr_any_op_gen_;
  idl_bool cli_stub_any_op_gen_;
  idl_bool cli_hdr_cdr_op_gen_;
  idl_bool cli_stub_cdr_op_gen_;
  idl_bool cli_inline_cdr_op_gen_;

  char *fullname_;
  // our full scoped name

  char *flatname_;
  // flattened fully scoped name


  char *repoID_;
  // repository ID

  char *prefix_;
  // The repository ID prefix

  SIZE_TYPE size_type_;
  // whether we are fixed or variable size (by default fixed)

};

#endif // if !defined
