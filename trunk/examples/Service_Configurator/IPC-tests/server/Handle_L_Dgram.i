/* -*- C++ -*- */
// $Id$


#include "ace/Get_Opt.h"

ACE_INLINE
Handle_L_Dgram::Handle_L_Dgram (void)
{
}

ACE_INLINE int
Handle_L_Dgram::open (const ACE_UNIX_Addr &suad, int async)
{
  if (this->ACE_LSOCK_Dgram::open (suad) == -1)
    return -1;
  else if (async && this->ACE_LSOCK_Dgram::enable (SIGIO) == -1)
    return -1;
  else 
    return 0;
}

ACE_INLINE int 
Handle_L_Dgram::info (char **strp, size_t length) const
{
  char      buf[BUFSIZ];
  ACE_UNIX_Addr sa;

  if (this->ACE_LSOCK_Dgram::get_local_addr (sa) == -1)
    return -1;
  
  ACE_OS::sprintf (buf, "%s %s", sa.get_path_name (), "# tests local datagram\n");

  if (*strp == 0 && (*strp = ACE_OS::strdup (buf)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, buf, length);
  return ACE_OS::strlen (buf);
}

ACE_INLINE int
Handle_L_Dgram::init (int argc, char *argv[])
{
  ACE_UNIX_Addr  sudg;
  ACE_Get_Opt    get_opt (argc, argv, "r:", 0);
  const char *r = Handle_L_Dgram::DEFAULT_RENDEZVOUS;

  for (int c; (c = get_opt ()) != -1; )
     switch (c)
       {
       case 'r': 
	 r = get_opt.optarg;
	 break;
       default:
	 break;
       }
  
  ACE_OS::strncpy (this->rendezvous, r, MAXPATHLEN);
  ACE_OS::unlink (this->rendezvous);
  sudg.set (this->rendezvous);
  if (this->open (sudg) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);
  else if (ACE_Service_Config::reactor ()->register_handler (this,
						      ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "registering service with ACE_Reactor\n"), -1);
  return 0;
}

ACE_INLINE int 
Handle_L_Dgram::fini (void) 
{
  return ACE_Service_Config::reactor ()->remove_handler (this, ACE_Event_Handler::READ_MASK);
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
    ACE_DEBUG ((LM_INFO, "received datagram from %s\n", sa.get_path_name ()));

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
