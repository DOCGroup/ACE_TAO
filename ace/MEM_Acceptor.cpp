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
}

// Do nothing routine for constructor.

ACE_MEM_Acceptor::ACE_MEM_Acceptor (void)
{
  ACE_TRACE ("ACE_MEM_Acceptor::ACE_MEM_Acceptor");
}

// General purpose routine for performing server ACE_SOCK creation.

ACE_MEM_Acceptor::ACE_MEM_Acceptor (const u_short local_port,
                                    int reuse_addr,
                                    int backlog,
                                    int protocol)
{
  ACE_TRACE ("ACE_MEM_Acceptor::ACE_MEM_Acceptor");
  if (this->open (local_port,
                  reuse_addr,
                  backlog,
                  protocol) == -1)
    ACE_ERROR ((LM_ERROR,
                "ACE_MEM_Acceptor::ACE_MEM_Acceptor"));
}

ACE_MEM_Acceptor::open (const u_short local_port,
                        int reuse_addr,
                        int back_log,
                        int protocol)
{
  ACE_TRACE ("ACE_MEM_Acceptor::open");
  ACE_INET_Addr local_addr (local_port,
                            ASYS_TEXT ("localhost"));

  return this->ACE_SOCK_Acceptor::open (local_addr,
                                        reuse_addr,
                                        PF_INET,
                                        back_log,
                                        protocol);
}

// General purpose routine for accepting new connections.

int
ACE_MEM_Acceptor::accept (ACE_MEM_Stream &new_stream,
                          u_short *remote_port,
                          ACE_Time_Value *timeout,
                          int restart,
                          int reset_new_handle) const
{
  ACE_TRACE ("ACE_MEM_Acceptor::accept");

  int *len_ptr = 0;
  sockaddr *addr = 0;

  int in_blocking_mode = 0;
  if (this->shared_accept_start (timeout,
                                 restart,
                                 in_blocking_mode) == -1)
    return -1;
  else
    {
      do
        new_stream.set_handle (ACE_OS::accept (this->get_handle (),
                                               addr,
                                               len_ptr));
      while (new_stream.get_handle () == ACE_INVALID_HANDLE
             && restart != 0
             && errno == EINTR
             && timeout == 0);

      if (remote_port != 0)
        *remote_port = (* (sockaddr_in*) addr).sin_port;
    }

  if (this->shared_accept_finish (new_stream,
                                  in_blocking_mode,
                                  reset_new_handle) == -1)
    return -1;

  char buf [MAXPATHLEN];
  ACE_INET_Addr local_addr;
  if (new_stream.get_local_addr (local_addr) == -1)
    return -1;

  // @@ Need to make the filename prefix configurable.  Perhaps we
  // should have something like ACE_MEM_Addr?
  ACE_OS::sprintf (buf, "MEM_Acceptor_%d_", local_addr.get_port_number ());
  char unique [MAXPATHLEN];
  ACE_OS::unique_name (&new_stream, unique, MAXPATHLEN);
  ACE_OS::strcat (buf, unique);

  // Make sure we have a fresh start.
  ACE_OS::unlink (buf);

  // Now set up the shared memory malloc pool.
  if (new_stream.create_shm_malloc (buf) == -1)
    return -1;

  // @@ Need to handle timeout here.
  ACE_UINT16 buf_len = ACE_OS::strlen (buf) + 1;
  ACE_HANDLE new_handle = new_stream.get_handle ();

  // No need to worry about byte-order because both parties should always
  // be on the same machine.
  if (ACE::send (new_handle, &buf_len, sizeof (ACE_UINT16)) == -1)
    return -1;

  // Now send the pathname of the mmap file.
  if (ACE::send (new_handle, buf, buf_len) == -1)
    return -1;
  return 0;
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

  return 0;
}
