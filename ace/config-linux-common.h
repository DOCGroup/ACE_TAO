/* -*- C++ -*- */
// $Id$

// This configuration file is designed to be included by another,
// specific configuration file.  It provides config information common
// to all Linux platforms.

#if !defined (ACE_LINUX_COMMON_H)
#define ACE_LINUX_COMMON_H

#if defined (__alpha__)
# define ACE_HAS_64BIT_LONGS

// The following might only be necessary on Alpha?
# define ACE_HAS_DLFCN_H_BROKEN_EXTERN_C
# define ACE_HAS_SIGWAIT
# define ACE_HAS_SIZET_SOCKET_LEN
# define ACE_LACKS_RPC_H
# define ACE_NEEDS_SYSTIME_H
#endif /* __alpha__ */

#endif /* ACE_LINUX_COMMON_H */
