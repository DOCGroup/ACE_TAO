/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Control.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef EC_CONTROL_H
#define EC_CONTROL_H

#include "Counting_Consumer.h"
#include "orbsvcs/Channel_Clients.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Consumer
 *
 * @brief Simple consumer object
 *
 */
class Consumer : public EC_Counting_Consumer
{
public:
  /// Constructor
  Consumer (const char* name,
            int event_count);

  // = The RtecEventComm::PushConsumer methods

  virtual void push (const RtecEventComm::EventSet& events
                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// After this number of events the consumer disconnects from the
  /// event service.
  CORBA::ULong shutdown_count_;

  /// Synchronize access to the counter
  TAO_SYNCH_MUTEX lock_;
};

#endif /* EC_CONTROL_H */
