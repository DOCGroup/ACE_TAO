// $Id$

#include "Object.h"
#include "Properties.h"
#include "Factory.h"
#include "POA_Helper.h"
#include "Worker_Task.h"
#include "QoSAdmin.h"
#include "tao/debug.h"

#if ! defined (__ACE_INLINE__)
#include "Object.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Object, "$Id$")

TAO_NS_Object::TAO_NS_Object (void)
  :event_manager_ (0), qos_admin_ (0), id_ (0), poa_ (0), worker_task_ (0), delete_worker_task_ (0)
{
  if (TAO_debug_level > 1 )
    ACE_DEBUG ((LM_DEBUG,"object:%x  created\n", this ));
}

TAO_NS_Object::~TAO_NS_Object ()
{
  if (TAO_debug_level > 1 )
    ACE_DEBUG ((LM_DEBUG,"object:%x  destroyed\n", this ));

  this->shutdown_worker_task ();

  delete qos_admin_;
}

void
TAO_NS_Object::init (TAO_NS_POA_Helper* poa, TAO_NS_Worker_Task* worker_task ACE_ENV_ARG_DECL)
{
  poa_ = poa;
  worker_task_ = worker_task;

  TAO_NS_Factory* factory = TAO_NS_PROPERTIES::instance ()->factory ();
  factory->create (this->qos_admin_ ACE_ENV_ARG_PARAMETER);
}

CORBA::Object_ptr
TAO_NS_Object::activate (ACE_ENV_SINGLE_ARG_DECL)
{
  return poa_->activate (this->servant (), id_ ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_Object::deactivate (ACE_ENV_SINGLE_ARG_DECL)
{
  poa_->deactivate (id_ ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_Object::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Object_ptr
TAO_NS_Object::ref (ACE_ENV_SINGLE_ARG_DECL)
{
  return poa_->id_to_reference (id_ ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_Object::shutdown_worker_task (void)
{
  /// Only do this if we are the owner.
  if (delete_worker_task_ == 1)
    {
      this->worker_task_->shutdown ();
      delete this->worker_task_;
    }
}

void
TAO_NS_Object::worker_task_own (TAO_NS_Worker_Task* worker_task)
{
  this->worker_task (worker_task);

  /// claim ownership.
  delete_worker_task_ = 1;
}

void
TAO_NS_Object::worker_task (TAO_NS_Worker_Task* worker_task)
{
  /// shutdown the current worker.
  this->shutdown_worker_task ();

  this->worker_task_ = worker_task;

  delete_worker_task_ = 0;
}
