// $Id$

#include "ace/TLI_Connector.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_unistd.h"



#if defined (ACE_HAS_TLI)

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (argc < 2)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Usage: %s filename ")
                       ACE_TEXT ("[server-host port-number]\n"),
                       argv[0]),
                      1);

  const ACE_TCHAR *filename  = argv[1];
  const ACE_TCHAR *host_name = argc < 3 ? ACE_DEFAULT_SERVER_HOST : argv[2];
  u_short port = argc < 4 ? ACE_DEFAULT_SERVER_PORT : ACE_OS::atoi (argv[3]);

  ACE_TLI_Stream client;
  ACE_TLI_Connector con;
  int fd;
  char buf[BUFSIZ];

  if (con.connect (client, ACE_INET_Addr (port, host_name)) == -1)
    ACE_OS::t_error ((char *) host_name), ACE_OS::exit (1);

  if ((fd = ACE_OS::open (filename, O_RDONLY)) == -1)
    ACE_OS::perror (filename), ACE_OS::exit (1);

  for (int n; (n = ACE_OS::read (fd, buf, sizeof buf)) > 0; )
    if (client.send_n (buf, n) != n)
      ACE_OS::t_error ("client.send error");

  if (client.close () == -1)
    ACE_OS::t_error ("cli_close"), ACE_OS::exit (1);

  return 0;
}
#else
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("your platform isn't configured to support TLI\n")),
                    1);
}
#endif /* ACE_HAS_TLI */
