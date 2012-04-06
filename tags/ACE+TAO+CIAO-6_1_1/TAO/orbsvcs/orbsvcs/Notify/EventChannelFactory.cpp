// $Id$

#include "orbsvcs/Notify/EventChannelFactory.h"
#include "orbsvcs/Notify/Properties.h"
#include "orbsvcs/Notify/Factory.h"
#include "orbsvcs/Notify/Builder.h"
#include "orbsvcs/Notify/Topology_Saver.h"
#include "orbsvcs/Notify/Topology_Loader.h"
#include "orbsvcs/Notify/Save_Persist_Worker_T.h"
#include "orbsvcs/Notify/Reconnect_Worker_T.h"
#include "orbsvcs/Notify/Event_Persistence_Strategy.h"
#include "orbsvcs/Notify/Routing_Slip_Persistence_Manager.h"
#include "orbsvcs/Notify/EventChannel.h"
#include "orbsvcs/Notify/Container_T.h"
#include "orbsvcs/Notify/Find_Worker_T.h"
#include "orbsvcs/Notify/Seq_Worker_T.h"
#include "orbsvcs/Notify/POA_Helper.h"
#include "orbsvcs/Notify/Validate_Worker_T.h"
#include "orbsvcs/Notify/Validate_Client_Task.h"
#include "orbsvcs/Notify/FilterFactory.h"

#include "ace/Dynamic_Service.h"

#include "tao/debug.h"
//#define DEBUG_LEVEL 9
#ifndef DEBUG_LEVEL
# define DEBUG_LEVEL TAO_debug_level
#endif //DEBUG_LEVEL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Include this here since this is the only file that
// requires Topology_Factory.
namespace TAO_Notify
{
  // virtual
  Topology_Factory::~Topology_Factory ()
  {
  }
}

typedef TAO_Notify_Find_Worker_T<TAO_Notify_EventChannel
                             , CosNotifyChannelAdmin::EventChannel
                             , CosNotifyChannelAdmin::EventChannel_ptr
                             , CosNotifyChannelAdmin::ChannelNotFound>
TAO_Notify_EventChannel_Find_Worker;

typedef TAO_Notify_Seq_Worker_T<TAO_Notify_EventChannel> TAO_Notify_EventChannel_Seq_Worker;

TAO_Notify_EventChannelFactory::TAO_Notify_EventChannelFactory (void)
  : topology_save_seq_ (0)
  , topology_factory_(0)
  , reconnect_registry_(*this)
  , loading_topology_ (false)
{
}

TAO_Notify_EventChannelFactory::~TAO_Notify_EventChannelFactory ()
{
}

void
TAO_Notify_EventChannelFactory::destroy (void)
{
  if (this->shutdown () == 1)
    return;

  TAO_Notify_Properties* properties = TAO_Notify_PROPERTIES::instance();

  // Reset references to CORBA objects.
  properties->orb (CORBA::ORB::_nil ());
  properties->default_poa (PortableServer::POA::_nil ());

  ec_container_.reset( 0 );
}

void
TAO_Notify_EventChannelFactory::init (PortableServer::POA_ptr poa)
{
  this->poa_ = PortableServer::POA::_duplicate (poa);

  ACE_ASSERT (this->ec_container_.get() == 0);

  // Init ec_container_
  TAO_Notify_EventChannel_Container* ecc = 0;
  ACE_NEW_THROW_EX (ecc,
                    TAO_Notify_EventChannel_Container (),
                    CORBA::INTERNAL ());
  this->ec_container_.reset( ecc );

  this->ec_container().init ();

  TAO_Notify_POA_Helper* object_poa = 0;

  // Bootstrap initial Object POA
  ACE_NEW_THROW_EX (object_poa,
                    TAO_Notify_POA_Helper (),
                    CORBA::NO_MEMORY ());

  ACE_Auto_Ptr<TAO_Notify_POA_Helper> auto_object_poa (object_poa);

  ACE_CString poa_name = object_poa->get_unique_id ();
#if defined (CORBA_E_MICRO)
  object_poa->init (poa, poa_name.c_str ());
#else
  object_poa->init_persistent (poa, poa_name.c_str ());
#endif /* CORBA_E_MICRO */

  this->adopt_poa (auto_object_poa.release ());

  // Note topology factory is configured separately from the "builder" mediated
  // objects since it is independant of the "style" of Notification Service.
  this->topology_factory_ =
    ACE_Dynamic_Service <TAO_Notify::Topology_Factory>::instance ("Topology_Factory");

  this->load_topology ();

  this->load_event_persistence ();

  if (TAO_Notify_PROPERTIES::instance()->validate_client() == true)
  {
    TAO_Notify_validate_client_Task* validate_client_task = 0;
    ACE_NEW_THROW_EX (validate_client_task,
      TAO_Notify_validate_client_Task (TAO_Notify_PROPERTIES::instance()->validate_client_delay (),
                                 TAO_Notify_PROPERTIES::instance()->validate_client_interval (),
                                 this),
      CORBA::INTERNAL ());
    this->validate_client_task_.reset (validate_client_task);
  }
}

