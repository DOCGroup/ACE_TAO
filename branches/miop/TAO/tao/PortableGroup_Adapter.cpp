// @(#) $Id$

#include "tao/PortableGroup_Adapter.h"
#include "ace/Dynamic_Service.h"
#include "tao/Object.h"
#include "tao/Environment.h"

ACE_RCSID(tao, PortableGroup_Adapter, "$Id$")


TAO_PortableGroup_Adapter::~TAO_PortableGroup_Adapter (void)
{
}

void
TAO_PortableGroup_Adapter::activate (CORBA::ORB_ptr,
                                     int,
                                     char *[],
                                     CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

CORBA::Object_ptr
TAO_PortableGroup_Adapter::miop (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC (())
{
  ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                    CORBA::Object::_nil ());
}

TAO_POA_PortableGroup_Hooks *
TAO_PortableGroup_Adapter::poa_hooks (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    0);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_PortableGroup_Adapter>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_PortableGroup_Adapter>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


