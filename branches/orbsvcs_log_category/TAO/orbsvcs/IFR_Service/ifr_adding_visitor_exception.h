
/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    ifr_adding_visitor_exception.h
 *
 *  $Id$
 *
 *  Header file for class ifr_adding_visitor_exception.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_IFR_ADDING_VISITOR_EXCEPTION_H
#define TAO_IFR_ADDING_VISITOR_EXCEPTION_H

#include "ifr_adding_visitor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ifr_adding_visitor_exception
 *
 * @brief ifr_adding_visitor_exception.
 *
 * This visitor overrides a few of the visit methods
 * for the case when an exception node is seen in the AST.
 */
class ifr_adding_visitor_exception : public ifr_adding_visitor
{
public:
  /// Constructor.
  ifr_adding_visitor_exception (AST_Decl *scope,
                                CORBA::Boolean in_reopened_);

  /// Destructor.
  virtual ~ifr_adding_visitor_exception (void);

  /// Visit a struct.
  virtual int visit_scope (UTL_Scope *node);

  /// Visit a struct.
  virtual int visit_structure (AST_Structure *node);

  /// Visit a struct.
  virtual int visit_exception (AST_Exception *node);

  /// Visit an enum.
  virtual int visit_enum (AST_Enum *node);

  /// Visit union.
  virtual int visit_union (AST_Union *node);

  /// Lets the visitor one level above acess this value.
  virtual CORBA::IDLType_ptr ir_current (void) const;

private:
  /// Common code called whether we are creating a new exception
  /// or filling out a forward declaration.
  int add_members (AST_Exception *node, CORBA::ExceptionDef_ptr except_def);

private:
  /// Holder for the member list passed to create_struct() or
  /// create_exception().
  CORBA::StructMemberSeq members_;

};

#endif /* TAO_IFR_ADDING_VISITOR_EXCEPTION_H */