void
TAO_Notify_EventChannelFactory::_add_ref (void)
{
  this->_incr_refcnt ();
}

void
TAO_Notify_EventChannelFactory::_remove_ref (void)
{
  this->_decr_refcnt ();
}

void
TAO_Notify_EventChannelFactory::release (void)
{
  delete this;
  //@@ inform factory
}

void
TAO_Notify_EventChannelFactory::remove (TAO_Notify_EventChannel* event_channel)
{
  this->ec_container().remove (event_channel);
  this->self_change ();
}

int
TAO_Notify_EventChannelFactory::shutdown (void)
{
  this->stop_validator();

  if (TAO_Notify_Object::shutdown () == 1)
    return 1;

  this->ec_container().shutdown ();

  return 0;
}


CosNotifyChannelAdmin::EventChannel_ptr
TAO_Notify_EventChannelFactory::create_named_channel (
    const CosNotification::QoSProperties& initial_qos,
    const CosNotification::AdminProperties& initial_admin,
    CosNotifyChannelAdmin::ChannelID_out id,
    const char*)
{
  return this->create_channel (initial_qos, initial_admin, id);
}


::CosNotifyChannelAdmin::EventChannel_ptr TAO_Notify_EventChannelFactory::create_channel (
    const CosNotification::QoSProperties & initial_qos,
    const CosNotification::AdminProperties & initial_admin,
    CosNotifyChannelAdmin::ChannelID_out id
  )
{
  CosNotifyChannelAdmin::EventChannel_var ec =
    TAO_Notify_PROPERTIES::instance()->builder()->build_event_channel (this
                                                                        , initial_qos
                                                                        , initial_admin
                                                                        , id);
  this->self_change ();
  return ec._retn ();
}

CosNotifyChannelAdmin::ChannelIDSeq*
TAO_Notify_EventChannelFactory::get_all_channels (void)
{
  TAO_Notify_EventChannel_Seq_Worker seq_worker;

  return seq_worker.create (this->ec_container());
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_Notify_EventChannelFactory::get_event_channel (CosNotifyChannelAdmin::ChannelID id)
{
  TAO_Notify_EventChannel_Find_Worker find_worker;

  return find_worker.resolve (id, this->ec_container());
}

void
TAO_Notify_EventChannelFactory::set_topology_factory(TAO_Notify::Topology_Factory* f)
{
  ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("(%P,%t) Debug Topology_Factory installed in EventChannelFactory.\n")
    ));
  // If the above meessage appears when you don't expect it
  // use svc.conf to install the topology factory rather
  // than calling this method.
  this->topology_factory_ = f;
}

void
TAO_Notify_EventChannelFactory::load_topology (void)
{
  this->loading_topology_ = true;
  if (this->topology_factory_ != 0)
  {
    // create_loader will open and load the persistence file for validation
    auto_ptr<TAO_Notify::Topology_Loader> tl(this->topology_factory_->create_loader());
    if (tl.get () != 0)
    {
      tl->load (this);
    }
  }
  else
  {
    if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Topology persistence disabled.\n")));
  }
  this->loading_topology_ = false;
}

void
TAO_Notify_EventChannelFactory::validate ()
{
  TAO_Notify::Validate_Worker<TAO_Notify_EventChannel> wrk;

  this->ec_container().collection()->for_each(&wrk);
}

