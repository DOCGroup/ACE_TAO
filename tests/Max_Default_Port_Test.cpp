// $Id$
// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Max_Default_Port_Test.cpp
//
// = DESCRIPTION
//    This is a test for ACE_MAX_DEFAULT_PORT value. The test tests the
//    highest value of the port number at which an event handler can be
//    registered and a Connector can be connected to.
//
//    Some weird behaviour has been reported on Windows NT (sp 3) when
//    the port number exceeds 65279 resulting ACE_MAX_DEFAULT_PORT to set
//    to zero on that platform.
//
//    In this test, the event handler is started at the port value
//    USHRT_MAX and decremented for 300 port values and tested if the
//    highest port number used agrees with ACE_MAX_DEFAULT_PORT value.
//
//
// = AUTHOR
//    Chanaka Liyanaarachchi <chanaka@ociweb.com>
//
// ============================================================================

#include "ace/OS.h"
#include "ace/streams.h"
#include "ace/Reactor.h"
#include "ace/SOCK_Connector.h"
#include "ace/Thread_Manager.h"

#include "Max_Default_Port_Test.h"
#include "test_config.h"

My_Accept_Handler::My_Accept_Handler (ACE_INET_Addr &addr)
  : addr_ (addr)
{
  if (addr.get_port_number() != 0)
    this->open (addr);
}


My_Accept_Handler::~My_Accept_Handler ()
{
  this->peer_acceptor_.close ();     // Prevent handle leaks
}


int
My_Accept_Handler::open (ACE_INET_Addr &addr)
{

  if (this->peer_acceptor_.open (addr, 1) == -1)
  {
     ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"),
                 ACE_TEXT ("My_Accept_Handler open")));
     ACE_OS::exit (1);
  }

  return 0;

}

ACE_HANDLE
My_Accept_Handler::get_handle () const
{
  return this->peer_acceptor_.get_handle ();
}

int
My_Accept_Handler::handle_input (ACE_HANDLE)
{

  if (this->peer_acceptor_.accept(this->stream_, NULL) == -1) {
    ACE_ERROR((LM_ERROR, ACE_TEXT ("%p\n"),
               ACE_TEXT ("peer_acceptor.accept")));
    ACE_OS::exit(1);
  }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("My_Accept_Handler::handle_input \n")));

  // Close the opened stream, else it'll leak a handle. Don't close
  // the acceptor here, though, because get_handle() needs it to
  // correctly allow removal from the reactor later. It gets closed
  // in the destructor.
  this->stream_.close ();

  return 0;
}

u_short
My_Accept_Handler::port ()
{
  return this->addr_.get_port_number();
}

long max_connected_port = 0;

static void *
client (void *arg)
{
  ACE_INET_Addr *remote_addr = ACE_reinterpret_cast (ACE_INET_Addr *,
                                                     arg);

  ACE_INET_Addr server_addr (remote_addr->get_port_number (),
                             ACE_LOCALHOST);

  ACE_SOCK_Stream cli_stream;
  ACE_SOCK_Connector con;
  ACE_Time_Value timeout (ACE_DEFAULT_TIMEOUT);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Connecting to port %d\n"),
              server_addr.get_port_number()));

  // Initiate connection with server; don't wait forever
  if (con.connect (cli_stream,
                   server_addr,
                   &timeout) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("connection failed")));

      return 0;
    }

  // if connect succesful, what is the max port number we connected
  // up to now.
  int connected_port = server_addr.get_port_number ();

  if (connected_port > max_connected_port)
    max_connected_port = connected_port;

  cli_stream.close ();

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{

  ACE_START_TEST (ACE_TEXT ("Max_Default_Port_Test"));

  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  u_short max_listened_port = 0;

  //Ports beyond 65279 were said to bad on NT sp 3.
  for (u_short idx = USHRT_MAX; idx != USHRT_MAX - 300; --idx)
    {
      ACE_INET_Addr addr (idx);

      My_Accept_Handler *eh = new My_Accept_Handler (addr);


      if ( ACE_Reactor::instance()->register_handler (
                                         eh,
                                         ACE_Event_Handler::ACCEPT_MASK) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("%p\n"),
                             ACE_TEXT ("Failed to register event handler")),
                            1);
        }

      ACE_DEBUG ((LM_DEBUG, "Registered event handler at %d\n", idx));

      ACE_Time_Value tv (1);

#if defined (ACE_HAS_THREADS)

      if (ACE_Thread_Manager::instance ()->spawn_n
          (1,
           ACE_THR_FUNC (client),
           ACE_reinterpret_cast (void *, &addr),
           THR_NEW_LWP | THR_DETACHED) == -1)

        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t) %p\n"),
                           ACE_TEXT ("thread create failed")),
                          1);

      ACE_Thread_Manager::instance ()->wait ();

#else
      ACE_UNUSED_ARG (client);
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) only one thread may be run in a process on this platform\n%a"),
                  1));
#endif //ACE_HAS_THREADS

      if (ACE_Reactor::instance()->handle_events (tv) == -1 )
        {
          ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                             ACE_TEXT ("Reactor::handle_events")),
                            1);
        }

      // see if I can register a reactor at this port.
      if (eh->port () == idx)
        {
          if (idx > max_listened_port)
            max_listened_port = idx;
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Test Fail, listening port %d\n"),
                             eh->port()),
                            1);
        }

      ACE_Reactor::instance()->remove_handler (
                                        eh,
                                        ACE_Event_Handler::ACCEPT_MASK |
                                        ACE_Event_Handler::DONT_CALL);
      delete eh;
    }

  ACE_DEBUG ((LM_DEBUG,
              "Value of ACE_MAX_DEFAULT_PORT       %d\n",
              ACE_MAX_DEFAULT_PORT));

  ACE_DEBUG ((LM_DEBUG,
              "Highest port value I can listen at  %d\n",
              max_listened_port));

  ACE_DEBUG ((LM_DEBUG,
              "Highest port value I can connect to %d\n",
              max_connected_port));

  if ((max_listened_port  == ACE_MAX_DEFAULT_PORT) &&
      (max_connected_port == ACE_MAX_DEFAULT_PORT))
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Valid ACE_MAX_DEFAULT_PORT value: %d\n"),
                  max_listened_port));
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Invalid ACE_MAX_DEFAULT_PORT ")
                  ACE_TEXT ("or %d port may be busy; got to %d\n"),
                  ACE_MAX_DEFAULT_PORT, max_listened_port));

    }

  ACE_END_TEST;
  return 0;
}
