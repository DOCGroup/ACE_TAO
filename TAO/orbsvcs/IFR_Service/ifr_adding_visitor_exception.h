// $Id$

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO_IFR_BE_DLL
//
// = FILENAME
//    ifr_adding_visitor_exception.h
//
// = DESCRIPTION
//    Header file for class ifr_adding_visitor_exception.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_IFR_ADDING_VISITOR_EXCEPTION_H
#define TAO_IFR_ADDING_VISITOR_EXCEPTION_H

#include "ifr_adding_visitor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ifr_adding_visitor_exception : public ifr_adding_visitor
{
  //
  // = TITLE
  //    ifr_adding_visitor_exception.
  //
  // = DESCRIPTION
  //    This visitor overrides a few of the visit methods
  //    for the case when an exception node is seen in the AST.
  //
public:
  ifr_adding_visitor_exception (AST_Decl *scope,
                                CORBA::Boolean in_reopened_);
  // Constructor.

  virtual ~ifr_adding_visitor_exception (void);
  // Destructor.

  virtual int visit_scope (UTL_Scope *node);
  // Visit a struct.

  virtual int visit_structure (AST_Structure *node);
  // Visit a struct.

  virtual int visit_exception (AST_Exception *node);
  // Visit a struct.

  virtual int visit_enum (AST_Enum *node);
  // Visit an enum.

  virtual int visit_union (AST_Union *node);
  // Visit union.

  virtual CORBA::IDLType_ptr ir_current (void) const;
  // Lets the visitor one level above acess this value.

private:
  CORBA::Boolean is_nested_;
  // Is this visitor visiting a struct that was defined inside
  // a union, an exception, or another struct?

  CORBA::StructMemberSeq members_;
  // Holder for the member list passed to create_struct() or
  // create_exception().

  ACE_Unbounded_Queue<CORBA::Contained_ptr> move_queue_;
  // IR objects that must be moved into the scope (struct or
  // exception) where they were declared in the IDL file.
};

#endif /* TAO_IFR_ADDING_VISITOR_EXCEPTION_H */
