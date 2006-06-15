#include "orbsvcs/Log/Iterator_i.h"

ACE_RCSID (Log,
           Iterator_i,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// iterator inactivity timeout
ACE_Time_Value
TAO_Iterator_i::timeout_(60 * 60);

TAO_Iterator_i::TAO_Iterator_i (ACE_Reactor* reactor)
  : reactor_ (reactor)
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
TAO_Iterator_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::POA_ptr poa = 
    this->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::ObjectId_var oid =
    poa->servant_to_id (this
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Goodbye cruel world...
  // deactivate from the poa.
  poa->deactivate_object (oid.in ()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  return;
}


int
TAO_Iterator_i::handle_timeout(const ACE_Time_Value&, const void*)
{
  ACE_TRY_NEW_ENV
    {
      this->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
    }
  ACE_ENDTRY;

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
