// file      : Gate.h
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
#ifndef GATE_H
#define GATE_H

#include "ace/INET_Addr.h"
#include "ace/UUID.h"

#include "ace/Thread_Mutex.h"
#include "ace/Thread_Manager.h"

#include "tao/corba.h"

#include "orbsvcs/CosNotificationC.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNotifyCommS.h"

#include "ace/RMCast/Socket.h"

#include "Export.h"

class Gate_Export Gate : public POA_CosNotifyComm::StructuredPushConsumer
{
public:
  ~Gate () override;

  Gate (ACE_INET_Addr const& group,
        CosNotifyChannelAdmin::EventChannel_ptr ch);

  Gate (ACE_INET_Addr const& group,
        CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin,
        CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin);

  Gate (Gate const&) = delete;
  Gate& operator= (Gate const&) = delete;
  Gate (Gate&&) = delete;
  Gate& operator= (Gate&&) = delete;

private:
  void
  init (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin,
        CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin);

  static ACE_THR_FUNC_RETURN
  tracker_thunk (void* arg);

  void
  tracker ();

private:
  // NotifyPublish interface.
  //
  void
  offer_change (CosNotification::EventTypeSeq const&,
                CosNotification::EventTypeSeq const&) override;

  // StructuredPushSupplier interface.
  //
  void
  push_structured_event (CosNotification::StructuredEvent const& e) override;


  void
  disconnect_structured_push_consumer () override;

private:
  ACE_thread_t thread_;

  CosNotifyChannelAdmin::ProxyID consumer_id_;
  CosNotifyChannelAdmin::StructuredProxyPushConsumer_var consumer_;

  CosNotifyChannelAdmin::ProxyID supplier_id_;
  CosNotifyChannelAdmin::StructuredProxyPushSupplier_var supplier_;

  ACE_RMCast::Socket socket_;
  CORBA::String_var id_;

  bool stop_;
  TAO_SYNCH_MUTEX mutex_;
  ACE_Thread_Manager thread_mgr_;
};

#endif // GATE_H
