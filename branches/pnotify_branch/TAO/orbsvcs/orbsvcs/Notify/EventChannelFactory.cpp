// $Id$

#include "EventChannelFactory.h"

#if ! defined (__ACE_INLINE__)
#include "EventChannelFactory.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_EventChannelFactory, "$Id$")

#include "ace/Dynamic_Service.h"
#include "Properties.h"
#include "Factory.h"
#include "Builder.h"
#include "Topology_Saver.h"
#include "Topology_Loader.h"
#include "Save_Persist_Worker_T.h"
#include "Reconnect_Worker_T.h"
#include "Event_Persistence_Strategy.h"
#include "Routing_Slip_Persistence_Manager.h"

#include "EventChannel.h"
#include "Container_T.h"
#include "Find_Worker_T.h"
#include "Seq_Worker_T.h"

#include "tao/debug.h"
#define DEBUG_OFFSET 0
#define DEBUG_LEVEL (TAO_debug_level + DEBUG_OFFSET)

typedef TAO_Notify_Find_Worker_T<TAO_Notify_EventChannel
                             , CosNotifyChannelAdmin::EventChannel
                             , CosNotifyChannelAdmin::EventChannel_ptr
                             , CosNotifyChannelAdmin::ChannelNotFound>
TAO_Notify_EventChannel_Find_Worker;

typedef TAO_Notify_Seq_Worker_T<TAO_Notify_EventChannel> TAO_Notify_EventChannel_Seq_Worker;

TAO_Notify_EventChannelFactory::TAO_Notify_EventChannelFactory (void)
  : ec_container_ (0)
  , topology_save_seq_ (0)
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
  if (this->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER) == 1)
    return;

  TAO_Notify_Properties* properties = TAO_Notify_PROPERTIES::instance();

  delete this->ec_container_;

  // Shutdown the ORB.
  CORBA::ORB_var orb = properties->orb ();
  orb->shutdown ();

  // Reset references to CORBA objects.
  properties->orb (CORBA::ORB::_nil ());
  properties->default_poa (PortableServer::POA::_nil ());
}

