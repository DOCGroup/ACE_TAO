// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Priority_Reactor_Test.h
//
// = DESCRIPTION
//    This class gets its own header file to work around AIX C++
//    compiler "features" related to template instantiation...  It is
//    only used by Priority_Reactor_Test.cpp.
//
// = AUTHOR
//    Carlos O'Ryan
//
// ============================================================================

#ifndef ACE_TESTS_PRIORITY_REACTOR_TEST_H
#define ACE_TESTS_PRIORITY_REACTOR_TEST_H

#include "ace/Service_Config.h"
#include "ace/Svc_Handler.h"

class Svc_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
  // = TITLE
  //      This class is the product created by both <ACE_Connector>
  //      and <ACE_Acceptor> objects.
  // 
  // = DESCRIPTION
  //    This class gets its own header file to work around AIX C++
  //    compiler "features" related to template instantiation...  It is
  //    only used by Conn_Test.cpp.
  //    Basically it will run as an active object on the clients,
  //    sending an stream of data to the server, where they will be
  //    demultiplexed using a reactive strategy.
{
public:
  Svc_Handler (ACE_Thread_Manager * = 0);
  // Do-nothing constructor.

  virtual int open (void *);
  // Initialization hook.

  void send_data (void);
  // Send data to server.

  virtual int handle_input(ACE_HANDLE handle);
  // Recv data from client.

private:
};

#endif /* ACE_TESTS_PRIORITY_REACTOR_TEST_H */
