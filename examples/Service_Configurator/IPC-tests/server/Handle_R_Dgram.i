/* -*- C++ -*- */
// $Id$


#include "ace/Get_Opt.h"

ACE_INLINE
Handle_R_Dgram::Handle_R_Dgram (void)
{
}

ACE_INLINE int
Handle_R_Dgram::open (const ACE_INET_Addr &r, int async)
{
  if (this->ACE_SOCK_Dgram::open (r) == -1)
    return -1;
  else if (async && this->ACE_SOCK_Dgram::enable (SIGIO) == -1)
    return -1;
  else
    return 0;
}

ACE_INLINE int 
Handle_R_Dgram::info (char **strp, size_t length) const
{
  char      buf[BUFSIZ];
  ACE_INET_Addr sa;

  if (this->get_local_addr (sa) == -1)
    return -1;
  
  ACE_OS::sprintf (buf, "%d/%s %s", sa.get_port_number (), "udp", "# tests remote dgram\n");

  if (*strp == 0 && (*strp = ACE_OS::strdup (buf)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, buf, length);
  return ACE_OS::strlen (buf);
}

ACE_INLINE int
Handle_R_Dgram::init (int argc, char *argv[])
{
  ACE_INET_Addr sidg (Handle_R_Dgram::DEFAULT_PORT);
  ACE_Get_Opt	get_opt (argc, argv, "p:", 0);

  for (int c; (c = get_opt ()) != -1; )
     switch (c)
       {
       case 'p': 
	 sidg.set (ACE_OS::atoi (get_opt.optarg));
	 break;
       default:
	 break;
       }
  
  if (this->open (sidg) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);
  else if (ACE_Service_Config::reactor ()->register_handler 
	   (this, ACE_Event_Handler::ACCEPT_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "registering service with ACE_Reactor\n"), -1);
  return 0;
}

ACE_INLINE int 
Handle_R_Dgram::fini (void) 
{
  return ACE_Service_Config::reactor ()->remove_handler 
    (this, ACE_Event_Handler::ACCEPT_MASK);
}

ACE_INLINE int 
Handle_R_Dgram::get_handle (void) const
{ 
  return ACE_SOCK_Dgram::get_handle (); 
}

ACE_INLINE int 
Handle_R_Dgram::handle_input (int)
{
  ACE_INET_Addr sa;
  char		buf[0x2000]; /* 8 k buffer */
  int		n;

  if ((n = this->recv (buf, sizeof buf, sa)) == -1)
    return -1;
  else
    ACE_DEBUG ((LM_INFO, "received datagram from host %s on port %d\n",
	       sa.get_host_name (), sa.get_port_number ()));

  ACE_OS::puts ("----------------------------------------");
  ACE_OS::write (ACE_STDOUT, buf, n);

  if (buf[n - 1] != '\n')
    putchar ('\n');

  ACE_OS::puts ("----------------------------------------");

  return 0;
}

ACE_INLINE int
Handle_R_Dgram::handle_close (int, ACE_Reactor_Mask)
{
  return this->ACE_SOCK_Dgram::close ();
}
