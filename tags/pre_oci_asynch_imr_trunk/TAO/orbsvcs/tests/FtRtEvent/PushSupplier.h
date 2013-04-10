// -*- C++ -*-
//=============================================================================
/**
 *  @file   PushSupplier.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef PUSHSUPPLIER_H
#define PUSHSUPPLIER_H

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "ace/Event_Handler.h"
#include "ace/Task.h"
#include "ace/Reactor.h"
#include "ace/Time_Value.h"

class PushSupplier_impl :
   public virtual ACE_Event_Handler
 , public virtual POA_RtecEventComm::PushSupplier
{
public:
  PushSupplier_impl(CORBA::ORB_ptr orb);
  ~PushSupplier_impl();

  int init(RtecEventChannelAdmin::EventChannel_ptr);

    virtual void  disconnect_push_supplier (
      );
private:

  class ReactorTask : public ACE_Task_Base
  {
  public:
    // ctor
    ReactorTask(ACE_Event_Handler* handler) : reactor_(0), handler_(handler){}
    virtual int svc (void);
    // The thread entry point.

    ACE_Reactor* reactor_;
    ACE_Event_Handler* handler_;
  };

    virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *act = 0);
  CORBA::ORB_var orb_;
  CORBA::ULong seq_no_;
  ReactorTask reactor_task_;
  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_;
  PushSupplier_impl(const PushSupplier_impl&);
  bool operator==(const PushSupplier_impl&);
};
#endif
