/* -*- C++ -*- */
// $Id$


#include "ace/SPIPE_Stream.h"
#include "ace/Get_Opt.h"

#if defined (ACE_HAS_STREAM_PIPES)

ACE_INLINE
Handle_L_SPIPE::Handle_L_SPIPE (void)
{
}

ACE_INLINE int
Handle_L_SPIPE::open (const ACE_SPIPE_Addr &rendezvous_spipe) 
{
  if (this->ACE_SPIPE_Acceptor::open (rendezvous_spipe) == -1)
    return -1;
  else
    return 0;
}

ACE_INLINE int 
Handle_L_SPIPE::info (char **strp, size_t length) const
{
  char       buf[BUFSIZ];
  ACE_SPIPE_Addr sa;

  if (ACE_SPIPE_Acceptor::get_local_addr (sa) == -1)
    return -1;

  ACE_OS::sprintf (buf, "%s %s", sa.get_path_name (), "# tests local STREAM pipe\n");

  if (*strp == 0 && (*strp = ACE_OS::strdup (buf)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, buf, length);
  return ACE_OS::strlen (buf);
}

ACE_INLINE int
Handle_L_SPIPE::init (int argc, char *argv[])
{
  ACE_SPIPE_Addr susp;
  const char	 *rendezvous = Handle_L_SPIPE::DEFAULT_RENDEZVOUS;
  ACE_Get_Opt	 get_opt (argc, argv, "r:", 0);

  for (int c; (c = get_opt ()) != -1; )
     switch (c)
       {
       case 'r': 
	 rendezvous = get_opt.optarg;
	 break;
       default:
	 break;
       }
  
  ACE_OS::unlink (rendezvous);
  susp.set (rendezvous);
  if (this->open (susp) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);
  else if (ACE_Reactor::instance()->register_handler 
	   (this, ACE_Event_Handler::ACCEPT_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "registering service with ACE_Reactor\n"), -1);
  return 0;
}

ACE_INLINE int 
Handle_L_SPIPE::fini (void) 
{
  return ACE_Reactor::instance()->remove_handler 
    (this, ACE_Event_Handler::ACCEPT_MASK);
}

ACE_INLINE int 
Handle_L_SPIPE::get_handle (void) const
{
  return ACE_SPIPE::get_handle();
}

ACE_INLINE int 
Handle_L_SPIPE::handle_input (int)
{
  ACE_SPIPE_Stream new_spipe;
  char	   buf[PIPE_BUF];
  ACE_Str_Buf  msg (buf, 0, sizeof buf);
  int	   flags = 0;

  /* Accept communication requests */
  if (this->ACE_SPIPE_Acceptor::accept (new_spipe) == -1)
    return -1;
  else
    {
      ACE_SPIPE_Addr sa;

      new_spipe.get_remote_addr (sa);

      ACE_DEBUG ((LM_INFO, "accepted request from %s (gid = %d, uid = %d)\n",
		 sa.get_path_name (), sa.group_id (), sa.user_id ()));
    }

  while (new_spipe.recv ((ACE_Str_Buf *) 0, &msg, &flags) >= 0)
    if (msg.len != 0)
      ACE_OS::write (ACE_STDOUT, (const char *) msg.buf, (int) msg.len);
    else
      break;

  if (new_spipe.close () == -1)
    return -1;
  return 0;
}

ACE_INLINE int
Handle_L_SPIPE::handle_close (int, ACE_Reactor_Mask)
{
  return this->ACE_SPIPE_Acceptor::remove ();
}
#endif /* ACE_HAS_STREAM_PIPES */
