// $Id$

#include "orbsvcs/Notify/Proxy.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Proxy.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_Proxy, "$Id$")

#include "orbsvcs/Notify/Peer.h"
#include "orbsvcs/Notify/Proxy.h"
#include "orbsvcs/Notify/Method_Request_Updates.h"
#include "orbsvcs/Notify/Worker_Task.h"
#include "orbsvcs/Notify/Properties.h"
#include "orbsvcs/Notify/POA_Helper.h"
#include "orbsvcs/Notify/Topology_Saver.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_Proxy::TAO_Notify_Proxy (void)
  : updates_off_ (0)
{
}

TAO_Notify_Proxy::~TAO_Notify_Proxy ()
{
}

CORBA::Object_ptr
TAO_Notify_Proxy::activate (PortableServer::Servant servant ACE_ENV_ARG_DECL)
{
  // Set the POA that we use to return our <ref>
  this->set_primary_as_proxy_poa();
  return TAO_Notify_Object::activate (servant ACE_ENV_ARG_PARAMETER);
}

CORBA::Object_ptr
TAO_Notify_Proxy::activate (PortableServer::Servant servant,
                            CORBA::Long id
                            ACE_ENV_ARG_DECL)
{
  // Set the POA that we use to return our <ref>
  this->set_primary_as_proxy_poa();
  return TAO_Notify_Object::activate (servant, id ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_Proxy::deactivate (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_ASSERT (this->proxy_poa() != 0 );
  this->proxy_poa()->deactivate (this->id() ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_Proxy::subscribed_types (TAO_Notify_EventTypeSeq& subscribed_types ACE_ENV_ARG_DECL)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                        CORBA::INTERNAL ());
  ACE_CHECK;

  // copy
  subscribed_types = this->subscribed_types_;
}

void
TAO_Notify_Proxy::types_changed (const TAO_Notify_EventTypeSeq& added, const TAO_Notify_EventTypeSeq& removed ACE_ENV_ARG_DECL)
{
  // return if the updates for this proxy are turned off or
  // if all the updates in the channel are switched off.
  if (this->updates_off_ == 1 || TAO_Notify_PROPERTIES::instance()->updates () == 0)
    return;

  TAO_Notify_Method_Request_Updates_No_Copy request (added, removed, this);

  if (TAO_Notify_PROPERTIES::instance()->asynch_updates () == 1) // if we should send the updates synchronously.
    {
      this->execute_task (request ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else // execute in the current thread context.
    {
      request.execute (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
}

CosNotification::EventTypeSeq*
TAO_Notify_Proxy::obtain_types (CosNotifyChannelAdmin::ObtainInfoMode mode, const TAO_Notify_EventTypeSeq& types ACE_ENV_ARG_DECL)
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
TAO_Notify_Proxy::qos_changed (const TAO_Notify_QoSProperties& qos_properties)
{
  //Inform Peers of qos changes.
  TAO_Notify_Peer* peer = this->peer ();

  if (peer != 0)
    peer->qos_changed (qos_properties);
}

void
TAO_Notify_Proxy::save_persistent (TAO_Notify::Topology_Saver& saver ACE_ENV_ARG_DECL)
{
  bool changed = this->children_changed_;
  this->children_changed_ = false;
  this->self_changed_ = false;

  if (is_persistent ())
  {
    TAO_Notify::NVPList attrs;
    this->save_attrs(attrs);

    const char * type_name = this->get_proxy_type_name ();
    bool want_all_children = saver.begin_object(this->id(), type_name, attrs, changed ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    if (want_all_children || this->filter_admin_.is_changed ())
    {
      this->filter_admin_.save_persistent(saver ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

    if (want_all_children || this->subscribed_types_.is_changed ())
    {
      this->subscribed_types_.save_persistent(saver ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

    // todo: handle removed children

    saver.end_object(this->id(), type_name ACE_ENV_ARG_PARAMETER);
  }
}

void
TAO_Notify_Proxy::save_attrs (TAO_Notify::NVPList& attrs)
{
  TAO_Notify_Object::save_attrs(attrs);
  TAO_Notify_Peer * peer = this->peer();
  if (peer != 0)
  {
    attrs.push_back (TAO_Notify::NVP("PeerIOR", peer->get_ior()));
  }
}

TAO_Notify::Topology_Object*
TAO_Notify_Proxy::load_child (const ACE_CString &type, CORBA::Long id,
  const TAO_Notify::NVPList& attrs ACE_ENV_ARG_DECL)
{
  ACE_UNUSED_ARG (id);
  ACE_UNUSED_ARG (attrs);
  TAO_Notify::Topology_Object* result = this;
  if (type == "subscriptions")
  {
    // since we initialized our subscribed types to everything
    // in the constructor. we have to clear it out first.
    this->subscribed_types_.reset();
    result = &this->subscribed_types_;
    ACE_CHECK_RETURN(0);
  }
  else if (type == "filter_admin")
  {
    result = & this->filter_admin_;
  }
  return result;
}

TAO_END_VERSIONED_NAMESPACE_DECL
