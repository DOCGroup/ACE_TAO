/* -*- C++ -*- */
// $Id$


#include "ace/Get_Opt.h"

ACE_INLINE
Handle_R_Stream::Handle_R_Stream (void)
{
  if (Handle_R_Stream::login_name == 0)
    Handle_R_Stream::login_name = ACE_OS::cuserid (0);
}

ACE_INLINE int
Handle_R_Stream::open (const ACE_INET_Addr &sia, int async)
{
  if (this->ACE_SOCK_Acceptor::open (sia) == -1)
    return -1;
  else if (async && this->ACE_SOCK_Acceptor::enable (SIGIO) == -1)
    return -1;
  else
    return 0;
}

ACE_INLINE int 
Handle_R_Stream::info (char **strp, size_t length) const
{
  char      buf[BUFSIZ];
  ACE_INET_Addr sa;

  if (this->get_local_addr (sa) == -1)
    return -1;
  
  ACE_OS::sprintf (buf, "%d/%s %s", sa.get_port_number (), "tcp", "# tests remote stream\n");

  if (*strp == 0 && (*strp = ACE_OS::strdup (buf)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, buf, length);
  return ACE_OS::strlen (buf);
}

ACE_INLINE int
Handle_R_Stream::init (int argc, char *argv[])
{
  ACE_INET_Addr sis (Handle_R_Stream::DEFAULT_PORT);
  ACE_Get_Opt	get_opt (argc, argv, "p:", 0);

  for (int c; (c = get_opt ()) != -1; )
     switch (c)
       {
       case 'p': 
	 sis.set (ACE_OS::atoi (get_opt.optarg));
	 break;
       default:
	 break;
       }
  
  if (this->open (sis) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);
  else if (ACE_Service_Config::reactor ()->register_handler (this,
						      ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "registering service with ACE_Reactor\n"), -1);
  return 0;
}

ACE_INLINE int 
Handle_R_Stream::fini (void) 
{
  return ACE_Service_Config::reactor ()->remove_handler (this, ACE_Event_Handler::READ_MASK);
}

ACE_INLINE int 
Handle_R_Stream::get_handle (void) const
{ 
  return ACE_SOCK_Acceptor::get_handle (); 
}

ACE_INLINE int 
Handle_R_Stream::handle_input (int)
{
  char buf[BUFSIZ];
  int  bytes;

  if (this->accept (this->new_remote_stream) == -1)
    return -1;
  else
    ACE_DEBUG ((LM_INFO, "new_remote_stream fd = %d\n",
	       this->new_remote_stream.get_handle ()));

  ACE_INET_Addr sa;

  if (this->new_remote_stream.get_remote_addr (sa) == -1)
    return -1;

  ACE_DEBUG ((LM_INFO, "accepted from host %s at port %d\n", 
	     sa.get_host_name (), sa.get_port_number ()));

  ACE_OS::puts ("----------------------------------------");

  while ((bytes = this->new_remote_stream.recv (buf, sizeof buf)) > 0)
    ACE_OS::write (ACE_STDOUT, buf, bytes);

  ACE_OS::puts ("----------------------------------------");

  time_t t = ACE_OS::time (0L);
  char *cs = ACE_OS::ctime (&t);

  if (this->new_remote_stream.send (4,
				    Handle_R_Stream::login_name, ACE_OS::strlen (Handle_R_Stream::login_name),
				    cs, ACE_OS::strlen (cs)) == -1)
    return -1;

  if (this->new_remote_stream.close () == -1)
    return -1;

  return 0;
}

ACE_INLINE int
Handle_R_Stream::handle_close (int, ACE_Reactor_Mask)
{
  return this->ACE_SOCK_Acceptor::close ();
}
