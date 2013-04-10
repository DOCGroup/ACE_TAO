/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    be_structure.h
 *
 *  $Id$
 *
 *  Extension of class AST_Structure that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#ifndef BE_STRUCTURE_H
#define BE_STRUCTURE_H

#include "be_scope.h"
#include "be_type.h"
#include "ast_structure.h"

class be_visitor;

class be_structure : public virtual AST_Structure,
                     public virtual be_scope,
                     public virtual be_type
{
public:
  be_structure (UTL_ScopedName *n,
                bool local,
                bool abstract);

  be_structure (AST_Decl::NodeType nt,
                UTL_ScopedName *n,
                bool local,
                bool abstract);

  /// Copy BE-specific values when redefining struct or union
  /// from a forward declaration.
  virtual void redefine (AST_Structure *from);

  /// Overridden from class be_type.
  virtual void gen_ostream_operator (TAO_OutStream *os,
                                     bool use_underscore);

  /// Cleanup method.
  virtual void destroy (void);

  /// Visiting.
  virtual int accept (be_visitor *visitor);

 // Narrowing.

  DEF_NARROW_FROM_DECL (be_structure);
  DEF_NARROW_FROM_SCOPE (be_structure);
};

#endif
