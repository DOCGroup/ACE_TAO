/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Cos Event Channel tests
//
// = FILENAME
//   MT_Disconnect.h
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef CEC_MT_DISCONNECT_H
#define CEC_MT_DISCONNECT_H

#include "ace/Task.h"
#include "orbsvcs/CosEventChannelAdminC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Task : public ACE_Task_Base
{
public:
  Task (CosEventChannelAdmin::EventChannel_ptr ec,
        int use_callbacks);
  // Create the task...

  // = Check the ACE_Task_Base documentation.
  int svc (void);

  void run_iteration (ACE_ENV_SINGLE_ARG_DECL);
  // Run a single iteration of the test

private:
  CosEventChannelAdmin::EventChannel_var event_channel;
  // The event channel used on the test

  int use_callbacks;
  // Does the event channel send any callback messages when a client
  // diconnects
};

#endif /* EC_DISCONNECT_H */
