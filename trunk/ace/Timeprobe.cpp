// $Id$

#include "ace/Timeprobe.h"

#if defined (ACE_COMPILE_TIMEPROBES)

#if !defined (__ACE_INLINE__)
#include "ace/Timeprobe.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Timeprobe<ACE_TIMEPROBE_MUTEX>;
template class ACE_Function_Timeprobe<ACE_Timeprobe<ACE_TIMEPROBE_MUTEX> >;
template class ACE_Unbounded_Set_Iterator<ACE_Event_Descriptions>;
template class ACE_Unbounded_Set<ACE_Event_Descriptions>;
template class ACE_Node<ACE_Event_Descriptions>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Timeprobe<ACE_TIMEPROBE_MUTEX>
#pragma instantiate ACE_Function_Timeprobe<ACE_Timeprobe<ACE_TIMEPROBE_MUTEX> >
#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_Event_Descriptions>
#pragma instantiate ACE_Unbounded_Set<ACE_Event_Descriptions>
#pragma instantiate ACE_Node<ACE_Event_Descriptions>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#  if defined (ACE_TSS_TIMEPROBES)
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_TSS_Singleton<ACE_TIMEPROBE_WITH_LOCKING, ACE_SYNCH_NULL_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_TSS_Singleton<ACE_TIMEPROBE_WITH_LOCKING, ACE_SYNCH_NULL_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
#  else /* ACE_TSS_TIMEPROBES */
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<ACE_TIMEPROBE_WITH_LOCKING, ACE_SYNCH_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<ACE_TIMEPROBE_WITH_LOCKING, ACE_SYNCH_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
#  endif /* ACE_TSS_TIMEPROBES */

#endif /* ACE_COMPILE_TIMEPROBES */
