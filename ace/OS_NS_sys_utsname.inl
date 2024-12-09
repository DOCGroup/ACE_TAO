// -*- C++ -*-
//
// $Id$

#include "ace/os_include/os_errno.h"

namespace ACE_OS
{
#if !defined (ACE_WIN32) && !defined (VXWORKS) && !defined (CHORUS) && !defined (ACE_PSOS)
// Don't inline on those platforms because this function contains
// string literals, and some compilers, e.g., g++, don't handle those
// efficiently in unused inline functions.
  ACE_INLINE int
  uname (ACE_utsname *name)
  {
#if defined (INTEGRITY178B)
# if defined (ACE_LACKS_GETHOSTNAME)
  ACE_UNUSED_ARG (name);
  ACE_NOTSUP_RETURN (-1);
# else
  if(!name) {
    errno = EFAULT;
    return -1;
  }
  strcpy(name->sysname,"INTEGRITY-178B");
  int status = gethostname(name->nodename, __SYS_NMLN);
  strcpy(name->release,"minor");
  strcpy(name->version,"5.0.0");
  strcpy(name->machine,"a standard name");
  return status;
# endif /* ACE_LACKS_GETHOSTNAME */
#else
    ACE_OS_TRACE ("ACE_OS::uname");
    ACE_OSCALL_RETURN (::uname (name), int, -1);
#endif /* INTEGRITY178B */
  }
#endif /* ! ACE_WIN32 && ! VXWORKS && ! CHORUS && ! ACE_PSOS */
} // namespace ACE_OS
