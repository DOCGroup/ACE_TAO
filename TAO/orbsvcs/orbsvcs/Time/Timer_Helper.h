// -*- C++ -*-

//=============================================================================
/**
 * @file Timer_Helper.h
 *
 * $Id$
 *
 * This class is registered with the Reactor and extends from the
 * event handler.It is a friend of the TAO_Time_Service_Clerk and
 * helps to update the clerk's notion of globally synchronized
 * time. This class obviates the need for multiple inheritance in
 * the clerk.
 *
 * @author Vishal Kachroo  <vishal@cs.wustl.edu>
 */
//=============================================================================


#ifndef TIMER_HELPER_H
#define	TIMER_HELPER_H

#include /**/ "ace/pre.h"


#include "ace/Event_Handler.h"
#include "ace/Containers.h"
#include "time_export.h"

#include "orbsvcs/TimeServiceS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Time_Service_Clerk;

/**
 * @class Timer_Helper
 *
 * @brief Timer Helper for the clerk.
 *
 * The handle timeout method of this class is called periodically
 * by the reactor. This method updates the clerk's notion of
 * globally synchronized time by contacting the various Time
 * Servers.
 */
class TAO_Time_Export Timer_Helper : public ACE_Event_Handler
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  Timer_Helper (void);

  /// Destructor.
  ~Timer_Helper (void);

  /// Constructor that sets the clerk.
  Timer_Helper (TAO_Time_Service_Clerk *clerk);

  /// This method is called periodically by the Reactor to update the
  /// clerk's time.
  int handle_timeout (const ACE_Time_Value &time,
		      const void *arg);

protected:
  /// Clerk's instance that this class helps to update time.
  TAO_Time_Service_Clerk *clerk_;

  /// The set of server IORs.
  typedef ACE_Array_Base<CosTime::TimeService_var> IORS;
};

#include /**/ "ace/post.h"

#endif /* TIMER_HELPER_H */
