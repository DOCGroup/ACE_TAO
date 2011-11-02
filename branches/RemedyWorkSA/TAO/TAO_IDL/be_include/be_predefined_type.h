/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    be_predefined_type.h
 *
 *  $Id$
 *
 *  Extension of class AST_PredefinedType that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_PREDEFINED_TYPE_H
#define _BE_PREDEFINED_TYPE_H

#include "be_type.h"
#include "ast_predefined_type.h"

class be_visitor;

class be_predefined_type : public virtual AST_PredefinedType,
                           public virtual be_type
{
public:
  be_predefined_type (AST_PredefinedType::PredefinedType t,
                      UTL_ScopedName *n);

  // Overridden from class be_type.
  virtual void gen_member_ostream_operator (TAO_OutStream *os,
                                            const char *instance_name,
                                            bool use_underscore,
                                            bool accessor = false);

  // Visiting.
  virtual int accept (be_visitor* visitor);

  // Cleanup.
  virtual void destroy (void);

  // Narrowing.
  DEF_NARROW_FROM_DECL (be_predefined_type);

protected:
  /// Overridden method.
  virtual void compute_tc_name (void);

  // Compute and return the value.

  /**
   * The object and TypeCode repository IDs have a mandatory omg.org
   * prefix, but the default compute_repoID() uses the #pragma's to
   * obtain prefixes.
   */
  virtual void compute_repoID (void);
};

#endif
