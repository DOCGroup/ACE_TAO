/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel examples
//
// = FILENAME
//   Dynamic_Supplier
//
// = AUTHOR
//   Bryan Thrall (thrall@cs.wustl.edu)
//
// ============================================================================

#ifndef DYNAMIC_SUPPLIER_H
#define DYNAMIC_SUPPLIER_H

#include "Supplier.h"
#include "Service_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Dynamic_Supplier : public Supplier
{
public:
  ///Calls up to Service_Handler every time a timeout
  ///occurs. normal_channel_id1 is used as id returned by get_id().
  Dynamic_Supplier(SourceID id,
                   EventType normal_type1,
                   EventType normal_type2,
                   EventType ft_type1,
                   EventType ft_type2,
                   Service_Handler *handler = 0);

  virtual ~Dynamic_Supplier(void);

  ///Overrides Supplier::timeout_occurred()
  virtual void timeout_occured (Object_ID& oid ACE_ENV_ARG_DECL);

protected:
  RtecEventComm::EventSourceID norm_type2_;
  RtecEventComm::EventSourceID ft_type2_;
}; //class Dynamic_Supplier

#endif //DYNAMIC_SUPPLIER_H
