// $Id$

/* Defines the member functions for the base class of the ACE_TLI_Stream
   abstraction. */

#include "ace/IPC/TLI_Stream.h"

ACE_RCSID(ace, TLI_Stream, "$Id$")

#if defined (ACE_HAS_TLI)

#if !defined (__ACE_INLINE__)
#include "ace/IPC/TLI_Stream.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_TLI_Stream)

void
ACE_TLI_Stream::dump (void) const
{
  ACE_TRACE ("ACE_TLI_Stream::dump");
}

ACE_TLI_Stream::ACE_TLI_Stream (void)
  : rwflag_ (0)
{
  ACE_TRACE ("ACE_TLI_Stream::ACE_TLI_Stream");
}

int
ACE_TLI_Stream::get_remote_addr (ACE_Addr &sa) const
{
  ACE_TRACE ("ACE_TLI_Stream::get_remote_addr");

#if defined (ACE_HAS_SVR4_TLI)
  struct netbuf name;
  name.maxlen = sa.get_size ();
  name.buf = (char *) sa.get_addr ();

  // if (ACE_OS::t_getname (this->get_handle (), &name, REMOTENAME) == -1)   
  if (ACE_OS::ioctl (this->get_handle (),
                     TI_GETPEERNAME,
                     &name) == -1)
    return -1;
  else
    return 0;
#else /* SunOS4 */
  ACE_UNUSED_ARG (sa);
  ACE_NOTSUP_RETURN (-1);
#endif /* ACE_HAS_SVR4_TLI */
}

// Send a release and then await the release from the other side.

int
ACE_TLI_Stream::active_close (void)
{
  ACE_TRACE ("ACE_TLI_Stream::active_close");
  char buf;

  if (this->sndrel () == -1) 
    return -1; 
  else if (this->recv (&buf, sizeof buf) == -1) 
    {
      if (t_errno == TLOOK && this->look () == T_ORDREL)
        {
          if (this->rcvrel () == -1) 
            return -1; 
        }
      else
        return -1; 
    }

  return this->close ();
}

// Acknowledge the release from the other side and then send the
// release to the other side.

int
ACE_TLI_Stream::passive_close (void)
{
  ACE_TRACE ("ACE_TLI_Stream::passive_close");

  if (this->rcvrel () == -1) 
    return -1; 
  else if (this->sndrel () == -1) 
    return -1; 

  return this->close ();
}

int
ACE_TLI_Stream::close (void)
{
  ACE_TRACE ("ACE_TLI_Stream::close");

  ACE_HANDLE fd = this->get_handle (); 

  this->set_handle (ACE_INVALID_HANDLE);

#if !defined (ACE_WIN32)
  if (this->rwflag_)
    return ACE_OS::close (fd);
  else
#endif /* ACE_WIN32 */
    return ACE_OS::t_close (fd);
}

#endif /* ACE_HAS_TLI */
