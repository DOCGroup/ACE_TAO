// -*- C++ -*-
//=============================================================================
/**
 *  @file   PushSupplier.h
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
  explicit PushSupplier_impl (CORBA::ORB_ptr orb);
  PushSupplier_impl (PushSupplier_impl const &) = delete;
  PushSupplier_impl (PushSupplier_impl &&) = delete;
  PushSupplier_impl &operator= (PushSupplier_impl const &) = delete;
  PushSupplier_impl &operator= (PushSupplier_impl &&) = delete;
  ~PushSupplier_impl () override;

  int init (RtecEventChannelAdmin::EventChannel_ptr);

  void disconnect_push_supplier () override;
private:
  class ReactorTask : public ACE_Task_Base
  {
  public:
    explicit ReactorTask (ACE_Event_Handler *handler);
    ReactorTask (ReactorTask const &) = delete;
    ReactorTask (ReactorTask &&) = delete;
    ReactorTask &operator= (ReactorTask const &) = delete;
    ReactorTask &operator= (ReactorTask &&) = delete;

    int svc () override;

    ACE_Reactor *reactor_ {}; // Borrowed from svc() while its event loop runs.
    ACE_Event_Handler *handler_ {}; // Borrowed from the owning PushSupplier_impl.
  };

  int handle_timeout (ACE_Time_Value const &current_time,
                      void const *act = nullptr) override;
  CORBA::ORB_var orb_ {};
  CORBA::ULong seq_no_ {};
  ReactorTask reactor_task_;
  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_ {};
};
#endif
