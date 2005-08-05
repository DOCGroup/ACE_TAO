/**
 * client for a basic connection establishment test using HTBP
 *
 * $Id$
 */


#include "ace/Log_Msg.h"

#include "ace/HTBP/HTBP_Session.h"
#include "ace/HTBP/HTBP_Stream.h"
#include "ace/HTBP/HTBP_Addr.h"
#include "ace/HTBP/HTBP_ID_Requestor.h"
#include "ace/HTBP/HTBP_Environment.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  ACE_OS::socket_init (ACE_WSOCK_VERSION);

  if (argc < 2)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Usage: client <remote host>\n"),
                      0);
  ACE::HTBP::Environment env;
#if 0 // this should be a taken from a command line argument
  env.import_config (ACE_TEXT("../HTBP_Config.conf"));
#endif /* 0 */


  ACE::HTBP::ID_Requestor req (&env);
  ACE::HTBP::Addr local(ACE_TEXT_ALWAYS_CHAR(req.get_HTID()));

  unsigned remote_port = 8088;
  const ACE_TCHAR * remote_host = argv[1];

  unsigned proxy_port = 0;
  ACE_TString proxy_host;

  if (env.get_proxy_port(proxy_port) != 0 ||
      env.get_proxy_host(proxy_host) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("no proxy address in ")
                  ACE_TEXT("config, using direct connect\n")));
      proxy_port = remote_port;
      proxy_host = remote_host;
    }

  ACE_INET_Addr proxy(proxy_port,proxy_host.c_str());
  ACE::HTBP::Addr remote (remote_port,ACE_TEXT_ALWAYS_CHAR(remote_host));

  ACE::HTBP::Session session(remote,
                           local,
                           ACE::HTBP::Session::next_session_id(),
                           &proxy);
  ACE::HTBP::Stream stream (&session);

  char buffer[1000];
  ssize_t n = 0;
  int retrycount = 10;
  for (int i = 0; i < 3; i++)
    {
      ACE_OS::sprintf (buffer,"Do you hear me? %d",i);
      ACE::HTBP::Channel *ob = session.outbound();
      n = stream.send (buffer,ACE_OS::strlen(buffer)+1);
      if (n == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n","stream send"),-1);

      ACE_DEBUG ((LM_DEBUG, "send returned %d\n",n));

      retrycount = 10;
      ACE_DEBUG ((LM_DEBUG,"after send, outbound = %x, ob = %x\n",
                  session.outbound(), ob));
      while ((n = ob->recv_ack()) == -1
             && (errno == EWOULDBLOCK || errno == ETIME)
             && retrycount > 0)
        {
          retrycount--;
          ACE_DEBUG ((LM_DEBUG,"waiting for ack, %d tries left\n",
                      retrycount));
          ACE_OS::sleep (1);
        }
      ACE_DEBUG ((LM_DEBUG,"After wait for ack, n = %d, retry = %d\n",
                  n,retrycount,errno));

      retrycount = 10;
      while ((n = stream.recv(buffer,1000)) == -1
             && (errno == EWOULDBLOCK || errno == ETIME)
             && retrycount > 0)
        {
          retrycount--;
          ACE_DEBUG ((LM_DEBUG,"waiting for inbound data, %d tries left\n",
                      retrycount));
          ACE_OS::sleep(1);
        }
      if (retrycount == 0 || n < 0)
        {
          ACE_DEBUG ((LM_DEBUG,"bailing after wait, %p\n","recv"));
          break;
        }

      buffer[n] = 0;

      ACE_DEBUG ((LM_DEBUG,"Got: \"%s\"\n",buffer));
    }

  ACE::HTBP::Channel *ob = session.outbound();
  n = stream.send ("goodbye",7);
  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n","stream send"),-1);

  ACE_DEBUG ((LM_DEBUG, "send returned %d\n",n));

  retrycount = 10;
  while ((n = ob->recv_ack()) == -1
         && (errno == EWOULDBLOCK || errno == ETIME)
         && retrycount > 0)
    {
      retrycount--;
      ACE_DEBUG ((LM_DEBUG,"waiting for ack, %d tries left\n",
                  retrycount));
      ACE_OS::sleep (1);
    }
  ACE_DEBUG ((LM_DEBUG,"After wait for ack, n = %d, retry = %d\n",
              n,retrycount,errno));

  return 0;
}
