
//=============================================================================
/**
 *  @file    Cached_Conn_Test.h
 *
 *  $Id$
 *
 *  Define class needed for generating templates. IBM C++ requires this to
 *  be in its own file for auto template instantiation.
 *
 *
 *  @author Kirthika Parameswaran  <kirthika@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_TESTS_CACHED_CONN_TEST_H
#define ACE_TESTS_CACHED_CONN_TEST_H

#include "ace/SOCK_Stream.h"
#include "ace/Svc_Handler.h"

class Svc_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:

  Svc_Handler (ACE_Thread_Manager *t = 0);
  int open (void *v = 0);
  int close (u_long flags = 0);
};

#endif /* ACE_TESTS_CACHED_ACCEPT_CONN_TEST_H */
