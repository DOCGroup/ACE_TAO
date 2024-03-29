/* -*- C++ -*- */
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"

ACE_INLINE
Handle_R_Dgram::Handle_R_Dgram ()
{
}

ACE_INLINE int
Handle_R_Dgram::open (const ACE_INET_Addr &r, int async)
{
  if (this->ACE_SOCK_Dgram::open (r) == -1)
    return -1;
  else if (async && this->ACE_SOCK_Dgram::enable (ACE_SIGIO) == -1)
    return -1;
  else
    return 0;
}

ACE_INLINE int
Handle_R_Dgram::info (ACE_TCHAR **strp, size_t length) const
{
  ACE_TCHAR buf[BUFSIZ];
  ACE_INET_Addr sa;

  if (this->get_local_addr (sa) == -1)
    return -1;

  ACE_OS::sprintf (buf, ACE_TEXT ("%d/"), sa.get_port_number ());
  ACE_OS::strcat (buf, ACE_TEXT ("udp # tests remote dgram\n"));

  if (*strp == 0 && (*strp = ACE_OS::strdup (buf)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, buf, length);
  return ACE_OS::strlen (buf);
}

ACE_INLINE int
Handle_R_Dgram::init (int argc, ACE_TCHAR *argv[])
{
  ACE_INET_Addr sidg (Handle_R_Dgram::DEFAULT_PORT);
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("p:"), 0);

  for (int c; (c = get_opt ()) != -1; )
    switch (c)
      {
      case 'p':
        sidg.set (ACE_OS::atoi (get_opt.opt_arg ()));
        break;
      default:
        break;
      }

  if (this->open (sidg) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("open")), -1);
  else if (ACE_Reactor::instance ()->register_handler (this, ACE_Event_Handler::ACCEPT_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("registering service with ACE_Reactor")), -1);
  return 0;
}

ACE_INLINE int
Handle_R_Dgram::fini ()
{
  return ACE_Reactor::instance ()->remove_handler
    (this, ACE_Event_Handler::ACCEPT_MASK);
}

ACE_INLINE ACE_HANDLE
Handle_R_Dgram::get_handle () const
{
  return ACE_SOCK_Dgram::get_handle ();
}

ACE_INLINE int
Handle_R_Dgram::handle_input (ACE_HANDLE)
{
  ACE_INET_Addr sa;
  char buf[8 * 1024]; /* 8 k buffer */
  ssize_t n = this->recv (buf, sizeof buf, sa);

  if (n == -1)
    return -1;
  else
    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("received datagram from host %C on port %d\n"),
                sa.get_host_name (), sa.get_port_number ()));

  ACE_OS::puts ("----------------------------------------");
  ACE_OS::write (ACE_STDOUT, buf, n);

  if (buf[n - 1] != '\n')
    putchar ('\n');

  ACE_OS::puts ("----------------------------------------");

  return 0;
}

ACE_INLINE int
Handle_R_Dgram::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  return this->ACE_SOCK_Dgram::close ();
}
