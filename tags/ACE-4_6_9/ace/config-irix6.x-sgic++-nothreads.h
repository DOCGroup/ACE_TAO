/* -*- C++ -*- */
// $Id$

// This is the config file for IRIX 6.2, 6.4 and hopefully 6.3, using
// the SGI C++ compiler (7.1 or higher).

// For IRIX 6.2 there are several patches that should be applied to
// get reliable operation with multi-threading and exceptions.
// Specifically you should get a reasonable current IRIX, Compiler
// and POSIX patch-sets.

// For IRIX 6.[34] it's less critical, but it's still recommended
// that you apply the applicable patch-sets (IRIX and Compiler I believe).

// These patches are updated frequently, so you should ask your support
// contact or search SGI's web site (http://www.sgi.com) for the latest
// version.

// Since this files gets included from config-irix6.x-sgic++.h we
// cannot use ACE_CONFIG_H here.
#ifndef ACE_CONFIG_IRIX6X_NTHR_H
#define ACE_CONFIG_IRIX6X_NTHR_H

// Include this file to set the _MIPS_SIM*ABI* macros.
#include /**/ <sgidefs.h>
#include "ace/config-irix6.x-common.h"

#define ACE_HAS_SGIDLADD
#define ACE_HAS_P_READ_WRITE
#define ACE_LACKS_LINEBUFFERED_STREAMBUF
#define ACE_HAS_SETOWN
#define ACE_HAS_SYSENT_H
#define ACE_HAS_SYSINFO
#define ACE_HAS_UALARM

// Platform has support for multi-byte character support compliant
// with the XPG4 Worldwide Portability Interface wide-character
// classification.
#define ACE_HAS_XPG4_MULTIBYTE_CHAR

// We need to setup a very high address or Naming_Test won't run.
#define ACE_DEFAULT_BASE_ADDR ((char *) (1024U * 1024 * 1024))

#define ACE_LACKS_SIGNED_CHAR

// Platform supports reentrant functions (i.e., all the POSIX *_r
// functions).
#define ACE_HAS_REENTRANT_FUNCTIONS

// Optimize ACE_Handle_Set for select().
#define ACE_HAS_HANDLE_SET_OPTIMIZED_FOR_SELECT

// Platform has terminal ioctl flags like TCGETS and TCSETS.
#define ACE_HAS_TERM_IOCTLS

// Platform does not support reentrant password file accessor functiions.
#define ACE_LACKS_PWD_REENTRANT_FUNCTIONS

// Platform does not support reentrant netdb functions (getprotobyname_r,
// getprotobynumber_r, gethostbyaddr_r, gethostbyname_r,
// getservbyname_r).
#define ACE_LACKS_NETDB_REENTRANT_FUNCTIONS

// uses ctime_r & asctime_r with only two parameters vs. three
#define ACE_HAS_2_PARAM_ASCTIME_R_AND_CTIME_R

// Prototypes for both signal() and struct sigaction are consistent.
#define ACE_HAS_CONSISTENT_SIGNAL_PROTOTYPES


// The following three should be enabled/disabled together.
#if _COMPILER_VERSION >= 720
#define ACE_HAS_TEMPLATE_SPECIALIZATION
#define ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION
#else
#define ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA
#endif /* _COMPILER_VERSION >= 720 */
#define ACE_TEMPLATES_REQUIRE_SOURCE
#define ACE_NEEDS_FUNC_DEFINITIONS

// Platform supports exceptions. Under 6.2 this requires an extra flag
// for the compiler, don't worry is already there in
// platform_irix6.x.GNU
#define ACE_HAS_EXCEPTIONS

// Platform supports STREAM pipes (note that this is disabled by
// default, see the manual page on pipe(2) to find out how to enable
// it).
// #define ACE_HAS_STREAM_PIPES

#if defined (_COMPILER_VERSION)
# define ACE_CC_NAME "SGI/MIPSPro"
# define ACE_CC_MAJOR_VERSION (_COMPILER_VERSION / 100)
# define ACE_CC_MINOR_VERSION (_COMPILER_VERSION % 100)
# define ACE_CC_BETA_VERSION  (0)
#endif /* _COMPILER_VERSION */

#endif /* ACE_CONFIG_IRIX6X_NTHR_H */
