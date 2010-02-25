/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    be_scope.h
 *
 *  $Id$
 *
 *  Extension of the UTL_Scope CFE class
 *
 *  @author Copyright 1994-1995 by Sun Microsystems Int.
 *  @author and Aniruddha Gokhale
 */
//=============================================================================


#ifndef TAO_BE_SCOPE_H
#define TAO_BE_SCOPE_H

#include "utl_scope.h"
#include "ast_decl.h"

class be_decl;
class be_visitor;
class UTL_ExceptList;

/**
 * @class be_scope
 *
 * @brief be_scope
 *
 * The back end extension of the UTL_Scope class
 */
class be_scope : public virtual UTL_Scope
{
public:
  /// Default constructor.
  be_scope (void);

  /// Constructor that sets the node type.
  be_scope (AST_Decl::NodeType nt);

  /// Destructor.
  virtual ~be_scope (void);

  /// To access the protected base class method fe_add_field.
  AST_Field *be_add_field (AST_Field *f);

  /// Overridden in be_operation and be_factory.
  virtual AST_Argument *be_add_argument (AST_Argument *arg);

  /// Set the comma producing state.
  virtual void comma (unsigned short set);

  /// Get the comma producing state.
  int comma (void) const;

  /// Return the be_decl node corresponding to this scope node.
  virtual be_decl *decl (void);

  /// Recursively clean up scope members.
  virtual void destroy (void);

  /// Visiting.
  virtual int accept (be_visitor *visitor);

  // Narrowing.
  DEF_NARROW_FROM_SCOPE (be_scope);

private:
  /// If set, generate a comma after every element is handled.
  unsigned short comma_;
};

#endif // if !defined
