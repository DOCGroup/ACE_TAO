// TLI_Stream.cpp
// $Id$

/* Defines the member functions for the base class of the ACE_TLI_Stream
   abstraction. */ 

#define ACE_BUILD_DLL
#include "ace/TLI_Stream.h"
#include "ace/Log_Msg.h"

#if defined (ACE_HAS_TLI)

ACE_ALLOC_HOOK_DEFINE(ACE_TLI_Stream)

void
ACE_TLI_Stream::dump (void) const
{
  ACE_TRACE ("ACE_TLI_Stream::dump");
}

ACE_TLI_Stream::ACE_TLI_Stream (void)
{
  ACE_TRACE ("ACE_TLI_Stream::ACE_TLI_Stream");
}

int
ACE_TLI_Stream::get_remote_addr (ACE_Addr &sa) const
{
  ACE_TRACE ("ACE_TLI_Stream::get_remote_addr");
  struct netbuf name;

  name.maxlen = sa.get_size ();
  name.buf    = (char *) sa.get_addr ();
  
#if defined (ACE_HAS_SVR4_TLI)
  if (ACE_OS::ioctl (this->get_handle (), TI_GETPEERNAME, &name) == -1) 
/*  if (ACE_OS::t_getname (this->get_handle (), &name, REMOTENAME) == -1) */

#else /* SunOS4 sucks... */
  if (0)
#endif /* ACE_HAS_SVR4_TLI */  
    return -1;
  else
    return 0;
}

/* Send a release and then await the release from the other side */

int 
ACE_TLI_Stream::active_close (void)
{
  ACE_TRACE ("ACE_TLI_Stream::active_close");
  char buf;

  if (this->sndrel () == ACE_INVALID_HANDLE)
    return ACE_INVALID_HANDLE;
  if (this->recv (&buf, sizeof buf) == ACE_INVALID_HANDLE)
    {
      if (t_errno == TLOOK && this->look () == T_ORDREL)
	{
	  if (this->rcvrel () == ACE_INVALID_HANDLE)
	    return ACE_INVALID_HANDLE;
	}
      else
	return ACE_INVALID_HANDLE;
    }
  return this->close ();
}

/* Acknowledge the release from the other side and then send the release to
   the other side. */

int 
ACE_TLI_Stream::passive_close (void)
{
  ACE_TRACE ("ACE_TLI_Stream::passive_close");
  if (this->rcvrel () == ACE_INVALID_HANDLE)
    return ACE_INVALID_HANDLE;
  if (this->sndrel () == ACE_INVALID_HANDLE)
    return ACE_INVALID_HANDLE;
  return this->close ();
}


int
ACE_TLI_Stream::close (void)
{
  ACE_TRACE ("ACE_TLI_Stream::close");
  int fd = this->get_handle ();

  this->set_handle (ACE_INVALID_HANDLE);

  if (this->rwflag_)
    return ACE_OS::close (fd);
  else
    return ACE_OS::t_close (fd);
}

#endif /* ACE_HAS_TLI */
