/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel tests
//
// = FILENAME
//   Control.h
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef EC_CONTROL_H
#define EC_CONTROL_H

#include "Counting_Consumer.h"
#include "orbsvcs/Channel_Clients.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Consumer : public EC_Counting_Consumer
{
  // = TITLE
  //   Simple consumer object
  //
  // = DESCRIPTION
  //
public:
  Consumer (const char* name,
            int event_count);
  // Constructor

  // = The RtecEventComm::PushConsumer methods

  virtual void push (const RtecEventComm::EventSet& events
                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ULong shutdown_count_;
  // After this number of events the consumer disconnects from the
  // event service.

  TAO_SYNCH_MUTEX lock_;
  // Synchronize access to the counter
};

#endif /* EC_CONTROL_H */
