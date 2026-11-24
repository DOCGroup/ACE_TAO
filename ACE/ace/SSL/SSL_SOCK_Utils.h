/**
 * @file SSL_SOCK_Utils.h
 * @brief Internal helpers shared by the SSL socket acceptor and connector.
 */

#ifndef ACE_SSL_SOCK_UTILS_H
#define ACE_SSL_SOCK_UTILS_H

#include /**/ "ace/pre.h"

#include "ace/Handle_Set.h"
#include "ace/OS_Errno.h"
#include "ace/OS_NS_errno.h"

#include <openssl/ssl.h>

namespace ACE_SSL_SOCK_Utils
{
  inline bool valid_handle (ACE_HANDLE const handle)
  {
#if !defined (ACE_WIN32)
    if (handle < 0 || handle >= ACE_FD_SETSIZE)
      {
        errno = EINVAL;
        return false;
      }
#else
    ACE_UNUSED_ARG (handle);
#endif /* !ACE_WIN32 */
    return true;
  }

  inline bool set_handle_bit (SSL *session,
                              ACE_HANDLE handle,
                              ACE_Handle_Set &read_handles,
                              ACE_Handle_Set &write_handles)
  {
    if (SSL_want_write (session))
      {
        write_handles.set_bit (handle);
        return true;
      }

    if (SSL_want_read (session))
      {
        read_handles.set_bit (handle);
        return true;
      }

    return false;
  }
}

#include /**/ "ace/post.h"

#endif /* ACE_SSL_SOCK_UTILS_H */
