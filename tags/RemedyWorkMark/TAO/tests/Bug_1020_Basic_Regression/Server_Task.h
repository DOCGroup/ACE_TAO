/**
 * @file Server_Task.h
 *
 * $Id$
 *
 * @author Balachandran Natarajan <bala@isis-server.isis.vanderbilt.edu>
 *
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
  Server_Task (Test::Echo_ptr echo,
               size_t sz);

  /// The thread entry point.
  virtual int svc (void);

private:
  /// Reference to the test interface
  Test::Echo_var echo_;

  /// Size of data that is traded
  size_t sz_;
};

#include /**/ "ace/post.h"
#endif /* BUG_1020_SERVER_TASK_H*/
