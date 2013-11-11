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


#ifndef CEC_MT_DISCONNECT_H
#define CEC_MT_DISCONNECT_H

#include "ace/Task.h"
#include "orbsvcs/CosEventChannelAdminC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class MTD_Task : public ACE_Task_Base
{
public:
  /// Create the task...
  MTD_Task (CosEventChannelAdmin::EventChannel_ptr ec,
        int use_callbacks);

  // = Check the ACE_Task_Base documentation.
  int svc (void);

  /// Run a single iteration of the test
  void run_iteration (void);

private:
  /// The event channel used on the test
  CosEventChannelAdmin::EventChannel_var event_channel;

  /// Does the event channel send any callback messages when a client
  /// diconnects
  int use_callbacks;
};

#endif /* EC_DISCONNECT_H */
