// $Id$

#include "Proxy.h"

#if ! defined (__ACE_INLINE__)
#include "Proxy.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Proxy, "$Id$")

#include "Peer.h"
#include "Proxy.h"
#include "Method_Request_Updates.h"
#include "Worker_Task.h"
#include "Properties.h"
#include "POA_Helper.h"

TAO_NS_Proxy::TAO_NS_Proxy (void)
  :updates_off_ (0)
{
}

TAO_NS_Proxy::~TAO_NS_Proxy ()
{
}

CORBA::Object_ptr
TAO_NS_Proxy::activate (PortableServer::Servant servant ACE_ENV_ARG_DECL)
{
  // Set the POA that we use to return our <ref>
  this->poa_ = this->proxy_poa_;

  return this->proxy_poa_->activate (servant, this->id_ ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_Proxy::deactivate (ACE_ENV_SINGLE_ARG_DECL)
{
  this->proxy_poa_->deactivate (this->id_ ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_Proxy::subscribed_types (TAO_NS_EventTypeSeq& subscribed_types ACE_ENV_ARG_DECL)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                        CORBA::INTERNAL ());
  ACE_CHECK;

  // copy
  subscribed_types = this->subscribed_types_;
}

void
TAO_NS_Proxy::types_changed (const TAO_NS_EventTypeSeq& added, const TAO_NS_EventTypeSeq& removed ACE_ENV_ARG_DECL_NOT_USED)
{
  TAO_NS_Method_Request_Updates request (added, removed, this);

  if (TAO_NS_PROPERTIES::instance()->asynch_updates () == 1) // if we should send the updates synchronously.
    {
      this->worker_task ()->exec (request);
    }
  else // execute in the current thread context.
    {
      ACE_DECLARE_NEW_CORBA_ENV;
      request.execute (ACE_ENV_SINGLE_ARG_PARAMETER);
      }
}

CORBA::Boolean
TAO_NS_Proxy::check_filters (const TAO_NS_Event_var &event
                             , TAO_NS_FilterAdmin& parent_filter_admin
                             , CosNotifyChannelAdmin::InterFilterGroupOperator filter_operator
                             ACE_ENV_ARG_DECL)
{
  // check if it passes the parent filter.
  CORBA::Boolean parent_val =
    parent_filter_admin.match (event ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Boolean val = 0;

  if (filter_operator == CosNotifyChannelAdmin::AND_OP)
    {
      val = parent_val && this->filter_admin_.match (event ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }
  else
    {
      val = parent_val || this->filter_admin_.match (event ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  return val;
}

CosNotification::EventTypeSeq*
TAO_NS_Proxy::obtain_types (CosNotifyChannelAdmin::ObtainInfoMode mode, const TAO_NS_EventTypeSeq& types ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CosNotification::EventTypeSeq_var event_type_seq;

  ACE_NEW_THROW_EX (event_type_seq,
                    CosNotification::EventTypeSeq (),
                    CORBA::NO_MEMORY ());

  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_, CORBA::INTERNAL ());
  ACE_CHECK_RETURN (event_type_seq._retn ());

  if (mode == CosNotifyChannelAdmin::ALL_NOW_UPDATES_OFF ||
      mode == CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON)
    {
      types.populate (event_type_seq);
    }

  if (mode == CosNotifyChannelAdmin::NONE_NOW_UPDATES_ON ||
      mode == CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON)
    {
      this->updates_off_ = 0;
    }
  else
    {
      this->updates_off_ = 1;
    }

  return event_type_seq._retn ();
}

void
TAO_NS_Proxy::qos_changed (const TAO_NS_QoSProperties& qos_properties)
{
  //Inform Peers of qos changes.
  TAO_NS_Peer* peer = this->peer ();

  if (peer != 0)
    peer->qos_changed (qos_properties);
}
