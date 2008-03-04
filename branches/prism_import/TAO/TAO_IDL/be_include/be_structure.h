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

  be_structure (UTL_ScopedName *n,
                bool local,
                bool abstract);

  virtual void redefine (AST_Structure *from);
  // Copy BE-specific values when redefining struct or union
  // from a forward declaration.
  
  virtual void gen_ostream_operator (TAO_OutStream *os);
  // Overridden from class be_type.

  virtual void destroy (void);
  // Cleanup method.

  virtual int accept (be_visitor *visitor);
  // Visiting.

  AST_Field *be_add_field (AST_Field *f);
  // To access the protected base class method fe_add_field.

 // Narrowing.

  DEF_NARROW_FROM_DECL (be_structure);
  DEF_NARROW_FROM_SCOPE (be_structure);
};

#endif
