// -*- C++ -*-

//=============================================================================
/**
 *  @file   Global_Macros.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 *  @This one is split from the famous OS.h
 */
//=============================================================================

#ifndef ACE_GLOBAL_MACROS_H
#define ACE_GLOBAL_MACROS_H
#include "ace/pre.h"

// Included just keep compilers that see #pragma dierctive first
// happy.
#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


// Start Global definitions
# define ACE_BEGIN_DUMP ACE_LIB_TEXT ("\n====\n(%P|%t|%x)")
# define ACE_END_DUMP ACE_LIB_TEXT ("====\n")

// A free list which create more elements when there aren't enough
// elements.
# define ACE_FREE_LIST_WITH_POOL 1

// A simple free list which doen't allocate/deallocate elements.
# define ACE_PURE_FREE_LIST 2

# if defined (ACE_NDEBUG)
#   define ACE_DB(X)
# else
#   define ACE_DB(X) X
# endif /* ACE_NDEBUG */

// ACE_NO_HEAP_CHECK macro can be used to suppress false report of
// memory leaks. It turns off the built-in heap checking until the
// block is left. The old state will then be restored Only used for
// Win32 (in the moment).
# if defined (ACE_WIN32)

#define ACE_MAX_USERID 32

// This is necessary to work around bugs with Win32 non-blocking
// connects...
#   if !defined (ACE_NON_BLOCKING_BUG_DELAY)
#     define ACE_NON_BLOCKING_BUG_DELAY 35000
#   endif /* ACE_NON_BLOCKING_BUG_DELAY */

#   if defined (_DEBUG) && !defined (ACE_HAS_WINCE) && !defined (__BORLANDC__)
# include /**/ <crtdbg.h>

class ACE_OS_Export ACE_No_Heap_Check
{
public:
  ACE_No_Heap_Check (void)
    : old_state (_CrtSetDbgFlag (_CRTDBG_REPORT_FLAG))
  { _CrtSetDbgFlag (old_state & ~_CRTDBG_ALLOC_MEM_DF);}
  ~ACE_No_Heap_Check (void) { _CrtSetDbgFlag (old_state);}
private:
  int old_state;
};
#     define ACE_NO_HEAP_CHECK ACE_No_Heap_Check ____no_heap;
#   else /* !_DEBUG */
#     define ACE_NO_HEAP_CHECK
#   endif /* _DEBUG */
# else /* !ACE_WIN32 */
#   define ACE_NO_HEAP_CHECK
#define ACE_MAX_USERID L_cuserid
# endif /* ACE_WIN32 */

// Turn a number into a string.
# define ACE_ITOA(X) #X

// Create a string of a server address with a "host:port" format.
# define ACE_SERVER_ADDRESS(H,P) H":"P

// A couple useful inline functions for checking whether bits are
// enabled or disabled.

// Efficiently returns the least power of two >= X...
# define ACE_POW(X) (((X) == 0)?1:(X-=1,X|=X>>1,X|=X>>2,X|=X>>4,X|=X>>8,X|=X>>16,(++X)))
# define ACE_EVEN(NUM) (((NUM) & 1) == 0)
# define ACE_ODD(NUM) (((NUM) & 1) == 1)
# define ACE_BIT_ENABLED(WORD, BIT) (((WORD) & (BIT)) != 0)
# define ACE_BIT_DISABLED(WORD, BIT) (((WORD) & (BIT)) == 0)
# define ACE_BIT_CMP_MASK(WORD, BIT, MASK) (((WORD) & (BIT)) == MASK)
# define ACE_SET_BITS(WORD, BITS) (WORD |= (BITS))
# define ACE_CLR_BITS(WORD, BITS) (WORD &= ~(BITS))

# if !defined (ACE_ENDLESS_LOOP)
#  define ACE_ENDLESS_LOOP
# endif /* ! ACE_ENDLESS_LOOP */

# if defined (ACE_NEEDS_FUNC_DEFINITIONS)
    // It just evaporated ;-)  Not pleasant.
#   define ACE_UNIMPLEMENTED_FUNC(f)
# else
#   define ACE_UNIMPLEMENTED_FUNC(f) f;
# endif /* ACE_NEEDS_FUNC_DEFINITIONS */

// Easy way to designate that a class is used as a pseudo-namespace.
// Insures that g++ "friendship" anamolies are properly handled.
# define ACE_CLASS_IS_NAMESPACE(CLASSNAME) \
private: \
CLASSNAME (void); \
CLASSNAME (const CLASSNAME&); \
friend class ace_dewarn_gplusplus


# if (ACE_NTRACE == 1)
#   define ACE_TRACE(X)
# else
#   define ACE_TRACE(X) ACE_Trace ____ (ACE_LIB_TEXT (X), __LINE__, ACE_LIB_TEXT (__FILE__))
# endif /* ACE_NTRACE */


#include "ace/post.h"
#endif /*ACE_GLOBAL_MACROS_H*/
