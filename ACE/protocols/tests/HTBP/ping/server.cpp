/**
 * server for a basic connection establishment test using HTBP
 *
 * $Id$
 */

#include "ace/Log_Msg.h"

#include "ace/HTBP/HTBP_Session.h"
#include "ace/HTBP/HTBP_Stream.h"
#include "ace/HTBP/HTBP_Addr.h"

#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_sys_socket.h"
#include "ace/os_include/os_netdb.h"

unsigned port = 8088;
const ACE_TCHAR *notifier_file = 0;
int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("p:o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        notifier_file = get_opts.opt_arg();
        break;
      case 'p':
        port = static_cast<unsigned>(ACE_OS::atoi (get_opts.opt_arg()));
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-p port "
                            "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc , ACE_TCHAR *argv[])
{
  char buffer[1000];
  ssize_t n = 0;
  ACE_OS::socket_init (ACE_WSOCK_VERSION);

  if (parse_args(argc, argv) != 0)
    return 1;

  ACE_TCHAR host[MAXHOSTNAMELEN+1];
  if (ACE_OS::hostname (host, MAXHOSTNAMELEN) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Server failure: %p\n"),
                       ACE_TEXT ("hostname")),
                      1);

  ACE_INET_Addr local (port, host);
  ACE_SOCK_Stream sock[2];
  ACE_SOCK_Acceptor acc(local, 1);
  local.addr_to_string (host, MAXHOSTNAMELEN);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Server is ready on %s\n"),
              host));
  if (notifier_file != 0)
    {
      FILE *f = ACE_OS::fopen (notifier_file,ACE_TEXT("w+"));
      const char *msg = "server ready";
      ACE_OS::fwrite (msg,ACE_OS::strlen(msg),1,f);
      ACE_OS::fclose (f);
    }
  acc.accept (sock[0]);
  ACE::HTBP::Channel channel1(sock[0]);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Server: ")
              ACE_TEXT ("Got sock[0], handle = %d\n"),
              sock[0].get_handle()));
  acc.accept (sock[1]);
  ACE::HTBP::Channel channel2 (sock[1]);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Server: ")
              ACE_TEXT ("Got sock[1], handle = %d\n"),
              sock[1].get_handle()));
  int res = 0;
  while ((res = channel1.pre_recv ()) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Server: ")
                  ACE_TEXT ("res = %d. waiting 1 sec. %p\n"),
                  res,
                  ACE_TEXT ("stream.pre_recv()")));
      ACE_OS::sleep (1);
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Read from channel2\n")));
  while ((res = channel2.pre_recv()) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Server: ")
                  ACE_TEXT ("res = %d, waiting 1 sec. %p\n"),
                  res,
                  ACE_TEXT ("stream2.pre_recv()")));
      ACE_OS::sleep (1);
    }

  ACE::HTBP::Session *session = channel1.session();
  ACE::HTBP::Stream stream (session);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Server: ")
              ACE_TEXT ("using streams %d, %d. Got session = %@\n"),
              sock[0].get_handle(),
              sock[1].get_handle(),
              session));

  for (int i = 0; i >= 0; i++)
    {
      int retrycount = 10;
      while ((n = stream.recv(buffer,1000)) == -1
             && (errno == EWOULDBLOCK || errno == ETIME)
             && retrycount > 0)
        {
          retrycount--;
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) Server: ")
                      ACE_TEXT ("waiting for inbound data, %d tries left\n"),
                      retrycount));
          ACE_OS::sleep(1);
        }
      if (retrycount == 0 || n < 0)
        break;

      buffer[n] = 0;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Server: ")
                  ACE_TEXT ("Got: \"%C\"\n"), buffer));

      if (ACE_OS::strstr (buffer,"goodbye") != 0)
        break;

      ACE_OS::sprintf (buffer,"I hear you %d",i);
      n = stream.send (buffer,ACE_OS::strlen(buffer)+1);
      if (n == -1)
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                           ACE_TEXT ("stream.send")),
                          -1);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Server: Send returned %d\n"), n));

      int got[2] = {-1,-1};
      while (got[0] == -1 || got[1] == -1)
        {
          if (got[0] == -1)
            {
              if ((got[0] = (res =channel1.pre_recv())) == -1)
                ACE_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("(%P|%t) Server: ")
                            ACE_TEXT ("res = %d, waiting 1 sec. %p\n"),
                            got[0],
                            ACE_TEXT ("channel1.pre_recv()")));
            }
          if (got[1] == -1)
            {
              if ((got[1] = (res =channel2.pre_recv())) == -1)
                ACE_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("(%P|%t) Server: ")
                            ACE_TEXT ("res = %d, waiting 1 sec. %p\n"),
                            got[1],
                            ACE_TEXT ("channel2.pre_recv()")));
            }
          if (got[0] == -1 || got[1] == -1)
            ACE_OS::sleep (1);
        }
    }
  ACE_OS::sleep(1); // prevent test failure on windows when the connection
                    // closes too fast.

  stream.close();
  acc.close();

  return 0;
}
