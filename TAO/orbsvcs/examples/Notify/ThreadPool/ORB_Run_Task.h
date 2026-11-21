/* -*- C++ -*- */
/**
 *  @file ORB_Run_Task.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_ORB_RUN_TASK_H
#define TAO_Notify_ORB_RUN_TASK_H

#include /**/ "ace/pre.h"
#include "ace/Task.h"

#include "ORB_Objects.h"

/**
 * @class TAO_Notify_ORB_Run_Task
 *
 * @brief Run the ORB::run method in a separate thread.
 */
class TAO_Notify_ORB_Run_Task : public ACE_Task_Base
{
public:
  explicit TAO_Notify_ORB_Run_Task (TAO_Notify_ORB_Objects& orb_objects);

  TAO_Notify_ORB_Run_Task (TAO_Notify_ORB_Run_Task const&) = delete;
  TAO_Notify_ORB_Run_Task (TAO_Notify_ORB_Run_Task&&) = delete;
  TAO_Notify_ORB_Run_Task& operator= (TAO_Notify_ORB_Run_Task const&) = delete;
  TAO_Notify_ORB_Run_Task& operator= (TAO_Notify_ORB_Run_Task&&) = delete;

  ~TAO_Notify_ORB_Run_Task () override = default;

  /// The thread entry point.
  int svc () override;

private:
  /// ORB Objects.
  TAO_Notify_ORB_Objects& orb_objects_;
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_ORB_RUN_TASK_H */
