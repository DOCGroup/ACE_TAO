/* -*- C++ -*- */
// $Id$

#include "ace/Get_Opt.h"

ACE_INLINE
Handle_L_Pipe::~Handle_L_Pipe (void)
{
}

/* Uppercase N bytes of S. */

static char *
upper_case (char s[], int n)
{
  while (--n >= 0)
    if (islower (s[n]))
      s[n] = toupper (s[n]);

  return s;
} 

ACE_INLINE
Handle_L_Pipe::Handle_L_Pipe (void)
{
}

ACE_INLINE int
Handle_L_Pipe::open (const ACE_UNIX_Addr &suap, int async)
{
  if (this->ACE_LSOCK_Acceptor::open (suap) == -1)
    return -1;
  else if (async && this->ACE_LSOCK_Acceptor::enable (SIGIO) == -1)
    return -1;
  else
    return 0;
}

ACE_INLINE int 
Handle_L_Pipe::info (char **strp, size_t length) const
{
  char buf[BUFSIZ];
  ACE_UNIX_Addr sa;

  if (ACE_LSOCK_Acceptor::get_local_addr (sa) == -1)
    return -1;
  
  ACE_OS::sprintf (buf, "%s %s", sa.get_path_name (), "# tests local pipe\n");

  if (*strp == 0 && (*strp = ACE_OS::strdup (buf)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, buf, length);
  return ACE_OS::strlen (buf);
}

ACE_INLINE int
Handle_L_Pipe::init (int argc, char *argv[])
{
  ACE_UNIX_Addr sup;
  const char	*r = Handle_L_Pipe::DEFAULT_RENDEZVOUS;
  ACE_Get_Opt	get_opt (argc, argv, "r:", 0);

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
  sup.set (this->rendezvous);
  if (this->open (sup) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);
  else if (ACE_Service_Config::reactor ()->register_handler 
	   (this, ACE_Event_Handler::ACCEPT_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
		       "registering service with ACE_Reactor\n"), -1);
  return 0;
}

ACE_INLINE int 
Handle_L_Pipe::fini (void) 
{
  return ACE_Service_Config::reactor ()->remove_handler 
    (this, ACE_Event_Handler::ACCEPT_MASK);
}

ACE_INLINE int
Handle_L_Pipe::get_handle (void) const
{ 
  return ACE_LSOCK_Acceptor::get_handle (); 
}

ACE_INLINE int 
Handle_L_Pipe::handle_input (ACE_HANDLE)
{
  ACE_LSOCK_Stream   new_local_stream;
  int n;
  ACE_HANDLE fd1 = ACE_INVALID_HANDLE;
  ACE_HANDLE fd2 = ACE_INVALID_HANDLE;
  char buf[BUFSIZ];

  if (this->accept (new_local_stream) == -1)
    return -1;

  if (new_local_stream.recv_handle (fd1) == -1 
      || new_local_stream.recv_handle (fd2) == -1)
    return -1;
  else
    ACE_DEBUG ((LM_INFO, "received file descriptors %d and %d\n", fd1, fd2));

  if ((n = ACE_OS::read (fd1, buf, sizeof buf)) == -1)
    return -1;
  else if (ACE_OS::write (fd2, upper_case (buf, n), n) == -1)
    return -1;
  if (ACE_OS::close (fd1) == -1 
      || ACE_OS::close (fd2) == -1)
    return -1;
  if (new_local_stream.close () == -1)
    return -1;

  return 0;
}

ACE_INLINE int
Handle_L_Pipe::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  this->ACE_LSOCK_Acceptor::close ();
  return ACE_OS::unlink (this->rendezvous);
}
