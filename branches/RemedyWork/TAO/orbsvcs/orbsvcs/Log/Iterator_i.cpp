// $Id$

#include "orbsvcs/Log/Iterator_i.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// iterator inactivity timeout
ACE_Time_Value
TAO_Iterator_i::timeout_(60 * 60);

TAO_Iterator_i::TAO_Iterator_i (PortableServer::POA_ptr poa,
                                ACE_Reactor* reactor)
  : poa_ (PortableServer::POA::_duplicate (poa)),
    reactor_ (reactor)
{
   if (this->timeout_ != ACE_Time_Value::zero)
     {
       this->timer_id_ =
         this->reactor_->schedule_timer (this, 0, this->timeout_);
     }
}


TAO_Iterator_i::~TAO_Iterator_i (void)
{
  // cancel timer
  if (this->timer_id_ != -1)
    {
      this->reactor_->cancel_timer (this->timer_id_);
    }
}


void
TAO_Iterator_i::destroy (void)
{
  PortableServer::ObjectId_var oid =
    this->poa_->servant_to_id (this);

  // Goodbye cruel world...
  // deactivate from the poa.
  this->poa_->deactivate_object (oid.in ());
  return;
}


int
TAO_Iterator_i::handle_timeout(const ACE_Time_Value&, const void*)
{
  try
    {
      this->destroy ();
    }
  catch (const CORBA::Exception&)
    {
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
