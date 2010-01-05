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

#include <algorithm>

#if !defined(__ACE_INLINE__)
#include "Implicit_Deactivator.inl"
#endif /* __ACE_INLINE__ */

Implicit_Deactivator::Implicit_Deactivator (PortableServer::Servant servant)
{
  this->poa_ =
    servant->_default_POA ();

  this->id_ =
    this->poa_->servant_to_id (servant);
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
  std::swap (this->poa_, tmp.poa_);
  std::swap (this->id_, tmp.id_);
  return *this;
}

Implicit_Deactivator::~Implicit_Deactivator ()
{
  if (this->id_.ptr () == 0)
    return;

  try
  {
    this->poa_->deactivate_object (this->id_.in ());
  }
  catch (const CORBA::Exception&)
  {
    // @@ TODO This event should be logged. Cannot throw because that
    //    would make it impossible to use this class effectively.
    //    Read Servant_var.cpp for more details.
  }
}

Implicit_Deactivator&
Implicit_Deactivator::operator= (PortableServer::Servant servant)
{
  Implicit_Deactivator tmp (servant);
  // @@ This seems bogus, there should be a more efficient way to swap
  //    vars
  std::swap (this->poa_, tmp.poa_);
  std::swap (this->id_, tmp.id_);
  return *this;
}
