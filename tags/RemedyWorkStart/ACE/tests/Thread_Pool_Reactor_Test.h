
//=============================================================================
/**
 *  @file    Thread_Pool_Reactor_Test.h
 *
 *  $Id$
 *
 *  This class gets its own header file to work around AIX C++
 *  compiler "features" related to template instantiation...  It is
 *  only used by Thread_Pool_Reactor_Test.cpp.
 *
 *
 *  @author   Irfan Pyarali <irfan@cs.wustl.edu>   Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_TESTS_THREAD_POOL_REACTOR_TEST_H
#define ACE_TESTS_THREAD_POOL_REACTOR_TEST_H

#include "ace/SOCK_Stream.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Svc_Handler.h"

/**
 * @class Request_Handler
 *
 * @brief This class is the Svc_Handler used by <Acceptor>.
 */
class Request_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>
{
public:
  /// The default constructor makes sure the right reactor is used.
  Request_Handler (ACE_Thread_Manager *tm = 0);

protected:
  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  virtual int handle_close (ACE_HANDLE fd, ACE_Reactor_Mask = 0);

private:
  size_t  nr_msgs_rcvd_;
};

#endif /* ACE_TESTS_THREAD_POOL_REACTOR_TEST_H */
