/* -*- C++ -*- */
// $Id$

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"

ACE_INLINE
Handle_L_Dgram::Handle_L_Dgram (void)
{
}

ACE_INLINE int
Handle_L_Dgram::open (const ACE_UNIX_Addr &suad, int async)
{
  if (this->ACE_LSOCK_Dgram::open (suad) == -1)
    return -1;
  else if (async && this->ACE_LSOCK_Dgram::enable (ACE_SIGIO) == -1)
    return -1;
  else
    return 0;
}

ACE_INLINE int
Handle_L_Dgram::info (ACE_TCHAR **strp, size_t length) const
{
  ACE_TCHAR      buf[BUFSIZ];
  ACE_UNIX_Addr sa;

  if (this->ACE_LSOCK_Dgram::get_local_addr (sa) == -1)
    return -1;

  ACE_OS::strcpy (buf, ACE_TEXT_CHAR_TO_TCHAR (sa.get_path_name ()));
  ACE_OS::strcat (buf, ACE_TEXT (" # tests local datagram\n"));

  if (*strp == 0 && (*strp = ACE_OS::strdup (buf)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, buf, length);
  return ACE_OS::strlen (buf);
}

ACE_INLINE int
Handle_L_Dgram::init (int argc, ACE_TCHAR *argv[])
{
  ACE_UNIX_Addr  sudg;
  ACE_Get_Opt    get_opt (argc, argv, ACE_TEXT ("r:"), 0);
  const ACE_TCHAR *r = Handle_L_Dgram::DEFAULT_RENDEZVOUS;

  for (int c; (c = get_opt ()) != -1; )
    switch (c)
      {
      case 'r':
        r = get_opt.opt_arg ();
        break;
      default:
        break;
      }

  ACE_OS::strncpy (this->rendezvous, r, MAXPATHLEN);
  ACE_OS::unlink (this->rendezvous);
  sudg.set (this->rendezvous);
  if (this->open (sudg) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("open")), -1);
  else if (ACE_Reactor::instance ()->register_handler (this,
                                                       ACE_Event_Handler::ACCEPT_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("registering service with ACE_Reactor")),
                      -1);
  return 0;
}

ACE_INLINE int
Handle_L_Dgram::fini (void)
{
  return ACE_Reactor::instance ()->remove_handler (this, ACE_Event_Handler::ACCEPT_MASK);
}

ACE_INLINE int
Handle_L_Dgram::get_handle (void) const
{
  return this->ACE_LSOCK_Dgram::get_handle ();
}

ACE_INLINE int
Handle_L_Dgram::handle_input (int)
{
  ACE_UNIX_Addr sa;
  char	    buf[8 * 1024]; /* 8 k buffer */
  int	    n;

  if ((n = this->recv (buf, sizeof buf, sa)) == -1)
    return -1;
  else
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("received datagram from %s\n"),
                sa.get_path_name ()));

  ACE_OS::puts ("----------------------------------------");
  ACE_OS::write (ACE_STDOUT, buf, n);

  if (buf[n - 1] != '\n')
    putchar ('\n');

  ACE_OS::puts ("----------------------------------------");

  return 0;
}

ACE_INLINE int
Handle_L_Dgram::handle_close (int, ACE_Reactor_Mask)
{
  this->ACE_LSOCK_Dgram::close ();
  return ACE_OS::unlink (this->rendezvous);
}
