// -*- C++ -*-
#ifndef MT_BIDIR_SERVER_TASK_H
#define MT_BIDIR_SERVER_TASK_H

#include /**/ "ace/pre.h"

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB.h"

class Sender_i;

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Manual_Event;
ACE_END_VERSIONED_NAMESPACE_DECL

/// Implement a Task to run the experiments using multiple threads.
class Server_Task : public ACE_Task_Base
{
public:
  /// Constructor
  Server_Task (Sender_i &sender,
               ACE_Manual_Event &event,
               int no_iterations,
               CORBA::ORB_ptr orb,
               ACE_Thread_Manager *thr_mgr);
  Server_Task (Server_Task const &) = delete;
  Server_Task (Server_Task &&) = delete;
  Server_Task &operator= (Server_Task const &) = delete;
  Server_Task &operator= (Server_Task &&) = delete;

  /// Thread entry point
  int svc () override;

private:
  /// The sender object, whose lifetime is managed by the POA.
  Sender_i &sender_;

  /// The synchronizer
  ACE_Manual_Event &event_;

  /// Number of messages that needs to be sent
  int const no_iterations_;

  /// The ORB on which we run..
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"

#endif /* MT_BIDIR_SERVER_TASK_H */
