/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_interface_fwd.h
//
// = DESCRIPTION
//    Extension of class AST_InterfaceFwd that provides additional
//    means for C++ mapping of a forward declared interface.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef BE_INTERFACE_FWD_H
#define BE_INTERFACE_FWD_H

#include "be_type.h"
#include "ast_interface_fwd.h"

class AST_Interface;
class be_visitor;

class be_interface_fwd : public virtual AST_InterfaceFwd,
                         public virtual be_type
{
  // =TITLE
  //   be_interface_fwd
  //
  // =DESCRIPTION
  //   Extensions to the AST_InterfaceFwd class
public:
  be_interface_fwd (void);
  // Default constructor.

  be_interface_fwd (AST_Interface *dummy,
                    UTL_ScopedName *n);
  // Constructor.

  virtual ~be_interface_fwd (void);
  // Destructor.

  virtual void destroy (void);
  // Cleanup function.

  // Visiting.
  virtual int accept (be_visitor* visitor);

  // Narrowing.
  DEF_NARROW_METHODS2 (be_interface_fwd, AST_InterfaceFwd, be_type);
  DEF_NARROW_FROM_DECL (be_interface_fwd);
};

#endif // if !defined
