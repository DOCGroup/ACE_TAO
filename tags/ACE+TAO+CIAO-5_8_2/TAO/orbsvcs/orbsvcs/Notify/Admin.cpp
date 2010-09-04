// $Id$
#include "orbsvcs/Notify/Admin.h"
#include "orbsvcs/Notify/Validate_Worker_T.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Admin.inl"
#endif /* __ACE_INLINE__ */

#include "orbsvcs/CosNotifyChannelAdminC.h"

#include "orbsvcs/Notify/Container_T.h"
#include "orbsvcs/Notify/Proxy.h"
#include "orbsvcs/Notify/EventChannel.h"
#include "orbsvcs/Notify/Topology_Saver.h"
#include "orbsvcs/Notify/Save_Persist_Worker_T.h"
#include "orbsvcs/Notify/Reconnect_Worker_T.h"
#include "orbsvcs/Notify/Consumer_Map.h"
#include "orbsvcs/Notify/Supplier_Map.h"
#include "orbsvcs/Notify/ConsumerAdmin.h"
#include "orbsvcs/Notify/SupplierAdmin.h"

#include "tao/debug.h"

//#define DEBUG_LEVEL 9
#ifndef DEBUG_LEVEL
# define DEBUG_LEVEL TAO_debug_level
#endif //DEBUG_LEVEL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_Admin::TAO_Notify_Admin ()
  : ec_ (0)
  , filter_operator_ (CosNotifyChannelAdmin::OR_OP)
  , is_default_ (false)
{
  // Initialize all Admin objects to initially be subscribed for all
  // events.  This is a reasonable default and is required to allow
  // Cos Event consumers/suppliers to send/receive events,
  this->subscribed_types_.insert (TAO_Notify_EventType::special ());
}

TAO_Notify_Admin::~TAO_Notify_Admin ()
{
}

void
TAO_Notify_Admin::init (TAO_Notify::Topology_Parent* parent)
{
  ACE_ASSERT (this->ec_.get() == 0);

  this->ec_.reset (dynamic_cast<TAO_Notify_EventChannel *>(parent));
  ACE_ASSERT (this->ec_.get() != 0);

  filter_admin_.event_channel (this->ec_.get ());

  // this-> on the following line confuses VC6
  initialize (parent);

  TAO_Notify_Proxy_Container* proxy_container = 0;
  ACE_NEW_THROW_EX (proxy_container,
                    TAO_Notify_Proxy_Container (),
                    CORBA::INTERNAL ());
  this->proxy_container_.reset (proxy_container);

  this->proxy_container().init ();

}

void
TAO_Notify_Admin::cleanup_proxy (TAO_Notify_Proxy *proxy,
                                 bool is_supplier,
                                 bool experienced_timeout)
{
  // We must clean up the proxy before calling remove.  Doing it in the
  // opposite order causes us to use invalid memory (through the call of
  // id() into the proxy).
  ec_->cleanup_proxy (proxy->id(), is_supplier, experienced_timeout);

  this->remove (proxy);
}

void
TAO_Notify_Admin::remove (TAO_Notify_Proxy* proxy)
{
  this->proxy_container().remove (proxy);
}

void
TAO_Notify_Admin::subscribed_types (TAO_Notify_EventTypeSeq& subscribed_types)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());

  // Adopt the Admin's subscription.
  TAO_Notify_EventTypeSeq added (this->subscribed_types_), removed;

  added.add_and_remove (subscribed_types, removed);

  subscribed_types = added;
}

int
TAO_Notify_Admin::shutdown (void)
{
  if (TAO_Notify_Object::shutdown () == 1)
    return 1;

  this->proxy_container().shutdown ();

  return 0;
}

void
TAO_Notify_Admin::insert (TAO_Notify_Proxy* proxy)
{
  this->proxy_container().insert (proxy);
}

void
TAO_Notify_Admin::save_persistent (TAO_Notify::Topology_Saver& saver)
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
        saver.begin_object(this->id(), type, attrs, changed);

      if (want_all_children || this->filter_admin_.is_changed ())
        {
          this->filter_admin_.save_persistent(saver);
        }
      if (want_all_children || this->subscribed_types_.is_changed ())
        {
          this->subscribed_types_.save_persistent(saver);
        }

      TAO_Notify::Save_Persist_Worker<TAO_Notify_Proxy>
        wrk(saver, want_all_children);
      this->proxy_container().collection()->for_each(&wrk);

      saver.end_object(this->id(), type);
    }
}

void
TAO_Notify_Admin::save_attrs (TAO_Notify::NVPList& attrs)
{
  TAO_Notify_Object::save_attrs(attrs);
  attrs.push_back(TAO_Notify::NVP("InterFilterGroupOperator",
                                  this->filter_operator_));
  if (this->is_default_)
    {
      attrs.push_back (TAO_Notify::NVP ("default", "yes"));
    }
}

void
TAO_Notify_Admin::load_attrs(const TAO_Notify::NVPList& attrs)
{
  TAO_Notify_Object::load_attrs (attrs);
  const char* value = 0;
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
                              CORBA::Long id,
                              const TAO_Notify::NVPList& attrs)
{
  ACE_UNUSED_ARG (attrs);
  TAO_Notify::Topology_Object* result = this;
  if (type == "subscriptions")
    {
      if (DEBUG_LEVEL)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) Admin reload subscription %d\n"),
                    static_cast<int> (id)
                    ));
      // since we initialized our subscribed types to everything
      // in the constructor. we have to clear it out first.
      this->subscribed_types_.reset();
      result = &this->subscribed_types_;
    }
  else if (type == "filter_admin")
    {
      if (DEBUG_LEVEL)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) Admin reload filter_admin %d\n"),
                    static_cast<int> (id)
                    ));
      result = & this->filter_admin_;
    }
  return result;
}

void
TAO_Notify_Admin::reconnect (void)
{
  TAO_Notify::Reconnect_Worker<TAO_Notify_Proxy> wrk;
  this->proxy_container().collection()->for_each(&wrk);
}

void
TAO_Notify_Admin::validate ()
{
  TAO_Notify::Validate_Worker<TAO_Notify_Proxy> wrk;
  this->proxy_container().collection()->for_each(&wrk);
}



TAO_END_VERSIONED_NAMESPACE_DECL
