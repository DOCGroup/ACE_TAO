// $Id$

// ========================================================================
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    Log_Constraint_Visitors.h
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ========================================================================

#ifndef TAO_LOG_CONSTRAINT_VISITOR_H
#define TAO_LOG_CONSTRAINT_VISITOR_H
#include "ace/pre.h"

#include "orbsvcs/orbsvcs/Trader/Constraint_Visitors.h"
#include "orbsvcs/DsLogAdminC.h"
#include "log_export.h"

class TAO_Log_Export TAO_Log_Constraint_Evaluator : public TAO_Constraint_Evaluator
{
 public:
  TAO_Log_Constraint_Evaluator (DsLogAdmin::LogRecord &rec);
  // Constructor.

  virtual int visit_property (TAO_Property_Constraint* literal);

 private:
  DsLogAdmin::LogRecord &rec_;
  // TAO_Property_Evaluator prop_eval_;
  // Utility with which to evaluate the properties of an offer, be
  // they dyanmic or static.
};

#include "ace/post.h"
#endif /* TAO_LOG_CONSTRAINT_VISITOR_H */
