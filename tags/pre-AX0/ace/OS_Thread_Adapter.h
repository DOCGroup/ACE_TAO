
//=============================================================================
/**
 *  @file    Thread_Adapter.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================

#ifndef ACE_OS_THREAD_ADAPTER_H
#define ACE_OS_THREAD_ADAPTER_H
#include "ace/pre.h"

#include "ace/Base_Thread_Adapter.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/OS_Export.h"

/**
 * @class ACE_OS_Thread_Adapter
 *
 * @brief Converts a C++ function into a function <ace_thread_adapter>
 * function that can be called from a thread creation routine
 * (e.g., <pthread_create> or <_beginthreadex>) that expects an
 * extern "C" entry point.  This class also makes it possible to
 * transparently provide hooks to register a thread with an
 * <ACE_Thread_Manager>.
 *
 * This class is used in <ACE_OS::thr_create>.  In general, the
 * thread that creates an object of this class is different from
 * the thread that calls <invoke> on this object.  Therefore,
 * the <invoke> method is responsible for deleting itself.
 */
class ACE_OS_Export ACE_OS_Thread_Adapter : public ACE_Base_Thread_Adapter
{
public:
  ACE_OS_Thread_Adapter (ACE_THR_FUNC user_func,
                         void *arg,
                         ACE_THR_C_FUNC entry_point = (ACE_THR_C_FUNC) ace_thread_adapter
# if defined (ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS)
                         , ACE_SEH_EXCEPT_HANDLER selector = 0
                         , ACE_SEH_EXCEPT_HANDLER handler = 0
# endif /* ACE_HAS_WIN32_STRUCTURAL_EXCEPTIONS */
  /// Constructor.
                         );

  /**
   * Execute the <user_func_> with the <arg>.  This function deletes
   * <this>, thereby rendering the object useless after the call
   * returns.
   */
  virtual void *invoke (void);

private:
  /// Ensure that this object must be allocated on the heap.
  ~ACE_OS_Thread_Adapter (void);

private:
  /// Friend declaration to avoid compiler warning:  only defines a private
  /// destructor and has no friends.
  friend class ACE_Thread_Adapter_Has_Private_Destructor;
};

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_Thread_Adapter.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

#include "ace/post.h"
#endif /* ACE_THREAD_ADAPTER_H */
