
/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    ifr_adding_visitor_union.h
 *
 *  $Id$
 *
 *  Header file for class ifr_adding_visitor_union.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_IFR_ADDING_VISITOR_UNION_H
#define TAO_IFR_ADDING_VISITOR_UNION_H

#include "ifr_adding_visitor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ifr_adding_visitor_union
 *
 * @brief ifr_adding_visitor_exception.
 *
 * This visitor overrides a few of the visit methods
 * for the case when a union node is seen in the AST.
 */
class ifr_adding_visitor_union : public ifr_adding_visitor
{
public:
  ifr_adding_visitor_union (AST_Decl *scope);

  virtual ~ifr_adding_visitor_union (void);

  virtual int visit_scope (UTL_Scope *node);
  virtual int visit_structure (AST_Structure *node);
  virtual int visit_enum (AST_Enum *node);
  virtual int visit_union (AST_Union *node);

  /// Lets the visitor one level above acess this value.
  virtual CORBA::IDLType_ptr ir_current (void) const;

private:
  /// Common code called whether we are creating a new union
  /// or filling out a forward declaration.
  int add_members (AST_Union *node, CORBA::UnionDef_ptr union_def);

private:
  /// Holder for the member list passed to create_struct() or
  /// create_exception().
  CORBA::UnionMemberSeq members_;

  /**
   * Holds the typecode of the discriminator (used only if the
   * discriminator is an enum type, so we can insert the label
   * values into Anys for UnionMemberSeq[i].label).
   */
  CORBA::TypeCode_var disc_tc_;
};

#endif /* TAO_IFR_ADDING_VISITOR_UNION_H */
