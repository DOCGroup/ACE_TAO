// $Id$

#include "ace/OS_NS_sys_sendfile.h"
#include "ace/OS_NS_sys_mman.h"
#include "ace/OS_NS_unistd.h"

#ifndef ACE_HAS_INLINED_OSCALLS
# include "ace/OS_NS_sys_sendfile.inl"
#endif  /* ACE_HAS_INLINED_OS_CALLS */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

#ifndef ACE_HAS_SENDFILE
ssize_t
ACE_OS::sendfile_emulation (ACE_HANDLE out_fd,
                            ACE_HANDLE in_fd,
                            ACE_LOFF_T * offset,
                            size_t count)
{
  void const * const buf =
    ACE_OS::mmap (0, count, PROT_READ, MAP_PRIVATE, in_fd, *offset);

  ssize_t r = ACE_OS::write (out_fd, buf, count);

  if (r > 0)
    *offset += static_cast<ACE_LOFF_T> (r);

  return r;
}
#endif  /* !ACE_HAS_SENDFILE */

ACE_END_VERSIONED_NAMESPACE_DECL
