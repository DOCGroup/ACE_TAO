/* -*- C++ -*- */
// $Id$

#include "ace/Get_Opt.h"
#include "ace/Truncate.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"

ACE_INLINE
Handle_Broadcast::~Handle_Broadcast (void)
{
}

ACE_INLINE
Handle_Broadcast::Handle_Broadcast (void)
{
}

ACE_INLINE int
Handle_Broadcast::open (const ACE_INET_Addr &r, int async)
{
  if (this->ACE_SOCK_Dgram::open (r) == -1)
    return -1;
  else if (async && ACE_SOCK_Dgram::enable (ACE_SIGIO) == -1)
    return -1;
  else
    return 0;
}

ACE_INLINE int
Handle_Broadcast::info (ACE_TCHAR **strp, size_t length) const
{
  ACE_TCHAR     buf[BUFSIZ];
  ACE_INET_Addr sa;

  if (this->get_local_addr (sa) == -1)
    {
      return -1;
    }

  ACE_OS::sprintf (buf, ACE_TEXT("%d/"), sa.get_port_number ());
  ACE_OS::strcat (buf, ACE_TEXT("udp # tests broadcasting\n"));

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
Handle_Broadcast::init (int argc, ACE_TCHAR *argv[])
{
  ACE_INET_Addr sba (Handle_Broadcast::DEFAULT_PORT);
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("p:"), 0);

  for (int c; (c = get_opt ()) != -1; )
     switch (c)
       {
       case 'p':
         sba.set (ACE_OS::atoi (get_opt.opt_arg ()));
         break;
       default:
         break;
       }

  if (this->open (sba) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("open")), -1);
  else if (ACE_Reactor::instance ()->register_handler
           (this, ACE_Event_Handler::ACCEPT_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("registering service with ACE_Reactor")), -1);
  return 0;
}

ACE_INLINE int
Handle_Broadcast::fini (void)
{
  return ACE_Reactor::instance ()->remove_handler
    (this, ACE_Event_Handler::ACCEPT_MASK);
}

ACE_INLINE ACE_HANDLE
Handle_Broadcast::get_handle (void) const
{
  return this->ACE_SOCK_Dgram::get_handle ();
}

ACE_INLINE int
Handle_Broadcast::handle_input (ACE_HANDLE)
{
  ACE_INET_Addr sa;
  char buf[8 * 1024]; /* 8 k buffer */
  ssize_t n = this->recv (buf, sizeof buf, sa);

  if (n== -1)
    return -1;
  else
    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("received broadcast datagram from host %C\n"),
                sa.get_host_name ()));

  ACE_OS::puts (ACE_TEXT ("----------------------------------------"));
  ACE_OS::write (ACE_STDOUT, buf, n);

  if (buf[n - 1] != '\n')
    putchar ('\n');

  ACE_OS::puts (ACE_TEXT ("----------------------------------------"));

  return 0;
}

ACE_INLINE int
Handle_Broadcast::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  return this->ACE_SOCK_Dgram::close ();
}
