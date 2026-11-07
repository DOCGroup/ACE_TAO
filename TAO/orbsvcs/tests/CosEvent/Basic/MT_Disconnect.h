/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   MT_Disconnect.h
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
            bool use_callbacks);
  MTD_Task (MTD_Task const&) = delete;
  MTD_Task (MTD_Task&&) = delete;
  MTD_Task& operator= (MTD_Task const&) = delete;
  MTD_Task& operator= (MTD_Task&&) = delete;

  // = Check the ACE_Task_Base documentation.
  int svc () override;

  /// Run a single iteration of the test
  void run_iteration ();

private:
  /// The event channel used on the test
  CosEventChannelAdmin::EventChannel_var event_channel;

  /// Does the event channel send any callback messages when a client
  /// diconnects
  bool use_callbacks_;
};

#endif /* EC_DISCONNECT_H */
