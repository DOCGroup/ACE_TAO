// SOCK_Connector.cpp
// $Id$

#include "ace/SOCK_Connector.h"
#include "ace/INET_Addr.h"
#include "ace/Log_Msg.h"
#include "ace/OS_QoS.h"

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/SOCK_Connector.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

ACE_RCSID(ace, SOCK_Connector, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_SOCK_Connector)

void
ACE_SOCK_Connector::dump (void) const
{
  ACE_TRACE ("ACE_SOCK_Connector::dump");
}

int
ACE_SOCK_Connector::shared_open (ACE_SOCK_Stream &new_stream,
                                 int protocol_family,
                                 int protocol,
                                 int reuse_addr)
{
  ACE_TRACE ("ACE_SOCK_Connector::shared_open");

  // Only open a new socket if we don't already have a valid handle.
  if (new_stream.get_handle () == ACE_INVALID_HANDLE
      && new_stream.open (SOCK_STREAM,
                          protocol_family,
                          protocol,
                          reuse_addr) == -1)
    return -1;
  else
    return 0;
}

int
ACE_SOCK_Connector::shared_open (ACE_SOCK_Stream &new_stream,
                                 int protocol_family,
                                 int protocol,
                                 ACE_Protocol_Info *protocolinfo,
                                 ACE_SOCK_GROUP g,
                                 u_long flags,
                                 int reuse_addr)
{
  ACE_TRACE ("ACE_SOCK_Connector::shared_open");

  // Only open a new socket if we don't already have a valid handle.
  if (new_stream.get_handle () == ACE_INVALID_HANDLE
      && new_stream.open (SOCK_STREAM,
                          protocol_family,
                          protocol,
                          protocolinfo,
                          g,
                          flags,
                          reuse_addr) == -1)
    return -1;
  else
    return 0;
}

int
ACE_SOCK_Connector::shared_connect_start (ACE_SOCK_Stream &new_stream,
                                          const ACE_Time_Value *timeout,
                                          const ACE_Addr &local_sap)
{
  ACE_TRACE ("ACE_SOCK_Connector::shared_connect_start");

  if (local_sap != ACE_Addr::sap_any)
    {
      sockaddr *laddr = ACE_reinterpret_cast (sockaddr *,
                                              local_sap.get_addr ());
      size_t size = local_sap.get_size ();

      if (ACE_OS::bind (new_stream.get_handle (),
                        laddr,
                        size) == -1)
        {
          // Save/restore errno.
          ACE_Errno_Guard error (errno);
          new_stream.close ();
          return -1;
        }
    }

  // Enable non-blocking, if required.
  if (timeout != 0
      && new_stream.enable (ACE_NONBLOCK) == -1)
    return -1;
  else
    return 0;
}

int
ACE_SOCK_Connector::shared_connect_finish (ACE_SOCK_Stream &new_stream,
                                           const ACE_Time_Value *timeout,
                                           int result)
{
  ACE_TRACE ("ACE_SOCK_Connector::shared_connect_finish");
  // Save/restore errno.
  ACE_Errno_Guard error (errno);

  if (result == -1 && timeout != 0)
    {
      // Check whether the connection is in progress.
      if (error == EINPROGRESS || error == EWOULDBLOCK)
        {
          // This expression checks if we were polling.
          if (timeout->sec () == 0
              && timeout->usec () == 0)
            error = EWOULDBLOCK;
          // Wait synchronously using timeout.
          else if (this->complete (new_stream,
                                   0,
                                   timeout) == -1)
            error = errno;
          else
            return 0;
        }
    }

  // EISCONN is treated specially since this routine may be used to
  // check if we are already connected.
  if (result != -1 || error == EISCONN)
    // Start out with non-blocking disabled on the <new_stream>.
    new_stream.disable (ACE_NONBLOCK);
  else if (!(error == EWOULDBLOCK || error == ETIMEDOUT))
    new_stream.close ();

  return result;
}

// Actively connect and produce a new ACE_SOCK_Stream if things go well...

int
ACE_SOCK_Connector::connect (ACE_SOCK_Stream &new_stream,
                             const ACE_Addr &remote_sap,
                             const ACE_Time_Value *timeout,
                             const ACE_Addr &local_sap,
                             int reuse_addr,
                             int /* flags */,
                             int /* perms */)
{
  ACE_TRACE ("ACE_SOCK_Connector::connect");

  if (this->shared_open (new_stream,
                         remote_sap.get_type (),
                         0,
                         reuse_addr) == -1)
    return -1;
  else if (this->shared_connect_start (new_stream,
                                       timeout,
                                       local_sap) == -1)
    return -1;

  int result = ACE_OS::connect (new_stream.get_handle (),
                                ACE_reinterpret_cast (sockaddr *,
                                                      remote_sap.get_addr ()),
                                remote_sap.get_size ());

  return this->shared_connect_finish (new_stream,
                                      timeout,
                                      result);
}

