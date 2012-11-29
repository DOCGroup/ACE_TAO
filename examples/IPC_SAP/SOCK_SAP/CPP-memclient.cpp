// $Id$

// This tests the features of the <ACE_MEM_Connector> and
// <ACE_MEM_Stream> classes.  In addition, it can be used to test the
// oneway and twoway latency and throughput at the socket-level.  This
// is useful as a baseline to compare against ORB-level performance
// for the same types of data.

#include "ace/OS_NS_string.h"
#include "ace/MEM_Connector.h"
#include "ace/INET_Addr.h"
#include "ace/Thread_Manager.h"
#include "ace/Singleton.h"
#include "ace/Get_Opt.h"
#include "ace/High_Res_Timer.h"

static int
run_client (void)
{
  ACE_MEM_Connector connector;
  ACE_MEM_Stream stream;
  ACE_MEM_Addr server_addr (ACE_DEFAULT_SERVER_PORT);

  if (connector.connect (stream, server_addr.get_remote_addr ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("connect")), -1);

  char buf [MAXPATHLEN];
  while (ACE_OS::fgets (buf, MAXPATHLEN, stdin) != 0)
    {
      stream.send (buf, ACE_OS::strlen (buf)+1);
      stream.recv (buf, MAXPATHLEN);
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Echo: %C\n"), buf));
    }

  return 0;
}

int ACE_TMAIN (int, ACE_TCHAR *argv[])
{
  // Initialize the logger.
  ACE_LOG_MSG->open (argv[0]);

  // Run the client
  run_client ();

  return 0;
}
