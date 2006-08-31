// $Id$

#if !defined (__CUTS_INLINE__)
#include "cuts/Event_Handler_Manager_T.inl"
#endif

#include "cuts/Async_Event_Handler_T.h"
#include "cuts/Sync_Event_Handler_T.h"

//
// CUTS_Event_Handler_Manager_T
//
template <typename COMPONENT, typename EVENTTYPE>
CUTS_Event_Handler_Manager_T <COMPONENT, EVENTTYPE>::
CUTS_Event_Handler_Manager_T (CUTS_Event_Handler::Event_Mode mode)
: mode_ (CUTS_Event_Handler::UNDEFINED)
{
  this->mode (mode);
}

//
// ~CUTS_Event_Handler_Manager_T
//
template <typename COMPONENT, typename EVENTTYPE>
CUTS_Event_Handler_Manager_T <COMPONENT, EVENTTYPE>::
~CUTS_Event_Handler_Manager_T (void)
{

}

//
// mode
//
template <typename COMPONENT, typename EVENTTYPE>
void CUTS_Event_Handler_Manager_T <COMPONENT, EVENTTYPE>::
mode (CUTS_Event_Handler::Event_Mode mode)
{
  if (this->mode_ == mode)
    return;

  CUTS_Event_Handler_Base_T <COMPONENT, EVENTTYPE> * impl = 0;

  switch (mode)
  {
  case CUTS_Event_Handler::SYNCHRONOUS:
    // Set the implemenation to synchronous mode.
    typedef CUTS_Sync_Event_Handler_T <
      COMPONENT, EVENTTYPE> CUTS_Sync_Event_Handler;

    ACE_NEW (impl, CUTS_Sync_Event_Handler (this->config_));
    break;

  case CUTS_Event_Handler::ASYNCHRONOUS:
    // Set the implemenation to asynchronous mode.
    typedef CUTS_Async_Event_Handler_T <
      COMPONENT, EVENTTYPE> CUTS_Async_Event_Handler;

    ACE_NEW (impl, CUTS_Async_Event_Handler (this->config_));
    break;

  default:
    /* do nothing */
    break;
  }

  // Save the new event handler as the <impl_>.
  this->impl_.reset (impl);
  this->mode_ = mode;
}
