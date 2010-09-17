// $Id$

#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"

#include "ace/HTBP/HTBP_Session.h"
#include "ace/HTBP/HTBP_Stream.h"
#include "ace/HTBP/HTBP_Addr.h"

#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_sys_socket.h"
#include "ace/os_include/os_netdb.h"

const size_t Send_Size = 4*1024;
const size_t Loops = 10;
const size_t Total_Size = Send_Size * Loops;
unsigned port = 8088;
const ACE_TCHAR *notifier_file = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:p:"));
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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  char buffer[5000];

  ACE_OS::socket_init (ACE_WSOCK_VERSION);

  if (parse_args (argc, argv) != 0)
    return 1;

  ACE_TCHAR host[MAXHOSTNAMELEN+1];
  if (ACE_OS::hostname (host,MAXHOSTNAMELEN) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Server failure: %p\n"),
                       ACE_TEXT ("hostname")),
                      1);

  ACE_INET_Addr local (port, host);
  ACE_SOCK_Stream sock[2];
  ACE::HTBP::Channel *channels[2];
  ACE_SOCK_Acceptor acc(local,1);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P|%t) Server: ")
              ACE_TEXT("server is ready\n")));

  if (notifier_file != 0)
    {
      FILE *f = ACE_OS::fopen (notifier_file,ACE_TEXT("w+"));
      const char *msg = "server ready";
      ACE_OS::fwrite (msg,ACE_OS::strlen(msg),1,f);
      ACE_OS::fclose (f);
    }

  acc.accept(sock[0]);
  channels[0] = new ACE::HTBP::Channel (sock[0]);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P|%t) Server: ")
              ACE_TEXT("Got sock[0], handle = %d\n"),
              sock[0].get_handle()));
  acc.accept(sock[1]);
  channels[1] = new ACE::HTBP::Channel(sock[1]);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P|%t) Server: ")
              ACE_TEXT("Got sock[1], handle = %d\n"),
              sock[1].get_handle()));
  int res = 0;
  while ((res = channels[0]->pre_recv()) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("(%P|%t) Server: ")
                  ACE_TEXT("res = %d. waiting 1 sec. %p\n"),
                  res,
                  ACE_TEXT("stream.pre_recv()")));
      ACE_OS::sleep (1);
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P|%t) Server: ")
              ACE_TEXT("Read from channel2\n")));
  while ((res = channels[1]->pre_recv()) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("(%P|%t) Server: ")
                  ACE_TEXT("res = %d, waiting 1 sec. %p\n"),
                  res,
                  ACE_TEXT("stream2.pre_recv()")));
      ACE_OS::sleep (1);
    }

  ACE::HTBP::Session *session = channels[0]->session();
  ACE::HTBP::Stream stream (session);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P|%t) Server: ")
              ACE_TEXT("using streams %d, %d. Got sesssion = %x\n"),
              sock[0].get_handle(),sock[1].get_handle(),session));

  ssize_t got = 1;
  ssize_t total_recv = 0;

  while (got != 0)
    {
      errno = 0;
      got = stream.recv (buffer, sizeof (buffer));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("(%P|%t) Server: ")
                  ACE_TEXT("got = %b\n"), got));

      if (got < 0)
        break;
      total_recv += got;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P|%t) Server: ")
              ACE_TEXT("received %b\n"),total_recv));


  ACE_OS::strcpy (buffer,"I hear you !");
  ssize_t n = stream.send (buffer, ACE_OS::strlen (buffer)+1);
  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("(%P|%t) Server: ")
                       ACE_TEXT("%p\n"),
                       ACE_TEXT("stream.send")),-1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P|%t) Server: ")
              ACE_TEXT("send returned %b\n"),
              n));

  ACE_OS::sleep(1); // prevent test failure on windows when the connection
                    // closes too fast.
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P|%t) Server: ")
              ACE_TEXT("deleting channels[1]\n")));
  delete channels[1];
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P|%t) Server: ")
              ACE_TEXT("deleting channels[0]\n")));
  delete channels[0];
  return 0;
}
