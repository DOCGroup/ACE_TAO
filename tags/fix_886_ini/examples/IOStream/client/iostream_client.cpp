// $Id$

#include "ace/SOCK_Connector.h"
#include "ace/IOStream.h"
#include "ace/Log_Msg.h"

ACE_RCSID(client, iostream_client, "$Id$")

// This client is a simple example of using the ACE_IOStream and
// ACE_Streambuf_T templates to create an object based on ACE_*_Stream
// classes, which mimic a C++ iostream.

int
main (int argc, char *argv[])
{
#if !defined (ACE_LACKS_ACE_IOSTREAM)
  const char *server_host = argc > 1 ? argv[1] : ACE_DEFAULT_SERVER_HOST;
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


#if !defined (ACE_LACKS_ACE_IOSTREAM)
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_IOStream <ACE_SOCK_Stream>;
template class ACE_Streambuf_T <ACE_SOCK_Stream>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_IOStream <ACE_SOCK_Stream>
#pragma instantiate ACE_Streambuf_T <ACE_SOCK_Stream>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* !ACE_LACKS_ACE_IOSTREAM */
