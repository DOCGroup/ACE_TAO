// $Id$

#include "tests/test_config.h"

#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"

#include "ace/HTBP/HTBP_Session.h"
#include "ace/HTBP/HTBP_Stream.h"
#include "ace/HTBP/HTBP_Addr.h"

#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"

const size_t Send_Size = 4*1024;
const size_t Loops = 10;
const size_t Total_Size = Send_Size * Loops;
unsigned port = 8088;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "p:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
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
  // Indicates sucessful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  char buffer[5000];

  ACE_OS::socket_init (ACE_WSOCK_VERSION);

  if (parse_args(argc, argv) != 0)
    return 1;

  ACE_INET_Addr local(port);
  ACE_SOCK_Stream sock[2];
  ACE::HTBP::Channel *channels[2];
  ACE_SOCK_Acceptor acc(local,1);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P|%t) Server: ")
              ACE_TEXT("server is ready\n")));

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
                  ACE_TEXT("got = %d\n"), got));

      if (got < 0)
        break;
      total_recv += got;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P|%t) Server: ")
              ACE_TEXT("received %d \n"),total_recv));


  ACE_OS::strcpy (buffer,"I hear you !");
  ssize_t n = stream.send (buffer,ACE_OS::strlen(buffer)+1);
  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("(%P|%t) Server: ")
                       ACE_TEXT("%p\n"),
                       ACE_TEXT("stream.send")),-1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P|%t) Server: ")
              ACE_TEXT("send returned %d\n"),n));

  ACE_DEBUG ((LM_DEBUG,"deleting channels[1]\n"));
  delete channels[1];
  ACE_DEBUG ((LM_DEBUG,"deleting channels[0]\n"));
  delete channels[0];
  return 0;
}
