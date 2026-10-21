/**
 * @file Server_Task.h
 * @author Will Otte <w.otte@dre.vanderbilt.edu>
 *
 * Defines the Server_Task class that acts as a process colocated ORB server
 * for Bug_1495_Regression.
 */

#ifndef BUG1495_SERVER_TASK_H
#define BUG1495_SERVER_TASK_H

#include /**/ "ace/pre.h"
#include "ace/Task.h"

#include "tao/ORB.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Manual_Event;
ACE_END_VERSIONED_NAMESPACE_DECL

/// Implements the Task to run the process colocated thread.
class Server_Task : public ACE_Task_Base
{
  public:
    /// Constructor
    Server_Task (ACE_TCHAR const *output,
                 CORBA::ORB_ptr sorb,
                 ACE_Manual_Event &me,
                 ACE_Thread_Manager *thr_mgr);
    Server_Task (Server_Task const &) = delete;
    Server_Task &operator= (Server_Task const &) = delete;
    Server_Task (Server_Task &&) = delete;
    Server_Task &operator= (Server_Task &&) = delete;

    /// Thread entry point
    int svc () override;

  private:
    /// Output file for the server IOR
    ACE_TCHAR const *output_;

    /// Manual thread event to signal the main thread to create
    /// the client thread.
    ACE_Manual_Event &me_;

    /// Points to my ORB.
    CORBA::ORB_var sorb_;
};

#include /**/ "ace/post.h"

#endif /* BUG1495_SERVER_TASK_H */
