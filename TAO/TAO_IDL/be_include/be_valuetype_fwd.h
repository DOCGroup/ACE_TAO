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

#ifdef IDL_HAS_VALUETYPE

#include "be_interface_fwd.h"

class be_valuetype_fwd : public virtual be_interface_fwd
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
                    UTL_ScopedName *n, 
                    UTL_StrList *p);
  // Constructor.

  virtual ~be_valuetype_fwd (void);
  // Destructor.

  virtual void set_abstract_valuetype (void);

  virtual int gen_var_defn (char *local_name = 0);
  // Generate the _var class definition.

  virtual int gen_var_impl (char *local_name = 0,
                            char *full_name = 0);
  // Generate the implementation for the _var class.

  virtual int gen_out_defn (char *local_name = 0);
  // Generate the _out class definition.

  virtual int gen_out_impl (char *local_name = 0,
                            char *full_name = 0);
  // Generate the _out implementation.

  // Visiting.
  virtual int accept (be_visitor* visitor);

  // Narrowing.
  DEF_NARROW_METHODS1 (be_valuetype_fwd, be_interface_fwd);
  DEF_NARROW_FROM_DECL (be_valuetype_fwd);

};

#endif /* IDL_HAS_VALUETYPE */

#endif // if !defined
