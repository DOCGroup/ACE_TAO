// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_NS_stropts.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 *
 *  Originally in OS.h.
 */
//=============================================================================

#ifndef ACE_OS_NS_STROPTS_H
# define ACE_OS_NS_STROPTS_H

# include /**/ "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/OS_Export.h"
#include "ace/os_include/os_stropts.h"
#include "ace/os_include/os_stdio.h"

#if defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)
typedef WSAPROTOCOL_INFO ACE_Protocol_Info;

// Callback function that's used by the QoS-enabled <ACE_OS::ioctl>
// method.
typedef LPWSAOVERLAPPED_COMPLETION_ROUTINE ACE_OVERLAPPED_COMPLETION_FUNC;
typedef GROUP ACE_SOCK_GROUP;
#else  /*  (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0) */
struct ACE_Protocol_Info
{
  u_long dwServiceFlags1;
  int iAddressFamily;
  int iProtocol;
  char szProtocol[255+1];
};

// Callback function that's used by the QoS-enabled <ACE_OS::ioctl>
// method.
typedef void (*ACE_OVERLAPPED_COMPLETION_FUNC) (u_long error,
                                                u_long bytes_transferred,
                                                ACE_OVERLAPPED *overlapped,
                                                u_long flags);
typedef u_long ACE_SOCK_GROUP;

#endif /* (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0) */

// @todo: move this to it's own file... dhinton
/**
 * @class ACE_Str_Buf
 *
 * @brief Simple wrapper for STREAM pipes strbuf.
 */
class ACE_OS_Export ACE_Str_Buf : public strbuf
{
public:
  // = Initialization method
  /// Constructor.
  ACE_Str_Buf (void *b = 0, int l = 0, int max = 0);

  /// Constructor.
  ACE_Str_Buf (strbuf &);
};

class ACE_QoS;

namespace ACE_OS {

  int getmsg (ACE_HANDLE handle,
              struct strbuf *ctl,
              struct strbuf
              *data, int *flags);
  int getpmsg (ACE_HANDLE handle,
               struct strbuf *ctl,
               struct strbuf
               *data,
               int *band,
               int *flags);

  int fattach (int handle,
               const char *path);
  int fdetach (const char *file);

  /// UNIX-style <ioctl>.
  int ioctl (ACE_HANDLE handle,
             int cmd,
             void * = 0);

#if !defined (ACE_HAS_WINCE)
  /// QoS-enabled <ioctl>.
  int ioctl (ACE_HANDLE socket,
             u_long io_control_code,
             void *in_buffer_p,
             u_long in_buffer,
             void *out_buffer_p,
             u_long out_buffer,
             u_long *bytes_returned,
             ACE_OVERLAPPED *overlapped,
             ACE_OVERLAPPED_COMPLETION_FUNC func);

  /// QoS-enabled <ioctl> when the I/O control code is either
  /// SIO_SET_QOS or SIO_GET_QOS.
  int ioctl (ACE_HANDLE socket,
             u_long io_control_code,
             ACE_QoS &ace_qos,
             u_long *bytes_returned,
             void *buffer_p = 0,
             u_long buffer = 0,
             ACE_OVERLAPPED *overlapped = 0,
             ACE_OVERLAPPED_COMPLETION_FUNC func = 0);
#endif  // ACE_HAS_WINCE

  int isastream (ACE_HANDLE handle);

  int putmsg (ACE_HANDLE handle,
              const struct strbuf *ctl,
              const struct strbuf *data,
              int flags);
  int putpmsg (ACE_HANDLE handle,
               const struct strbuf *ctl,
               const struct strbuf *data,
               int band,
               int flags);

} /* namespace ACE_OS */

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_NS_stropts.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

# include /**/ "ace/post.h"
#endif /* ACE_OS_NS_STROPTS_H */
