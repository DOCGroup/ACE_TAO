
//=============================================================================
/**
 *  @file    Timeprobe.h
 *
 *  $Id$
 *
 *  @author Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_TIMEPROBE_H
#define TAO_TIMEPROBE_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

// Please see ace/Timeprobe.h for details on these #defines.

// #define TAO_ENABLE_TIMEPROBES
// #define TAO_MT_TIMEPROBES
// #define TAO_TSS_TIMEPROBES

#if defined (TAO_ENABLE_TIMEPROBES)
#define ACE_ENABLE_TIMEPROBES
#endif /* TAO_ENABLE_TIMEPROBES */

#if defined (TAO_MT_TIMEPROBES)
#define ACE_MT_TIMEPROBES
#endif /* TAO_MT_TIMEPROBES */

#if defined (TAO_TSS_TIMEPROBES)
#define ACE_TSS_TIMEPROBES
#endif /* TAO_TSS_TIMEPROBES */

#include "ace/Timeprobe.h"

// The macro for turning off all timeprobes except
// the ones that are pertinent to pluggable protocols.
//#define TAO_SELECT_PP_TIMEPROBES

// The macro for turning off all timeprobes except
// four - 2 to measure client send time and 2 to measure
// server upcall processing and return time.
// NOTE: If both this and the pluggable protocols macro
// above are defined, code below will undefine this macro.
// They should not both be defined at the same time.
//#define TAO_SELECT_MINIMAL_TIMEPROBES


#if defined (ACE_ENABLE_TIMEPROBES)

# define TAO_PP_TIMEPROBE(id) \
         ACE_TIMEPROBE_SINGLETON::instance ()->timeprobe (id)
# define TAO_FUNCTION_PP_TIMEPROBE(X) \
         ACE_Function_Timeprobe<ACE_TIMEPROBE_WITH_LOCKING> \
         function_timeprobe (*ACE_TIMEPROBE_SINGLETON::instance (), X)
# define TAO_MINIMAL_TIMEPROBE(id)

# if defined (TAO_SELECT_PP_TIMEPROBES)
#   if defined TAO_SELECT_MINIMAL_TIMEPROBES
#     undef TAO_SELECT_MINIMIAL_TIMEPROBES
#   endif /* TAO_SELECT_MINIMAL_TIMEPROBES */
#   undef ACE_TIMEPROBE
#   define ACE_TIMEPROBE(id)
#   undef ACE_FUNCTION_TIMEPROBE
#   define ACE_FUNCTION_TIMEPROBE(X)
#   define TAO_MINIMAL_TIMEPROBE(id)
# endif /* TAO_SELECT_PP_TIMEPROBES */

# if defined (TAO_SELECT_MINIMAL_TIMEPROBES)
#   undef TAO_MINIMAL_TIMEPROBE
#   define TAO_MINIMAL_TIMEPROBE(id) \
           ACE_TIMEPROBE_SINGLETON::instance ()->timeprobe (id)
#   undef ACE_TIMEPROBE
#   define ACE_TIMEPROBE(id)
#   undef ACE_FUNCTION_TIMEPROBE
#   define ACE_FUNCTION_TIMEPROBE(X)
#   undef TAO_PP_TIMEPROBE
#   define TAO_PP_TIMEPROBE(id)
#   undef TAO_FUNCTION_PP_TIMEPROBE
#   define TAO_FUNCTION_PP_TIMEPROBE(X)
# endif /* TAO_SELECT_MINIMAL_TIMEPROBES */

#else /* ACE_ENABLE_TIMEPROBES */

# define TAO_PP_TIMEPROBE(id)
# define TAO_FUNCTION_PP_TIMEPROBE(X)
# define TAO_MINIMAL_TIMEPROBE(id)

#endif /* ACE_ENABLE_TIMEPROBES */


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "ace/post.h"
#endif /* TAO_TIMEPROBE_H */
