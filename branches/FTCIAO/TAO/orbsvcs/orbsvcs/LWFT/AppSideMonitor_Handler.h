// -*- C++ -*-
// $Id$

/**
 *  @file  C++ Interface: AppSideMonitor_Handler
 *
 *  @brief Declares interface for AppSideMonitor_Handler.
 *
 */

#ifndef __APPSIDEMONITOR_HANDLER_H_
#define __APPSIDEMONITOR_HANDLER_H_

#include "ace/Svc_Handler.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Acceptor.h"

/**
 *  @class  AppSideMonitor_Handler
 *
 *  @brief Encapsulates AppSideMonitor_Handler
 */

class AppSideMonitor_Handler : public ACE_Svc_Handler <ACE_SOCK_Acceptor::PEER_STREAM, ACE_NULL_SYNCH>
{
  public:

    typedef ACE_Svc_Handler <ACE_SOCK_Acceptor::PEER_STREAM, ACE_NULL_SYNCH> super;
    typedef ACE_Acceptor <AppSideMonitor_Handler, ACE_SOCK_Acceptor> FactoryAcceptor;

  //private:
    AppSideMonitor_Handler ();
  public:

    virtual int handle_input (ACE_HANDLE fd);
    virtual int open (void *factory);

  private:
    FactoryAcceptor *acceptor_;
};

#endif /// __APPSIDEMONITOR_HANDLER_H_

