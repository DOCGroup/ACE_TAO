
//=============================================================================
/**
 *  @file    NonBlocking_Conn_Test.h
 *
 *  $Id$
 *
 *  This test checks for the proper working of the following:
 *  - blocking connections
 *  - blocking connections with timeouts
 *  - non-blocking connections
 *  - non-blocking connections without waiting for completions
 *  - non-blocking connections with timeouts
 *
 *
 *  @author Irfan Pyarali <irfan@oomworks.com>
 */
//=============================================================================


#ifndef NONBLOCKING_CONN_TEST_H
#define NONBLOCKING_CONN_TEST_H

#include "ace/Svc_Handler.h"
#include "ace/SOCK_Stream.h"

class Svc_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:

  enum Connection_Status
    {
      Conn_SUCCEEDED,
      Conn_FAILED
    };

  enum Completion_Status
    {
      Comp_YES,
      Comp_NO,
      Comp_IGNORE
    };

  Svc_Handler (bool is_ref_counted = false);

  void connection_status (Connection_Status &status,
                          int &completion_counter);

  int open (void *);

  int handle_close (ACE_HANDLE handle,
                    ACE_Reactor_Mask mask);

  Connection_Status *status_;
  int *completion_counter_;
  bool is_ref_counted_;
};

#endif /* NONBLOCKING_CONN_TEST_H */
