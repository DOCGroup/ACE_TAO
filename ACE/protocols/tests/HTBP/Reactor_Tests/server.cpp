/**
 * server for a reactor based connection establishment test using HTBP
 *
 * $Id$
 */

#include "ace/Log_Msg.h"

#include "ace/HTBP/HTBP_Session.h"
#include "ace/HTBP/HTBP_Stream.h"
#include "ace/HTBP/HTBP_Addr.h"

#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/Event_Handler.h"
#include "ace/Reactor.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_socket.h"
#include "ace/os_include/os_netdb.h"

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
  // Indicates sucessful parsing of the command line
  return 0;
}

class Accept_Handler : public ACE_Event_Handler
{
public:
  Accept_Handler (ACE_SOCK_Acceptor& a);
  virtual ~Accept_Handler (void);
  virtual int handle_input (ACE_HANDLE );
private:
  ACE_SOCK_Acceptor& acceptor_;
  ACE::HTBP::Channel *channels_[2];
};

class Stream_Handler : public ACE_Event_Handler
{
public:
  Stream_Handler (ACE::HTBP::Stream &s);
  virtual ~Stream_Handler ();
  virtual int handle_input (ACE_HANDLE );
private:
  ACE::HTBP::Stream &stream_;
};


Accept_Handler::Accept_Handler(ACE_SOCK_Acceptor &a)
  :ACE_Event_Handler(),
   acceptor_(a)
{
  this->channels_[0] = this->channels_[1] = 0;
  if (this->reactor() == 0)
    this->reactor(ACE_Reactor::instance());
  this->reactor()->register_handler (acceptor_.get_handle(),
                                     this,
                                     ACE_Event_Handler::ACCEPT_MASK);
}

Accept_Handler::~Accept_Handler()
{
  this->reactor()->remove_handler (acceptor_.get_handle(),
                                   ACE_Event_Handler::ACCEPT_MASK|
                                   ACE_Event_Handler::DONT_CALL);
  acceptor_.close();
}

int
Accept_Handler::handle_input (ACE_HANDLE h)
{
  ACE::HTBP::Channel **ch = 0;
  if (h == acceptor_.get_handle())
    {
      ACE_SOCK_Stream *sock = new ACE_SOCK_Stream;
      acceptor_.accept(*sock);
      ch = channels_[0] == 0 ? &channels_[0] :& channels_[1];
      *ch = new ACE::HTBP::Channel(*sock);
      this->reactor()->register_handler (sock->get_handle(),
                                         this,
                                         ACE_Event_Handler::READ_MASK);
      return 0;
    }
  for (int i = 0; i < 2; i++)
    if (channels_[i] && channels_[i]->get_handle() == h)
      {
        ch = &channels_[i];
        break;
      }
  if (ch == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Server Accept_Handler::handle_input, ")
                       ACE_TEXT ("unknown handle %d\n") ,h),
                      -1);
  int result = (*ch)->pre_recv();
  if (result == 0)
    {
      this->reactor()->remove_handler (h,
                                      ACE_Event_Handler::READ_MASK |
                                      ACE_Event_Handler::DONT_CALL);

      (*ch)->register_notifier(this->reactor());
      ACE::HTBP::Session *session = (*ch)->session();

      ACE::HTBP::Stream *stream = new ACE::HTBP::Stream(session);
      ACE_Event_Handler *handler = session->handler();

      if (handler == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) Server Accept_Handler::handle_input ")
                      ACE_TEXT ("Creating new stream handler for %d\n"),
                      stream->get_handle()));
          Stream_Handler *sh = new Stream_Handler(*stream);
          session->handler (sh);
        }
      else
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) Server Accept_Handler::handle_input ")
                    ACE_TEXT ("There is already a handler for %d\n"),
                    stream->get_handle()));

      if ((*ch)->state() == ACE::HTBP::Channel::Data_Queued)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) Server Accept_Handler::handle_input\n"),
                      ACE_TEXT ("Issuing notification on handler\n")));
          this->reactor()->notify (session->handler(),
                                   ACE_Event_Handler::READ_MASK);
        }

      *ch = 0;
    }
  return 0;
}

Stream_Handler::Stream_Handler (ACE::HTBP::Stream &s)
  :stream_(s)
{}
Stream_Handler::~Stream_Handler ()
{
}

int
Stream_Handler::handle_input (ACE_HANDLE h)
{
  char buffer[1000];
  ssize_t n = this->stream_.recv (buffer,1000);
  if (n == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Server Stream_Handler::handle_input %p\n"),
                       ACE_TEXT ("recv")),
                      0);
  buffer[n] = 0;
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Server Stream_Handler::handle_input ")
              ACE_TEXT (" (%d) read %b:\n%C\n"),
              h, n, buffer));

  const char *tok_loc = ACE_OS::strstr (buffer, "goodbye");
  if (tok_loc != 0)
    this->reactor()->end_event_loop();
  else
    {
      ACE::HTBP::Channel *ch = stream_.session()->outbound();
      if (ch != 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) Server Stream_Handler::handle_input ")
                    ACE_TEXT ("Sending reply on %d\n"),
                    ch->ace_stream().get_handle()));
      else
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) Server Stream_Handler::handle_input ")
                    ACE_TEXT ("Can't send reply on nul channel\n")));
      this->stream_.send ("Back atcha!",11);
    }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR * argv[])
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Server: ")
              ACE_TEXT ("At start of main\n")));
  ACE_OS::socket_init (ACE_WSOCK_VERSION);

  if (parse_args (argc, argv) != 0)
    return 1;

  ACE_TCHAR host[MAXHOSTNAMELEN+1];
  if (ACE_OS::hostname (host, MAXHOSTNAMELEN) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%P|%t) Server failure: %p\n"),
                       ACE_TEXT ("hostname")),
                      1);

  ACE_INET_Addr local (port, host);
  local.addr_to_string (host, MAXHOSTNAMELEN);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Server: ")
              ACE_TEXT ("listening at %s\n"),
              host));

  ACE_SOCK_Acceptor acc (local, 1);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Server: ")
              ACE_TEXT ("opened listener\n")));

  Accept_Handler handler (acc);
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Server: ")
              ACE_TEXT ("server is ready\n")));

  if (notifier_file != 0)
    {
      FILE *f = ACE_OS::fopen (notifier_file,ACE_TEXT("w+"));
      const char *msg = "server ready";
      ACE_OS::fwrite (msg,ACE_OS::strlen(msg),1,f);
      ACE_OS::fclose (f);
    }

  ACE_Reactor::instance()->run_reactor_event_loop();
  return 0;
}
