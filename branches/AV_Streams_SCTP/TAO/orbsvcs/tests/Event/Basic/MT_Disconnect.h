/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   MT_Disconnect.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef EC_MT_DISCONNECT_H
#define EC_MT_DISCONNECT_H

#include "ace/Task.h"
#include "orbsvcs/RtecEventChannelAdminC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Task : public ACE_Task_Base
{
public:
  /// Create the task...
  Task (RtecEventChannelAdmin::EventChannel_ptr ec,
        int use_callbacks);

  // = Check the ACE_Task_Base documentation.
  int svc (void);

  /// Run a single iteration of the test
  void run_iteration (ACE_ENV_SINGLE_ARG_DECL);

private:
  /// The event channel used on the test
  RtecEventChannelAdmin::EventChannel_var event_channel;

  /// Does the event channel send any callback messages when a client
  /// diconnects
  int use_callbacks;
};

#endif /* EC_DISCONNECT_H */
