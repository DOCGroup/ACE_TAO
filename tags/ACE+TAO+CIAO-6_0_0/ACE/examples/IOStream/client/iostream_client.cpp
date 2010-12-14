// $Id$

#include "ace/SOCK_Connector.h"
#include "ace/IOStream.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_unistd.h"

// This client is a simple example of using the ACE_IOStream and
// ACE_Streambuf_T templates to create an object based on ACE_*_Stream
// classes, which mimic a C++ iostream.

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
#if !defined (ACE_LACKS_ACE_IOSTREAM)
  const ACE_TCHAR *server_host = argc > 1 ? argv[1] : ACE_DEFAULT_SERVER_HOST;
  u_short server_port = argc > 2 ? ACE_OS::atoi (argv[2]) : ACE_DEFAULT_SERVER_PORT;

  ACE_IOStream<ACE_SOCK_Stream> server;
  ACE_SOCK_Connector connector;
  ACE_INET_Addr addr (server_port,
                      server_host);

  if (connector.connect (server, addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "open"),
                      -1);

  // Buffer up some things to send to the server.
  server << "1 2.3 testing" << endl;

  int i;
  float f;

#if defined (ACE_HAS_STRING_CLASS)
  ACE_IOStream_String s1;
  ACE_IOStream_String s2;
  server >> s1 >> i >> f >> s2;

  cerr << "Server said:\n\t";
  cerr << s1 << " ";
  cerr << i << " ";
  cerr << f << " ";
  cerr << s2 << endl;
#else
  server >> i >> f;

  cerr << "(" << ACE_OS::getpid () << ") Server sent:\n\t";
  cerr << "(" << i << ") ";
  cerr << "(" << f << ")" << endl;
#endif /* ACE_HAS_STRING_CLASS */

  if (server.close () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "close"),
                      -1);
#else
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_ERROR ((LM_ERROR, "ACE_IOSTREAM not supported on this platform\n"));
#endif /* !ACE_LACKS_ACE_IOSTREAM */
  return 0;
}

