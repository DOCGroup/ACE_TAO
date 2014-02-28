
/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    ifr_removing_visitor.h
 *
 *  $Id$
 *
 *  Header file for class ifr_removing_visitor.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_IFR_REMOVING_VISITOR_H
#define TAO_IFR_REMOVING_VISITOR_H

#include "ifr_visitor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ifr_removing_visitor
 *
 * @brief ifr_removing_visitor.
 *
 * This visitor removes items found in the IDL file being processed
 * from the Interface Repository.
 */
class ifr_removing_visitor : public ifr_visitor
{
public:
  /// Constructor.
  ifr_removing_visitor (void);

  /// Destructor.
  virtual ~ifr_removing_visitor (void);

  /// Visit the scope.
  virtual int visit_scope (UTL_Scope *node);

  /// Visit the root.
  virtual int visit_root (AST_Root *node);
};

#endif /* TAO_IFR_REMOVING_VISITOR_H */

