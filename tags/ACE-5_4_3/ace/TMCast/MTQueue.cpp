// file      : TMCast/MTQueue.cpp
// author    : Steve Huston  <shuston@riverace.com>
// cvs-id    : $Id$

#include "LinkListener.hpp"
#include "MTQueue.hpp"

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Node<ACE_Refcounted_Auto_Ptr<TMCast::Message,ACE_Null_Mutex> >;
template class ACE_Unbounded_Queue<ACE_Refcounted_Auto_Ptr<TMCast::Message,ACE_Null_Mutex> >;
template class ACE_Unbounded_Queue_Iterator<ACE_Refcounted_Auto_Ptr<TMCast::Message,ACE_Null_Mutex> >;
template class ACE_Node<ACE_Condition<ACE_Thread_Mutex>* >;
template class ACE_Unbounded_Set<ACE_Condition<ACE_Thread_Mutex>* >;
template class ACE_Unbounded_Set_Iterator<ACE_Condition<ACE_Thread_Mutex> *>;
template class ACE_Unbounded_Set_Const_Iterator<ACE_Condition<ACE_Thread_Mutex> *>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Node<ACE_Refcounted_Auto_Ptr<TMCast::Message,ACE_Null_Mutex> >
#pragma instantiate ACE_Unbounded_Queue<ACE_Refcounted_Auto_Ptr<TMCast::Message,ACE_Null_Mutex> >
#pragma instantiate ACE_Unbounded_Queue_Iterator<ACE_Refcounted_Auto_Ptr<TMCast::Message,ACE_Null_Mutex> >
#pragma instantiate ACE_Node<ACE_Condition<ACE_Thread_Mutex> *>
#pragma instantiate ACE_Unbounded_Set<ACE_Condition<ACE_Thread_Mutex> *>
#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_Condition<ACE_Thread_Mutex> *>
#pragma instantiate ACE_Unbounded_Set_Const_Iterator<ACE_Condition<ACE_Thread_Mutex> *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
