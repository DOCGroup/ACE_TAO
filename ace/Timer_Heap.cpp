// $Id$

#include "ace/Timer_Heap.h"
#include "ace/Synch_Traits.h"
#include "ace/Recursive_Thread_Mutex.h"

#if defined (ACE_HAS_BROKEN_HPUX_TEMPLATES)
#include "ace/Timer_Hash.h"
#include "ace/Timer_Heap_T.cpp"

ACE_RCSID(ace,
          Timer_Heap,
          "$Id$")

#endif /* ACE_HAS_BROKEN_HPUX_TEMPLATES */

