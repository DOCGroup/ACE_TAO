// $Id$

#include "orbsvcs/Notify/EventChannel.h"

#include "orbsvcs/Notify/Container_T.h"
#include "orbsvcs/Notify/EventChannelFactory.h"
#include "orbsvcs/Notify/ConsumerAdmin.h"
#include "orbsvcs/Notify/SupplierAdmin.h"
#include "orbsvcs/Notify/Properties.h"
#include "orbsvcs/Notify/Factory.h"
#include "orbsvcs/Notify/Builder.h"
#include "orbsvcs/Notify/Find_Worker_T.h"
#include "orbsvcs/Notify/Seq_Worker_T.h"
#include "orbsvcs/Notify/Topology_Saver.h"
#include "orbsvcs/Notify/Save_Persist_Worker_T.h"
#include "orbsvcs/Notify/Reconnect_Worker_T.h"
#include "orbsvcs/Notify/Proxy.h"
#include "orbsvcs/Notify/Event_Manager.h"
#include "orbsvcs/Notify/POA_Helper.h"
#include "orbsvcs/Notify/Validate_Worker_T.h"

#include "tao/debug.h"
//#define DEBUG_LEVEL 9
#ifndef DEBUG_LEVEL
# define DEBUG_LEVEL TAO_debug_level
#endif //DEBUG_LEVEL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

typedef TAO_Notify_Find_Worker_T<TAO_Notify_ConsumerAdmin
                             , CosNotifyChannelAdmin::ConsumerAdmin
                             , CosNotifyChannelAdmin::ConsumerAdmin_ptr
                             , CosNotifyChannelAdmin::AdminNotFound>
TAO_Notify_ConsumerAdmin_Find_Worker;

typedef TAO_Notify_Find_Worker_T<TAO_Notify_SupplierAdmin
                             , CosNotifyChannelAdmin::SupplierAdmin
                             , CosNotifyChannelAdmin::SupplierAdmin_ptr
                             , CosNotifyChannelAdmin::AdminNotFound>
TAO_Notify_SupplierAdmin_Find_Worker;

typedef TAO_Notify_Seq_Worker_T<TAO_Notify_ConsumerAdmin> TAO_Notify_ConsumerAdmin_Seq_Worker;
typedef TAO_Notify_Seq_Worker_T<TAO_Notify_SupplierAdmin> TAO_Notify_SupplierAdmin_Seq_Worker;

TAO_Notify_EventChannel::TAO_Notify_EventChannel (void)
  : ecf_ (0)
  , ca_container_ (0)
  , sa_container_ (0)
  , default_filter_factory_ (CosNotifyFilter::FilterFactory::_nil ())
  , default_filter_factory_servant_ (0)
{
}

TAO_Notify_EventChannel::~TAO_Notify_EventChannel ()
{
}

void
TAO_Notify_EventChannel::init (TAO_Notify_EventChannelFactory* ecf
                           , const CosNotification::QoSProperties & initial_qos
                           , const CosNotification::AdminProperties & initial_admin)
{
  ACE_ASSERT (this->ca_container_.get() == 0);

  // this-> on the following line confuses VC6
  initialize (ecf);

  this->ecf_.reset (ecf);

  // Init ca_container_
  TAO_Notify_ConsumerAdmin_Container* ca_container = 0;
  ACE_NEW_THROW_EX (ca_container,
                    TAO_Notify_ConsumerAdmin_Container (),
                    CORBA::INTERNAL ());
  this->ca_container_.reset (ca_container);

  this->ca_container().init ();

  // Init ca_container_
  TAO_Notify_SupplierAdmin_Container* sa_container = 0;
  ACE_NEW_THROW_EX (sa_container,
                    TAO_Notify_SupplierAdmin_Container (),
                    CORBA::INTERNAL ());
  this->sa_container_.reset (sa_container);

  this->sa_container().init ();

  // Set the admin properties.
  TAO_Notify_AdminProperties* admin_properties = 0;
  ACE_NEW_THROW_EX (admin_properties,
                    TAO_Notify_AdminProperties (),
                    CORBA::NO_MEMORY ());
  this->set_admin_properties (admin_properties);

  // create the event manager. @@ use factory
  TAO_Notify_Event_Manager* event_manager = 0;
  ACE_NEW_THROW_EX (event_manager,
                    TAO_Notify_Event_Manager (),
                    CORBA::INTERNAL ());
  this->set_event_manager (event_manager);

  this->event_manager().init ();

  const CosNotification::QoSProperties &default_ec_qos =
    TAO_Notify_PROPERTIES::instance ()->default_event_channel_qos_properties ();

  this->set_qos (default_ec_qos);

  this->set_qos (initial_qos);

  this->set_admin (initial_admin);

  PortableServer::POA_var default_poa =
    TAO_Notify_PROPERTIES::instance ()->default_poa ();

  this->default_filter_factory_ =
    TAO_Notify_PROPERTIES::instance()->builder()->build_filter_factory (
    default_poa.in(), this->default_filter_factory_servant_);

  // Note originally default admins were allocated here, bt this
  // caused problems attempting to save the topology changes before
  // the Event Channel was completely constructed and linked to the
  // ECF.  Lazy evaluation also avoids creating unneded admins.
}


