
/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    ifr_adding_visitor_operation.h
 *
 *  $Id$
 *
 *  Header file for class ifr_adding_visitor_operation.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_IFR_ADDING_VISITOR_OPERATION_H
#define TAO_IFR_ADDING_VISITOR_OPERATION_H

#include "ifr_adding_visitor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ifr_adding_visitor_operation
 *
 * @brief ifr_adding_visitor_operation.
 *
 * This visitor overrides a few of the visit methods
 * for the case when an operation node is seen in the AST.
 */
class ifr_adding_visitor_operation : public ifr_adding_visitor
{
public:
  /// Constructor.
  ifr_adding_visitor_operation (AST_Decl *scope);

  /// Destructor.
  virtual ~ifr_adding_visitor_operation (void);

  /// Visit an operation.
  virtual int visit_operation (AST_Operation *node);

  /// Visit argument.
  virtual int visit_argument (AST_Argument *node);

private:
  /// Holder for the operation parameter info passed to the
  /// interface repository method create_operation().
  CORBA::ParDescriptionSeq params_;

  /// Counter for building the parameter list.
  CORBA::ULong index_;
};

#endif /* TAO_IFR_ADDING_VISITOR_OPERATION_H */