void
TAO_Notify_EventChannelFactory::init (PortableServer::POA_ptr poa ACE_ENV_ARG_DECL)
{
  this->default_filter_factory_ =
    TAO_Notify_PROPERTIES::instance()->builder()->build_filter_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Init ec_container_
  ACE_NEW_THROW_EX (this->ec_container_,
                    TAO_Notify_EventChannel_Container (),
                    CORBA::INTERNAL ());
  ACE_CHECK;

  this->ec_container_->init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  TAO_Notify_POA_Helper* object_poa = 0;

  // Bootstrap initial Object POA
  ACE_NEW_THROW_EX (object_poa,
                    TAO_Notify_POA_Helper (),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  auto_ptr<TAO_Notify_POA_Helper> auto_object_poa (object_poa);

  object_poa->init (poa ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // release auto_ref.
  auto_object_poa.release ();

  this->object_poa_own (object_poa);

  // We are also activated in the same Object POA.
  this->poa_ = this->object_poa_;

  // Make the Proxys acivate in this same POA.
  this->proxy_poa_ = this->object_poa_;

  // Note topology factory is configured separately from the "builder" mediated
  // objects since it is independant of the "style" of Notification Service.
  this->topology_factory_ =
    ACE_Dynamic_Service <TAO_NOTIFY::Topology_Factory>::instance ("Topology_Factory");

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
  this->ec_container_->remove (event_channel ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->self_change (ACE_ENV_SINGLE_ARG_PARAMETER);
}

int
TAO_Notify_EventChannelFactory::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  if (TAO_Notify_Object::shutdown (ACE_ENV_SINGLE_ARG_PARAMETER) == 1)
    return 1;

  this->ec_container_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
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
  ACE_CHECK_RETURN (ec._retn ());
  this->self_change (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (ec._retn ());
  return ec._retn ();
}

CosNotifyChannelAdmin::ChannelIDSeq*
TAO_Notify_EventChannelFactory::get_all_channels (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  TAO_Notify_EventChannel_Seq_Worker seq_worker;

  return seq_worker.create (*this->ec_container_ ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_Notify_EventChannelFactory::get_event_channel (CosNotifyChannelAdmin::ChannelID id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   , CosNotifyChannelAdmin::ChannelNotFound
                   ))
{
  TAO_Notify_EventChannel_Find_Worker find_worker;

  return find_worker.resolve (id, *this->ec_container_ ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_EventChannelFactory::set_topology_factory(TAO_NOTIFY::Topology_Factory* f)
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
    auto_ptr<TAO_NOTIFY::Topology_Loader> tl(this->topology_factory_->create_loader());
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
TAO_Notify_EventChannelFactory::save_persistent (TAO_NOTIFY::Topology_Saver& saver ACE_ENV_ARG_DECL)
{
  bool changed = this->self_changed_;
  this->self_changed_ = false;
  this->children_changed_ = false;

  TAO_NOTIFY::NVPList attrs; // ECF has no attributes

  bool want_all_children =
    saver.begin_object(0, "channel_factory", attrs, changed ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // for each deleted child
  //  delete_child  // if the child has persistence.

  TAO_NOTIFY::Save_Persist_Worker<TAO_Notify_EventChannel> wrk(saver, want_all_children);

  ACE_ASSERT(this->ec_container_ != 0);
  this->ec_container_->collection()->for_each(&wrk ACE_ENV_ARG_PARAMETER);
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
#ifdef EVENT_PERISISTENCE_SUPPORT //@@todo
  TAO_NOTIFY::Event_Persistence_Strategy * strategy =
    ACE_Dynamic_Service <TAO_NOTIFY::Event_Persistence_Strategy>::instance ("Event_Persistence");
  if (strategy != 0)
  {
    if (this->topology_factory_ != 0)
    {
      Event_Persistence_Factory * factory = strategy->get_factory ();
      if (factory != 0)
      {
        for (
          Routing_Slip_Persistence_Manager * rspm = factory->first_reload_manager();
          rspm != 0;
          rspm = rspm->load_next ())
        {
          TAO_NOTIFY::Routing_Slip_Ptr routing_slip = Routing_Slip::create (*this, rspm);
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
#else //EVENT_PERISISTENCE_SUPPORT //@@todo
#pragma message ("TODO: EVENT_PERISISTENCE_SUPPORT")
#endif //EVENT_PERISISTENCE_SUPPORT //@@todo
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
        auto_ptr<TAO_NOTIFY::Topology_Saver> saver(this->topology_factory_->create_saver());
        if (saver.get() != 0)
        {
          this->save_persistent(*saver ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN(false);
          saver->close (ACE_ENV_SINGLE_ARG_PARAMETER);
        }
        this->topology_save_seq_ += 1;
      }
    }
  }
  return saving;
}

TAO_NOTIFY::Topology_Object*
TAO_Notify_EventChannelFactory::load_child (const ACE_CString& type,
                                        CORBA::Long id,
                                        const TAO_NOTIFY::
                                        NVPList& attrs
                                        ACE_ENV_ARG_DECL)
{
  // ignore anything but our valid children (ie channel)
  TAO_NOTIFY::Topology_Object * result = this;
  if (type == "channel")
  {
    if (DEBUG_LEVEL) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) EventChannelFactory reload channel %d\n")
      , ACE_static_cast (int, id)
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
  else if (type == TAO_NOTIFY::REGISTRY_TYPE)
  {
    result = & this->reconnect_registry_;
  }
  return result;
}

void
TAO_Notify_EventChannelFactory::reconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  // Reconnect all children first
  TAO_NOTIFY::Reconnect_Worker<TAO_Notify_EventChannel> wrk;

  this->ec_container_->collection()->for_each(&wrk ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Then send reconnection announcement to registered clients
  CORBA::Object_var obj = this->poa()->id_to_reference (this->id () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyChannelAdmin::EventChannelFactory_var this_reference = CosNotifyChannelAdmin::EventChannelFactory::_narrow (obj.in ());
  ACE_ASSERT (CORBA::is_nil (this_reference.in ()));
  // todo: Is there an easier way?
  this->reconnect_registry_.send_reconnect (this_reference.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

#ifdef TODO_EVENT_PERSISTENCE //
  // reactivate events in-progress
  Routing_Slip_Set::CONST_ITERATOR iter (this->routing_slip_restart_set_);
  Routing_Slip_Ptr * routing_slip;
  for (iter.first(); iter.next(routing_slip); iter.advance())
  {
    (*routing_slip)->reconnect(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
  }
  this->routing_slip_restart_set_.reset ();
#else
  int TODO_Event_Persistence;
#endif
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Notify_Find_Worker_T<TAO_Notify_EventChannel
                              , CosNotifyChannelAdmin::EventChannel
                              , CosNotifyChannelAdmin::EventChannel_ptr
                              , CosNotifyChannelAdmin::ChannelNotFound>;
template class TAO_Notify_Seq_Worker_T<TAO_Notify_EventChannel>;

template class TAO_Notify_Container_T <TAO_Notify_EventChannel>;

template class TAO_ESF_Shutdown_Proxy<TAO_Notify_EventChannel>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_Notify_Find_Worker_T<TAO_Notify_EventChannel
                              , CosNotifyChannelAdmin::EventChannel
                              , CosNotifyChannelAdmin::EventChannel_ptr
                              , CosNotifyChannelAdmin::ChannelNotFound>
#pragma instantiate TAO_Notify_Seq_Worker_T<TAO_Notify_EventChannel>


#pragma instantiate TAO_Notify_Container_T <TAO_Notify_EventChannel>

#pragma instantiate TAO_ESF_Shutdown_Proxy<TAO_Notify_EventChannel>;

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
