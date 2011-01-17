// $Id$

/* -*- C++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO_IFR_BE_DLL
//
// = FILENAME
//    ifr_adding_visitor_union.h
//
// = DESCRIPTION
//    Header file for class ifr_adding_visitor_union.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_IFR_ADDING_VISITOR_UNION_H
#define TAO_IFR_ADDING_VISITOR_UNION_H

#include "ifr_adding_visitor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ifr_adding_visitor_union : public ifr_adding_visitor
{
  //
  // = TITLE
  //    ifr_adding_visitor_exception.
  //
  // = DESCRIPTION
  //    This visitor overrides a few of the visit methods
  //    for the case when a union node is seen in the AST.
  //
public:
  ifr_adding_visitor_union (AST_Decl *scope);

  virtual ~ifr_adding_visitor_union (void);

  virtual int visit_scope (UTL_Scope *node);
  virtual int visit_structure (AST_Structure *node);
  virtual int visit_enum (AST_Enum *node);
  virtual int visit_union (AST_Union *node);

  virtual CORBA::IDLType_ptr ir_current (void) const;
  // Lets the visitor one level above acess this value.

private:
  int add_members (AST_Union *node, CORBA::UnionDef_ptr union_def);
  // Common code called whether we are creating a new union
  // or filling out a forward declaration.

private:
  CORBA::UnionMemberSeq members_;
  // Holder for the member list passed to create_struct() or
  // create_exception().

  CORBA::TypeCode_var disc_tc_;
  // Holds the typecode of the discriminator (used only if the
  // discriminator is an enum type, so we can insert the label
  // values into Anys for UnionMemberSeq[i].label).
};

#endif /* TAO_IFR_ADDING_VISITOR_UNION_H */
