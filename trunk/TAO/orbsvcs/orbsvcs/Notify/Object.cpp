// $Id$

#include "Object.h"
#include "POA_Helper.h"
#include "Worker_Task.h"
#include "Properties.h"
#include "Builder.h"
#include "ThreadPool_Task.h"
#include "Reactive_Task.h"
#include "tao/debug.h"

#if ! defined (__ACE_INLINE__)
#include "Object.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_Object, "$Id$")



TAO_Notify_Object::TAO_Notify_Object (void)
  : event_manager_ (0)
  , admin_properties_ (0)
  , id_ (0)
  , poa_ (0)
  , proxy_poa_ (0)
  , own_proxy_poa_ (0)
  , object_poa_ (0)
  , own_object_poa_ (0)
  , worker_task_ (0)
  , own_worker_task_ (0)
  , shutdown_ (0)
{
  if (TAO_debug_level > 2 )
    ACE_DEBUG ((LM_DEBUG,"object:%x  created\n", this ));
}

TAO_Notify_Object::~TAO_Notify_Object ()
{
  if (TAO_debug_level > 2 )
    ACE_DEBUG ((LM_DEBUG,"object:%x  destroyed\n", this ));

  this->shutdown_proxy_poa ();
  this->shutdown_object_poa ();
}

void
TAO_Notify_Object::initialize (TAO_Notify_Object* parent)
{
  this->event_manager_ = parent->event_manager_;
  this->admin_properties_ = parent->admin_properties_;

  this->poa_ = parent->poa_;
  this->object_poa_ = parent->object_poa_;
  this->proxy_poa_ = parent->proxy_poa_;
  this->worker_task_ = parent->worker_task_;

  if (this->worker_task_)
    this->worker_task_->_incr_refcnt ();

  // Pass  QoS
  parent->qos_properties_.transfer (this->qos_properties_);
  this->qos_changed (this->qos_properties_);
}

CORBA::Object_ptr
TAO_Notify_Object::activate (PortableServer::Servant servant ACE_ENV_ARG_DECL)
{
  return this->poa_->activate (servant, this->id_ ACE_ENV_ARG_PARAMETER);
}

/// Activate with existing id
CORBA::Object_ptr
TAO_Notify_Object::activate (
    PortableServer::Servant servant,
    CORBA::Long id
    ACE_ENV_ARG_DECL)
{
  this->id_ = id;
  return this->poa_->activate_with_id (servant, this->id_ ACE_ENV_ARG_PARAMETER);
}


void
TAO_Notify_Object::deactivate (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_TRY
    {
      this->poa_->deactivate (this->id_ ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level > 2)
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "(%P|%t)\n");
    ACE_DEBUG ((LM_DEBUG, "Could not deactivate object %d\n", this->id_));
  }
      // Do not propagate any exceptions
    }
  ACE_ENDTRY;
}

int
TAO_Notify_Object::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 1);

    if (this->shutdown_ == 1)
      return 1; // Another thread has already run shutdown.

    this->shutdown_ = 1;
  }

  this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);

  this->shutdown_worker_task ();

  return 0;
}

CORBA::Object_ptr
TAO_Notify_Object::ref (ACE_ENV_SINGLE_ARG_DECL)
{
  return this->poa_->id_to_reference (this->id_ ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_Object::shutdown_worker_task (void)
{
  // Only do this if we are the owner.
  if (this->own_worker_task_ == 1)
    {
      this->worker_task_->shutdown (); // the worker deletes itself when its <close> hook is eventually called.
    }

  if (this->worker_task_)
    this->worker_task_->_decr_refcnt ();
}

void
TAO_Notify_Object::shutdown_proxy_poa (void)
{
  if (this->own_proxy_poa_ == 1)
    {
      ACE_TRY_NEW_ENV
        {
          this->proxy_poa_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          delete this->proxy_poa_;
        }
      ACE_CATCHANY
        {
          if (TAO_debug_level > 2)
            ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                                 "Proxy shutdown error (%P|%t)\n");
        }
      ACE_ENDTRY;
    }
}

void
TAO_Notify_Object::shutdown_object_poa (void)
{
  if (this->own_object_poa_ == 1)
    {
      ACE_TRY_NEW_ENV
        {
          this->object_poa_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          delete this->object_poa_;
        }
      ACE_CATCHANY
        {
          if (TAO_debug_level > 2)
            ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                                 "Proxy shutdown error (%P|%t)\n");
        }
      ACE_ENDTRY;
    }
}

void
TAO_Notify_Object::worker_task_own (TAO_Notify_Worker_Task* worker_task)
{
  this->worker_task (worker_task);

  // claim ownership.
  this->own_worker_task_ = 1;
}

