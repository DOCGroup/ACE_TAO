/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE_wrappers/tests/
//
// = FILENAME
//    QOS_Event_Handler.cpp
//
// = AUTHOR
//    Vishal Kachroo
//
// ============================================================================

#include "QOS_Event_Handler.h"

// Constructor.
ACE_QOS_Event_Handler::ACE_QOS_Event_Handler ()
{}

ACE_QOS_Event_Handler::ACE_QOS_Event_Handler (const ACE_SOCK_Dgram_Mcast &dgram_mcast)
{
  this->read_handler_.dgram_mcast (dgram_mcast);
}

// Destructor.
ACE_QOS_Event_Handler::~ACE_QOS_Event_Handler ()
{
}

// Return the handle of the Dgram_Mcast. This method is 
// called internally by the reactor.

ACE_HANDLE
ACE_QOS_Event_Handler::get_handle (void) const
{
  return this->read_handler_.get_handle ();
}

int
ACE_QOS_Event_Handler::handle_qos (ACE_HANDLE)
{

  ACE_DEBUG ((LM_DEBUG,
              "Inside handle_qos ()\nRegistering the Read Handler\n"));

  if (ACE_Reactor::instance ()->register_handler (&this->read_handler_,
                                                  ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in registering the read_handler_\n"),
                      -1);

  return 0;
}
