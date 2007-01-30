/**
 * client for a reactor based connection establishment test using HTBP
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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

  ACE_OS::socket_init (ACE_WSOCK_VERSION);

  if (argc < 2)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("Usage: client <remote host>\n")),
                      0);

  ACE::HTBP::Environment env (0,0,ACE_TEXT("inside.env"));
#if 0 // this should be a taken from a command line argument.
  env.import_config ("inside.conf");
#endif /* 0 */

  ACE::HTBP::ID_Requestor req (&env);
  ACE::HTBP::Addr local = ACE_TEXT_ALWAYS_CHAR(req.get_HTID());

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

  ACE::HTBP::Session session(remote,local,ACE::HTBP::Session::next_session_id(),&proxy);
  ACE::HTBP::Stream stream (&session);

  char buffer[1000];
  ssize_t n = 0;
  int retrycount = 10;
  for (int i = 0; i < 3; i++)
    {
      ACE::HTBP::Channel *ch = session.outbound();
      ACE_OS::sprintf (buffer,"Do you hear me? %d",i);
      n = stream.send (buffer,ACE_OS::strlen(buffer)+1);
      if (n == -1)
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT("%p\n"),
                           ACE_TEXT("stream send")),-1);

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT("send returned %d\n"),n));

      retrycount = 10;
      while ((n = ch->recv_ack()) == -1
             && (errno == EWOULDBLOCK || errno == ETIME)
             && retrycount > 0)
        {
          retrycount--;
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("waiting for ack, %d tries left\n"),
                      retrycount));
          ACE_OS::sleep (1);
        }
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("After wait for ack, n = %d, retry = %d\n"),
                  n,retrycount,errno));

      retrycount = 10;
      while ((n = stream.recv(buffer,1000)) == -1
             && (errno == EWOULDBLOCK || errno == ETIME)
             && retrycount > 0)
        {
          retrycount--;
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("waiting for inbound data, %d tries left\n"),
                      retrycount));
          ACE_OS::sleep(1);
        }
      if (retrycount == 0 || n < 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("bailing after wait, %p\n"),
                      ACE_TEXT("recv")));
          break;
        }

      buffer[n] = 0;

      ACE_DEBUG ((LM_DEBUG,"Got: \"%s\"\n",buffer));
    }
  ACE::HTBP::Channel *ch = session.outbound();
  if (ch == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT("session's outbound channel is null!\n")),1);
  n = stream.send ("goodbye",7);
  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT("%p\n"),
                       ACE_TEXT("stream send")),-1);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT("send returned %d\n"),n));

  retrycount = 10;
  while (ch &&
         (n = ch->recv_ack()) == -1
         && (errno == EWOULDBLOCK || errno == ETIME)
         && retrycount > 0)
    {
      retrycount--;
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("waiting for ack, %d tries left\n"),
                  retrycount));
      ACE_OS::sleep (1);
    }
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("After wait for ack, n = %d, retry = %d\n"),
              n,retrycount,errno));

  return 0;
}
