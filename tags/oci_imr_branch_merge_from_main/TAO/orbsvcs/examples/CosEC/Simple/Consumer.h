/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS COS Event Channel examples
//
// = FILENAME
//   Consumer
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef CONSUMER_H
#define CONSUMER_H

#include "orbsvcs/CosEventCommS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Consumer : public POA_CosEventComm::PushConsumer
{
  // = TITLE
  //   Simple consumer object
  //
  // = DESCRIPTION
  //   This class is a consumer of events.
  //
public:
  Consumer (void);
  // Constructor

  int run (int argc, char* argv[]);
  // Run the test

  // = The CosEventComm::PushConsumer methods

  virtual void push (const CORBA::Any &event
                     ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The skeleton methods.

private:
  CORBA::ULong event_count_;
  // Keep track of the number of events received.

  CORBA::ORB_ptr orb_;
  // The orb, just a pointer because the ORB does not outlive the
  // run() method...
};

#endif /* CONSUMER_H */
