// $Id$
#include "Admin.h"

#if ! defined (__ACE_INLINE__)
#include "Admin.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (Notify,
           TAO_Notify_Admin,
           "$Id$")

#include "orbsvcs/CosNotifyChannelAdminC.h"

#include "Container_T.h"
#include "Proxy.h"
#include "EventChannel.h"
#include "Topology_Saver.h"
#include "Save_Persist_Worker_T.h"
#include "Reconnect_Worker_T.h"
#include "tao/debug.h"
//#define DEBUG_LEVEL 9
#ifndef DEBUG_LEVEL
# define DEBUG_LEVEL TAO_debug_level
#endif //DEBUG_LEVEL

TAO_Notify_Admin::TAO_Notify_Admin ()
  : ec_ (0)
  , proxy_container_ (0)
  , filter_operator_ (CosNotifyChannelAdmin::OR_OP)
  , is_default_ (false)
{
  // Initialize all Admin objects to initially be subscribed for all events.
  // This is a reasonable default and is required to allow Cos Event consumers/suppliers to send/receive events,
  this->subscribed_types_.insert (TAO_Notify_EventType::special ());
}

TAO_Notify_Admin::~TAO_Notify_Admin ()
{
  this->ec_->_decr_refcnt ();
}

void
TAO_Notify_Admin::init (TAO_Notify_EventChannel *ec ACE_ENV_ARG_DECL)
{
  this->ec_ = ec;

  this->ec_->_incr_refcnt ();

  // this-> on the following line confuses VC6
  TAO_Notify::Topology_Parent::init (ec ACE_ENV_ARG_PARAMETER);

  ACE_NEW_THROW_EX (this->proxy_container_,
                    TAO_Notify_Proxy_Container (),
                    CORBA::INTERNAL ());
  ACE_CHECK;

  this->proxy_container_->init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

}

void
TAO_Notify_Admin::remove (TAO_Notify_Proxy* proxy ACE_ENV_ARG_DECL)
{
  this->proxy_container_->remove (proxy ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Notify_Admin::subscribed_types (TAO_Notify_EventTypeSeq& subscribed_types ACE_ENV_ARG_DECL)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Adopt the Admin's subscription.
  TAO_Notify_EventTypeSeq added (this->subscribed_types_), removed;

  added.init (subscribed_types, removed);

  subscribed_types = added;
}

int
TAO_Notify_Admin::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  if (TAO_Notify_Object::shutdown (ACE_ENV_SINGLE_ARG_PARAMETER) == 1)
    return 1;

  this->proxy_container_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (1);

  delete this->proxy_container_;

  return 0;
}

void
TAO_Notify_Admin::insert (TAO_Notify_Proxy* proxy ACE_ENV_ARG_DECL)
{
  this->proxy_container_->insert (proxy ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_Admin::save_persistent (TAO_Notify::Topology_Saver& saver ACE_ENV_ARG_DECL)
{
  bool changed = this->children_changed_;
  this->children_changed_ = false;
  this->self_changed_ = false;

  if (is_persistent ())
  {
    TAO_Notify::NVPList attrs;
    this->save_attrs(attrs);

    const char* type = this->get_admin_type_name();

    bool want_all_children =
      saver.begin_object(this->id(), type, attrs, changed ACE_ENV_ARG_PARAMETER);
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

    TAO_Notify::Save_Persist_Worker<TAO_Notify_Proxy> wrk(saver, want_all_children);
    this->proxy_container_->collection()->for_each(&wrk ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    saver.end_object(this->id(), type ACE_ENV_ARG_PARAMETER);
  }
}

void
TAO_Notify_Admin::save_attrs (TAO_Notify::NVPList& attrs)
{
  TAO_Notify_Object::save_attrs(attrs);
  attrs.push_back(TAO_Notify::NVP("InterFilterGroupOperator", this->filter_operator_));
  if (this->is_default_)
    {
      attrs.push_back (TAO_Notify::NVP ("default", "yes"));
    }
}

void
TAO_Notify_Admin::load_attrs(const TAO_Notify::NVPList& attrs)
{
  TAO_Notify_Object::load_attrs (attrs);
  const char * value = 0;
  if (attrs.find ("InterFilterGroupOperator", value))
    {
      this->filter_operator_ = static_cast <CosNotifyChannelAdmin::InterFilterGroupOperator> (ACE_OS::atoi (value));
    }
  if (attrs.find ("default", value))
  {
    this->is_default_ = (ACE_OS::strcmp (value, "yes") == 0);
  }
}

TAO_Notify::Topology_Object*
TAO_Notify_Admin::load_child (const ACE_CString &type,
  CORBA::Long id, const TAO_Notify::NVPList& attrs ACE_ENV_ARG_DECL_NOT_USED)
{
  ACE_UNUSED_ARG (attrs);
  TAO_Notify::Topology_Object* result = this;
  if (type == "subscriptions")
  {
    if (DEBUG_LEVEL) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Admin reload subscription %d\n")
      , ACE_static_cast (int, id)
      ));
    // since we initialized our subscribed types to everything
    // in the constructor. we have to clear it out first.
    this->subscribed_types_.reset();
    result = &this->subscribed_types_;
  }
  else if (type == "filter_admin")
  {
    if (DEBUG_LEVEL) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Admin reload filter_admin %d\n")
      , ACE_static_cast (int, id)
      ));
    result = & this->filter_admin_;
  }
  return result;
}

void
TAO_Notify_Admin::reconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_Notify::Reconnect_Worker<TAO_Notify_Proxy> wrk;
  this->proxy_container_->collection()->for_each(&wrk ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_ESF_Shutdown_Proxy<TAO_Notify_Proxy>;
template class TAO_Notify_Container_T <TAO_Notify_Proxy>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma TAO_ESF_Shutdown_Proxy<TAO_Notify_Proxy>
#pragma instantiate TAO_Notify_Container_T <TAO_Notify_Proxy>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
