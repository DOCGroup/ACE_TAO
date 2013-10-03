// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/Object.h"
#include "orbsvcs/Notify/POA_Helper.h"
#include "orbsvcs/Notify/Worker_Task.h"
#include "orbsvcs/Notify/Properties.h"
#include "orbsvcs/Notify/Builder.h"
#include "orbsvcs/Notify/ThreadPool_Task.h"
#include "orbsvcs/Notify/Reactive_Task.h"
#include "tao/debug.h"
#include "orbsvcs/Notify/Event_Manager.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Object.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_Object::TAO_Notify_Object (void)
: poa_ (0)
, proxy_poa_ (0)
, own_proxy_poa_ (false)
, object_poa_ (0)
, own_object_poa_ (false)
, id_ (0)
, own_worker_task_ (false)
, shutdown_ (false)
{
  if (TAO_debug_level > 2 )
    ORBSVCS_DEBUG ((LM_DEBUG,"object:%x  created\n", this ));
}

TAO_Notify_Object::~TAO_Notify_Object ()
{
  if (TAO_debug_level > 2 )
    ORBSVCS_DEBUG ((LM_DEBUG,"object:%x  destroyed\n", this ));

  this->destroy_proxy_poa ();
  this->destroy_object_poa ();
  this->destroy_poa ();
}

void
TAO_Notify_Object::initialize (TAO_Notify_Object* parent)
{
  ACE_ASSERT (parent != 0 && this->event_manager_.get() == 0);

  // Do not use sets to avoid asserts.
  // Object must be able to inherit NULL references
  // due to current design.
  this->event_manager_ = parent->event_manager_;
  this->admin_properties_ = parent->admin_properties_;
  this->inherit_poas( *parent );
  this->worker_task_ = parent->worker_task_;

  // Pass  QoS
  parent->qos_properties_.transfer (this->qos_properties_);
  this->qos_changed (this->qos_properties_);
}

void
TAO_Notify_Object::set_event_manager( TAO_Notify_Event_Manager* event_manager )
{
  ACE_ASSERT( event_manager != 0 );
  this->event_manager_.reset( event_manager );
}

CORBA::Object_ptr
TAO_Notify_Object::activate (PortableServer::Servant servant)
{
  return this->poa_->activate (servant, this->id_);
}

/// Activate with existing id
CORBA::Object_ptr
TAO_Notify_Object::activate (
    PortableServer::Servant servant,
    CORBA::Long id)
{
  this->id_ = id;
  return this->poa_->activate_with_id (servant, this->id_);
}


void
TAO_Notify_Object::deactivate (void)
{
  try
  {
    this->poa_->deactivate (this->id_);
  }
  catch (const CORBA::Exception& ex)
  {
    // Do not propagate any exceptions
    if (TAO_debug_level > 2)
    {
      ex._tao_print_exception ("(%P|%t)\n");
      ORBSVCS_DEBUG ((LM_DEBUG, "Could not deactivate object %d\n", this->id_));
    }
  }
}

int
TAO_Notify_Object::shutdown (void)
{
  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 1);

    if (this->shutdown_ == 1)
      return 1; // Another thread has already run shutdown.

    this->shutdown_ = 1;
  }

  this->deactivate ();

  this->shutdown_worker_task ();

  return 0;
}

CORBA::Object_ptr
TAO_Notify_Object::ref (void)
{
  return this->poa_->id_to_reference (this->id_);
}

void
TAO_Notify_Object::shutdown_worker_task (void)
{
  // Only do this if we are the owner.
  TAO_Notify_Worker_Task::Ptr task( this->worker_task_ );
  this->worker_task_.reset();
  if ( task.isSet() )
  {
    if ( this->own_worker_task_ )
    {
      task->shutdown ();
    }
  }
}

void
TAO_Notify_Object::destroy_proxy_poa (void)
{
  if (this->proxy_poa_ != 0)
  {
    try
    {
      if ( this->proxy_poa_ == this->object_poa_ ) this->object_poa_ = 0;
      if ( this->proxy_poa_ == this->poa_ ) this->poa_ = 0;

      if ( this->own_proxy_poa_ == true )
      {
        this->own_proxy_poa_ = false;
        ACE_Auto_Ptr< TAO_Notify_POA_Helper > app( proxy_poa_ );
        this->proxy_poa_->destroy ();
      }
      this->proxy_poa_ = 0;
    }
    catch (const CORBA::Exception& ex)
    {
      if (TAO_debug_level > 2)
        ex._tao_print_exception ("Proxy shutdown error (%P|%t)\n");
    }
  }
}

void
TAO_Notify_Object::destroy_object_poa (void)
{
  if (this->object_poa_ != 0)
  {
    try
    {
      if ( this->object_poa_ == this->proxy_poa_ ) this->proxy_poa_ = 0;
      if ( this->object_poa_ == this->poa_ ) this->poa_ = 0;

      if ( this->own_object_poa_ == true )
      {
        this->own_object_poa_ = false;
        ACE_Auto_Ptr< TAO_Notify_POA_Helper > aop( object_poa_ );
        this->object_poa_->destroy ();
      }
      this->object_poa_ = 0;
    }
    catch (const CORBA::Exception& ex)
    {
      if (TAO_debug_level > 2)
        ex._tao_print_exception ("Proxy shutdown error (%P|%t)\n");
    }
  }
}

