/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    be_valuetype_fwd.h
 *
 *  $Id$
 *
 *  Extension of class AST_Interface_Fwd that provides additional
 *  means for C++ mapping of a valuetype.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems Inc.
 *  @author Aniruddha Gokhale
 *  @author Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
 */
//=============================================================================

#ifndef BE_VALUETYPE_FWD_H
#define BE_VALUETYPE_FWD_H

#include "be_interface_fwd.h"
#include "ast_valuetype_fwd.h"

class be_valuetype_fwd : public virtual be_interface_fwd,
                         public virtual AST_ValueTypeFwd
{
  // =TITLE
  //   be_valuetype_fwd
  //
  // =DESCRIPTION
  //   Extensions to the be_interface_fwd class
public:
  be_valuetype_fwd (AST_Interface *dummy,
                    UTL_ScopedName *n);

  virtual ~be_valuetype_fwd (void);

  // Visiting.
  virtual int accept (be_visitor* visitor);

  // Cleanup
  virtual void destroy (void);

  // Narrowing.
  DEF_NARROW_FROM_DECL (be_valuetype_fwd);
};

#endif // if !defined
