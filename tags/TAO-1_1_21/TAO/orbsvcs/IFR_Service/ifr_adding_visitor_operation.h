// $Id$

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO_IFR_BE_DLL
//
// = FILENAME
//    ifr_adding_visitor_operation.h
//
// = DESCRIPTION
//    Header file for class ifr_adding_visitor_operation.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_IFR_ADDING_VISITOR_OPERATION_H
#define TAO_IFR_ADDING_VISITOR_OPERATION_H

#include "ifr_adding_visitor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ifr_adding_visitor_operation : public ifr_adding_visitor
{
  //
  // = TITLE
  //    ifr_adding_visitor_operation.
  //
  // = DESCRIPTION
  //    This visitor overrides a few of the visit methods
  //    for the case when an operation node is seen in the AST.
  //
public:
  ifr_adding_visitor_operation (AST_Decl *scope);
  // Constructor.

  virtual ~ifr_adding_visitor_operation (void);
  // Destructor.

  virtual int visit_operation (AST_Operation *node);
  // Visit an operation.

  virtual int visit_argument (AST_Argument *node);
  // Visit argument.

private:
  CORBA_ParDescriptionSeq params_;
  // Holder for the operation parameter info passed to the
  // interface repository method create_operation().

  CORBA::ULong index_;
  // Counter for building the parameter list.
};

#endif /* TAO_IFR_ADDING_VISITOR_OPERATION_H */

