/* -*- C++ -*- */
// $Id$

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_unistd.h"

ACE_INLINE
Handle_L_Stream::~Handle_L_Stream (void)
{
}

ACE_INLINE
Handle_L_Stream::Handle_L_Stream (void)
{
  if (Handle_L_Stream::login_name == 0) {
#if !defined(ACE_LACKS_CUSERID)
    Handle_L_Stream::login_name = ACE_OS::cuserid (Handle_L_Stream::login);
#else
    Handle_L_Stream::login[0] = '.';
    Handle_L_Stream::login[1] = '\0';
    Handle_L_Stream::login_name = Handle_L_Stream::login;
#endif
  }
}

ACE_INLINE int
Handle_L_Stream::open (const ACE_UNIX_Addr &suas,
                       int async)
{
  if (this->ACE_LSOCK_Acceptor::open (suas) == -1)
    return -1;
  else if (async && this->ACE_LSOCK_Acceptor::enable (ACE_SIGIO) == -1)
    return -1;
  else
    return 0;
}

ACE_INLINE int
Handle_L_Stream::info (ACE_TCHAR **strp, size_t length) const
{
  ACE_TCHAR buf[BUFSIZ];
  ACE_UNIX_Addr sa;

  if (this->get_local_addr (sa) == -1)
    return -1;

  ACE_OS::strcpy (buf, ACE_TEXT_CHAR_TO_TCHAR (sa.get_path_name ()));
  ACE_OS::strcat (buf, ACE_TEXT (" # tests local ACE_Stream\n"));

  if (*strp == 0 && (*strp = ACE_OS::strdup (buf)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, buf, length);
  return ACE_OS::strlen (buf);
}

ACE_INLINE int
Handle_L_Stream::init (int argc, ACE_TCHAR *argv[])
{
  ACE_UNIX_Addr sus;
  const ACE_TCHAR *r = Handle_L_Stream::DEFAULT_RENDEZVOUS;
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("r:"), 0);

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
  sus.set (this->rendezvous);

  if (this->open (sus) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("open")), -1);
  else if (ACE_Reactor::instance ()->register_handler (this, ACE_Event_Handler::ACCEPT_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("registering service with ACE_Reactor")), -1);
  return 0;
}

ACE_INLINE int
Handle_L_Stream::fini (void)
{
  return ACE_Reactor::instance ()->remove_handler
    (this, ACE_Event_Handler::ACCEPT_MASK);
}

ACE_INLINE ACE_HANDLE
Handle_L_Stream::get_handle (void) const
{
  return ACE_LSOCK_Acceptor::get_handle ();
}

ACE_INLINE int
Handle_L_Stream::handle_input (ACE_HANDLE)
{
  ACE_LSOCK_Stream new_local_stream;
  ACE_UNIX_Addr sa;
  ACE_HANDLE handle = ACE_INVALID_HANDLE;
  char buf[BUFSIZ];

  if (this->accept (new_local_stream, &sa) == -1)
    return -1;
  else if (new_local_stream.recv_handle (handle) == -1)
    return -1;
  else
    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("received file descriptor %d on ACE_Stream %s\n"),
                handle, sa.get_path_name ()));

  ACE_OS::puts ("----------------------------------------");

  for (;;)
    {
      ssize_t n = ACE_OS::read (handle, buf, sizeof buf);

      if (n <= 0)
        break;

      ACE_OS::write (ACE_STDOUT, buf, n);
    }

  ACE_OS::puts ("----------------------------------------");

  time_t t = ACE_OS::time (0L);
  ACE_TCHAR *cs = ACE_OS::ctime (&t);

  if (new_local_stream.send (4,
                             Handle_L_Stream::login_name,
                             ACE_OS::strlen (Handle_L_Stream::login_name),
                             ACE_TEXT_ALWAYS_CHAR (cs),
                             ACE_OS::strlen (cs)) == -1)
    return -1;
  else if (ACE_OS::close (handle) == -1)
    return -1;
  else if (new_local_stream.close () == -1)
    return -1;
  else
    return 0;
}

ACE_INLINE int
Handle_L_Stream::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  this->ACE_LSOCK_Acceptor::close ();
  return ACE_OS::unlink (this->rendezvous);
}
