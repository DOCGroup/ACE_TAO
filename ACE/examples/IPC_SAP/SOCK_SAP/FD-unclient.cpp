// $Id$

#include "ace/OS_NS_fcntl.h"
#include "ace/LSOCK_Connector.h"
#include "ace/UNIX_Addr.h"
#include "ace/Log_Msg.h"
#include "ace/OS_main.h"



#if defined (ACE_HAS_MSG) && !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)
// ACE_LSOCK Client.

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  const ACE_TCHAR *file_name = argc > 1 ? argv[1] : ACE_TEXT ("./local_data");
  const ACE_TCHAR *rendezvous = argc > 2 ? argv[2] : ACE_DEFAULT_RENDEZVOUS;

  ACE_LSOCK_Stream cli_stream;
  ACE_UNIX_Addr addr (rendezvous);

  // Establish the connection with server.
  ACE_LSOCK_Connector connector;

  if (connector.connect (cli_stream, addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p"), ACE_TEXT ("connect")), -1);

  ACE_HANDLE handle = ACE_OS::open (file_name, O_RDONLY);

  if (handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p"), ACE_TEXT ("open")), -1);

  // Send handle to server (correctly handles incomplete writes).
  if (cli_stream.send_handle (handle) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p"), ACE_TEXT ("send")), -1);

  char buf[BUFSIZ];
  ssize_t n = cli_stream.recv (buf, sizeof buf);

  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p"), ACE_TEXT ("recv")), -1);
  else if (n == 0)
    //FUZZ: disable check_for_lack_ACE_OS
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("server shutdown (bug in kernel?)\n")));
    //FUZZ: enable check_for_lack_ACE_OS
  else
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("server %*C shutdown\n"), n, buf));

  // Explicitly close the connection.
  if (cli_stream.close () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p"), ACE_TEXT ("close")), -1);

  return 0;
}
#else
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("your platform must support sendmsg/recvmsg to run this test\n")), -1);
}
#endif /* ACE_HAS_MSG */
