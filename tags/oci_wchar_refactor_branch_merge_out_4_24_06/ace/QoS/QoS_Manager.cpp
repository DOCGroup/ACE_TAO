// QoS_Manager.cpp
// $Id$

#include "QoS_Manager.h"
#include "ace/Log_Msg.h"

ACE_RCSID(ace, QoS_Manager, "$Id $")

ACE_ALLOC_HOOK_DEFINE(ACE_QOS_MANAGER)

ACE_QoS_Manager::ACE_QoS_Manager (void)
{}

ACE_QoS_Manager::~ACE_QoS_Manager (void)
{}

// Adds the given session to the list of session objects joined by
// this socket.

int
ACE_QoS_Manager::join_qos_session (ACE_QoS_Session *qos_session)
{
  if (this->qos_session_set ().insert (qos_session) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("Error in adding a new session to the ")
                       ACE_LIB_TEXT ("socket session set\n")),
                      -1);
  return 0;
}

// Returns the QoS session set for this socket.

ACE_Unbounded_Set <ACE_QoS_Session *>
ACE_QoS_Manager::qos_session_set (void)
{
  return this->qos_session_set_;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Node<ACE_QoS_Session *>;
template class ACE_Unbounded_Set<ACE_QoS_Session *>;
template class ACE_Unbounded_Set_Iterator<ACE_QoS_Session *>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Node<ACE_QoS_Session *>
#pragma instantiate ACE_Unbounded_Set<ACE_QoS_Session *>
#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_QoS_Session *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
