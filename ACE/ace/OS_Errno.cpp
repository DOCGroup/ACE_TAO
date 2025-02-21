// -*- C++ -*-
#include "ace/OS_Errno.h"

// Inlining this class on debug builds with gcc deadlocks during static
// initialization. On non debug builds it causes compilation errors.
#if !defined (ACE_HAS_INLINED_OSCALLS)
# if defined (ACE_INLINE)
#  undef ACE_INLINE
# endif /* ACE_INLINE */
# define ACE_INLINE
# include "ace/OS_Errno.inl"
#endif /* ACE_HAS_INLINED_OSCALLS */

