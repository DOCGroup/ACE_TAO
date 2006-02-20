/**
 * @file Implicit_Deactivator.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "Implicit_Deactivator.h"
#include "tao/PortableServer/Servant_Base.h"
#include "tao/Environment.h"
#include "ace/Swap.h"

#if !defined(__ACE_INLINE__)
#include "Implicit_Deactivator.inl"
#endif /* __ACE_INLINE__ */

Implicit_Deactivator::Implicit_Deactivator (PortableServer::Servant servant
                                            ACE_ENV_ARG_DECL)
{
  this->poa_ =
    servant->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->id_ =
    this->poa_->servant_to_id (servant ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

Implicit_Deactivator::Implicit_Deactivator (Implicit_Deactivator &rhs)
  : poa_ (rhs.poa_)
  , id_ (rhs.id_)
{
  rhs.release ();
}

Implicit_Deactivator&
Implicit_Deactivator::operator= (Implicit_Deactivator &rhs)
{
  Implicit_Deactivator tmp (rhs);
  // @@ This seems bogus, there should be a more efficient way to swap
  //    vars
  ACE_Swap<PortableServer::POA_var>::swap (this->poa_, tmp.poa_);
  ACE_Swap<PortableServer::ObjectId_var>::swap (this->id_, tmp.id_);
  return *this;
}

Implicit_Deactivator::~Implicit_Deactivator ()
{
  if (this->id_.ptr () == 0)
    return;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY {
    this->poa_->deactivate_object (this->id_.in ()
                                   ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
  } ACE_CATCHANY {
    // @@ TODO This event should be logged. Cannot throw because that
    //    would make it impossible to use this class effectively.
    //    Read Servant_var.cpp for more details.
  } ACE_ENDTRY;
}

Implicit_Deactivator&
Implicit_Deactivator::operator= (PortableServer::Servant servant)
{
  Implicit_Deactivator tmp (servant);
  // @@ This seems bogus, there should be a more efficient way to swap
  //    vars
  ACE_Swap<PortableServer::POA_var>::swap (this->poa_, tmp.poa_);
  ACE_Swap<PortableServer::ObjectId_var>::swap (this->id_, tmp.id_);
  return *this;
}
