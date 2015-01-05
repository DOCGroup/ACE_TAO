// $Id$

#include "ace/HTBP/HTBP_Session.h"
#include "ace/HTBP/HTBP_Stream.h"
#include "ace/HTBP/HTBP_Addr.h"
#include "ace/HTBP/HTBP_ID_Requestor.h"
#include "ace/HTBP/HTBP_Environment.h"

#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_sys_socket.h"

const ssize_t Send_Size = 4*1024;
const size_t Loops = 10;
const size_t Total_Size = Send_Size * Loops;
const ACE_TCHAR * remote_host = 0;
const ACE_TCHAR * config_file = 0;
unsigned remote_port = 8088;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("p:h:c:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'p':
        remote_port = static_cast<unsigned>(ACE_OS::atoi (get_opts.opt_arg()));
        break;
      case 'h':
        remote_host = get_opts.opt_arg ();
        break;
      case 'c':
        config_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s ")
                           ACE_TEXT ("-h remote_host ")
                           ACE_TEXT ("-p remote_port ")
                           ACE_TEXT ("-c config_file ")
                           ACE_TEXT ("\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_OS::socket_init (ACE_WSOCK_VERSION);

  if (parse_args(argc, argv) != 0)
    return 1;
  if (remote_host == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Client: No remote host specified\n")),1);

  ACE::HTBP::Environment env;
  if (config_file != 0)
    env.import_config (config_file);

  ACE::HTBP::ID_Requestor req (&env);
  ACE::HTBP::Addr local(ACE_TEXT_ALWAYS_CHAR(req.get_HTID()));

  unsigned proxy_port = 0;
  ACE_TString proxy_host;

  if (env.get_proxy_port(proxy_port) != 0 ||
      env.get_proxy_host(proxy_host) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("(%P|%t) Client: ")
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

  ACE::HTBP::Stream *stream = new ACE::HTBP::Stream(&session);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P|%t) Client: ")
              ACE_TEXT("Sending message\n")));
  char buffer[Send_Size];
  ACE_OS::memset (buffer,'a',Send_Size);
  ssize_t n = 0;
  for (size_t i = 0; i < Loops ; ++i)
  {
    errno = 0;
    while (n < Send_Size)
      {
        n += stream->send (buffer+n,Send_Size);
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("(%P|%t) Client: ")
                    ACE_TEXT("Sending %d of %d\n"),
                    n, Send_Size));
      }
    if (n == -1 && errno != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("(%P|%t) Client: ")
                           ACE_TEXT("%p\n %d"),
                           ACE_TEXT("stream send"), ACE_ERRNO_GET),
                          -1);
      }
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("(%P|%t) Client: ")
                ACE_TEXT("In round %d, send returned %d\n"),
                i, n));
  }

  buffer[0] = 0;
  n = stream->recv (buffer,1000);
  while (n == -1)
    {
      if (errno == EWOULDBLOCK)
        n = stream->recv (buffer,1000);
      else
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT("(%P|%t) Client: ")
                           ACE_TEXT("%p\n"),
                           ACE_TEXT("stream.recv")),
                          -1);
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%P|%t) Client: ")
              ACE_TEXT("received %d, %s\n"),
              n,buffer));
  return 0;
}
