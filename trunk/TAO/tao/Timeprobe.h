// $Id$

#ifndef TAO_TIMEPROBE_H
#define TAO_TIMEPROBE_H

// Please see ace/Timeprobe.h for details on these #defines.

#define ACE_ENABLE_TIMEPROBES
//#define ACE_MT_TIMEPROBES
//#define ACE_TSS_TIMEPROBES

#include "ace/Timeprobe.h"

// The macro for turning off all timeprobes except
// the ones that are pertinent to pluggable protocols.
#define TAO_SELECT_PP_TIMEPROBES

#if defined (ACE_ENABLE_TIMEPROBES)
#  define TAO_PP_TIMEPROBE(id) ACE_TIMEPROBE_SINGLETON::instance ()->timeprobe (id) 
#  define TAO_FUNCTION_PP_TIMEPROBE(X) ACE_Function_Timeprobe<ACE_TIMEPROBE_WITH_LOCKING> function_timeprobe (*ACE_TIMEPROBE_SINGLETON::instance (), X)
#else /* ACE_ENABLE_TIMEPROBES */
#  define TAO_PP_TIMEPROBE(id)
#  define TAO_FUNCTION_PP_TIMEPROBE(X)
#endif /* ACE_ENABLE_TIMEPROBES */

#if defined (TAO_SELECT_PP_TIMEPROBES)
#  undef ACE_TIMEPROBE
#  define ACE_TIMEPROBE(id)
#  undef ACE_FUNCTION_TIMEPROBE
#  define ACE_FUNCTION_TIMEPROBE(X)
#endif /* TAO_SELECT_PP_TIMEPROBES */

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#endif /* TAO_TIMEPROBE_H */
