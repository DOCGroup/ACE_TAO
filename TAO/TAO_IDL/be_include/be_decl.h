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
  enum SIZE_TYPE
  {
    SIZE_UNKNOWN,
    FIXED,
    VARIABLE
  };
  // Undicates if we are fixed size or variable. Most useful for structs,
  // unions, and arrays.

  be_decl (void);
  // Default constructor.

  be_decl (AST_Decl::NodeType type, 
           UTL_ScopedName *n);
  // Constructor that sets the node type.

  ~be_decl (void);
  // Destructor.

  virtual void size_type (SIZE_TYPE);
  // Set the size type.

  virtual SIZE_TYPE size_type (void);
  // Return our size type.

  const char *flat_name (void);
  // Return the flattened full scoped name.

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

  virtual idl_bool is_nested (void);
  // Determines if we are inside of a nested scope or not.

  virtual be_scope *scope (void);
  // Return the scope created by this node (if one exists).

  // Visiting
  virtual int accept (be_visitor *visitor);

  idl_bool has_constructor (void);
  // Accessor for protected member.

  void has_constructor (idl_bool value);
  // Mutator for protected member.

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

  // Narrowing
  DEF_NARROW_METHODS1 (be_decl, AST_Decl);
  DEF_NARROW_FROM_DECL (be_decl);

protected:
  virtual int compute_size_type (void);
  // Determine our size type and set it if it is unknown.

  virtual void compute_flat_name (void);
  // Compute the flattened fully scoped name.

  virtual void destroy (void);
  // Cleanup function.

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

  char *flat_name_;
  // Flattened fully scoped name.

  SIZE_TYPE size_type_;
  // Whether we are fixed or variable size (by default fixed).

  idl_bool has_constructor_;
  // Attribute that helps a union determine whether a member
  // should be included by value or by reference.
};

#endif // if !defined
