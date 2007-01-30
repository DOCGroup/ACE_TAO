// $Id$

#include "ace/Timer_List.h"
#include "ace/Synch_Traits.h"
#include "ace/Recursive_Thread_Mutex.h"

ACE_RCSID(ace, Timer_List, "$Id$")

#if defined (ACE_HAS_BROKEN_HPUX_TEMPLATES)
#include "ace/Timer_Hash.h"
#include "ace/Timer_List_T.cpp"
#endif /* ACE_HAS_BROKEN_HPUX_TEMPLATES */

