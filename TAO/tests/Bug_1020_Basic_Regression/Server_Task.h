/**
 * @file Server_Task.h
 *
 * @author Balachandran Natarajan <bala@isis-server.isis.vanderbilt.edu>
 */
#ifndef BUG_1020_SERVER_TASK_H
#define BUG_1020_SERVER_TASK_H
#include /**/ "ace/pre.h"

#include "TestC.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Server_Task : public ACE_Task_Base
{
public:
  /// Constructor
  explicit Server_Task (Test::Echo_ptr echo);
  Server_Task (Server_Task const &) = delete;
  Server_Task &operator= (Server_Task const &) = delete;
  Server_Task (Server_Task &&) = delete;
  Server_Task &operator= (Server_Task &&) = delete;

  /// The thread entry point.
  int svc () override;

private:
  /// Reference to the test interface
  Test::Echo_var echo_;
};

#include /**/ "ace/post.h"
#endif /* BUG_1020_SERVER_TASK_H*/
