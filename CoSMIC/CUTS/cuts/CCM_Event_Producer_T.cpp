// $Id$

#include "ace/OS_NS_time.h"

#if !defined (__CUTS_INLINE__)
#include "cuts/CCM_Event_Producer_T.inl"
#endif

#include "ace/Reactor.h"
#include "ace/TP_Reactor.h"
#include "ace/streams.h"

//
// CUTS_CCM_Event_Producer_T
//
template <typename CONTEXT>
CUTS_CCM_Event_Producer_T <CONTEXT>::CUTS_CCM_Event_Producer_T (void)
: context_ (0),
  owner_ (-1),
  active_ (false)
{

}

//
// ~CUTS_CCM_Event_Producer_T
//
template <typename CONTEXT>
CUTS_CCM_Event_Producer_T <CONTEXT>::~CUTS_CCM_Event_Producer_T (void)
{

}

//
// push_event
//
template <typename CONTEXT>
template <typename OBV_EVENTTYPE, typename EVENTTYPE>
void CUTS_CCM_Event_Producer_T <CONTEXT>::push_event (
  void (CONTEXT::*event_method)(EVENTTYPE *),
  ACE_Time_Value & toc)
{
  // Creata new event and let the extended version of the
  // event publication handle the rest.
  typename OBV_EVENTTYPE::_var_type event = new OBV_EVENTTYPE ();

  this->push_event_ex (event_method, event.in (), toc);
}

//
// push_event_ex
//
template <typename CONTEXT>
template <typename EVENTTYPE>
void CUTS_CCM_Event_Producer_T <CONTEXT>::push_event_ex (
  void (CONTEXT::*event_method)(EVENTTYPE *),
  EVENTTYPE * event,
  ACE_Time_Value & toc)
{
  // Set the sender of the event and set the time of completion
  // for the publication for client.
  event->sender (this->owner_);
  toc = ACE_OS::gettimeofday ();

  if (this->active_)
  {
    try
    {
      (this->context_->*event_method) (event);
    }
    catch (const CORBA::Exception & ex)
    {
      ACE_ERROR ((LM_ERROR,
                  "[%M] -%T - exception %s has occurred\n",
                  ex._name ()));
    }
    catch (...)
    {
      ACE_ERROR ((LM_ERROR,
                  "[%M] -%T - unknown exception has occurred\n"));
    }
  }
  else
  {
    ACE_ERROR ((LM_INFO,
                "[%M] -%T - event producer is not active\n"));
  }
}

//
// push_data_event
//
template <typename CONTEXT>
template <typename OBV_EVENTTYPE, typename EVENTTYPE>
void CUTS_CCM_Event_Producer_T <CONTEXT>::push_data_event (
  size_t size,
  void (CONTEXT::*event_method)(EVENTTYPE *),
  ACE_Time_Value & toc)
{
  // Create a new event
  typename OBV_EVENTTYPE::_var_type event = new OBV_EVENTTYPE ();

  // Allocate a buffer of that size and fill it with garbage.
  char value = 'A';
  char * buffer = ::CORBA::string_alloc (size);
  std::fill (buffer, buffer + size - 1, value);
  buffer[size - 1] = '\0';

  // Initialize the buffer with garbage data.
  event->data (buffer);

  // Pass control to the extended method which will do the publication
  // of the event for us.
  this->push_event_ex (event_method, event.in (), toc);
}
