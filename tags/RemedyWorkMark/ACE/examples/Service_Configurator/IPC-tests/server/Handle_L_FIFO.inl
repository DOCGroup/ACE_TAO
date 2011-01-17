/* -*- C++ -*- */
// $Id$

#include "ace/Get_Opt.h"
#include "ace/Truncate.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_stropts.h"
#include "ace/OS_NS_unistd.h"

ACE_INLINE
Handle_L_FIFO::Handle_L_FIFO (void)
{
}

ACE_INLINE int
Handle_L_FIFO::open (const ACE_TCHAR *rendezvous_fifo)
{
  if (this->ACE_FIFO_Recv_Msg::open (rendezvous_fifo) == -1)
    return -1;
  else
    return 0;
}

ACE_INLINE int
Handle_L_FIFO::info (ACE_TCHAR **strp, size_t length) const
{
  ACE_TCHAR buf[BUFSIZ];
  const ACE_TCHAR *rendezvous_fifo;

  this->get_local_addr (rendezvous_fifo);

  ACE_OS::strcpy (buf, rendezvous_fifo);
  ACE_OS::strcat (buf, ACE_TEXT(" # tests local ACE_FIFO\n"));

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
Handle_L_FIFO::init (int argc, ACE_TCHAR *argv[])
{
  const ACE_TCHAR *rendezvous_fifo = Handle_L_FIFO::DEFAULT_RENDEZVOUS;
  ACE_Get_Opt    get_opt (argc, argv, ACE_TEXT("r:"), 0);

  for (int c; (c = get_opt ()) != -1; )
    switch (c)
      {
      case 'r':
        rendezvous_fifo = get_opt.opt_arg ();
        break;
      default:
        break;
      }

  ACE_OS::unlink (rendezvous_fifo);
  if (this->open (rendezvous_fifo) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("open")), -1);
  else if (ACE_Reactor::instance ()->register_handler (this, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("registering service with ACE_Reactor")),
                      -1);
  return 0;
}

ACE_INLINE int
Handle_L_FIFO::fini (void)
{
  return ACE_Reactor::instance ()->remove_handler
    (this, ACE_Event_Handler::ACCEPT_MASK);
}

ACE_INLINE ACE_HANDLE
Handle_L_FIFO::get_handle (void) const
{
  return this->ACE_FIFO::get_handle ();
}

ACE_INLINE int
Handle_L_FIFO::handle_input (ACE_HANDLE)
{
  char buf[PIPE_BUF];
  ACE_Str_Buf msg (buf, 0, sizeof buf);

  /* Accept communication requests */
  if (this->recv (msg) == -1)
    return -1;
  else

  ACE_OS::write (ACE_STDOUT, (const char *) msg.buf, (int) msg.len);
  return 0;
}

ACE_INLINE int
Handle_L_FIFO::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  return this->ACE_FIFO::remove ();
}