void
TAO_Notify_EventChannelFactory::stop_validator ()
{
  if (this->validate_client_task_.get () != 0)
    {
      this->validate_client_task_->shutdown ();
    }
}

bool
TAO_Notify_EventChannelFactory::is_persistent () const
{
  return true;
}

void
TAO_Notify_EventChannelFactory::save_persistent (TAO_Notify::Topology_Saver& saver)
{
  bool changed = this->self_changed_;
  this->self_changed_ = false;
  this->children_changed_ = false;

  TAO_Notify::NVPList attrs; // ECF has no attributes

  bool want_all_children =
    saver.begin_object(0, "channel_factory", attrs, changed);

  // for each deleted child
  //  delete_child  // if the child has persistence.

  TAO_Notify::Save_Persist_Worker<TAO_Notify_EventChannel> wrk(saver, want_all_children);

  this->ec_container().collection()->for_each(&wrk);

  if (want_all_children || this->reconnect_registry_.is_changed ())
  {
    this->reconnect_registry_.save_persistent(saver);
  }
  saver.end_object(0, "channel_factory");
}

void
TAO_Notify_EventChannelFactory::load_event_persistence (void)
{
  TAO_Notify::Event_Persistence_Strategy * strategy =
    ACE_Dynamic_Service <TAO_Notify::Event_Persistence_Strategy>::instance ("Event_Persistence");
  if (strategy != 0)
  {
    if (this->topology_factory_ != 0)
    {
      TAO_Notify::Event_Persistence_Factory * factory = strategy->get_factory ();
      if (factory != 0)
      {
        for (
          TAO_Notify::Routing_Slip_Persistence_Manager * rspm = factory->first_reload_manager();
          rspm != 0;
          rspm = rspm->load_next ())
        {
          TAO_Notify::Routing_Slip_Ptr routing_slip = TAO_Notify::Routing_Slip::create (*this, rspm);
          if (!routing_slip.null ())
          {
            this->routing_slip_restart_set_.insert (routing_slip);
          }
          else
          {
            //@@todo: tell the rspm it's an orphan, but we can't during reload
            // we need collect these and come back later to remove them
            ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) Reload persistent event failed.\n")
              ));
          }
        }
      }
    }
    else
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("(%P|%t) Notify Service: Configuration error.  Event Persistence requires Topology Persistence.\n")
        ));
      throw CORBA::PERSIST_STORE();
    }
  }
}

bool
TAO_Notify_EventChannelFactory::change_to_parent (void)
{
  bool saving = false;
  if (! this->loading_topology_)
  {
    // A null pointer means that saving of topology is disabled.
    if (this->topology_factory_ != 0)
    {
      saving = true;
      // seq is used to check save-in-progress
      // if it changes while we're waiting for the lock
      // then our change may have already been saved, so
      // just return.  Caller will signal change again if necessary.
      short seq = this->topology_save_seq_;
      ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->topology_save_lock_, CORBA::INTERNAL ());
      if (seq == this->topology_save_seq_)
      {
        auto_ptr<TAO_Notify::Topology_Saver> saver(this->topology_factory_->create_saver());
        if (saver.get() != 0)
        {
          this->save_persistent(*saver);
          saver->close ();
        }
        this->topology_save_seq_ += 1;
      }
    }
  }
  return saving;
}

TAO_Notify::Topology_Object*
TAO_Notify_EventChannelFactory::load_child (const ACE_CString& type,
                                        CORBA::Long id,
                                        const TAO_Notify::
                                        NVPList& attrs)
{
  // ignore anything but our valid children (ie channel)
  TAO_Notify::Topology_Object * result = this;
  if (type == "channel")
  {
    if (DEBUG_LEVEL) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) EventChannelFactory reload channel %d\n")
      , static_cast<int> (id)
      ));

    TAO_Notify_Builder* bld = TAO_Notify_PROPERTIES::instance()->builder();
    TAO_Notify_EventChannel * ec = bld->build_event_channel(
        this ,
        id);

    ec->load_attrs (attrs);

    result = ec;
  }
  else if (type == TAO_Notify::REGISTRY_TYPE)
  {
    result = & this->reconnect_registry_;
  }
  return result;
}

