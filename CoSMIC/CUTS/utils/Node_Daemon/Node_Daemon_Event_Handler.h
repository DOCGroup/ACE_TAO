/* -*- C++ -*- */

//=============================================================================
/**
 * @file      Node_Daemon_Event_Handler.h
 *
 * $Id$
 *
 * @author    James H. Hill
 */
//=============================================================================

#ifndef _NODE_DAEMON_EVENT_HANDLER_H_
#define _NODE_DAEMON_EVENT_HANDLER_H_

#include "ace/Event_Handler.h"
#include "ace/Task.h"
#include "ace/Reactor.h"

namespace CUTS
{
  // Forward decl.
  class Node_Daemon_i;
}

//=============================================================================
/**
 * @class Node_Daemon_Event_Handler
 */
//=============================================================================

class Node_Daemon_Event_Handler : public ACE_Task_Base
{
public:
  Node_Daemon_Event_Handler (::CUTS::Node_Daemon_i * daemon);

  virtual ~Node_Daemon_Event_Handler (void);

  bool activate (void);

  void deactivate (void);

private:
  int handle_exit (ACE_Process * process);

  int svc (void);

  ::CUTS::Node_Daemon_i * daemon_;

  ACE_Reactor reactor_;

  bool active_;
};

#endif  /* !defined _NODE_DAEMON_EVENT_HANDLER_H_ */
