// -*- C++ -*-

//=============================================================================
/**
 * @file    Event_Handler_Message.h
 *
 * @brief   Defines the message object use by the event handler.
 *
 * $Id$
 *
 * @author  James H. Hill
 */
//=============================================================================

#ifndef _CUTS_EVENT_HANDLER_MESSAGE_H_
#define _CUTS_EVENT_HANDLER_MESSAGE_H_

#include "ace/pre.h"
#include "cuts/config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Time_Value.h"

//=============================================================================
/**
 * @class    CUTS_Event_Handler_Message
 */
//=============================================================================

class CUTS_Event_Handler_Message
{
public:
  CUTS_Event_Handler_Message (long sender, long dispatch);

  CUTS_Event_Handler_Message (const CUTS_Event_Handler_Message & message);

  ~CUTS_Event_Handler_Message (void);

  const CUTS_Event_Handler_Message & operator = (
    const CUTS_Event_Handler_Message & message);

  long sender (void) const;

  const ACE_Time_Value & dispatch_time (void) const;

  const ACE_Time_Value & creation_time (void) const;

private:
  long sender_;

  ACE_Time_Value dispatch_time_;

  ACE_Time_Value creation_time_;
};

#if defined (__CUTS_INLINE__)
#include "cuts/Event_Handler_Message.inl"
#endif

#include "ace/post.h"

#endif  // !defined _CUTS_EVENT_HANDLER_MESSAGE_H_
