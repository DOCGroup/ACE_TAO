/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel examples
//
// = FILENAME
//   Dynamic_Consumer
//
// = AUTHOR
//   Bryan Thrall
//
// ============================================================================

#ifndef DYNAMIC_CONSUMER_H
#define DYNAMIC_CONSUMER_H

#include "Consumer.h"
#include "Supplier.h"
#include "Service_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Dynamic_Consumer : public Consumer
{
public:
  Dynamic_Consumer (SourceID normal_type,
                    SourceID ft_type,
                    SourceID normal_type2,
                    SourceID ft_type2,
                    ACE_Time_Value& worktime,
                    Supplier *fwddest = 0, Service_Handler *handler = 0);
  // Constructor

  Dynamic_Consumer (SourceID normal_type,
                    SourceID ft_type,
                    SourceID normal_type2,
                    SourceID ft_type2,
                    Supplier *fwddest = 0, Service_Handler *handler = 0);
  // Constructor

  virtual ~Dynamic_Consumer(void);

  // = The RtecEventComm::PushConsumer methods

  virtual void push (const RtecEventComm::EventSet& events
                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  EventType norm_type2_;
  EventType ft_type2_;

  long window_; //ordinal of instance of execution
};

#endif /* DYNAMIC_CONSUMER_H */
