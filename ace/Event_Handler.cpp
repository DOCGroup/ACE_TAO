// Event_Handler.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Event_Handler.h"
#include "ace/Message_Block.h"

#if !defined (__ACE_INLINE__)
#include "ace/Event_Handler.i"
#endif /* __ACE_INLINE__ */

// Implement conceptually abstract virtual functions in the base class
// so derived classes don't have to implement unused ones.

ACE_Event_Handler::ACE_Event_Handler (void)
  : priority_ (ACE_Event_Handler::LO_PRIORITY),
    reactor_ (0),
    reactorex_ (0),
    proactor_ (0)
{
  ACE_TRACE ("ACE_Event_Handler::ACE_Event_Handler");
}

ACE_Event_Handler::~ACE_Event_Handler (void) 
{
  ACE_TRACE ("ACE_Event_Handler::~ACE_Event_Handler");
}

// Gets the file descriptor associated with this I/O device. 

ACE_HANDLE
ACE_Event_Handler::get_handle (void) const
{
  ACE_TRACE ("ACE_Event_Handler::get_handle");
  return ACE_INVALID_HANDLE;
}

// Sets the file descriptor associated with this I/O device.

void
ACE_Event_Handler::set_handle (ACE_HANDLE)
{
  ACE_TRACE ("ACE_Event_Handler::set_handle");
}

// Gets the priority of this handler. 

int
ACE_Event_Handler::priority (void) const
{
  ACE_TRACE ("ACE_Event_Handler::priority");
  return this->priority_;
}

// Sets the priority 

void
ACE_Event_Handler::priority (int priority)
{
  ACE_TRACE ("ACE_Event_Handler::priority");
  this->priority_ = priority;
}

// Called when the object is about to be removed from the Dispatcher
// tables.

int
ACE_Event_Handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_TRACE ("ACE_Event_Handler::handle_close");
  return -1;
}

// Called when input becomes available on fd. 

int
ACE_Event_Handler::handle_input (ACE_HANDLE)
{
  ACE_TRACE ("ACE_Event_Handler::handle_input");
  return -1;
}

// Called when output is possible on fd.

int 
ACE_Event_Handler::handle_output (ACE_HANDLE)
{
  ACE_TRACE ("ACE_Event_Handler::handle_output");
  return -1;
}

// Called when urgent data is available on fd. 

int 
ACE_Event_Handler::handle_exception (ACE_HANDLE)
{
  ACE_TRACE ("ACE_Event_Handler::handle_exception");
  return -1;
}

// Called when timer expires, TV stores the current time.

int
ACE_Event_Handler::handle_timeout (const ACE_Time_Value &, const void *)
{
  ACE_TRACE ("ACE_Event_Handler::handle_timeout");
  return -1;
}

// Called when a registered signal occurs.

int 
ACE_Event_Handler::handle_signal (int, siginfo_t *, ucontext_t *)
{
  ACE_TRACE ("ACE_Event_Handler::handle_signal");
  return -1;
}

int
ACE_Event_Handler::handle_input_complete (ACE_Message_Block *, long)
{
  ACE_TRACE ("ACE_Event_Handler::handle_input_complete");
  return -1;
}

int
ACE_Event_Handler::handle_output_complete (ACE_Message_Block *, long)
{
  ACE_TRACE ("ACE_Event_Handler::handle_input_complete");
  return -1;
}

ACE_Message_Block *
ACE_Event_Handler::get_message (void)
{
  ACE_TRACE ("ACE_Event_Handler::get_message");
  return 0;
}

void 
ACE_Event_Handler::reactor (ACE_Reactor *reactor)
{
  ACE_TRACE ("ACE_Event_Handler::reactor");
  this->reactor_ = reactor;
}

ACE_Reactor *
ACE_Event_Handler::reactor (void) const
{
  ACE_TRACE ("ACE_Event_Handler::Reactor");
  return this->reactor_;
}

void 
ACE_Event_Handler::reactorEx (ACE_ReactorEx *reactorex)
{
  ACE_TRACE ("ACE_Event_Handler::reactorex");
  this->reactorex_ = reactorex;
}

ACE_ReactorEx *
ACE_Event_Handler::reactorEx (void) const
{
  ACE_TRACE ("ACE_Event_Handler::ReactorEx");

  return this->reactorex_;
}

void 
ACE_Event_Handler::proactor (ACE_Proactor *proactor)
{
  ACE_TRACE ("ACE_Event_Handler::proactor");

  this->proactor_ = proactor;
}

ACE_Proactor *
ACE_Event_Handler::proactor (void) const
{
  ACE_TRACE ("ACE_Event_Handler::Proactor");

  return this->proactor_;
}

ACE_Notification_Buffer::ACE_Notification_Buffer (void) 
{
  ACE_TRACE ("ACE_Notification_Buffer::ACE_Notification_Buffer");
}

ACE_Notification_Buffer::ACE_Notification_Buffer (ACE_Event_Handler *eh,
						  ACE_Reactor_Mask mask)
  : eh_ (eh), 
    mask_ (mask)
{
  ACE_TRACE ("ACE_Notification_Buffer::ACE_Notification_Buffer");
}
