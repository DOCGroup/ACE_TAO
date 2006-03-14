// $Id$

#include "orbsvcs/Notify/EventChannelFactory.h"

ACE_RCSID(Notify, TAO_Notify_EventChannelFactory, "$Id$")

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
TAO_Notify_EventChannelFactory::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  int result = this->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  if ( result == 1)
    return;

  TAO_Notify_Properties* properties = TAO_Notify_PROPERTIES::instance();

  // Reset references to CORBA objects.
  properties->orb (CORBA::ORB::_nil ());
  properties->default_poa (PortableServer::POA::_nil ());

  ec_container_.reset( 0 );
}

void
TAO_Notify_EventChannelFactory::init (PortableServer::POA_ptr poa ACE_ENV_ARG_DECL)
{
  ACE_ASSERT (this->ec_container_.get() == 0);

  this->default_filter_factory_ =
    TAO_Notify_PROPERTIES::instance()->builder()->build_filter_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Init ec_container_
  TAO_Notify_EventChannel_Container* ecc = 0;
  ACE_NEW_THROW_EX (ecc,
                    TAO_Notify_EventChannel_Container (),
                    CORBA::INTERNAL ());
  ACE_CHECK;
  this->ec_container_.reset( ecc );

  this->ec_container().init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  TAO_Notify_POA_Helper* object_poa = 0;

  // Bootstrap initial Object POA
  ACE_NEW_THROW_EX (object_poa,
                    TAO_Notify_POA_Helper (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  ACE_Auto_Ptr<TAO_Notify_POA_Helper> auto_object_poa (object_poa);

  object_poa->init (poa ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->adopt_poa (auto_object_poa.release ());

  // Note topology factory is configured separately from the "builder" mediated
  // objects since it is independant of the "style" of Notification Service.
  this->topology_factory_ =
    ACE_Dynamic_Service <TAO_Notify::Topology_Factory>::instance ("Topology_Factory");

  this->load_topology (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->load_event_persistence (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Notify_EventChannelFactory::_add_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_incr_refcnt ();
}

void
TAO_Notify_EventChannelFactory::_remove_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
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
TAO_Notify_EventChannelFactory::remove (TAO_Notify_EventChannel* event_channel ACE_ENV_ARG_DECL)
{
  this->ec_container().remove (event_channel ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->self_change (ACE_ENV_SINGLE_ARG_PARAMETER);
}

int
TAO_Notify_EventChannelFactory::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  int sd_ret = TAO_Notify_Object::shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (1);

  if (sd_ret == 1)
    return 1;

  this->ec_container().shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (1);

  return 0;
}

CosNotifyFilter::FilterFactory_ptr
TAO_Notify_EventChannelFactory::get_default_filter_factory (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return CosNotifyFilter::FilterFactory::_duplicate (this->default_filter_factory_.in ());
}

::CosNotifyChannelAdmin::EventChannel_ptr TAO_Notify_EventChannelFactory::create_channel (
    const CosNotification::QoSProperties & initial_qos,
    const CosNotification::AdminProperties & initial_admin,
    CosNotifyChannelAdmin::ChannelID_out id ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotification::UnsupportedQoS
                   , CosNotification::UnsupportedAdmin
                   ))
{
  CosNotifyChannelAdmin::EventChannel_var ec =
    TAO_Notify_PROPERTIES::instance()->builder()->build_event_channel (this
                                                                        , initial_qos
                                                                        , initial_admin
                                                                        , id
                                                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannel::_nil());
  this->self_change (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannel::_nil());
  return ec._retn ();
}

CosNotifyChannelAdmin::ChannelIDSeq*
TAO_Notify_EventChannelFactory::get_all_channels (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  TAO_Notify_EventChannel_Seq_Worker seq_worker;

  return seq_worker.create (this->ec_container() ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_Notify_EventChannelFactory::get_event_channel (CosNotifyChannelAdmin::ChannelID id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::ChannelNotFound
                   ))
{
  TAO_Notify_EventChannel_Find_Worker find_worker;

  return find_worker.resolve (id, this->ec_container() ACE_ENV_ARG_PARAMETER);
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
TAO_Notify_EventChannelFactory::load_topology (ACE_ENV_SINGLE_ARG_DECL)
{
  this->loading_topology_ = true;
  if (this->topology_factory_ != 0)
  {
    // create_loader will open and load the persistence file for validation
    auto_ptr<TAO_Notify::Topology_Loader> tl(this->topology_factory_->create_loader());
    if (tl.get () != 0)
    {
      tl->load (this ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  }
  else
  {
    if (TAO_debug_level > 0)
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Topology persistence disabled.\n")));
  }
  this->loading_topology_ = false;
}
bool
TAO_Notify_EventChannelFactory::is_persistent () const
{
  return true;
}

void
TAO_Notify_EventChannelFactory::save_persistent (TAO_Notify::Topology_Saver& saver ACE_ENV_ARG_DECL)
{
  bool changed = this->self_changed_;
  this->self_changed_ = false;
  this->children_changed_ = false;

  TAO_Notify::NVPList attrs; // ECF has no attributes

  bool want_all_children =
    saver.begin_object(0, "channel_factory", attrs, changed ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // for each deleted child
  //  delete_child  // if the child has persistence.

  TAO_Notify::Save_Persist_Worker<TAO_Notify_EventChannel> wrk(saver, want_all_children);

  this->ec_container().collection()->for_each(&wrk ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (want_all_children || this->reconnect_registry_.is_changed ())
  {
    this->reconnect_registry_.save_persistent(saver ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }
  saver.end_object(0, "channel_factory" ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_EventChannelFactory::load_event_persistence (ACE_ENV_SINGLE_ARG_DECL)
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
      ACE_THROW (CORBA::PERSIST_STORE());
      ACE_CHECK;
    }
  }
}

bool
TAO_Notify_EventChannelFactory::change_to_parent (ACE_ENV_SINGLE_ARG_DECL)
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
      ACE_CHECK_RETURN(false);
      if (seq == this->topology_save_seq_)
      {
        auto_ptr<TAO_Notify::Topology_Saver> saver(this->topology_factory_->create_saver());
        if (saver.get() != 0)
        {
          this->save_persistent(*saver ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN(false);
          saver->close (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (false);
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
                                        NVPList& attrs
                                        ACE_ENV_ARG_DECL)
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
        id
        ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN(0);

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
TAO_Notify_EventChannelFactory::reconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  // Reconnect all children first
  TAO_Notify::Reconnect_Worker<TAO_Notify_EventChannel> wrk;

  this->ec_container().collection()->for_each(&wrk ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Then send reconnection announcement to registered clients
  ACE_ASSERT (!CORBA::is_nil (this->channel_factory_.in ()));
  this->reconnect_registry_.send_reconnect (this->channel_factory_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // reactivate events in-progress
  Routing_Slip_Set::CONST_ITERATOR iter (this->routing_slip_restart_set_);
  TAO_Notify::Routing_Slip_Ptr * routing_slip;
  for (iter.first(); iter.next(routing_slip); iter.advance())
  {
    (*routing_slip)->reconnect(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
  }
  this->routing_slip_restart_set_.reset ();
}

NotifyExt::ReconnectionRegistry::ReconnectionID
TAO_Notify_EventChannelFactory::register_callback (
    NotifyExt::ReconnectionCallback_ptr reconnection
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->reconnect_registry_.register_callback (
    reconnection
    ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_EventChannelFactory::unregister_callback (
    NotifyExt::ReconnectionRegistry::ReconnectionID id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->reconnect_registry_.unregister_callback (
    id
    ACE_ENV_ARG_PARAMETER);
}

CORBA::Boolean
TAO_Notify_EventChannelFactory::is_alive (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::Boolean (1);
}

void
TAO_Notify_EventChannelFactory::save_topology (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->self_change (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

TAO_Notify_ProxyConsumer *
TAO_Notify_EventChannelFactory::find_proxy_consumer (TAO_Notify::IdVec & id_path, size_t position ACE_ENV_ARG_DECL)
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

    TAO_Notify_EventChannel * ec = find_worker.find (id_path[position], this->ec_container() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);
    ++position;
    if (ec != 0)
    {
      result = ec->find_proxy_consumer (id_path, position
        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN(0);
    }
  }
  return result;
}

TAO_Notify_ProxySupplier *
TAO_Notify_EventChannelFactory::find_proxy_supplier (TAO_Notify::IdVec & id_path, size_t position ACE_ENV_ARG_DECL)
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
    TAO_Notify_EventChannel * ec = find_worker.find (id_path[position], this->ec_container() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);
    ++position;
    if (ec != 0)
    {
      result = ec->find_proxy_supplier (id_path, position
        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN(0);
    }
  }
  return result;
}

CosNotifyChannelAdmin::EventChannelFactory_ptr
TAO_Notify_EventChannelFactory::activate_self (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::Object_var obj = this->activate (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannelFactory::_nil ());
  this->channel_factory_
    = CosNotifyChannelAdmin::EventChannelFactory::_narrow (obj.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannelFactory::_nil ());
  CosNotifyChannelAdmin::EventChannelFactory::_narrow (obj.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::EventChannelFactory::_nil ());

  ACE_TRY_NEW_ENV
  {
    if (DEBUG_LEVEL > 9)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) TAO_Notify_EventChannelFactory::activate_self") ));
    }
    this->reconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }
  ACE_CATCHANY
  {
    // ignore for now
  }
  ACE_ENDTRY;
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
