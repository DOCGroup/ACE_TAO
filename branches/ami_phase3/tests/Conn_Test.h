// $Id$

#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Svc_Handler.h"
#include "ace/SOCK_Stream.h"

class Svc_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
  // = TITLE
  //      This class is the product created by both <ACE_Connector>
  //      and <ACE_Acceptor> objects.
  //
  // = DESCRIPTION
  //    This class gets its own header file to work around AIX C++
  //    compiler "features" related to template instantiation...  It is
  //    only used by Conn_Test.cpp.
{
public:
  Svc_Handler (ACE_Thread_Manager * = 0);
  // Do-nothing constructor.

  virtual int open (void *);
  // Initialization hook.

  virtual int recycle (void * = 0);
  // Prepare for recycling.

  void send_data (void);
  // Send data to server.

  void recv_data (void);
  // Recv data from client.

  int close (u_long = 0);
  // Shutdown the <Svc_Handler>.

  int idle (u_long flags);
  // Set <this> to idle.
};
