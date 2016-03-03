// == == == == == == == == == == == == == == == == == == == == == == ==
// Stolen from $ACE_ROOT/tests/Thread_Pool_Reactor_Test.cpp
// Thread_Pool_Reactor_Test.cpp, v 1.29 2001/03/20 01:07:21 irfan Exp
// = AUTHOR
//      Irfan Pyarali <irfan@cs.wustl.edu> and
//      Nanbor Wang <nanbor@cs.wustl.edu>
// == == == == == == == == == == == == == == == == == == == == == == ==

#include "ace/config-lite.h"
#if defined (ACE_HAS_THREADS)

#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Acceptor.h"
#include "ace/Thread_Manager.h"
#include "ace/TP_Reactor.h"
#include "ace/Truncate.h"

#include "Request_Handler.h"

// Accepting end point.  This is actually "localhost:10010", but some
// platform couldn't resolve the name so we use the IP address
// directly here.
static const ACE_TCHAR *rendezvous = ACE_TEXT ("127.0.0.1:10010");

// Total number of server threads.
static size_t svr_thrno = 5;

// Total number of client threads.
static size_t cli_runs = 2;

// Total connection attempts of a client thread.
static size_t cli_conn_no = 2;

// Total requests a client thread sends.
static size_t cli_req_no = 5;

// Delay before a thread sending the next request (in msec.)
static int req_delay = 50;


typedef ACE_Strategy_Acceptor <Request_Handler, ACE_SOCK_ACCEPTOR> ACCEPTOR;


Request_Handler::Request_Handler (ACE_Thread_Manager *thr_mgr)
    : ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH> (thr_mgr),
    nr_msgs_rcvd_(0)
{
  this->reactor (ACE_Reactor::instance ());
}

int
Request_Handler::handle_input (ACE_HANDLE fd)
{
  ACE_TCHAR buffer[BUFSIZ];
  ACE_TCHAR len = 0;
  ssize_t result = this->peer ().recv (&len, sizeof (ACE_TCHAR));

  if (result > 0
      && this->peer ().recv_n (buffer, len * sizeof (ACE_TCHAR))
      == static_cast<ssize_t> (len * sizeof (ACE_TCHAR)))
    {
      ++this->nr_msgs_rcvd_;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) svr input; fd: 0x%x; input: %s\n"),
                  fd,
                  buffer));
      if (ACE_OS::strcmp (buffer, ACE_TEXT ("shutdown")) == 0)
        ACE_Reactor::instance()->end_reactor_event_loop ();
      return 0;
    }
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) Request_Handler: 0x%x peer closed (0x%x)\n"),
                this, fd));
  return -1;
}

int
Request_Handler::handle_close (ACE_HANDLE fd, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) svr close; fd: 0x%x, rcvd %d msgs\n"),
              fd,
              this->nr_msgs_rcvd_));

  if (this->nr_msgs_rcvd_ != cli_req_no)
    ACE_ERROR((LM_ERROR,
               ACE_TEXT ("(%t) Handler 0x%x: Expected %d messages; got %d\n"),
               this,
               cli_req_no,
               this->nr_msgs_rcvd_));

  this->destroy ();
  return 0;
}

// Listing 2 code/ch16
static int
reactor_event_hook (ACE_Reactor *)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) handling events ....\n")));

  return 0;
}

class ServerTP : public ACE_Task_Base
{
public:
  virtual int svc (void)
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) Running the event loop\n")));

    int result =
      ACE_Reactor::instance ()->run_reactor_event_loop
        (&reactor_event_hook);

    if (result == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%t) %p\n"),
                         ACE_TEXT ("Error handling events")),
                        0);

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) Done handling events.\n")));

    return 0;
  }
};
// Listing 2

class Client: public ACE_Task_Base
  {
  public:
    Client()
        :addr_(rendezvous)
    {}

    virtual int svc()
    {
      ACE_OS::sleep (3);
      const ACE_TCHAR *msg =
        ACE_TEXT ("Message from Connection worker");

      ACE_TCHAR buf [BUFSIZ];
      buf[0] =
        ACE_Utils::truncate_cast<ACE_TCHAR> (ACE_OS::strlen (msg) + 1);
      ACE_OS::strcpy (&buf[1], msg);

      for (size_t i = 0; i < cli_runs; i++)
        send_work_to_server(buf);

      shut_down();

      return 0;
    }

  private:
    void send_work_to_server(ACE_TCHAR* arg)
    {
      ACE_SOCK_Stream stream;
      ACE_SOCK_Connector connect;
      ACE_Time_Value delay (0, req_delay);
      size_t len = * reinterpret_cast<ACE_TCHAR *> (arg);

      for (size_t i = 0 ; i < cli_conn_no; i++)
        {
          if (connect.connect (stream, addr_) < 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("(%t) %p\n"),
                          ACE_TEXT ("connect")));
              continue;
            }

          for (size_t j = 0; j < cli_req_no; j++)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("Sending work to server on handle 0x%x, req %d\n"),
                          stream.get_handle (),
                          j+1));
              if (stream.send_n (arg,
                                 (len + 1) * sizeof (ACE_TCHAR)) == -1)
                {
                  ACE_ERROR ((LM_ERROR,
                              ACE_TEXT ("(%t) %p\n"),
                              ACE_TEXT ("send_n")));
                  continue;
                }
              ACE_OS::sleep (delay);
            }

          stream.close ();
        }

    }

    void shut_down()
    {
      ACE_SOCK_Stream stream;
      ACE_SOCK_Connector connect;

      if (connect.connect (stream, addr_) == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%t) %p Error while connecting\n"),
                    ACE_TEXT ("connect")));

      const ACE_TCHAR *sbuf = ACE_TEXT ("\011shutdown");

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("shutdown stream handle = %x\n"),
                  stream.get_handle ()));

      if (stream.send_n (sbuf, (ACE_OS::strlen (sbuf) + 1) * sizeof (ACE_TCHAR)) == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%t) %p\n"),
                    ACE_TEXT ("send_n")));

      stream.close ();
    }
  private:
    ACE_INET_Addr addr_;
  };
// Listing 1 code/ch16
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_TP_Reactor sr;
  ACE_Reactor new_reactor (&sr);
  ACE_Reactor::instance (&new_reactor);

  ACCEPTOR acceptor;
  ACE_INET_Addr accept_addr (rendezvous);

  if (acceptor.open (accept_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("open")),
                      1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Spawning %d server threads...\n"),
              svr_thrno));

  ServerTP serverTP;
  serverTP.activate (THR_NEW_LWP | THR_JOINABLE,
                     ACE_Utils::truncate_cast<int> (svr_thrno));

  Client client;
  client.activate ();

  ACE_Thread_Manager::instance ()->wait ();

  return 0;
}
// Listing 1
#else
#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_OS::puts (ACE_TEXT ("This example requires threads."));
  return 0;
}

#endif /* ACE_HAS_THREADS */
