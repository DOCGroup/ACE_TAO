// QoS_Session_Factory.cpp
// $Id$

#include "ace/QoS/QoS_Session_Factory.h"
#include "ace/QoS/QoS_Session_Impl.h"

#ifdef ACE_SUBSET_0
#include "ace/Logging/Log_Msg.h"
#endif

ACE_RCSID(ace, QoS_Session_Factory, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_QoS_Session_Factory)

ACE_QoS_Session_Factory::ACE_QoS_Session_Factory (void)
{
  ACE_TRACE ("ACE_QoS_Session_Factory::ACE_QoS_Session_Factory");
}

ACE_QoS_Session_Factory::~ACE_QoS_Session_Factory (void)
{
  ACE_TRACE ("ACE_QoS_Session_Factory::~ACE_QoS_Session_Factory");
}

// Create a QoS session of the given type (RAPI or GQoS).
ACE_QoS_Session *
ACE_QoS_Session_Factory::create_session (ACE_QoS_Session_Type qos_session_type)
{

  ACE_QoS_Session * qos_session = 0;

#if defined (ACE_HAS_RAPI)
  if (qos_session_type == ACE_RAPI_SESSION)
    ACE_NEW_RETURN (qos_session,
                    ACE_RAPI_Session,
                    0);
#endif /* ACE_HAS_RAPI */

  if (qos_session_type == ACE_GQOS_SESSION)
    ACE_NEW_RETURN (qos_session,
                    ACE_GQoS_Session,
                    0);

#ifdef ACE_SUBSET_0
  if (this->add_session (qos_session) == -1)
    {
      delete qos_session;
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_LIB_TEXT ("Error in adding session\n")),
                        0);
    }
#else
  this->add_session (qos_session);
#endif
  return qos_session;
}

// Destroy the QoS Session.
int
ACE_QoS_Session_Factory::destroy_session (ACE_QoS_Session *qos_session)
{

#ifdef ACE_SUBSET_0
  if ((qos_session != 0) && (this->remove_session (qos_session) == -1))
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("Error in destroying session\n")),
                      -1);
#else
  if (qos_session != 0) {
    this->remove_session(qos_session);
  }
#endif
  return 0;
}

// Add a session to the set of sessions created by this factory. This is a
// private method called by the create_session ().
int
ACE_QoS_Session_Factory::add_session (ACE_QoS_Session *qos_session)
{
#ifdef ACE_SUBSET_0
  if (this->qos_session_set_.insert (qos_session) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("Error in adding a new session")
                       ACE_LIB_TEXT ("to the session set\n")),
                      -1);
#else
  this->qos_session_set_.insert (qos_session);
#endif
  return 0;
}

// Remove a session from the set of sessions created by this factory. This is
// a private method called by the destroy_session ().
int
ACE_QoS_Session_Factory::remove_session (ACE_QoS_Session *qos_session)
{
#ifdef ACE_SUBSET_0
  if (this->qos_session_set_.remove (qos_session) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_LIB_TEXT ("Error in removing a session")
                       ACE_LIB_TEXT ("from the session set\n")),
                      -1);
#else
  this->qos_session_set_.remove (qos_session);
#endif
  return 0;
}