void
TAO_Notify_Object::worker_task (TAO_Notify_Worker_Task* worker_task)
{
  // shutdown the current worker.
  this->shutdown_worker_task ();

  this->worker_task_ = worker_task;

  this->own_worker_task_ = 0;

  if (this->worker_task_)
    this->worker_task_->_incr_refcnt ();
}

void
TAO_Notify_Object::proxy_poa_own (TAO_Notify_POA_Helper* proxy_poa)
{
  // shutdown current proxy poa.
  this->shutdown_proxy_poa ();

  this->proxy_poa_ = proxy_poa;

  // claim ownership.
  own_proxy_poa_ = 1;
}

void
TAO_Notify_Object::object_poa_own (TAO_Notify_POA_Helper* object_poa)
{
  // shutdown current object poa.
  this->shutdown_object_poa ();

  // shutdown current object poa
  this->object_poa_ = object_poa;

  // claim ownership.
  this->own_object_poa_ = 1;
}

void
TAO_Notify_Object::set_qos (const CosNotification::QoSProperties & qos ACE_ENV_ARG_DECL)
{
  CosNotification::PropertyErrorSeq err_seq;

  TAO_Notify_QoSProperties new_qos_properties;

  if (new_qos_properties.init (qos, err_seq) == -1)
    ACE_THROW (CORBA::INTERNAL ());

  // Apply the appropriate concurrency QoS
  if (new_qos_properties.thread_pool ().is_valid ())
    {
      if (new_qos_properties.thread_pool ().value ().static_threads == 0)
        TAO_Notify_PROPERTIES::instance()->builder()->apply_reactive_concurrency (*this ACE_ENV_ARG_PARAMETER);
      else
        TAO_Notify_PROPERTIES::instance()->builder()->
          apply_thread_pool_concurrency (*this, new_qos_properties.thread_pool ().value () ACE_ENV_ARG_PARAMETER);
    }
  else if (new_qos_properties.thread_pool_lane ().is_valid ())
    TAO_Notify_PROPERTIES::instance()->builder()->
      apply_lane_concurrency (*this, new_qos_properties.thread_pool_lane ().value () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Update the Thread Task's QoS properties..
  this->worker_task_->update_qos_properties (new_qos_properties);

  // Inform subclasses of QoS changed.
  this->qos_changed (new_qos_properties);

  // Init the the overall QoS on this object.
  if (new_qos_properties.copy (this->qos_properties_) == -1)
    ACE_THROW (CORBA::INTERNAL ());

  if (err_seq.length () > 0) // Unsupported Property
    ACE_THROW (CosNotification::UnsupportedQoS (err_seq));
}

CosNotification::QoSProperties*
TAO_Notify_Object::get_qos (ACE_ENV_SINGLE_ARG_DECL)
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
  return this->qos_properties_.find (name, value);
}


void
TAO_Notify_Object::qos_changed (const TAO_Notify_QoSProperties& /*qos_properties*/)
{
  // NOP.
}

TAO_Notify_Timer*
TAO_Notify_Object::timer (void)
{
  return this->worker_task_->timer ();
}

namespace {
  template<class T>
    void add_qos_attr(TAO_Notify::NVPList& attrs, const T& prop) {
      if (prop.is_valid())
      {
        attrs.push_back(TAO_Notify::NVP (prop));
      }
    }
// Note : These instantiations have to be here because each namespace {}
// is unique.
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template void add_qos_attr<TAO_Notify_Property_Boolean>(
  TAO_Notify::NVPList&, const TAO_Notify_Property_Boolean&);
template void add_qos_attr<TAO_Notify_Property_T<int> >(
  TAO_Notify::NVPList&, const TAO_Notify_Property_T<int>&);
template void add_qos_attr<TAO_Notify_Property_T<unsigned long long> >(
  TAO_Notify::NVPList&, const TAO_Notify_Property_T<unsigned long long>&);
template void add_qos_attr<TAO_Notify_Property_T<short> >(
  TAO_Notify::NVPList&, const TAO_Notify_Property_T<short>&);
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate void add_qos_attr<TAO_Notify_Property_Boolean>(\
  TAO_Notify::NVPList&, const TAO_Notify_Property_Boolean&)
#pragma instantiate void add_qos_attr<TAO_Notify_Property_T<int> >(\
  TAO_Notify::NVPList&, const TAO_Notify_Property_T<int>&)
#pragma instantiate void add_qos_attr<TAO_Notify_Property_T<unsigned long long> >(\
  TAO_Notify::NVPList&, const TAO_Notify_Property_T<unsigned long long>&)
#pragma instantiate void add_qos_attr<TAO_Notify_Property_T<short> >(\
  TAO_Notify::NVPList&, const TAO_Notify_Property_T<short>&)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
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

