#include "ace/SOCK_Connector.h"
#include "ace/IOStream.h"

int main (int argc, char *argv[])
{
  char *server_host = argc > 1 ? argv[1] : ACE_DEFAULT_SERVER_HOST;
  u_short server_port = argc > 2 ? ACE_OS::atoi (argv[2]) : ACE_DEFAULT_SERVER_PORT;

  ACE_IOStream<ACE_SOCK_Stream> server;
  ACE_SOCK_Connector connector;
  ACE_INET_Addr addr (server_port, server_host);

  if (connector.connect (server, addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);

  server << "1 2.3 testing" << endl;	// Buffer up some things to send to the server.

  int i;
  float f;

#if defined (__GNUC__)
  String s1;
  String s2;
  server >> s1 >> i >> f >> s2;

  cerr << "Server said:\n\t";
  cerr << s1 << " ";
  cerr << i << " ";
  cerr << f << " ";
  cerr << s2 << endl;
#else
  server >> i >> f;

  cerr << "Server said:\n\t";
  cerr << i << " ";
  cerr << f << endl;
#endif /* __GNUC__ */

  if (server.close () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "close"), -1);

  return 0;
}
