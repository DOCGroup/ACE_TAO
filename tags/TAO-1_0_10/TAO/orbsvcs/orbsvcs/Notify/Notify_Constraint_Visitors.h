/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_Constraint_Visitors.h
//
// = DESCRIPTION
//
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef NOTIFY_CONSTRAINT_VISITORS_H
#define NOTIFY_CONSTRAINT_VISITORS_H

#include "ace/Hash_Map_Manager.h"
#include "orbsvcs/orbsvcs/Trader/Constraint_Visitors.h"
#include "orbsvcs/CosNotificationC.h"

class TAO_Notify_Constraint_Evaluator : public TAO_Constraint_Evaluator
{
 public:
  TAO_Notify_Constraint_Evaluator (const CosNotification::StructuredEvent& s_event);
  // Constructor.

  virtual int visit_property (TAO_Property_Constraint* literal);

 private:
  ACE_Hash_Map_Manager <ACE_CString, const CORBA::Any*,
    ACE_SYNCH_MUTEX> property_lookup_;
  // Used to lookup property name and values
};

#endif /* NOTIFY_CONSTRAINT_VISITORS_H */
