// $Id$

#include "tests/test_config.h"

#include "ace/Log_Msg.h"

#include "ace/HTBP/HTBP_Session.h"
#include "ace/HTBP/HTBP_Stream.h"
#include "ace/HTBP/HTBP_Addr.h"

#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"

const size_t Send_Size = 4*1024;
const size_t Loops = 10;
const size_t Total_Size = Send_Size * Loops;

int
main (int argc, char *argv[])
{

  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  ACE_START_TEST (ACE_TEXT ("HTBP_Send_Large_Msg_server"));

  char buffer[1000];


  ACE_OS::socket_init (ACE_WSOCK_VERSION);

  ACE_INET_Addr local(8088);
  ACE_SOCK_Stream sock[2];
  ACE_SOCK_Acceptor acc(local,1);
  ACE_DEBUG ((LM_DEBUG,"server is ready\n"));

  acc.accept(sock[0]);
  ACE::HTBP::Channel channel1(sock[0]);
  ACE_DEBUG ((LM_DEBUG,"Got sock[0], handle = %d\n",sock[0].get_handle()));
  acc.accept(sock[1]);
  ACE::HTBP::Channel channel2(sock[1]);
  ACE_DEBUG ((LM_DEBUG,"Got sock[1], handle = %d\n",sock[1].get_handle()));
  int res = 0;
  while ((res =channel1.pre_recv()) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,"res = %d. waiting 1 sec. %p\n",res,
                  "stream.pre_recv()"));
      ACE_OS::sleep (1);
    }

  ACE_DEBUG ((LM_DEBUG,"Read from channel2\n"));
  while ((res = channel2.pre_recv()) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,"res = %d, waiting 1 sec. %p\n",res,
                  "stream2.pre_recv()"));
      ACE_OS::sleep (1);
    }

  ACE::HTBP::Session *session = channel1.session();
  ACE::HTBP::Stream stream (session);

  ACE_DEBUG ((LM_DEBUG,"using streams %d, %d. Got sesssion = %x\n",
              sock[0].get_handle(),sock[1].get_handle(),session));

  ssize_t got = 1;
  ssize_t total_recv = 0;

  while (got != 0)
    {
      errno = 0;
      got = stream.recv (buffer, sizeof (buffer));
      ACE_DEBUG ((LM_DEBUG,
                  "got : %s %d ", buffer, got));

      if (got < 0)
        break;
      total_recv += got;
    }

  ACE_DEBUG ((LM_DEBUG, "received %d, %s\n",total_recv,buffer));


  ACE_OS::strcpy (buffer,"I hear you !");
  ssize_t n = stream.send (buffer,ACE_OS::strlen(buffer)+1);
  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n","stream.send"),-1);

  ACE_DEBUG ((LM_DEBUG, "send returned %d\n",n));

  ACE_OS::sleep (1);

  ACE_END_TEST;
  return 0;

}
