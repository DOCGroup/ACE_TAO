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
  /*
  int deadline_missed_;
  ACE_Time_Value worktime_;

  Supplier *fwddest_;
  //RtecScheduler::handle_t rt_info_;
  InfoHandle rt_info_;

  Service_Handler * handler_;

  EventType norm_type_;
  EventType ft_type_;
  */

  EventType norm_type2_;
  EventType ft_type2_;

  bool received_ft_type1_;
  bool received_ft_type2_;
};

#endif /* DYNAMIC_CONSUMER_H */
