
/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    ifr_adding_visitor_structure.h
 *
 *  $Id$
 *
 *  Header file for class ifr_adding_visitor_structure.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_IFR_ADDING_VISITOR_STRUCTURE_H
#define TAO_IFR_ADDING_VISITOR_STRUCTURE_H

#include "ifr_adding_visitor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ifr_adding_visitor_structure
 *
 * @brief ifr_adding_visitor_structure.
 *
 * This visitor overrides a few of the visit methods
 * for the case when a struct or exception node is seen in the AST.
 */
class ifr_adding_visitor_structure : public ifr_adding_visitor
{
public:
  ifr_adding_visitor_structure (AST_Decl *scope);

  virtual ~ifr_adding_visitor_structure (void);

  virtual int visit_scope (UTL_Scope *node);
  virtual int visit_structure (AST_Structure *node);
  virtual int visit_enum (AST_Enum *node);
  virtual int visit_union (AST_Union *node);

  /// Lets the visitor one level above acess this value.
  virtual CORBA::IDLType_ptr ir_current (void) const;

private:
  /// Common code called whether we are creating a new struct
  /// or filling out a forward declaration.
  int add_members (AST_Structure *node, CORBA::StructDef_ptr struct_def);

private:
  /// Holder for the member list passed to create_struct() or
  /// create_exception().
  CORBA::StructMemberSeq members_;
};

#endif /* TAO_IFR_ADDING_VISITOR_STRUCTURE_H */