void
TAO_Notify_EventChannelFactory::reconnect (void)
{
  // Reconnect all children first
  TAO_Notify::Reconnect_Worker<TAO_Notify_EventChannel> wrk;

  this->ec_container().collection()->for_each(&wrk);

  // Then send reconnection announcement to registered clients
  ACE_ASSERT (!CORBA::is_nil (this->channel_factory_.in ()));
  this->reconnect_registry_.send_reconnect (this->channel_factory_.in ());

  // reactivate events in-progress
  Routing_Slip_Set::CONST_ITERATOR iter (this->routing_slip_restart_set_);
  TAO_Notify::Routing_Slip_Ptr * routing_slip;
  for (iter.first(); iter.next(routing_slip); iter.advance())
  {
    (*routing_slip)->reconnect();
  }
  this->routing_slip_restart_set_.reset ();
}

NotifyExt::ReconnectionRegistry::ReconnectionID
TAO_Notify_EventChannelFactory::register_callback (
    NotifyExt::ReconnectionCallback_ptr reconnection)
{
  return this->reconnect_registry_.register_callback (
    reconnection);
}

void
TAO_Notify_EventChannelFactory::unregister_callback (
    NotifyExt::ReconnectionRegistry::ReconnectionID id)
{
  this->reconnect_registry_.unregister_callback (
    id);
}

CORBA::Boolean
TAO_Notify_EventChannelFactory::is_alive (void)
{
  return CORBA::Boolean (1);
}

void
TAO_Notify_EventChannelFactory::save_topology (void)
{
  this->self_change ();
}

TAO_Notify_ProxyConsumer *
TAO_Notify_EventChannelFactory::find_proxy_consumer (TAO_Notify::IdVec & id_path, size_t position)
{
  TAO_Notify_ProxyConsumer * result = 0;
  size_t path_size = id_path.size ();

  // EventChannelFactory only:  The first id is proably for the ECF itself
  // if so, silently consume it.
  if (position < path_size && id_path[position] == this->id())
  {
    ++position;
  }
  if (position < path_size)
  {
    TAO_Notify_EventChannel_Find_Worker find_worker;

    TAO_Notify_EventChannel * ec = find_worker.find (id_path[position], this->ec_container());
    ++position;
    if (ec != 0)
    {
      result = ec->find_proxy_consumer (id_path, position);
    }
  }
  return result;
}

TAO_Notify_ProxySupplier *
TAO_Notify_EventChannelFactory::find_proxy_supplier (TAO_Notify::IdVec & id_path, size_t position)
{
  TAO_Notify_ProxySupplier * result = 0;
  size_t path_size = id_path.size ();

  // EventChannelFactory only:  The first id is proably for the ECF itself
  // if so, silently consume it.
  if (position < path_size && id_path[position] == this->id())
  {
    ++position;
  }
  if (position < path_size)
  {
    TAO_Notify_EventChannel_Find_Worker find_worker;
    TAO_Notify_EventChannel * ec = find_worker.find (id_path[position], this->ec_container());
    ++position;
    if (ec != 0)
    {
      result = ec->find_proxy_supplier (id_path, position);
    }
  }
  return result;
}

CosNotifyChannelAdmin::EventChannelFactory_ptr
TAO_Notify_EventChannelFactory::activate_self (void)
{
  CORBA::Object_var obj = this->activate (this);
  this->channel_factory_
    = CosNotifyChannelAdmin::EventChannelFactory::_narrow (obj.in());

  try
  {
    if (DEBUG_LEVEL > 9)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) TAO_Notify_EventChannelFactory::activate_self\n") ));
    }
    this->reconnect ();
  }
  catch (const CORBA::Exception&)
  {
    // ignore for now
  }
  return this->channel_factory_._retn();
}


TAO_Notify_Object::ID
TAO_Notify_EventChannelFactory::get_id () const
{
  return id();
}

TAO_Notify_EventChannelFactory::TAO_Notify_EventChannel_Container&
TAO_Notify_EventChannelFactory::ec_container()
{
  ACE_ASSERT( this->ec_container_.get() != 0 );
  return *ec_container_;
}


TAO_END_VERSIONED_NAMESPACE_DECL
