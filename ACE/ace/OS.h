// -*- C++ -*-

//=============================================================================
/**
 *  @file  OS.h
 *
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 */
//=============================================================================

#ifndef ACE_OS_H
#define ACE_OS_H

#include /**/ "pre.h"

#include /**/ "config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @namespace ACE_OS
 *
 * @brief This namespace defines an OS independent programming API that
 *     shields developers from nonportable aspects of writing
 *     efficient system programs on Win32, POSIX and other versions
 *     of UNIX, and various real-time operating systems.
 *
 * This namespace encapsulates the differences between various OS
 * platforms.  When porting ACE to a new platform, this class is
 * the place to focus on.  Once this file is ported to a new
 * platform, pretty much everything else comes for "free."  See
 * docs/ACE-porting.html for instructions on porting ACE.
 * Please see the README file in this directory for complete
 * information on the meaning of the various macros.
 */
namespace ACE_OS
{
  // = A set of wrappers for miscellaneous operations.
}  /* namespace ACE_OS */

ACE_END_VERSIONED_NAMESPACE_DECL

#if !defined (DO_NOT_INCLUDE_OS_H)
#include "Cleanup.h"
#include "Object_Manager_Base.h"
#include "OS_main.h"
#include "OS_NS_arpa_inet.h"
#include "OS_NS_ctype.h"
#include "OS_NS_dirent.h"
#include "OS_NS_dlfcn.h"
#include "OS_NS_errno.h"
#include "OS_NS_fcntl.h"
#include "OS_NS_math.h"
#include "OS_NS_netdb.h"
#include "OS_NS_poll.h"
#include "OS_NS_pwd.h"
#include "OS_NS_regex.h"
#include "OS_NS_signal.h"
#include "OS_NS_stdio.h"
#include "OS_NS_stdlib.h"
#include "OS_NS_string.h"
#include "OS_NS_strings.h"
#include "OS_NS_stropts.h"
#include "OS_NS_sys_mman.h"
#include "OS_NS_sys_msg.h"
#include "OS_NS_sys_resource.h"
#include "OS_NS_sys_select.h"
#include "OS_NS_sys_shm.h"
#include "OS_NS_sys_socket.h"
#include "OS_NS_sys_stat.h"
#include "OS_NS_sys_time.h"
#include "OS_NS_sys_uio.h"
#include "OS_NS_sys_utsname.h"
#include "OS_NS_sys_wait.h"
#include "OS_NS_Thread.h"
#include "OS_NS_time.h"
#include "OS_NS_unistd.h"
#include "OS_NS_wchar.h"

// Include the split up ACE_OS classes
#include "OS_Memory.h"
#include "OS_TLI.h"
#include "OS_Errno.h"

// Here are all ACE-specific default constants, needed throughout ACE
// and its applications.  The values can be over written by user
// specific values in config.h files.
#include "Default_Constants.h"

// Here are all ACE-specific global declarations needed throughout
// ACE.
#include "Global_Macros.h"

#endif /* DO_NOT_INCLUDE_OS_H */

#include /**/ "post.h"
#endif  /* ACE_OS_H */