void
TAO_Notify_EventChannel::init (TAO_Notify::Topology_Parent* parent)
{
  ACE_ASSERT (this->ecf_.get() == 0);
  // this-> on the following line confuses VC6
  initialize (parent);

  this->ecf_.reset (dynamic_cast <TAO_Notify_EventChannelFactory*>(parent));
  ACE_ASSERT (this->ecf_.get() !=0);

  // Init ca_container_
  TAO_Notify_ConsumerAdmin_Container* ca_container = 0;
  ACE_NEW_THROW_EX (ca_container,
                    TAO_Notify_ConsumerAdmin_Container (),
                    CORBA::INTERNAL ());
  this->ca_container_.reset (ca_container);

  this->ca_container().init ();

  TAO_Notify_SupplierAdmin_Container* sa_container = 0;
  // Init ca_container_
  ACE_NEW_THROW_EX (sa_container,
                    TAO_Notify_SupplierAdmin_Container (),
                    CORBA::INTERNAL ());
  this->sa_container_.reset (sa_container);

  this->sa_container().init ();

  // Set the admin properties.
  TAO_Notify_AdminProperties* admin_properties = 0;
  ACE_NEW_THROW_EX (admin_properties,
                    TAO_Notify_AdminProperties (),
                    CORBA::NO_MEMORY ());
  this->set_admin_properties (admin_properties);

  // create the event manager. @@ use factory
  TAO_Notify_Event_Manager* event_manager = 0;
  ACE_NEW_THROW_EX (event_manager,
                    TAO_Notify_Event_Manager (),
                    CORBA::INTERNAL ());
  this->set_event_manager (event_manager);

  this->event_manager().init ();

  const CosNotification::QoSProperties &default_ec_qos =
    TAO_Notify_PROPERTIES::instance ()->default_event_channel_qos_properties ();

  this->set_qos (default_ec_qos);

  PortableServer::POA_var default_poa = TAO_Notify_PROPERTIES::instance ()->default_poa ();
  this->default_filter_factory_ =
    TAO_Notify_PROPERTIES::instance()->builder()->build_filter_factory (
    default_poa.in(), this->default_filter_factory_servant_);
}


void
TAO_Notify_EventChannel::_add_ref (void)
{
  this->_incr_refcnt ();
}

void
TAO_Notify_EventChannel::_remove_ref (void)
{
  this->_decr_refcnt ();
}

void
TAO_Notify_EventChannel::release (void)
{
  delete this;
  //@@ inform factory
}

void
TAO_Notify_EventChannel::cleanup_proxy (CosNotifyChannelAdmin::ProxyID,
                                        bool, bool)
{
}

int
TAO_Notify_EventChannel::shutdown (void)
{
  int sd_ret = TAO_Notify_Object::shutdown ();

  if (sd_ret == 1)
    return 1;

  this->ca_container().shutdown ();

  this->sa_container().shutdown ();

  this->event_manager().shutdown ();

  return 0;
}

void
TAO_Notify_EventChannel::destroy (void)
{
  TAO_Notify_EventChannel::Ptr guard( this );

  if (this->shutdown () == 1)
    return;

  this->ecf_->remove (this);

  this->sa_container ().destroy ();
  this->ca_container ().destroy ();

  this->sa_container_.reset( 0 );
  this->ca_container_.reset( 0 );

  this->default_filter_factory_ = CosNotifyFilter::FilterFactory::_nil();

  this->default_filter_factory_servant_->destroy();
}

