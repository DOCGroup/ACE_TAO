/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE_wrappers/tests/
//
// = FILENAME
//    Read_Handler.h
//
// = AUTHOR
//    Vishal Kachroo
//
// ============================================================================

#ifndef READ_HANDLER_H
#define READ_HANDLER_H

#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/Event_Handler.h"
#include "ace/Reactor.h"

ACE_RCSID(Read_Handler, Read_Handler, "$Id$")

  class ACE_Read_Handler : public ACE_Event_Handler
  {
  public:

    ACE_Read_Handler (void);
    ~ACE_Read_Handler (void);
    // Initialization and Termination methods.

    virtual int handle_input (ACE_HANDLE fd);
    // Handles a READ event.

    virtual ACE_HANDLE get_handle (void) const;
    // Override this to return the handle of the Dgram_Mcast
    // that we are using.

    virtual void dgram_mcast (const ACE_SOCK_Dgram_Mcast &dgram_mcast);
    // Set the dgram_mcast;

  private:

    ACE_SOCK_Dgram_Mcast dgram_mcast_;
    ACE_INET_Addr remote_addr_;
  };

#endif  /* READ_HANDLER_H */
