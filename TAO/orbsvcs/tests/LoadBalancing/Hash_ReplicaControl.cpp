// -*- C++ -*-

// $Id$

#include "Hash_ReplicaControl.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

Hash_ReplicaControl::Hash_ReplicaControl (void)
  : adapter_ (this),
    replica_ (this),
    interval_start_ (ACE_OS::gettimeofday ()),
    request_count_ (0),
    current_load_ (0)
{
}

void
Hash_ReplicaControl::init (CORBA::ORB_ptr orb,
                           LoadBalancing::LoadBalancer_ptr balancer
                           ACE_ENV_ARG_DECL)
{
  ACE_DEBUG ((LM_DEBUG,
              "Hash_ReplicaControl::init\n"));
  ACE_Time_Value interval (1, 0);
  ACE_Time_Value restart (1, 0);
  ACE_Reactor *reactor = orb->orb_core ()->reactor ();
  reactor->schedule_timer (&this->adapter_, 0, interval, restart);

  LoadBalancing::ReplicaControl_var control =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Object_var replica =
    this->replica_._this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->group_ =
    balancer->group_identity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->proxy_ =
    balancer->connect (control.in (),
                       replica.in ()
                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

int
Hash_ReplicaControl::handle_timeout (const ACE_Time_Value &,
                                     const void *)
{
  ACE_Time_Value elapsed_time =
    ACE_OS::gettimeofday () - this->interval_start_;
  this->interval_start_ = ACE_OS::gettimeofday ();

  CORBA::Float load =
    CORBA::Float(this->request_count_) / elapsed_time.msec ();
  this->request_count_ = 0;
  this->interval_start_ = ACE_OS::gettimeofday ();

  // @@ Ossama: here is the dampening algorithm that i implemented, it
  // is not rocket science, but helps...
  this->current_load_ =
    0.9F * this->current_load_ + 0.1F * load;

  ACE_TRY_NEW_ENV
    {
      this->proxy_->current_load (this->current_load_
                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG, "Current_Load = %f\n", this->current_load_));
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;
  return 0;
}

void
Hash_ReplicaControl::request_received (void)
{
  this->request_count_++;
}


void
Hash_ReplicaControl::request_rejected (ACE_ENV_SINGLE_ARG_DECL)
{
  // @@ Ossama: notice how we reject a single request.  Maybe the
  // advisory should include how many are we supposed to shed?
  this->replica_.reject_requests (0);
  ACE_THROW (PortableServer::ForwardRequest (
                 CORBA::Object::_duplicate (this->group_.in ())));

}

void
Hash_ReplicaControl::high_load_advisory (ACE_ENV_SINGLE_ARG_DECL_NOT_USED
                                         /* ACE_ENV_SINGLE_ARG_PARAMETER */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Notify the replica that it should reject all requests.
  this->replica_.reject_requests (1);
  ACE_DEBUG ((LM_DEBUG, "**** Load is high\n"));
}

void
Hash_ReplicaControl::nominal_load_advisory (ACE_ENV_SINGLE_ARG_DECL_NOT_USED
                                            /* ACE_ENV_SINGLE_ARG_PARAMETER */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Notify the replica that it should once again accept requests.
  this->replica_.reject_requests (0);
  ACE_DEBUG ((LM_DEBUG, "**** Load is nominal\n"));
}

// ****************************************************************

Timeout_Adapter::Timeout_Adapter (Hash_ReplicaControl *adaptee)
  :  adaptee_ (adaptee)
{
}

int
Timeout_Adapter::handle_timeout (const ACE_Time_Value &current_time,
                                 const void *arg)
{
  return this->adaptee_->handle_timeout (current_time, arg);
}