void
TAO_Notify_EventChannel::remove (TAO_Notify_ConsumerAdmin* consumer_admin)
{
  this->ca_container().remove (consumer_admin);
}

void
TAO_Notify_EventChannel::remove (TAO_Notify_SupplierAdmin* supplier_admin)
{
  this->sa_container().remove (supplier_admin);
}

void
TAO_Notify_EventChannel::set_qos (const CosNotification::QoSProperties & qos)
{
  this->TAO_Notify_Object::set_qos (qos);
}

CosNotification::QoSProperties*
TAO_Notify_EventChannel::get_qos (void)
{
  return this->TAO_Notify_Object::get_qos ();
}

CosNotifyChannelAdmin::EventChannelFactory_ptr
TAO_Notify_EventChannel::MyFactory (void)
{
  return this->ecf_->_this ();
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_EventChannel::default_consumer_admin (void)
{
  if (CORBA::is_nil (default_consumer_admin_.in ()))
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->default_admin_mutex_, CosNotifyChannelAdmin::ConsumerAdmin::_nil());
      if (CORBA::is_nil (default_consumer_admin_.in ()))
        {
          CosNotifyChannelAdmin::AdminID id;
          this->default_consumer_admin_ =
            this->new_for_consumers (TAO_Notify_PROPERTIES::instance ()->defaultConsumerAdminFilterOp(), id);
          // Wish there was a better way to do this!
          PortableServer::ServantBase * admin_servant =
            this->poa()->reference_to_servant (
              this->default_consumer_admin_.in ());
          TAO_Notify_Admin * pAdmin = dynamic_cast <TAO_Notify_Admin *> (admin_servant);
          ACE_ASSERT (pAdmin != 0); // if this assert triggers, we have mixed implementations?
          if (pAdmin != 0)
            {
              pAdmin->set_default (true);
            }
        }
    }

  return CosNotifyChannelAdmin::ConsumerAdmin::_duplicate (this->default_consumer_admin_.in ());
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_Notify_EventChannel::default_supplier_admin (void)
{
  if (CORBA::is_nil (default_supplier_admin_.in ()))
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->default_admin_mutex_, CosNotifyChannelAdmin::SupplierAdmin::_nil());
      if (CORBA::is_nil (default_supplier_admin_.in ()))
        {
          CosNotifyChannelAdmin::AdminID id;
          this->default_supplier_admin_ =
            this->new_for_suppliers (TAO_Notify_PROPERTIES::instance ()->defaultSupplierAdminFilterOp(), id);
          PortableServer::ServantBase * admin_servant =
            this->poa()->poa()->reference_to_servant (
              this->default_supplier_admin_.in ());
          TAO_Notify_Admin * pAdmin = dynamic_cast <TAO_Notify_Admin *> (admin_servant);
          ACE_ASSERT (pAdmin != 0); // if this assert triggers, we have mixed implementations?
          if (pAdmin != 0)
            {
              pAdmin->set_default (true);
            }
        }
    }
  return CosNotifyChannelAdmin::SupplierAdmin::_duplicate (this->default_supplier_admin_.in ());
}

::CosNotifyFilter::FilterFactory_ptr
TAO_Notify_EventChannel::default_filter_factory (void)
{
  return CosNotifyFilter::FilterFactory::_duplicate (this->default_filter_factory_.in ());
}

TAO_Notify_FilterFactory*
TAO_Notify_EventChannel::default_filter_factory_servant () const
{
  return this->default_filter_factory_servant_;
}

::CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_EventChannel::new_for_consumers (CosNotifyChannelAdmin::InterFilterGroupOperator op,
                                        CosNotifyChannelAdmin::AdminID_out id)
{
  ::CosNotifyChannelAdmin::ConsumerAdmin_var ca =
    TAO_Notify_PROPERTIES::instance()->builder()->build_consumer_admin (this, op, id);
  this->self_change ();
  return ca._retn ();
}

::CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_Notify_EventChannel::new_for_suppliers (CosNotifyChannelAdmin::InterFilterGroupOperator op,
                                        CosNotifyChannelAdmin::AdminID_out id)
{
  ::CosNotifyChannelAdmin::SupplierAdmin_var sa =
    TAO_Notify_PROPERTIES::instance()->builder()->build_supplier_admin (this, op, id);
  this->self_change ();
  return sa._retn ();
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_EventChannel::get_consumeradmin (CosNotifyChannelAdmin::AdminID id)
{
  // because default admins are allocated on-demand, we need to be sure one exists for id 0
  if (id == 0)
    {
       return default_consumer_admin ();
    }

  TAO_Notify_ConsumerAdmin_Find_Worker find_worker;

  return find_worker.resolve (id, this->ca_container());
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_Notify_EventChannel::get_supplieradmin (CosNotifyChannelAdmin::AdminID id)
{
  // because default admins are allocated on-demand, we need to be sure one exists for id 0
  if (id == 0)
    {
      return default_supplier_admin ();
    }

  TAO_Notify_SupplierAdmin_Find_Worker find_worker;

  return find_worker.resolve (id, this->sa_container());
}

CosNotifyChannelAdmin::AdminIDSeq*
TAO_Notify_EventChannel::get_all_consumeradmins (void)
{
  TAO_Notify_ConsumerAdmin_Seq_Worker seq_worker;

  return seq_worker.create (this->ca_container());
}

CosNotifyChannelAdmin::AdminIDSeq*
TAO_Notify_EventChannel::get_all_supplieradmins (void)
{
  TAO_Notify_SupplierAdmin_Seq_Worker seq_worker;

  return seq_worker.create (this->sa_container());
}

void
TAO_Notify_EventChannel::set_admin (const CosNotification::AdminProperties & admin)
{
  this->admin_properties().init (admin);
}

CosNotification::AdminProperties*
TAO_Notify_EventChannel::get_admin (void)
{
  CosNotification::AdminProperties_var properties;

  ACE_NEW_THROW_EX (properties,
                    CosNotification::AdminProperties (),
                    CORBA::NO_MEMORY ());

  this->admin_properties().populate (properties);

  return properties._retn ();
}

CosEventChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_EventChannel::for_consumers (void)
{
  return this->default_consumer_admin();
}

CosEventChannelAdmin::SupplierAdmin_ptr
TAO_Notify_EventChannel::for_suppliers (void)
{
  return this->default_supplier_admin ();
}

void
TAO_Notify_EventChannel::validate_qos (const CosNotification::QoSProperties & /*required_qos*/,
                                   CosNotification::NamedPropertyRangeSeq_out /*available_qos*/)
{
  throw CORBA::NO_IMPLEMENT ();
}

void
TAO_Notify_EventChannel::save_persistent (TAO_Notify::Topology_Saver& saver)
{
  bool changed = this->self_changed_;
  this->self_changed_ = false;
  this->children_changed_ = false;

  if (is_persistent ())
  {
    TAO_Notify::NVPList attrs;
    this->save_attrs(attrs);

    bool want_all_children = saver.begin_object(
      this->id(), "channel", attrs, changed);

    this->default_filter_factory_servant_->save_persistent (saver);

    TAO_Notify::Save_Persist_Worker<TAO_Notify_ConsumerAdmin> ca_wrk(saver, want_all_children);

    this->ca_container().collection()->for_each(&ca_wrk);

    TAO_Notify::Save_Persist_Worker<TAO_Notify_SupplierAdmin> sa_wrk(saver, want_all_children);
    this->sa_container().collection()->for_each(&sa_wrk);

    saver.end_object(this->id(), "channel");
  }
}

namespace {
  template<class T>
    void add_attr(TAO_Notify::NVPList& attrs, const T& prop) {
      if (prop.is_valid())
      {
        attrs.push_back(TAO_Notify::NVP (prop));
      }
    }
}

void
TAO_Notify_EventChannel::save_attrs(TAO_Notify::NVPList& attrs)
{
  TAO_Notify_Object::save_attrs(attrs);
  add_attr(attrs, this->admin_properties().max_global_queue_length());
  add_attr(attrs, this->admin_properties().max_consumers());
  add_attr(attrs, this->admin_properties().max_suppliers());
  add_attr(attrs, this->admin_properties().reject_new_events());
}

void
TAO_Notify_EventChannel::load_attrs(const TAO_Notify::NVPList& attrs)
{
  TAO_Notify_Object::load_attrs(attrs);
  attrs.load(this->admin_properties().max_global_queue_length());
  attrs.load(this->admin_properties().max_consumers());
  attrs.load(this->admin_properties().max_suppliers());
  attrs.load(this->admin_properties().reject_new_events());
  this->admin_properties().init();
}

TAO_Notify::Topology_Object *
TAO_Notify_EventChannel::load_child (const ACE_CString &type,
                                     CORBA::Long id,
                                     const TAO_Notify::NVPList& attrs)
{
  TAO_Notify::Topology_Object* result = this;
  if (type == "filter_factory")
  {
    return this->default_filter_factory_servant_;
  }
  else if (type == "consumer_admin")
  {
    if (DEBUG_LEVEL)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) EventChannel reload consumer_admin %d\n")
                  , static_cast<int> (id)
                  ));

    // call special builder method to reload
    TAO_Notify_Builder* bld = TAO_Notify_PROPERTIES::instance()->builder();
    TAO_Notify_ConsumerAdmin * ca = bld->build_consumer_admin (
      this,
      id);
    ca->load_attrs (attrs);
    if (ca->is_default ())
      {
        CORBA::Object_var caob = this->poa()->servant_to_reference (ca);
        this->default_consumer_admin_ =
          CosNotifyChannelAdmin::ConsumerAdmin::_narrow (
          caob.in ());
      }
    result = ca;
  }
  else if (type == "supplier_admin")
  {
    if (DEBUG_LEVEL)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) EventChannel reload supplier_admin %d\n")
                  , static_cast<int> (id)
                  ));
    TAO_Notify_Builder* bld = TAO_Notify_PROPERTIES::instance()->builder();

    TAO_Notify_SupplierAdmin * sa = bld->build_supplier_admin (
      this,
      id);
    sa->load_attrs (attrs);
    if (sa->is_default ())
      {
        CORBA::Object_var saob = this->poa()->servant_to_reference (sa);
        this->default_supplier_admin_ =
          CosNotifyChannelAdmin::SupplierAdmin::_narrow (
          saob.in ());
      }
    result = sa;
  }
  return result;
}
TAO_Notify_ProxyConsumer *
TAO_Notify_EventChannel::find_proxy_consumer (TAO_Notify::IdVec & id_path, size_t position)
{
  TAO_Notify_ProxyConsumer * result = 0;
  size_t path_size = id_path.size ();
  if (position < path_size)
  {
    TAO_Notify_SupplierAdmin_Find_Worker find_worker;
    TAO_Notify_SupplierAdmin * admin = find_worker.find (id_path[position], this->sa_container());
    ++position;
    if (admin != 0)
    {
      result = admin->find_proxy_consumer (id_path, position);
    }
  }
  return result;
}

