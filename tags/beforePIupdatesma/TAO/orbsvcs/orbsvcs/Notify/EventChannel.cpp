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

#include "tao/debug.h"
//#define DEBUG_LEVEL 9
#ifndef DEBUG_LEVEL
# define DEBUG_LEVEL TAO_debug_level
#endif //DEBUG_LEVEL

ACE_RCSID(Notify, TAO_Notify_EventChannel, "$Id$")

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
{
}

TAO_Notify_EventChannel::~TAO_Notify_EventChannel ()
{
}

void
TAO_Notify_EventChannel::init (TAO_Notify_EventChannelFactory* ecf
                           , const CosNotification::QoSProperties & initial_qos
                           , const CosNotification::AdminProperties & initial_admin
                           ACE_ENV_ARG_DECL)
{
  ACE_ASSERT (this->ca_container_.get() == 0);

  // this-> on the following line confuses VC6
  initialize (ecf ACE_ENV_ARG_PARAMETER);

  this->ecf_.reset (ecf);

  // Init ca_container_
  TAO_Notify_ConsumerAdmin_Container* ca_container = 0;
  ACE_NEW_THROW_EX (ca_container,
                    TAO_Notify_ConsumerAdmin_Container (),
                    CORBA::INTERNAL ());
  ACE_CHECK;
  this->ca_container_.reset (ca_container);

  this->ca_container().init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Init ca_container_
  TAO_Notify_SupplierAdmin_Container* sa_container = 0;
  ACE_NEW_THROW_EX (sa_container,
                    TAO_Notify_SupplierAdmin_Container (),
                    CORBA::INTERNAL ());
  ACE_CHECK;
  this->sa_container_.reset (sa_container);

  this->sa_container().init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Set the admin properties.
  TAO_Notify_AdminProperties* admin_properties = 0;
  ACE_NEW_THROW_EX (admin_properties,
                    TAO_Notify_AdminProperties (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;
  this->set_admin_properties (admin_properties);

  // create the event manager. @@ use factory
  TAO_Notify_Event_Manager* event_manager = 0;
  ACE_NEW_THROW_EX (event_manager,
                    TAO_Notify_Event_Manager (),
                    CORBA::INTERNAL ());
  ACE_CHECK;
  this->set_event_manager (event_manager);

  this->event_manager().init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  const CosNotification::QoSProperties &default_ec_qos =
    TAO_Notify_PROPERTIES::instance ()->default_event_channel_qos_properties ();

  this->set_qos (default_ec_qos ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->set_qos (initial_qos ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->set_admin (initial_admin ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Note originally default admins were allocated here, bt this caused problems
  // attempting to save the topology changes before the Event Channel was completely
  // constructed and linked to the ECF.
  // Lazy evaluation also avoids creating unneded admins.
}


void
TAO_Notify_EventChannel::init (TAO_Notify::Topology_Parent* parent ACE_ENV_ARG_DECL)
{
  ACE_ASSERT (this->ecf_.get() == 0);
  // this-> on the following line confuses VC6
  initialize (parent ACE_ENV_ARG_PARAMETER);

  this->ecf_.reset (dynamic_cast <TAO_Notify_EventChannelFactory*>(parent));
  ACE_ASSERT (this->ecf_.get() !=0);

  // Init ca_container_
  TAO_Notify_ConsumerAdmin_Container* ca_container = 0;
  ACE_NEW_THROW_EX (ca_container,
                    TAO_Notify_ConsumerAdmin_Container (),
                    CORBA::INTERNAL ());
  ACE_CHECK;
  this->ca_container_.reset (ca_container);

  this->ca_container().init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  TAO_Notify_SupplierAdmin_Container* sa_container = 0;
  // Init ca_container_
  ACE_NEW_THROW_EX (sa_container,
                    TAO_Notify_SupplierAdmin_Container (),
                    CORBA::INTERNAL ());
  ACE_CHECK;
  this->sa_container_.reset (sa_container);

  this->sa_container().init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Set the admin properties.
  TAO_Notify_AdminProperties* admin_properties = 0;
  ACE_NEW_THROW_EX (admin_properties,
                    TAO_Notify_AdminProperties (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;
  this->set_admin_properties (admin_properties);

  // create the event manager. @@ use factory
  TAO_Notify_Event_Manager* event_manager = 0;
  ACE_NEW_THROW_EX (event_manager,
                    TAO_Notify_Event_Manager (),
                    CORBA::INTERNAL ());
  ACE_CHECK;
  this->set_event_manager (event_manager);

  this->event_manager().init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  const CosNotification::QoSProperties &default_ec_qos =
    TAO_Notify_PROPERTIES::instance ()->default_event_channel_qos_properties ();

  this->set_qos (default_ec_qos ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

}


void
TAO_Notify_EventChannel::_add_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_incr_refcnt ();
}

void
TAO_Notify_EventChannel::_remove_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_decr_refcnt ();
}

void
TAO_Notify_EventChannel::release (void)
{
  delete this;
  //@@ inform factory
}

int
TAO_Notify_EventChannel::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  int sd_ret = TAO_Notify_Object::shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (1);

  if (sd_ret == 1)
    return 1;

  this->ca_container().shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (1);

  this->sa_container().shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (1);

  this->event_manager().shutdown ();

  return 0;
}

void
TAO_Notify_EventChannel::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  TAO_Notify_EventChannel::Ptr guard( this );

  int result = this->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  if ( result == 1)
    return;

  this->ecf_->remove (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->sa_container_.reset( 0 );
  this->ca_container_.reset( 0 );
}

void
TAO_Notify_EventChannel::remove (TAO_Notify_ConsumerAdmin* consumer_admin ACE_ENV_ARG_DECL)
{
  this->ca_container().remove (consumer_admin ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_EventChannel::remove (TAO_Notify_SupplierAdmin* supplier_admin ACE_ENV_ARG_DECL)
{
  this->sa_container().remove (supplier_admin ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_EventChannel::set_qos (const CosNotification::QoSProperties & qos ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotification::UnsupportedQoS
                   ))
{
  this->TAO_Notify_Object::set_qos (qos ACE_ENV_ARG_PARAMETER);
}

CosNotification::QoSProperties*
TAO_Notify_EventChannel::get_qos (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->TAO_Notify_Object::get_qos (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosNotifyChannelAdmin::EventChannelFactory_ptr
TAO_Notify_EventChannel::MyFactory (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->ecf_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_EventChannel::default_consumer_admin (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  if (CORBA::is_nil (default_consumer_admin_.in ()))
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->default_admin_mutex_, CosNotifyChannelAdmin::ConsumerAdmin::_nil());
      if (CORBA::is_nil (default_consumer_admin_.in ()))
        {
          CosNotifyChannelAdmin::AdminID id;
          this->default_consumer_admin_ = this->new_for_consumers (CosNotifyChannelAdmin::OR_OP, id ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (CosNotifyChannelAdmin::ConsumerAdmin::_nil());
          // Wish there was a better way to do this!
          PortableServer::ServantBase * admin_servant =
            this->poa()->reference_to_servant (
              this->default_consumer_admin_.in ()
              ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (CosNotifyChannelAdmin::ConsumerAdmin::_nil());
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
TAO_Notify_EventChannel::default_supplier_admin (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  if (CORBA::is_nil (default_supplier_admin_.in ()))
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, guard, this->default_admin_mutex_, CosNotifyChannelAdmin::SupplierAdmin::_nil());
      if (CORBA::is_nil (default_supplier_admin_.in ()))
        {
          CosNotifyChannelAdmin::AdminID id;
          this->default_supplier_admin_ = this->new_for_suppliers (CosNotifyChannelAdmin::OR_OP, id ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (CosNotifyChannelAdmin::SupplierAdmin::_nil());
          PortableServer::ServantBase * admin_servant =
            this->poa()->poa()->reference_to_servant (
              this->default_supplier_admin_.in ()
              ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);
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

::CosNotifyFilter::FilterFactory_ptr TAO_Notify_EventChannel::default_filter_factory (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->ecf_->get_default_filter_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
}

::CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_EventChannel::new_for_consumers (CosNotifyChannelAdmin::InterFilterGroupOperator op,
                                        CosNotifyChannelAdmin::AdminID_out id ACE_ENV_ARG_DECL
                                        )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))

{
  ::CosNotifyChannelAdmin::ConsumerAdmin_var ca =
    TAO_Notify_PROPERTIES::instance()->builder()->build_consumer_admin (this, op, id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (ca._retn ());
  this->self_change (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (ca._retn ());
  return ca._retn ();
}

::CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_Notify_EventChannel::new_for_suppliers (CosNotifyChannelAdmin::InterFilterGroupOperator op,
                                        CosNotifyChannelAdmin::AdminID_out id
                                        ACE_ENV_ARG_DECL
                                        )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ::CosNotifyChannelAdmin::SupplierAdmin_var sa =
    TAO_Notify_PROPERTIES::instance()->builder()->build_supplier_admin (this, op, id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (sa._retn ());
  this->self_change (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (sa._retn ());
  return sa._retn ();
}

CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_EventChannel::get_consumeradmin (CosNotifyChannelAdmin::AdminID id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::AdminNotFound
                   ))
{
  TAO_Notify_ConsumerAdmin_Find_Worker find_worker;

  return find_worker.resolve (id, this->ca_container() ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_Notify_EventChannel::get_supplieradmin (CosNotifyChannelAdmin::AdminID id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::AdminNotFound
                   ))
{
  TAO_Notify_SupplierAdmin_Find_Worker find_worker;

  return find_worker.resolve (id, this->sa_container() ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::AdminIDSeq*
TAO_Notify_EventChannel::get_all_consumeradmins (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  TAO_Notify_ConsumerAdmin_Seq_Worker seq_worker;

  return seq_worker.create (this->ca_container() ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::AdminIDSeq*
TAO_Notify_EventChannel::get_all_supplieradmins (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  TAO_Notify_SupplierAdmin_Seq_Worker seq_worker;

  return seq_worker.create (this->sa_container() ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_EventChannel::set_admin (const CosNotification::AdminProperties & admin ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotification::UnsupportedAdmin
                   ))
{
  this->admin_properties().init (admin);
}

CosNotification::AdminProperties*
TAO_Notify_EventChannel::get_admin (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CosNotification::AdminProperties_var properties;

  ACE_NEW_THROW_EX (properties,
                    CosNotification::AdminProperties (),
                    CORBA::NO_MEMORY ());

  this->admin_properties().populate (properties);

  return properties._retn ();
}

CosEventChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_EventChannel::for_consumers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->default_consumer_admin(ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosEventChannelAdmin::SupplierAdmin_ptr
TAO_Notify_EventChannel::for_suppliers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->default_supplier_admin (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_EventChannel::validate_qos (const CosNotification::QoSProperties & /*required_qos*/,
                                   CosNotification::NamedPropertyRangeSeq_out /*available_qos*/
                                   ACE_ENV_ARG_DECL
                                   )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotification::UnsupportedQoS
                   ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

void
TAO_Notify_EventChannel::save_persistent (TAO_Notify::Topology_Saver& saver ACE_ENV_ARG_DECL)
{
  bool changed = this->self_changed_;
  this->self_changed_ = false;
  this->children_changed_ = false;

  if (is_persistent ())
  {
    TAO_Notify::NVPList attrs;
    this->save_attrs(attrs);

    bool want_all_children = saver.begin_object(
      this->id(), "channel", attrs, changed ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    TAO_Notify::Save_Persist_Worker<TAO_Notify_ConsumerAdmin> ca_wrk(saver, want_all_children);

    this->ca_container().collection()->for_each(&ca_wrk ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    TAO_Notify::Save_Persist_Worker<TAO_Notify_SupplierAdmin> sa_wrk(saver, want_all_children);
    this->sa_container().collection()->for_each(&sa_wrk ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    saver.end_object(this->id(), "channel" ACE_ENV_ARG_PARAMETER);
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
                                                  const TAO_Notify::NVPList& attrs
                                                  ACE_ENV_ARG_DECL)
{
  TAO_Notify::Topology_Object* result = this;
  if (type == "consumer_admin")
  {
    if (DEBUG_LEVEL) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) EventChannel reload consumer_admin %d\n")
      , static_cast<int> (id)
      ));

    // call special builder method to reload
    TAO_Notify_Builder* bld = TAO_Notify_PROPERTIES::instance()->builder();
    TAO_Notify_ConsumerAdmin * ca = bld->build_consumer_admin (
      this,
      id
      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN(0);
    ca->load_attrs (attrs);
    if (ca->is_default ())
      {
        CORBA::Object_var caob = this->poa()->servant_to_reference (ca ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN(0);
        this->default_consumer_admin_ =
          CosNotifyChannelAdmin::ConsumerAdmin::_narrow (
          caob.in () ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN(0);
      }
    result = ca;
  }
  else if (type == "supplier_admin")
  {
    if (DEBUG_LEVEL) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) EventChannel reload supplier_admin %d\n")
      , static_cast<int> (id)
      ));
    TAO_Notify_Builder* bld = TAO_Notify_PROPERTIES::instance()->builder();

    TAO_Notify_SupplierAdmin * sa = bld->build_supplier_admin (
      this,
      id
      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN(0);
    sa->load_attrs (attrs);
    if (sa->is_default ())
      {
        CORBA::Object_var saob = this->poa()->servant_to_reference (sa ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN(0);
        this->default_supplier_admin_ =
          CosNotifyChannelAdmin::SupplierAdmin::_narrow (
          saob.in () ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN(0);
      }
    result = sa;
  }
  return result;
}
TAO_Notify_ProxyConsumer *
TAO_Notify_EventChannel::find_proxy_consumer (TAO_Notify::IdVec & id_path, size_t position ACE_ENV_ARG_DECL)
{
  TAO_Notify_ProxyConsumer * result = 0;
  size_t path_size = id_path.size ();
  if (position < path_size)
  {
    TAO_Notify_SupplierAdmin_Find_Worker find_worker;
    TAO_Notify_SupplierAdmin * admin = find_worker.find (id_path[position], this->sa_container() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);
    ++position;
    if (admin != 0)
    {
      result = admin->find_proxy_consumer (id_path, position
        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN(0);
    }
  }
  return result;
}

TAO_Notify_ProxySupplier *
TAO_Notify_EventChannel::find_proxy_supplier (TAO_Notify::IdVec & id_path, size_t position ACE_ENV_ARG_DECL)
{
  TAO_Notify_ProxySupplier * result = 0;
  size_t path_size = id_path.size ();
  if (position < path_size)
  {
    TAO_Notify_ConsumerAdmin_Find_Worker find_worker;
    TAO_Notify_ConsumerAdmin * admin = find_worker.find (id_path[position], this->ca_container() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);
    ++position;
    if (admin != 0)
    {
      result = admin->find_proxy_supplier (id_path, position
        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN(0);
    }
  }
  return result;
}


void
TAO_Notify_EventChannel::reconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_Notify::Reconnect_Worker<TAO_Notify_ConsumerAdmin> ca_wrk;
  this->ca_container().collection()->for_each(&ca_wrk ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  TAO_Notify::Reconnect_Worker<TAO_Notify_SupplierAdmin> sa_wrk;
  this->sa_container().collection()->for_each(&sa_wrk ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
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

TAO_END_VERSIONED_NAMESPACE_DECL
