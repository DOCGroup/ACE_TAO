
#ifndef AMI_BUFFERING_CLIENT_TASK_H
#define AMI_BUFFERING_CLIENT_TASK_H
#include /**/ "ace/pre.h"

#include "tao/ORB.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/// Implement a Task to run the experiments using multiple threads.
class Client_Task : public ACE_Task_Base
{
public:
  /// Constructor
  explicit Client_Task (CORBA::ORB_ptr orb);
  Client_Task (Client_Task const &) = delete;
  Client_Task &operator= (Client_Task const &) = delete;
  Client_Task (Client_Task &&) = delete;
  Client_Task &operator= (Client_Task &&) = delete;

  /// Terminate the loop
  void terminate_loop ();

  /// Thread entry point
  int svc () override;

private:
  /// Reference to the ORB
  CORBA::ORB_var orb_;

  /// A mutex, synchronize access to the termination flag
  TAO_SYNCH_MUTEX mutex_;

  /// Termination flag, set to 1 when the client wants this task to
  /// exit
  bool terminate_loop_ {};
};

#include /**/ "ace/post.h"
#endif /* AMI_BUFFERING_CLIENT_TASK_H */
