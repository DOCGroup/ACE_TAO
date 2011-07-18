// $Id$

// ACE_LSOCK Client.

#include "ace/LSOCK_Connector.h"
#include "ace/UNIX_Addr.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"
#include "ace/OS_NS_unistd.h"



#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)
int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  const ACE_TCHAR *rendezvous = argc > 1 ? argv[1] : ACE_DEFAULT_RENDEZVOUS;
  char buf[BUFSIZ];

  ACE_LSOCK_Stream cli_stream;
  ACE_LSOCK_Connector con;
  ACE_UNIX_Addr remote_addr (rendezvous);

  // Establish the connection with server.
  if (con.connect (cli_stream, remote_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("connect")),
                      1);

  // Send data to server (correctly handles "incomplete writes").

  for (int r_bytes;
       (r_bytes = ACE_OS::read (ACE_STDIN, buf, sizeof buf)) > 0;
       )
    if (cli_stream.send_n (buf, r_bytes) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%p\n"),
                         ACE_TEXT ("send_n")),
                        1);

  // Explicitly close the writer-side of the connection.
  if (cli_stream.close_writer () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("close_writer")),
                      1);

  // Wait for handshake with server.
  if (cli_stream.recv_n (buf, 1) != 1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("recv_n")),
                      1);

  // Close the connection completely.
  if (cli_stream.close () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("close")),
                      1);
  return 0;
}
#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "this platform does not support UNIX-domain sockets\n"), -1);
}
#endif /* ACE_LACKS_UNIX_DOMAIN_SOCKETS */
