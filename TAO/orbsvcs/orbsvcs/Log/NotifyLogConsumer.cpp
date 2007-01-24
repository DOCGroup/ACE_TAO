#include "orbsvcs/Log/NotifyLog_i.h"
#include "orbsvcs/Log/LogMgr_i.h"
#include "orbsvcs/Log/LogNotification.h"

ACE_RCSID (Log,
           NotifyLogConsumer,
           "$Id$")

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
  CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin
  ACE_ENV_ARG_DECL)
{
  // Activate the consumer with the default_POA_
  CosNotifyComm::PushConsumer_var objref =
    this->_this ();

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (CosNotifyChannelAdmin::ANY_EVENT, proxy_supplier_id_ ACE_ENV_ARG_PARAMETER);

  ACE_ASSERT (!CORBA::is_nil (proxysupplier.in ()));

  // narrow
  this->proxy_supplier_ =
    CosNotifyChannelAdmin::ProxyPushSupplier::
    _narrow (proxysupplier.in () ACE_ENV_ARG_PARAMETER);

  ACE_ASSERT (!CORBA::is_nil (proxy_supplier_.in ()));

  proxy_supplier_->connect_any_push_consumer (objref.in ()
                                              ACE_ENV_ARG_PARAMETER);

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
    const CosNotification::EventTypeSeq & /*removed*/
    ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ))
{
  // No-Op.
}

void
TAO_Notify_LogConsumer::push
   (const CORBA::Any& event
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventComm::Disconnected
                   ))
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
  ACE_TRY
    {
      // log the RecordList.
      this->log_->write_recordlist (recList ACE_ENV_ARG_PARAMETER);
    }
  ACE_CATCH (DsLogAdmin::LogFull, ex)
    {
      ACE_THROW (CORBA::NO_RESOURCES ());
    }
  ACE_CATCH (DsLogAdmin::LogOffDuty, ex)
    {
      ACE_THROW (CORBA::NO_RESOURCES ());
    }
  ACE_CATCH (DsLogAdmin::LogLocked, ex)
    {
      ACE_THROW (CORBA::NO_PERMISSION ());
    }
  ACE_CATCH (DsLogAdmin::LogDisabled, ex)
    {
      ACE_THROW (CORBA::TRANSIENT ());
    }
  ACE_ENDTRY;
}

void
TAO_Notify_LogConsumer::disconnect_push_consumer
   (void)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