int
ACE_SOCK_Connector::connect (ACE_SOCK_Stream &new_stream,
                             const ACE_Addr &remote_sap,
                             ACE_QoS_Params qos_params,
                             const ACE_Time_Value *timeout,
                             const ACE_Addr &local_sap,
                             ACE_Protocol_Info * protocolinfo,
                             ACE_SOCK_GROUP g,
                             u_long flags,
                             int reuse_addr,
                             int /* perms */)
{
  ACE_TRACE ("ACE_SOCK_Connector::connect");

  if (this->shared_open (new_stream,
                         remote_sap.get_type (),
                         0,
                         protocolinfo,
                         g,
                         flags,
                         reuse_addr) == -1)
    return -1;
  else if (this->shared_connect_start (new_stream,
                                       timeout,
                                       local_sap) == -1)
    return -1;

  int result = ACE_OS::connect (new_stream.get_handle (),
                                ACE_reinterpret_cast (sockaddr *,
                                                      remote_sap.get_addr ()),
                                remote_sap.get_size (),
                                qos_params);

  return this->shared_connect_finish (new_stream,
                                      timeout,
                                      result);
}

// Try to complete a non-blocking connection.

int
ACE_SOCK_Connector::complete (ACE_SOCK_Stream &new_stream,
                              ACE_Addr *remote_sap,
                              const ACE_Time_Value *tv)
{
  ACE_TRACE ("ACE_SOCK_Connector::complete");
  ACE_HANDLE h = ACE::handle_timed_complete (new_stream.get_handle (),
                                             tv);
  // We failed to get connected.
  if (h == ACE_INVALID_HANDLE)
    {
#if defined (ACE_WIN32)
      // Win32 has a timing problem - if you check to see if the
      // connection has completed too fast, it will fail - so wait
      // <ACE_NON_BLOCKING_BUG_DELAY> microseconds to let it catch up
      // then retry to see if it's a real failure.
      ACE_Time_Value time (0, ACE_NON_BLOCKING_BUG_DELAY);
      ACE_OS::sleep (time);
      h = ACE::handle_timed_complete (new_stream.get_handle (),
                                      tv);
      if (h == ACE_INVALID_HANDLE)
        {
#endif /* ACE_WIN32 */
      // Save/restore errno.
      ACE_Errno_Guard error (errno);
      new_stream.close ();
      return -1;
#if defined (ACE_WIN32)
        }
#endif /* ACE_WIN32 */
    }

  if (remote_sap != 0)
    {
      int len = remote_sap->get_size ();
      sockaddr *addr = ACE_reinterpret_cast (sockaddr *,
                                             remote_sap->get_addr ());
      if (ACE_OS::getpeername (h,
                               addr,
                               &len) == -1)
        {
          // Save/restore errno.
          ACE_Errno_Guard error (errno);
          new_stream.close ();
          return -1;
        }
    }

  // Start out with non-blocking disabled on the <new_stream>.
  new_stream.disable (ACE_NONBLOCK);
  return 0;
}

ACE_SOCK_Connector::ACE_SOCK_Connector (ACE_SOCK_Stream &new_stream,
                                        const ACE_Addr &remote_sap,
                                        const ACE_Time_Value *timeout,
                                        const ACE_Addr &local_sap,
                                        int reuse_addr,
                                        int flags,
                                        int perms)
{
  ACE_TRACE ("ACE_SOCK_Connector::ACE_SOCK_Connector");

  if (this->connect (new_stream,
                     remote_sap,
                     timeout,
                     local_sap,
                     reuse_addr,
                     flags,
                     perms) == -1
      && timeout != 0
      && !(errno == EWOULDBLOCK || errno == ETIME || errno == ETIMEDOUT))
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("ACE_SOCK_Connector::ACE_SOCK_Connector")));
}

ACE_SOCK_Connector::ACE_SOCK_Connector (ACE_SOCK_Stream &new_stream,
                                        const ACE_Addr &remote_sap,
                                        ACE_QoS_Params qos_params,
                                        const ACE_Time_Value *timeout,
                                        const ACE_Addr &local_sap,
                                        ACE_Protocol_Info *protocolinfo,
                                        ACE_SOCK_GROUP g,
                                        u_long flags,
                                        int reuse_addr,
                                        int perms)
{
  ACE_TRACE ("ACE_SOCK_Connector::ACE_SOCK_Connector");

  if (this->connect (new_stream,
                     remote_sap,
                     qos_params,
                     timeout,
                     local_sap,
                     protocolinfo,
                     g,
                     flags,
                     reuse_addr,
                     perms) == -1
      && timeout != 0
      && !(errno == EWOULDBLOCK || errno == ETIME || errno == ETIMEDOUT))
    ACE_ERROR ((LM_ERROR,
                ACE_LIB_TEXT ("%p\n"),
                ACE_LIB_TEXT ("ACE_SOCK_Connector::ACE_SOCK_Connector")));
}
