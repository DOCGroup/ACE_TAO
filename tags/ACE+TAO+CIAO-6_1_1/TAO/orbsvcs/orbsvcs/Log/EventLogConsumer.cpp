// $Id$

#include "orbsvcs/Log/EventLog_i.h"
#include "orbsvcs/Log/LogMgr_i.h"
#include "orbsvcs/Log/LogNotification.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Event_LogConsumer::TAO_Event_LogConsumer (TAO_EventLog_i *log)
: log_ (log)
{
}

TAO_Event_LogConsumer::~TAO_Event_LogConsumer (void)
{
}

void
TAO_Event_LogConsumer::connect (CosEventChannelAdmin::ConsumerAdmin_ptr consumer_admin)
{
  // Connect to the event channel.
  CosEventComm::PushConsumer_var myself = this->_this ();
  this->supplier_proxy_ = consumer_admin->obtain_push_supplier ();
  this->supplier_proxy_->connect_push_consumer (myself.in());
}

void
TAO_Event_LogConsumer::push (const CORBA::Any& data)
{
  // create a record list...
  DsLogAdmin::RecordList recList (1);
  recList.length (1);

  recList [0].info = data;

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
TAO_Event_LogConsumer::disconnect_push_consumer (void)
{
  this->supplier_proxy_->disconnect_push_supplier ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
