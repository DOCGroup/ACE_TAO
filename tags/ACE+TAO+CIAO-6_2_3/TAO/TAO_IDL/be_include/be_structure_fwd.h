/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    be_structure_fwd.h
 *
 *  $Id$
 *
 *  Extension of class AST_StructureFwd that provides additional
 *  means for C++ mapping of a struct.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef BE_STRUCTURE_FWD_H
#define BE_STRUCTURE_FWD_H

#include "be_type.h"
#include "ast_structure_fwd.h"

class be_visitor;
class be_structure;

class be_structure_fwd : public virtual AST_StructureFwd,
                         public virtual be_type
{
  // =TITLE
  //   be_structure_fwd
  //
  // =DESCRIPTION
  //   Extensions to the AST_StructureFwd class
public:
  be_structure_fwd (AST_Structure *dummy,
                    UTL_ScopedName *n);

  virtual ~be_structure_fwd (void);

  /// Cleanup function.
  virtual void destroy (void);

  // Visiting.
  virtual int accept (be_visitor* visitor);

  // Narrowing.
  DEF_NARROW_FROM_DECL (be_structure_fwd);
};

#endif // if !defined
