/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_valuetype_fwd.h
//
// = DESCRIPTION
//    Extension of class AST_Interface_Fwd that provides additional
//    means for C++ mapping of a valuetype.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale,
//    changed for valuetypes from Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//
// ============================================================================

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
  be_valuetype_fwd (void);
  // Default constructor.

  be_valuetype_fwd (AST_Interface *dummy,
                    UTL_ScopedName *n);
  // Constructor.

  virtual ~be_valuetype_fwd (void);
  // Destructor.

  // Visiting.
  virtual int accept (be_visitor* visitor);

  // Cleanup
  virtual void destroy (void);

  // Narrowing.
  DEF_NARROW_METHODS2 (be_valuetype_fwd, be_interface_fwd, AST_ValueTypeFwd);
  DEF_NARROW_FROM_DECL (be_valuetype_fwd);
};

#endif // if !defined
