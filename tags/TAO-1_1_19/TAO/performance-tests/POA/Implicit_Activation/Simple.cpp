//
// $Id$
//
#include "Simple.h"

ACE_RCSID(Activation, Simple, "$Id$")

Simple::Simple (void)
{
}

void
Simple::destroy (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::POA_var poa = this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::ObjectId_var oid =
    poa->servant_to_id (this, ACE_TRY_ENV);
  ACE_CHECK;

  poa->deactivate_object (oid.in (), ACE_TRY_ENV);
  ACE_CHECK;
}
