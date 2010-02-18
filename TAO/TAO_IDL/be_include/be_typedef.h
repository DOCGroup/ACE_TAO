
//=============================================================================
/**
 *  @file    be_typedef.h
 *
 *  $Id$
 *
 *  Extension of class AST_typedef that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#ifndef BE_TYPEDEF_H
#define BE_TYPEDEF_H

#include "be_type.h"
#include "ast_typedef.h"

class be_visitor;

class be_typedef : public virtual AST_Typedef,
                   public virtual be_type
{
public:
  be_typedef (AST_Type *bt,
              UTL_ScopedName *n,
              bool l,
              bool a);

  /// Overrides for be_type members. If we have been
  /// defined, we want the underlying type to be set as well.
  virtual void seen_in_sequence (bool val);
  virtual bool seen_in_operation (void) const;
  virtual void seen_in_operation (bool val);

  /// Return the most primitive base type by traversing the chain of typedefed
  /// base types.
  be_type *primitive_base_type (void);

  /// Return the most "unaliased" type node for the base type (see
  /// be_type.h).
  virtual AST_Decl::NodeType base_node_type (void) const;

  /// Overridden from class be_type.
  virtual void gen_member_ostream_operator (TAO_OutStream *os,
                                            const char *instance_name,
                                            bool use_underscore,
                                            bool accessor = false);

  /// Cleanup function.
  virtual void destroy (void);

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Narrowing.

  DEF_NARROW_FROM_DECL (be_typedef);
};

#endif
