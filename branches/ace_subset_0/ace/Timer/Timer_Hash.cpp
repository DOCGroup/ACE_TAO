// $Id$

// Timer_Hash.cpp

#if !defined (ACE_TIMER_HASH_C)
#define ACE_TIMER_HASH_C

#include "ace/Timer/Timer_Hash.h"

#if defined (ACE_HAS_BROKEN_HPUX_TEMPLATES)
#include "ace/Timer/Timer_Hash_T.cpp"
#endif /* ACE_HAS_BROKEN_HPUX_TEMPLATES */

ACE_RCSID(ace, Timer_Hash, "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Free_List<ACE_Timer_Node_T<ACE_Event_Handler *> >;
template class ACE_Locked_Free_List<ACE_Timer_Node_T<ACE_Event_Handler *>,
                                    ACE_Null_Mutex>;
template class ACE_Timer_Hash_Upcall <ACE_Event_Handler *,
                                      ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>,
                                      ACE_SYNCH_RECURSIVE_MUTEX>;

template class ACE_Timer_Queue_T <ACE_Event_Handler *,
                                  ACE_Hash_Upcall,
                                  ACE_Null_Mutex>;

template class ACE_Timer_Queue_Iterator_T <ACE_Event_Handler *,
                                           ACE_Hash_Upcall,
                                           ACE_Null_Mutex>;

template class ACE_Timer_List_T <ACE_Event_Handler *,
                                 ACE_Hash_Upcall,
                                 ACE_Null_Mutex>;

template class ACE_Timer_List_Iterator_T <ACE_Event_Handler *,
                                          ACE_Hash_Upcall,
                                          ACE_Null_Mutex>;

template class ACE_Timer_Heap_T <ACE_Event_Handler *,
                                 ACE_Hash_Upcall,
                                 ACE_Null_Mutex>;

template class ACE_Timer_Heap_Iterator_T <ACE_Event_Handler *,
                                          ACE_Hash_Upcall,
                                          ACE_Null_Mutex>;

template class ACE_Timer_Hash_T<ACE_Event_Handler *,
                        ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>,
                        ACE_SYNCH_RECURSIVE_MUTEX,
                        ACE_Hash_Timer_List>;

template class ACE_Timer_Hash_Iterator_T<ACE_Event_Handler *,
                                  ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>,
                                  ACE_SYNCH_RECURSIVE_MUTEX,
                                  ACE_Hash_Timer_List>;

template class ACE_Timer_Hash_T<ACE_Event_Handler *,
                        ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>,
                        ACE_SYNCH_RECURSIVE_MUTEX,
                        ACE_Hash_Timer_Heap>;

template class ACE_Timer_Hash_Iterator_T<ACE_Event_Handler *,
                                  ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>,
                                  ACE_SYNCH_RECURSIVE_MUTEX,
                                  ACE_Hash_Timer_Heap>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Free_List<ACE_Timer_Node_T<ACE_Event_Handler *> >
#pragma instantiate ACE_Locked_Free_List<ACE_Timer_Node_T<ACE_Event_Handler *>, \
                                    ACE_Null_Mutex>
#pragma instantiate ACE_Timer_Hash_Upcall <ACE_Event_Handler *, \
                                      ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, \
                                      ACE_SYNCH_RECURSIVE_MUTEX>

#pragma instantiate ACE_Timer_Queue_T <ACE_Event_Handler *, \
                                  ACE_Hash_Upcall, \
                                  ACE_Null_Mutex>

#pragma instantiate ACE_Timer_Queue_Iterator_T <ACE_Event_Handler *, \
                                           ACE_Hash_Upcall, \
                                           ACE_Null_Mutex>

#pragma instantiate ACE_Timer_List_T <ACE_Event_Handler *, \
                                 ACE_Hash_Upcall, \
                                 ACE_Null_Mutex>

#pragma instantiate ACE_Timer_List_Iterator_T <ACE_Event_Handler *, \
                                          ACE_Hash_Upcall, \
                                          ACE_Null_Mutex>

#pragma instantiate ACE_Timer_Heap_T <ACE_Event_Handler *, \
                                 ACE_Hash_Upcall, \
                                 ACE_Null_Mutex>

#pragma instantiate ACE_Timer_Heap_Iterator_T <ACE_Event_Handler *, \
                                          ACE_Hash_Upcall, \
                                          ACE_Null_Mutex>

#pragma instantiate ACE_Timer_Hash_T<ACE_Event_Handler *, \
                        ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, \
                        ACE_SYNCH_RECURSIVE_MUTEX, \
                        ACE_Hash_Timer_List>

#pragma instantiate ACE_Timer_Hash_Iterator_T<ACE_Event_Handler *, \
                                  ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, \
                                  ACE_SYNCH_RECURSIVE_MUTEX, \
                                  ACE_Hash_Timer_List>

#pragma instantiate ACE_Timer_Hash_T<ACE_Event_Handler *, \
                        ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, \
                        ACE_SYNCH_RECURSIVE_MUTEX, \
                        ACE_Hash_Timer_Heap>

#pragma instantiate ACE_Timer_Hash_Iterator_T<ACE_Event_Handler *, \
                                  ACE_Event_Handler_Handle_Timeout_Upcall<ACE_SYNCH_RECURSIVE_MUTEX>, \
                                  ACE_SYNCH_RECURSIVE_MUTEX, \
                                  ACE_Hash_Timer_Heap>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


#endif /* ACE_TIMER_HASH_C */
