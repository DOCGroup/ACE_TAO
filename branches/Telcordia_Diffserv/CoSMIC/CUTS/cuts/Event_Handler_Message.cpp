// $Id$

#include "cuts/Event_Handler_Message.h"

#if !defined (__CUTS_INLINE__)
#include "cuts/Event_Handler_Message.inl"
#endif

#include "ace/OS_NS_sys_time.h"
#include "ace/Log_Msg.h"

//
// CUTS_Event_Handler_Message
//
CUTS_Event_Handler_Message::CUTS_Event_Handler_Message (
  long sender,
  long dispatch)
: sender_ (sender),
  creation_time_ (ACE_OS::gettimeofday ())
{
  this->dispatch_time_.msec (dispatch);
}

//
// CUTS_Event_Handler_Message
//
CUTS_Event_Handler_Message::CUTS_Event_Handler_Message (
  const CUTS_Event_Handler_Message & message)
: sender_ (message.sender_),
  dispatch_time_ (message.dispatch_time_),
  creation_time_ (message.creation_time_)
{

}

//
// ~CUTS_Event_Handler_Message
//
CUTS_Event_Handler_Message::~CUTS_Event_Handler_Message (void)
{

}

//
// operator =
//
const CUTS_Event_Handler_Message &
CUTS_Event_Handler_Message::operator = (
  const CUTS_Event_Handler_Message & message)
{
  this->sender_ = message.sender_;
  this->dispatch_time_ = message.dispatch_time_;
  this->creation_time_ = message.creation_time_;
  return *this;
}
