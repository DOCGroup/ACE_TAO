// $Id$

#if !defined (__CUTS_INLINE__)
#include "cuts/Event_Handler_T.inl"
#endif

//
// CUTS_Event_Handler_Config_T
//
template <typename COMPONENT, typename EVENTTYPE>
CUTS_Event_Handler_Config_T <COMPONENT, EVENTTYPE>::
CUTS_Event_Handler_Config_T (void)
: component_ (0),
  method_ (0)
{

}

//
// CUTS_Event_Handler_Base_T
//
template <typename COMPONENT, typename EVENTTYPE>
CUTS_Event_Handler_Base_T <COMPONENT, EVENTTYPE>::
CUTS_Event_Handler_Base_T (Config_Type & config)
: active_ (false),
  config_ (config)
{

}

//
// ~CUTS_Event_Handler_Base_T
//
template <typename COMPONENT, typename EVENTTYPE>
CUTS_Event_Handler_Base_T <COMPONENT, EVENTTYPE>::
~CUTS_Event_Handler_Base_T (void)
{
  this->deactivate ();
}

//
// handle_event_i
//
template <typename COMPONENT, typename EVENTTYPE>
void
CUTS_Event_Handler_Base_T <COMPONENT, EVENTTYPE>::
handle_event_i (EVENTTYPE * ev,
                const ACE_Time_Value & queue_time)
{
  // Create a new activation record.
  CUTS_Activation_Record * record =
    this->port_agent ().create_activation_record ();
  record->activate (ev->sender ());

  // Make an upcall to the callback and pass it a record for
  // logging its performance.
  if (!this->config_.dispatch_event (ev, record))
  {
    ACE_ERROR ((LM_ERROR,
                "[%M] -%T - failed to dispatch an event\n"));
  }

  // Store the <queueing_time> for the event in the record.
  record->close ();
  record->transit_time (queue_time);

  // Close the activation record.
  this->port_agent ().destroy_activation_record (record);
}

//
// activate
//
template <typename COMPONENT, typename EVENTTYPE>
void CUTS_Event_Handler_Base_T <COMPONENT, EVENTTYPE>::activate (void)
{
  this->active_ = true;
  this->port_agent ().activate ();
}

//
// deactivate
//
template <typename COMPONENT, typename EVENTTYPE>
void CUTS_Event_Handler_Base_T <COMPONENT, EVENTTYPE>::deactivate (void)
{
  this->active_ = false;
  this->port_agent ().deactivate ();
}

