/* -*- C++ -*- */
/**
 *  @file   Consumer.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 *  Consumer
 */

#ifndef CONSUMER_H
#define CONSUMER_H

#include "orbsvcs/RtecEventCommS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Consumer
 *
 * @brief Simple consumer object
 *
 * This class is a consumer of events. It simply registers for one event type.
 */
class Consumer : public POA_RtecEventComm::PushConsumer
{
public:
  /// Constructor
  Consumer (void);

  /// Run the test
  int run (int argc, char* argv[]);

  // = The RtecEventComm::PushConsumer methods

  // The skeleton methods.
  virtual void push (const RtecEventComm::EventSet& events
                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  int parse_args (int argc, char *argv[]);

  /// Keep track of the number of events received.
  CORBA::ULong event_count_;

  /// The orb, just a pointer because the ORB does not outlive the
  /// run() method...
  CORBA::ORB_ptr orb_;
};

#endif /* CONSUMER_H */
