// $Id$

#include "ace/Svcconf/Service_Templates.h"

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Node<ACE_Static_Svc_Descriptor *>;
template class ACE_Unbounded_Set<ACE_Static_Svc_Descriptor *>;
template class ACE_Unbounded_Set_Iterator<ACE_Static_Svc_Descriptor *>;
template class ACE_Node<ACE_TString>;
template class ACE_Unbounded_Queue<ACE_TString>;
template class ACE_Unbounded_Queue_Iterator<ACE_TString>;
template class ACE_Unbounded_Set<ACE_TString>;
template class ACE_Unbounded_Set_Iterator<ACE_TString>;
template class auto_ptr<ACE_Obstack>;
template class ACE_Auto_Basic_Ptr<ACE_Obstack>;

template class ACE_Message_Queue<ACE_SYNCH>;
template class ACE_Message_Queue_Iterator<ACE_SYNCH>;
template class ACE_Message_Queue_Reverse_Iterator<ACE_SYNCH>;
template class ACE_Message_Queue_Factory<ACE_SYNCH>;
template class ACE_Dynamic_Message_Queue<ACE_SYNCH>;
template class ACE_Module<ACE_SYNCH>;
template class ACE_Stream<ACE_SYNCH>;
template class ACE_Stream_Head<ACE_SYNCH>;
template class ACE_Stream_Tail<ACE_SYNCH>;
template class ACE_Task<ACE_SYNCH>;
template class ACE_Thru_Task<ACE_SYNCH>;

// Even with threads, these ACE_NULL_SYNCH specializations are necessary.
#if defined (ACE_HAS_THREADS)
  template class ACE_Message_Queue<ACE_NULL_SYNCH>;
  template class ACE_Message_Queue_Iterator<ACE_NULL_SYNCH>;
  template class ACE_Message_Queue_Reverse_Iterator<ACE_NULL_SYNCH>;
  template class ACE_Message_Queue_Factory<ACE_NULL_SYNCH>;
  template class ACE_Dynamic_Message_Queue<ACE_NULL_SYNCH>;
  template class ACE_Module<ACE_NULL_SYNCH>;
  template class ACE_Task<ACE_NULL_SYNCH>;
  template class ACE_Thru_Task<ACE_NULL_SYNCH>;
#endif /* ACE_HAS_THREADS */
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Node<ACE_Static_Svc_Descriptor *>
#pragma instantiate ACE_Unbounded_Set<ACE_Static_Svc_Descriptor *>
#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_Static_Svc_Descriptor *>
#pragma instantiate ACE_Node<ACE_TString>
#pragma instantiate ACE_Unbounded_Queue<ACE_TString>
#pragma instantiate ACE_Unbounded_Queue_Iterator<ACE_TString>
#pragma instantiate ACE_Unbounded_Set<ACE_TString>
#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_TString>
#pragma instantiate auto_ptr<ACE_Obstack>
#pragma instantiate ACE_Auto_Basic_Ptr<ACE_Obstack>

#pragma instantiate ACE_Message_Queue<ACE_SYNCH>
#pragma instantiate ACE_Message_Queue_Iterator<ACE_SYNCH>
#pragma instantiate ACE_Message_Queue_Reverse_Iterator<ACE_SYNCH>
#pragma instantiate ACE_Message_Queue_Factory<ACE_SYNCH>
#pragma instantiate ACE_Dynamic_Message_Queue<ACE_SYNCH>
#pragma instantiate ACE_Module<ACE_SYNCH>
#pragma instantiate ACE_Stream<ACE_SYNCH>
#pragma instantiate ACE_Stream_Head<ACE_SYNCH>
#pragma instantiate ACE_Stream_Tail<ACE_SYNCH>
#pragma instantiate ACE_Task<ACE_SYNCH>
#pragma instantiate ACE_Thru_Task<ACE_SYNCH>
// Even with threads, these ACE_NULL_SYNCH specializations are necessary.
#if defined (ACE_HAS_THREADS)
  #pragma instantiate ACE_Message_Queue<ACE_NULL_SYNCH>
  #pragma instantiate ACE_Message_Queue_Iterator<ACE_NULL_SYNCH>
  #pragma instantiate ACE_Message_Queue_Reverse_Iterator<ACE_NULL_SYNCH>
  #pragma instantiate ACE_Message_Queue_Factory<ACE_NULL_SYNCH>
  #pragma instantiate ACE_Dynamic_Message_Queue<ACE_NULL_SYNCH>
  #pragma instantiate ACE_Module<ACE_NULL_SYNCH>
  #pragma instantiate ACE_Task<ACE_NULL_SYNCH>
  #pragma instantiate ACE_Thru_Task<ACE_NULL_SYNCH>
#endif /* ACE_HAS_THREADS */
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
