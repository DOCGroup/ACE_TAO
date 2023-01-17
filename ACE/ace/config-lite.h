// -*- C++ -*-

//==========================================================================
/**
 *  @file   config-lite.h
 *
 *  @author Doug Schmidt <d.schmidt@vanderbilt.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 *
 *  This file contains the contents of the old config-all.h in order to
 *  avoid a circular dependency problem caused by some of the new
 *  includes added to config-all.h, e.g., OS_main.h.
 */
//==========================================================================

#ifndef ACE_CONFIG_LITE_H
#define ACE_CONFIG_LITE_H

#include /**/ "ace/pre.h"

#include "ace/config-macros.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// ============================================================================
// UNICODE macros (to be added later)
// ============================================================================

// Get the unicode (i.e. ACE_TCHAR) defines
# include "ace/ace_wchar.h"

// ============================================================================
// at_exit declarations
// ============================================================================

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// Marker for cleanup, used by ACE_Exit_Info.
extern int ace_exit_hook_marker;

ACE_END_VERSIONED_NAMESPACE_DECL

// For use by <ACE_OS::exit>.
extern "C"
{
  typedef void (*ACE_EXIT_HOOK) ();
}

// Signature for registering a cleanup function that is used by the
// ACE_Object_Manager and the ACE_Thread_Manager.
# if defined (ACE_HAS_SIG_C_FUNC)
extern "C" {
# endif /* ACE_HAS_SIG_C_FUNC */
typedef void (*ACE_CLEANUP_FUNC)(void *object, void *param) /* noexcept */;
# if defined (ACE_HAS_SIG_C_FUNC)
}
# endif /* ACE_HAS_SIG_C_FUNC */

// ============================================================================
// log_msg declarations
// ============================================================================

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

# if defined (ACE_HAS_WIN32_STRUCTURED_EXCEPTIONS)
typedef int (*ACE_SEH_EXCEPT_HANDLER)(void *);
// Prototype of win32 structured exception handler functions.
// They are used to get the exception handling expression or
// as exception handlers.
# endif /* ACE_HAS_WIN32_STRUCTURED_EXCEPTIONS */

class ACE_OS_Thread_Descriptor;
class ACE_OS_Log_Msg_Attributes;
typedef void (*ACE_INIT_LOG_MSG_HOOK) (ACE_OS_Log_Msg_Attributes &attr
# if defined (ACE_HAS_WIN32_STRUCTURED_EXCEPTIONS)
                                       , ACE_SEH_EXCEPT_HANDLER selector
                                       , ACE_SEH_EXCEPT_HANDLER handler
# endif /* ACE_HAS_WIN32_STRUCTURED_EXCEPTIONS */
                                       );
typedef void (*ACE_INHERIT_LOG_MSG_HOOK) (ACE_OS_Thread_Descriptor*,
                                          ACE_OS_Log_Msg_Attributes &);

typedef void (*ACE_CLOSE_LOG_MSG_HOOK) ();

typedef void (*ACE_SYNC_LOG_MSG_HOOK) (const ACE_TCHAR *prog_name);

typedef ACE_OS_Thread_Descriptor *(*ACE_THR_DESC_LOG_MSG_HOOK) ();

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* ACE_CONFIG_LITE_H */