/// Shutdown the current poa.
void TAO_Notify_Object::destroy_poa (void)
{
  this->poa_ = 0;
}

void
TAO_Notify_Object::set_worker_task (TAO_Notify_Worker_Task* worker_task)
{
  ACE_ASSERT( worker_task != 0 );

  // shutdown the current worker.
  this->shutdown_worker_task ();

  this->worker_task_.reset (worker_task);

  this->own_worker_task_ = true;
}

void
TAO_Notify_Object::set_proxy_poa (TAO_Notify_POA_Helper* proxy_poa)
{
  // shutdown current proxy poa.
  this->destroy_proxy_poa ();

  this->proxy_poa_ = proxy_poa;

  this->own_proxy_poa_ = true;
}

void
TAO_Notify_Object::set_object_poa (TAO_Notify_POA_Helper* object_poa)
{
  // shutdown current object poa.
  this->destroy_object_poa ();

  this->object_poa_ = object_poa;

  this->own_object_poa_ = true;
}

void
TAO_Notify_Object::set_poa (TAO_Notify_POA_Helper* poa)
{
  this->poa_ = poa;
}

void
TAO_Notify_Object::set_qos (const CosNotification::QoSProperties & qos)
{
  CosNotification::PropertyErrorSeq err_seq;

  TAO_Notify_QoSProperties new_qos_properties;

  if (new_qos_properties.init (qos, err_seq) == -1)
    throw CORBA::INTERNAL ();

  // Apply the appropriate concurrency QoS
  if (new_qos_properties.thread_pool ().is_valid ())
  {
    if (new_qos_properties.thread_pool ().value ().static_threads == 0)
      {
        TAO_Notify_PROPERTIES::instance()->builder()->apply_reactive_concurrency (*this);
      }
    else
      {
        TAO_Notify_PROPERTIES::instance()->builder()->
        apply_thread_pool_concurrency (*this, new_qos_properties.thread_pool ().value ());
      }
  }
  else if (new_qos_properties.thread_pool_lane ().is_valid ())
    TAO_Notify_PROPERTIES::instance()->builder()->
    apply_lane_concurrency (*this, new_qos_properties.thread_pool_lane ().value ());

  // Update the Thread Task's QoS properties..
  this->worker_task_->update_qos_properties (new_qos_properties);

  // Inform subclasses of QoS changed.
  this->qos_changed (new_qos_properties);

  // Init the the overall QoS on this object.
  if (new_qos_properties.copy (this->qos_properties_) == -1)
    throw CORBA::INTERNAL ();

  if (err_seq.length () > 0) // Unsupported Property
    throw CosNotification::UnsupportedQoS (err_seq);
}

CosNotification::QoSProperties*
TAO_Notify_Object::get_qos (void)
{
  CosNotification::QoSProperties_var properties;

  ACE_NEW_THROW_EX (properties,
    CosNotification::QoSProperties (),
    CORBA::NO_MEMORY ());

  this->qos_properties_.populate (properties);

  return properties._retn ();
}

bool
TAO_Notify_Object::find_qos_property_value (
  const char * name,
  CosNotification::PropertyValue & value) const
{
  // qos_properties_ is essentially a map and the find() method returns
  // zero on success.  We must convert this to a boolean value.
  return (this->qos_properties_.find (name, value) == 0);
}


void
TAO_Notify_Object::qos_changed (const TAO_Notify_QoSProperties& /*qos_properties*/)
{
  // NOP.
}

TAO_Notify_Timer*
TAO_Notify_Object::timer (void)
{
  ACE_ASSERT (worker_task_.get() != 0);
  return this->worker_task_->timer ();
}

namespace
{
  template<class T>
  void add_qos_attr(TAO_Notify::NVPList& attrs, const T& prop)
  {
    if (prop.is_valid())
    {
      attrs.push_back(TAO_Notify::NVP (prop));
    }
  }
} // namespace

void
TAO_Notify_Object::save_attrs (TAO_Notify::NVPList& attrs)
{
  add_qos_attr(attrs, this->qos_properties_.event_reliability ());
  add_qos_attr(attrs, this->qos_properties_.connection_reliability ());
  add_qos_attr(attrs, this->qos_properties_.priority ());
  add_qos_attr(attrs, this->qos_properties_.timeout ());
  add_qos_attr(attrs, this->qos_properties_.stop_time_supported ());
  add_qos_attr(attrs, this->qos_properties_.maximum_batch_size ());
  add_qos_attr(attrs, this->qos_properties_.pacing_interval ());
}

void
TAO_Notify_Object::load_attrs(const TAO_Notify::NVPList& attrs)
{
  attrs.load (this->qos_properties_.event_reliability ());
  attrs.load (this->qos_properties_.connection_reliability ());
  attrs.load (this->qos_properties_.priority ());
  attrs.load (this->qos_properties_.timeout ());
  attrs.load (this->qos_properties_.stop_time_supported ());
  attrs.load (this->qos_properties_.maximum_batch_size ());
  attrs.load (this->qos_properties_.pacing_interval ());
  this->qos_properties_.init ();
}

TAO_Notify_Worker_Task*
TAO_Notify_Object::get_worker_task (void)
{
  return this->worker_task_.get ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
