//
// $Id$
//
#include "Oneway_Receiver.h"
#include "Shutdown_Helper.h"

ACE_RCSID(Reliable_Oneways, Oneway_Receiver, "$Id$")

Oneway_Receiver::Oneway_Receiver (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Oneway_Receiver::raise_no_permission (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW (CORBA::NO_PERMISSION ());
}

void
Oneway_Receiver::destroy (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::POA_var poa = this->_default_POA (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  poa->deactivate_object (id.in () TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

Test::Shutdown_Helper_ptr
Oneway_Receiver::get_shutdown_helper (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Shutdown_Helper *shutdown_helper;
  ACE_NEW_THROW_EX (shutdown_helper,
                    Shutdown_Helper (this->orb_.in ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (Test::Shutdown_Helper::_nil ());

  PortableServer::ServantBase_var transfer_ownership(shutdown_helper);

  return shutdown_helper->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
}
