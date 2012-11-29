/* -*- C++ -*- */
// $Id$

#include "ace/Get_Opt.h"
#include "ace/WFMO_Reactor.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Truncate.h"

ACE_INLINE
Handle_R_Stream::Handle_R_Stream (void)
{
  if (Handle_R_Stream::login_name == 0)
    Handle_R_Stream::login_name = ACE_OS::cuserid (Handle_R_Stream::login);
}

ACE_INLINE int
Handle_R_Stream::open (const ACE_INET_Addr &sia, int async)
{
  if (this->ACE_SOCK_Acceptor::open (sia) == -1)
    return -1;
  else if (async && this->ACE_SOCK_Acceptor::enable (ACE_SIGIO) == -1)
    return -1;
  else
    return 0;
}

ACE_INLINE int
Handle_R_Stream::info (ACE_TCHAR **strp, size_t length) const
{
  ACE_TCHAR buf[BUFSIZ];
  ACE_INET_Addr sa;

  if (this->get_local_addr (sa) == -1)
    {
      return -1;
    }

  ACE_OS::sprintf (buf,
                   ACE_TEXT("%d/%s %s"),
                   sa.get_port_number (),
                   ACE_TEXT("tcp"),
                   ACE_TEXT("# tests remote stream\n"));

  if (*strp == 0 && (*strp = ACE_OS::strdup (buf)) == 0)
    {
      return -1;
    }
  else
    {
      ACE_OS::strncpy (*strp, buf, length);
    }

  return ACE_Utils::truncate_cast<int> (ACE_OS::strlen (buf));
}

ACE_INLINE int
Handle_R_Stream::init (int argc, ACE_TCHAR *argv[])
{
  ACE_INET_Addr sis (Handle_R_Stream::DEFAULT_PORT);
  ACE_Get_Opt   get_opt (argc, argv, ACE_TEXT("p:"), 0);

  for (int c; (c = get_opt ()) != -1; )
     switch (c)
       {
       case 'p':
         sis.set (ACE_OS::atoi (get_opt.opt_arg ()));
         break;
       default:
         break;
       }

  if (this->open (sis) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);

  else if (ACE_Reactor::instance ()->register_handler
           (this, ACE_Event_Handler::ACCEPT_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "registering service with ACE_Reactor\n"),
                      -1);
  return 0;
}

ACE_INLINE int
Handle_R_Stream::fini (void)
{
  return ACE_Reactor::instance ()->remove_handler
    (this, ACE_Event_Handler::ACCEPT_MASK);
}

ACE_INLINE ACE_HANDLE
Handle_R_Stream::get_handle (void) const
{
  return ACE_SOCK_Acceptor::get_handle ();
}

ACE_INLINE int
Handle_R_Stream::handle_input (ACE_HANDLE)
{
  char buf[BUFSIZ];
  int  bytes;

  // Try to find out if the implementation of the reactor that we are
  // using requires us to reset the event association for the newly
  // created handle. This is because the newly created handle will
  // inherit the properties of the listen handle, including its event
  // associations.
  int reset_new_handle =
    ACE_Reactor::instance ()->uses_event_associations ();

  if (this->accept (this->new_remote_stream, // stream
                    0, // remote address
                    0, // timeout
                    1, // restart
                    reset_new_handle  // reset new handler
                    ) == -1)
    return -1;
  else
    ACE_DEBUG ((LM_INFO, "new_remote_stream fd = %d\n",
               this->new_remote_stream.get_handle ()));

  ACE_INET_Addr sa;

  if (this->new_remote_stream.get_remote_addr (sa) == -1)
    return -1;

  ACE_DEBUG ((LM_INFO,
              "accepted from host %s at port %d\n",
              sa.get_host_name (),
              sa.get_port_number ()));

  ACE_OS::puts (ACE_TEXT ("----------------------------------------"));

  while ((bytes = this->new_remote_stream.recv (buf, sizeof buf)) > 0)
    ACE_OS::write (ACE_STDOUT, buf, bytes);

  ACE_OS::puts (ACE_TEXT ("----------------------------------------"));

  time_t t = ACE_OS::time (0L);
  ACE_TCHAR *cs = ACE_OS::ctime (&t);

  if (this->new_remote_stream.send (4,
                                    Handle_R_Stream::login_name,
                                    ACE_OS::strlen (Handle_R_Stream::login_name),
                                    cs,
                                    ACE_OS::strlen (cs)) == -1)
    return -1;

  if (this->new_remote_stream.close () == -1)
    return -1;

  return 0;
}

ACE_INLINE int
Handle_R_Stream::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  return this->ACE_SOCK_Acceptor::close ();
}
