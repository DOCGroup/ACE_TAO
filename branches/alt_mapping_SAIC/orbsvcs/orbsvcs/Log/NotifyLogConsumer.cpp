// $Id$

#include "orbsvcs/Log/NotifyLog_i.h"
#include "orbsvcs/Log/LogMgr_i.h"
#include "orbsvcs/Log/LogNotification.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_LogConsumer::TAO_Notify_LogConsumer (TAO_NotifyLog_i *log)
: log_ (log)
{
}

TAO_Notify_LogConsumer::~TAO_Notify_LogConsumer (void)
{
}

void
TAO_Notify_LogConsumer::connect (
  CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin)
{
  // Activate the consumer with the default_POA_
  CosNotifyComm::PushConsumer_var objref =
    this->_this ();

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (CosNotifyChannelAdmin::ANY_EVENT, proxy_supplier_id_);

  ACE_ASSERT (!CORBA::is_nil (proxysupplier.in ()));

  // narrow
  this->proxy_supplier_ =
    CosNotifyChannelAdmin::ProxyPushSupplier::
    _narrow (proxysupplier.in ());

  ACE_ASSERT (!CORBA::is_nil (proxy_supplier_.in ()));

  proxy_supplier_->connect_any_push_consumer (objref.in ());

}

void
TAO_Notify_LogConsumer::disconnect (void)
{
  this->proxy_supplier_->
    disconnect_push_supplier();
}

void
TAO_Notify_LogConsumer::offer_change
   (const CosNotification::EventTypeSeq & /*added*/,
    const CosNotification::EventTypeSeq & /*removed*/)
{
  // No-Op.
}

void
TAO_Notify_LogConsumer::push
   (const CORBA::Any& event)
{
  // create a record list...
  DsLogAdmin::RecordList recList (1);
  recList.length (1);

  recList [0].info = event;

  // @@ The current revision of the specification (formal/03-07-01)
  // states:
  //
  // * When a push operation is invoked and a log is full, then a
  //   NO_RESOURCE (sic) SystemException is raised with a LOGFULL
  //   minor code.
  //
  // * When a push operation is invoked on a log that is off-duty,
  //   then a NO_RESOURCE (sic) SystemException is raised with a
  //   LOGOFFDUTY minor code.
  //
  // * When a push operation is invoked on a log that is locked, then
  //   a NO_PERMISSIONS (sic) SystemException is raised with a LOGLOCKED
  //   minor code.
  //
  // * When a push operation is invoked on a log that is disabled,
  //   then a TRANSIENT SystemException is raised with a LOGDISABLED
  //   minor code.
  //
  // But neither the Telecom Logging or the CORBA specification define
  // the values for these minor codes.
  //
  // I have submitted a defect report to the OMG for clarification.
  //    --jtc
  try
    {
      // log the RecordList.
      this->log_->write_recordlist (recList);
    }
  catch (const DsLogAdmin::LogFull& )
    {
      throw CORBA::NO_RESOURCES ();
    }
  catch (const DsLogAdmin::LogOffDuty& )
    {
      throw CORBA::NO_RESOURCES ();
    }
  catch (const DsLogAdmin::LogLocked& )
    {
      throw CORBA::NO_PERMISSION ();
    }
  catch (const DsLogAdmin::LogDisabled& )
    {
      throw CORBA::TRANSIENT ();
    }
}

void
TAO_Notify_LogConsumer::disconnect_push_consumer
   (void)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
