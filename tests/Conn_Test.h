#include "ace/Service_Config.h"
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

  void send_data (void);
  // Send data to server.

  void recv_data (void);
  // Recv data from client.

  int close (u_long = 0);
  // Shutdown the <Svc_Handler>.

  void in_use (int);
  // Set the use flag (i.e., 1 means "in use", 0 means "not in use").

  int in_use (void);
  // Returns the current use flag.

  int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
		    ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);

private:
  int in_use_;
  // Are we currently in use?
};
