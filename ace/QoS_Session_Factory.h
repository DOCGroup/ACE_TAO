/* -*- C++ -*- */
// $Id$

// ===========================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    QoS_Session_Factory.h
//
// = AUTHOR
//    Vishal Kachroo <vishal@cs.wustl.edu>
//
// ===========================================================================

#ifndef ACE_QOS_SESSION_FACTORY_H
#define ACE_QOS_SESSION_FACTORY_H

#include "ace/QoS_Session.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Containers_T.h"

class ACE_QoS_Session;
// Forward declare this, so the factory uses only references to this.

class ACE_Export ACE_QoS_Session_Factory
{
  // = TITLE
  //    Concrete factory for the QoS Session objects.
  //
  // = DESCRIPTION
  //    This class manages the life cycle of QoS Session objects. These 
  //    objects are currently either RAPI session objects or GQoS session 
  //    objects. It stores the sessions in an unbounded set.

public :
  
  // = Initialization and termination methods.
  ACE_QoS_Session_Factory (void);
  // Default constructor.
  
  ~ACE_QoS_Session_Factory (void);
  // Default destructor.

  enum ACE_QoS_Session_Type
  {
    ACE_RAPI_SESSION,
    ACE_GQOS_SESSION
  };
  // Types of sessions for this factory to manage.

  ACE_QoS_Session * create_session (ACE_QoS_Session_Type qos_session_type);
  // Create a QoS session of the given type (RAPI or GQoS).

  int destroy_session (ACE_QoS_Session *qos_session);
  // Destroy the QoS Session.

private:

  int add_session (ACE_QoS_Session *qos_session);
  // Used by the create_session () to add new sessions to the 
  // set of sessions created by this factory.

  int remove_session (ACE_QoS_Session *qos_session);
  // Used by the destroy_session () to remove a session from the set
  // of sessions created by this factory.

  typedef ACE_Unbounded_Set <ACE_QoS_Session *> QOS_SESSION_SET;
  QOS_SESSION_SET qos_session_set_;
  // Unordered set of QoS Sessions.

};

#endif /* ACE_QOS_SESSION_FACTORY_H */









