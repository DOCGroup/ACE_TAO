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

#include "ast_decl.h"

class be_scope;
class be_visitor;

class be_decl : public virtual AST_Decl
{
  // = TITLE
  //    be_decl
  // = DESCRIPTION
  //    The back end extension of the AST_Decl class. Provides an abstract
  //    interface.
  //
public:
  be_decl (void);
  // Default constructor.

  be_decl (AST_Decl::NodeType type,
           UTL_ScopedName *n);
  // Constructor that sets the node type.

  ~be_decl (void);
  // Destructor.

  // Methods used by the interface type strategy.
  void compute_full_name  (const char *prefix,
                           const char *suffix,
                           char *&name);
  // Both the arguments should be non-null!!!. Applies prefix and
  // suffix to the local name and makes a flat name.

  void compute_flat_name (const char *prefix,
                          const char *suffix,
                          char *& name);
  // Both the arguments should be non-null!!!. Applies prefix and
  // suffix to the local name and makes a flat name.

  // End of Methods use by the interface type strategy.

  virtual be_scope *scope (void);
  // Return the scope created by this node (if one exists).

  // Visiting
  virtual int accept (be_visitor *visitor);

  virtual void destroy (void);
  // Cleanup function.

  // Boolean methods to test if code was already generated.
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
  idl_bool cli_inline_cdr_decl_gen (void);
  idl_bool cli_traits_gen (void);

  // Set the flag indicating that code generation is done.
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
  void cli_inline_cdr_decl_gen (idl_bool);
  void cli_traits_gen (idl_bool);

  // Narrowing
  DEF_NARROW_METHODS1 (be_decl, AST_Decl);
  DEF_NARROW_FROM_DECL (be_decl);

private:
  // Variables that indicate if the code generation for that node is already
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
  idl_bool cli_inline_cdr_decl_gen_;
  idl_bool cli_traits_gen_;
};

#endif // if !defined
