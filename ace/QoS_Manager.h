/* -*- C++ -*- */
// $Id$

//============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    QoS_Manager.h
//
// = AUTHOR
//    Vishal Kachroo
//
//============================================================================

#ifndef ACE_QOS_MANAGER_H
#define ACE_QOS_MANAGER_H
#include "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Addr.h"
#include "ace/IPC_SAP.h"
#include "ace/Containers_T.h"

class ACE_QoS_Session;

class ACE_Export ACE_QoS_Manager
{
  // = TITLE
  //     This class manages the QoS sessions associated with ACE_SOCK.
  //
  // = DESCRIPTION 
  //     This class provides functions to manage the QoS
  //     associated with a socket.  The idea is to keep the management of
  //     QoS for a socket separate from the socket itself. Currently, the
  //     manager is used to manage the QoS session set. It will handle more
  //     responsibilities in the future.

public:
  ACE_QoS_Manager (void);
  ~ACE_QoS_Manager (void);
  // Default ctor/dtor.
  
  int join_qos_session (ACE_QoS_Session *qos_session);
  // Join the given QoS session. A socket can join multiple QoS
  // sessions.  This call adds the given QoS session to the list of
  // QoS sessions that the socket has already joined.

  typedef ACE_Unbounded_Set <ACE_QoS_Session *> ACE_QOS_SESSION_SET;

  ACE_QOS_SESSION_SET qos_session_set (void);
  // Get the QoS session set.

private:

  ACE_QOS_SESSION_SET qos_session_set_;
  // Set of QoS sessions that this socket has joined.
};

#include "ace/post.h"
#endif /* ACE_QOS_MANAGER_H */