TAO_Notify_ProxySupplier *
TAO_Notify_EventChannel::find_proxy_supplier (TAO_Notify::IdVec & id_path, size_t position)
{
  TAO_Notify_ProxySupplier * result = 0;
  size_t path_size = id_path.size ();
  if (position < path_size)
  {
    TAO_Notify_ConsumerAdmin_Find_Worker find_worker;
    TAO_Notify_ConsumerAdmin * admin = find_worker.find (id_path[position], this->ca_container());
    ++position;
    if (admin != 0)
    {
      result = admin->find_proxy_supplier (id_path, position);
    }
  }
  return result;
}


void
TAO_Notify_EventChannel::reconnect (void)
{
  TAO_Notify::Reconnect_Worker<TAO_Notify_ConsumerAdmin> ca_wrk;
  this->ca_container().collection()->for_each(&ca_wrk);

  TAO_Notify::Reconnect_Worker<TAO_Notify_SupplierAdmin> sa_wrk;
  this->sa_container().collection()->for_each(&sa_wrk);
}

TAO_Notify_EventChannel::TAO_Notify_ConsumerAdmin_Container&
TAO_Notify_EventChannel::ca_container()
{
  ACE_ASSERT( this->ca_container_.get() != 0 );
  return *ca_container_;
}

TAO_Notify_EventChannel::TAO_Notify_SupplierAdmin_Container&
TAO_Notify_EventChannel::sa_container()
{
  ACE_ASSERT( this->sa_container_.get() != 0 );
  return *sa_container_;
}


void
TAO_Notify_EventChannel::validate ()
{
  TAO_Notify::Validate_Worker<TAO_Notify_ConsumerAdmin> ca_wrk;
  this->ca_container().collection()->for_each(&ca_wrk);

  TAO_Notify::Validate_Worker<TAO_Notify_SupplierAdmin> sa_wrk;
  this->sa_container().collection()->for_each(&sa_wrk);
}



TAO_END_VERSIONED_NAMESPACE_DECL
