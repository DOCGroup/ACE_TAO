// -*- C++ -*-

//=============================================================================
/**
 *  @file   ConnectionAcceptHandler.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================
#ifndef CONNECTIONACCEPTHANDLER_H
#define CONNECTIONACCEPTHANDLER_H

#include "ace/Event_Handler.h"
#include "ace/Svc_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template <ACE_PEER_STREAM_1>
class ConnectionAcceptHandler : public ACE_Svc_Handler <ACE_PEER_STREAM_2, ACE_NULL_SYNCH>
{
public:
  virtual int open (void *);
  virtual void destroy (void);
  virtual int close (u_long flags = 0);
protected:

  virtual int handle_input (ACE_HANDLE);
  virtual int handle_close (ACE_HANDLE,
                            ACE_Reactor_Mask);
  ~ConnectionAcceptHandler(){}
private:
   ACE_Reactor* reactor_;
};

class TAO_FTEC_Fault_Listener;

template <ACE_PEER_STREAM_1>
class ConnectionDetectHandler : public ACE_Svc_Handler <ACE_PEER_STREAM_2, ACE_NULL_SYNCH>
{
public:
  ConnectionDetectHandler(TAO_FTEC_Fault_Listener* listener = NULL)
    : listener_(listener){}

    virtual int close (u_long flags = 0);

    virtual int handle_close (ACE_HANDLE,
                              ACE_Reactor_Mask);
private:
  TAO_FTEC_Fault_Listener* listener_;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ConnectionHandler_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ConnectionHandler_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif

