// MEM_Acceptor.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/MEM_Acceptor.h"

#if !defined (__ACE_INLINE__)
#include "ace/MEM_Acceptor.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, MEM_Acceptor, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_MEM_Acceptor)

void
ACE_MEM_Acceptor::dump (void) const
{
  ACE_TRACE ("ACE_MEM_Acceptor::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  this->local_addr_.dump ();
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

// Do nothing routine for constructor.

ACE_MEM_Acceptor::ACE_MEM_Acceptor (void)
{
  ACE_TRACE ("ACE_MEM_Acceptor::ACE_MEM_Acceptor");
}

// General purpose routine for performing server ACE_SOCK creation.

ACE_MEM_Acceptor::ACE_MEM_Acceptor (const ACE_Addr &remote_sap,
                                    int reuse_addr,
                                    int protocol_family,
                                    int backlog,
                                    int protocol)
{
  ACE_TRACE ("ACE_MEM_Acceptor::ACE_MEM_Acceptor");
  if (this->open (remote_sap,
                  reuse_addr,
                  protocol_family,
                  backlog,
                  protocol) == -1)
    ACE_ERROR ((LM_ERROR,
                "ACE_MEM_Acceptor::ACE_MEM_Acceptor"));
}

// General purpose routine for accepting new connections.

int
ACE_MEM_Acceptor::accept (ACE_MEM_Stream &new_stream,
                          ACE_Addr *remote_addr,
                          ACE_Time_Value *timeout,
                          int restart,
                          int reset_new_handle) const
{
  ACE_TRACE ("ACE_MEM_Acceptor::accept");

  int in_blocking_mode = 0;
  if (this->shared_accept_start (timeout,
                                 restart,
                                 in_blocking_mode) == -1)
    return -1;
  else
    {
      sockaddr *addr = 0;
      int len = 0;

      if (remote_addr != 0)
        {
          len = remote_addr->get_size ();
          addr = (sockaddr *) remote_addr->get_addr ();
        }

      do
        new_stream.set_handle (ACE_OS::accept (this->get_handle (),
                                               addr,
                                               &len));
      while (new_stream.get_handle () == ACE_INVALID_HANDLE
             && restart != 0
             && errno == EINTR
             && timeout == 0);

      // Reset the size of the addr, which is only necessary for UNIX
      // domain sockets.
      if (new_stream.get_handle () != ACE_INVALID_HANDLE
          && remote_addr != 0)
        remote_addr->set_size (len);
    }

  return this->shared_accept_finish (new_stream,
                                     in_blocking_mode,
                                     reset_new_handle);
}

int
ACE_MEM_Acceptor::shared_accept_finish (ACE_MEM_Stream new_stream,
                                        int in_blocking_mode,
                                        int reset_new_handle) const
{
  ACE_TRACE ("ACE_MEM_Acceptor::shared_accept_finish ()");

  ACE_HANDLE new_handle = new_stream.get_handle ();

  // Check to see if we were originally in blocking mode, and if so,
  // set the <new_stream>'s handle and <this> handle to be in blocking
  // mode.
  if (in_blocking_mode)
    {
      // Save/restore errno.
      ACE_Errno_Guard error (errno);

      // Only disable ACE_NONBLOCK if we weren't in non-blocking mode
      // originally.
      ACE::clr_flags (this->get_handle (),
                      ACE_NONBLOCK);
      ACE::clr_flags (new_handle,
                      ACE_NONBLOCK);
    }

#if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
  if (reset_new_handle)
    // Reset the event association inherited by the new handle.
    ::WSAEventSelect ((SOCKET) new_handle, 0, 0);
#else
  ACE_UNUSED_ARG (reset_new_handle);
#endif /* ACE_WIN32 */
  if (new_handle == ACE_INVALID_HANDLE)
    return -1;

  char buf [MAXPATHLEN];
  ACE_INET_Addr local_addr;
  if (new_stream.get_local_addr (local_addr) == -1)
    return -1;

  // @@ Need to make the filename prefix configurable.  Perhaps we
  // should have something like ACE_MEM_Addr?
  ACE_OS::sprintf (buf, "MEM_Acceptor_%d_", local_addr.get_port_number ());
  ACE_OS::unique_name (this, buf, MAXPATHLEN);

  // Make sure we have a fresh start.
  ACE_OS::unlink (buf);

  // Now set up the shared memory malloc pool.
  if (new_stream.create_shm_malloc (buf) == -1)
    return -1;

  // @@ Need to handle timeout here.
  ACE_UINT16 buf_len = ACE_OS::strlen (buf) + 1;

  // No need to worry about byte-order because both parties should always
  // be on the same machine.
  if (ACE::send (new_handle, &buf_len, sizeof (ACE_UINT16)) == -1)
    return -1;

  // Now send the pathname of the mmap file.
  if (ACE::send (new_handle, buf, buf_len) == -1)
    return -1;
  return 0;
}
