/**
 * $Id$
 *
 * client for a basic connection establishment test using HTBP
 */

#include "ace/ACE.h"
#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_sys_socket.h"
#include "ace/os_include/os_netdb.h"

#include "ace/HTBP/HTBP_Session.h"
#include "ace/HTBP/HTBP_Stream.h"
#include "ace/HTBP/HTBP_Addr.h"
#include "ace/HTBP/HTBP_ID_Requestor.h"
#include "ace/HTBP/HTBP_Environment.h"

const ACE_TCHAR * remote_host = 0;
const ACE_TCHAR * config_file = 0;
unsigned int remote_port = 8088;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("p:h:c:d"));
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
      case 'd':
        ACE::debug (true);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s ")
                           ACE_TEXT ("-h remote_host ")
                           ACE_TEXT ("-p remote_port ")
                           ACE_TEXT ("-c config_file ")
                           ACE_TEXT ("-d (turn on extra logging)")
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

  if (parse_args (argc, argv) != 0)
    return 1;
  if (remote_host == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Client: No remote host specified\n")),
                      1);

  ACE::HTBP::Environment env;
  if (config_file != 0)
    env.import_config (config_file);

  ACE::HTBP::ID_Requestor req (&env);
  ACE::HTBP::Addr local (ACE_TEXT_ALWAYS_CHAR (req.get_HTID ()));

  unsigned int proxy_port = 0;
  ACE_TString proxy_host;

  if (env.get_proxy_port (proxy_port) != 0 ||
      env.get_proxy_host (proxy_host) != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Client: ")
                  ACE_TEXT("no proxy address in ")
                  ACE_TEXT("config, using direct connect\n")));
      proxy_port = remote_port;
      proxy_host = remote_host;
    }

  ACE_INET_Addr proxy (proxy_port, proxy_host.c_str ());
  ACE::HTBP::Addr remote (remote_port, ACE_TEXT_ALWAYS_CHAR (remote_host));
  ACE_TCHAR proxy_s[MAXHOSTNAMELEN+1];
  proxy.addr_to_string (proxy_s, MAXHOSTNAMELEN);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Client: connecting to %s\n"),
              proxy_s));
  ACE::HTBP::Session session (remote,
                              local,
                              ACE::HTBP::Session::next_session_id (),
                              &proxy);
  ACE::HTBP::Stream stream (&session);

  char buffer[1000];
  ssize_t n = 0;
  int retrycount = 10;
  for (int i = 0; i < 3; i++)
    {
      ACE_OS::sprintf (buffer, "Do you hear me? %d", i);
      ACE::HTBP::Channel *ob = session.outbound ();
      n = stream.send (buffer, ACE_OS::strlen (buffer)+1);
      if (n == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) Client: ")
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT("stream send")),
                          -1);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Client: ")
                  ACE_TEXT ("send returned %b\n"),
                  n));

      retrycount = 10;
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Client: ")
                  ACE_TEXT ("after send, outbound = %@, ob = %@\n"),
                  session.outbound (), ob));
      while ((n = ob->recv_ack ()) == -1
             && (errno == EWOULDBLOCK || errno == ETIME)
             && retrycount > 0)
        {
          retrycount--;
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) Client: ")
                      ACE_TEXT ("waiting for ack, %d tries left\n"),
                      retrycount));
          ACE_OS::sleep (1);
        }
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Client: ")
                  ACE_TEXT ("After wait for ack, n = %b, retry = %d\n"),
                  n, retrycount));

      retrycount = 10;
      while ((n = stream.recv (buffer, 1000)) == -1
             && (errno == EWOULDBLOCK || errno == ETIME)
             && retrycount > 0)
        {
          retrycount--;
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) Client: ")
                      ACE_TEXT ("waiting for inbound data, %d tries left\n"),
                      retrycount));
          ACE_OS::sleep (1);
        }
      if (retrycount == 0 || n < 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) Client: ")
                      ACE_TEXT ("bailing after wait, %p\n"),
                      ACE_TEXT ("recv")));
          break;
        }

      buffer[n] = 0;
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Client: Got: \"%C\"\n"),
                  buffer));
    }

  ACE::HTBP::Channel *ob = session.outbound ();
  n = stream.send ("goodbye", 7);
  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Client: %p\n"),
                       ACE_TEXT ("stream send")),
                      -1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Client: ")
              ACE_TEXT ("send returned %b\n"),
              n));

  retrycount = 10;
  while ((n = ob->recv_ack ()) == -1
         && (errno == EWOULDBLOCK || errno == ETIME)
         && retrycount > 0)
    {
      retrycount--;
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) Client: ")
                  ACE_TEXT ("waiting for ack, %d tries left\n"),
                  retrycount));
      ACE_OS::sleep (1);
    }
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Client: ")
              ACE_TEXT ("After wait for ack, n = %b, retry = %d\n"),
              n, retrycount));
  stream.close ();
  return 0;
}
