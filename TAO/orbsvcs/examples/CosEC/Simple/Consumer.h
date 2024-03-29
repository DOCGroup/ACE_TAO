/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Consumer.h
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef CONSUMER_H
#define CONSUMER_H

#include "orbsvcs/CosEventCommS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Consumer
 *
 * @brief Simple consumer object
 *
 * This class is a consumer of events.
 */
class Consumer : public POA_CosEventComm::PushConsumer
{
public:
  /// Constructor
  Consumer ();

  /// Run the test
  int run (int argc, ACE_TCHAR* argv[]);

  // = The CosEventComm::PushConsumer methods

  /// The skeleton methods.
  virtual void push (const CORBA::Any &event);
  virtual void disconnect_push_consumer ();

private:
  /// Keep track of the number of events received.
  CORBA::ULong event_count_;

  /// The orb, just a pointer because the ORB does not outlive the
  /// run() method...
  CORBA::ORB_ptr orb_;
};

#endif /* CONSUMER_H */
