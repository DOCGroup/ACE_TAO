// $Id$

#include "tests/test_config.h"

#include "ace/HTBP/HTBP_Session.h"
#include "ace/HTBP/HTBP_Stream.h"
#include "ace/HTBP/HTBP_Addr.h"
#include "ace/HTBP/HTBP_ID_Requestor.h"
#include "ace/HTBP/HTBP_Environment.h"

#include "ace/Log_Msg.h"

const ssize_t Send_Size = 4*1024;
const size_t Loops = 10;
const size_t Total_Size = Send_Size * Loops;

int
main (int, char *[])
{
  ACE_START_TEST (ACE_TEXT ("HTBP_Send_Large_Msg_client"));

  ACE_OS::socket_init (ACE_WSOCK_VERSION);

  ACE::HTBP::Environment ht_env;
#if 0 // this should be a taken from a command line argument
  ht_env.import_config ("../HTBP_Config.conf");
#endif /* 0 */

  ACE::HTBP::ID_Requestor req (&ht_env);
  ACE::HTBP::Addr local(req.get_HTID());

  char hostname [128];
  if (ACE_OS::hostname (hostname,128) == -1)
    {
      ACE_DEBUG ((LM_DEBUG, "Could not get the host name\n"));
      return -1;
    }

  ACE::HTBP::Addr remote (8088, hostname);

  unsigned pport;
  ACE_CString phost;
  ht_env.get_proxy_port(pport);
  ht_env.get_proxy_host(phost);

  ACE_INET_Addr proxy(pport,phost.c_str());
  ACE::HTBP::Session session(remote,
                           local,
                           ACE::HTBP::Session::next_session_id(),
                           &proxy);
  ACE::HTBP::Stream *stream = new ACE::HTBP::Stream(&session);
  ACE_DEBUG ((LM_DEBUG,"Sending message\n"));
  char buffer[Send_Size];
  ACE_OS::memset (buffer,'a',Send_Size);
  ssize_t n = 0;
  for (size_t i = 0; i < Loops ; ++i)
  {
    errno = 0;
    while (n < Send_Size)
      {
        n += stream->send (buffer+n,Send_Size);
        ACE_DEBUG ((LM_DEBUG,"Sending %d of %d\n", n, Send_Size));
      }
    if (n == -1 && errno != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n %d","stream send", errno), -1);
      }
    ACE_DEBUG ((LM_DEBUG, "In round %d, send returned %d\n", i, n));
  }

  buffer[0] = 0;
  n = stream->recv (buffer,1000);
  while (n == -1)
    {
      if (errno == EWOULDBLOCK)
        n = stream->recv (buffer,1000);
      else
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n","stream.recv"),-1);
    }

  ACE_DEBUG ((LM_DEBUG, "received %d, %s\n",n,buffer));

  ACE_END_TEST;
  return 0;
}
