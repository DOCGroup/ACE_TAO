/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_structure.h
//
// = DESCRIPTION
//    Extension of class AST_Structure that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

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
  be_structure (void);
  // Default constructor.

  be_structure (UTL_ScopedName *n,
                idl_bool local,
                idl_bool abstract);
  // Constructor.

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

  virtual void destroy (void);
  // Cleanup method.

  virtual int accept (be_visitor *visitor);
  // Visiting.

 // Narrowing.
  DEF_NARROW_METHODS3 (be_structure, AST_Structure, be_scope, be_type);
  DEF_NARROW_FROM_DECL (be_structure);
  DEF_NARROW_FROM_SCOPE (be_structure);

protected:
  virtual int compute_size_type (void);
  // Compute the size type if it is unknown.
};

#endif
