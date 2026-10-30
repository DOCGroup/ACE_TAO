/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    be_interface_fwd.h
 *
 *  Extension of class AST_InterfaceFwd that provides additional
 *  means for C++ mapping of a forward declared interface.
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

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
  be_interface_fwd (AST_Interface *dummy,
                    UTL_ScopedName *n);

  ~be_interface_fwd () override;

  /// Mutator overrides for be_type members. If we have been
  /// defined, we want the full definition to be set as well.
  void seen_in_sequence (bool val) override;
  void seen_in_operation (bool val) override;

  /// Cleanup function.
  void destroy () override;

  // Visiting.
  int accept (be_visitor* visitor) override;
};

#endif // if !defined
