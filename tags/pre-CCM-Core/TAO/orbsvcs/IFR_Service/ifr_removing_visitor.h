// $Id$

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO_IFR_BE_DLL
//
// = FILENAME
//    ifr_removing_visitor.h
//
// = DESCRIPTION
//    Header file for class ifr_removing_visitor.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_IFR_REMOVING_VISITOR_H
#define TAO_IFR_REMOVING_VISITOR_H

#include "ifr_visitor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ifr_removing_visitor : public ifr_visitor
{
  //
  // = TITLE
  //    ifr_removing_visitor.
  //
  // = DESCRIPTION
  //    This visitor removes items found in the IDL file being processed
  //    from the Interface Repository.
  //
public:
  ifr_removing_visitor (void);
  // Constructor.

  virtual ~ifr_removing_visitor (void);
  // Destructor.

  virtual int visit_scope (UTL_Scope *node);
  // Visit the scope.
};

#endif /* TAO_IFR_REMOVING_VISITOR_H */

