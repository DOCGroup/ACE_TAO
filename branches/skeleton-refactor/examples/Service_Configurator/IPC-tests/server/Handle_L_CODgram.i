/* -*- C++ -*- */
// $Id$

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"

ACE_INLINE
Handle_L_CODgram::Handle_L_CODgram (void)
{
}

ACE_INLINE int
Handle_L_CODgram::open (const ACE_UNIX_Addr &suad, int async)
{
  if (this->ACE_LSOCK_CODgram::open (ACE_Addr::sap_any, suad) == -1)
    return -1;
  else if (async && this->ACE_LSOCK_CODgram::enable (ACE_SIGIO) == -1)
    return -1;
  else
    return 0;
}

ACE_INLINE int 
Handle_L_CODgram::info (ACE_TCHAR **strp, size_t length) const
{
  ACE_TCHAR      buf[BUFSIZ];
  ACE_UNIX_Addr sa;

  if (ACE_LSOCK_CODgram::get_local_addr (sa) == -1)
    return -1;
  
  ACE_OS::strcpy (buf, ACE_TEXT_CHAR_TO_TCHAR (sa.get_path_name ()));
  ACE_OS::strcat (buf, ACE_TEXT (" # tests local connected datagram\n"));

  if (*strp == 0 && (*strp = ACE_OS::strdup (buf)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, buf, length);
  return ACE_OS::strlen (buf);
}

ACE_INLINE int
Handle_L_CODgram::init (int argc, ACE_TCHAR *argv[])
{
  ACE_UNIX_Addr  sucd;
  ACE_Get_Opt    get_opt (argc, argv, ACE_TEXT ("r:"), 0);
  const ACE_TCHAR *r = Handle_L_CODgram::DEFAULT_RENDEZVOUS;

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
  sucd.set (this->rendezvous);
  if (this->open (sucd) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("open")), -1);
  else if (ACE_Reactor::instance ()->register_handler 
	   (this, ACE_Event_Handler::ACCEPT_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
		       ACE_TEXT ("registering service with ACE_Reactor")),
                      -1);
  return 0;
}

ACE_INLINE int 
Handle_L_CODgram::fini(void) 
{
  return ACE_Reactor::instance ()->remove_handler 
    (this, ACE_Event_Handler::ACCEPT_MASK);
}

ACE_INLINE ACE_HANDLE
Handle_L_CODgram::get_handle (void) const
{ 
  return ACE_LSOCK_CODgram::get_handle ();
}

ACE_INLINE int 
Handle_L_CODgram::handle_input (ACE_HANDLE)
{
  ACE_HANDLE handle = ACE_INVALID_HANDLE;
  char buf[BUFSIZ];

  if (this->recv_handle (handle) == -1)
    return -1;
  else
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("received handle (%d)\n"), handle));

  ACE_OS::puts ("----------------------------------------");

  for (;;)
    {
      ssize_t n = ACE_OS::read (handle, buf, sizeof buf);

      if (n <= 0)
	break;

      ACE_OS::write (ACE_STDOUT, buf, n);
    }

  ACE_OS::puts ("----------------------------------------");

  if (ACE_OS::close (handle) == -1)
    return -1;

  return 0;
}

ACE_INLINE int
Handle_L_CODgram::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  this->ACE_LSOCK_CODgram::close ();
  return ACE_OS::unlink (this->rendezvous);
}
