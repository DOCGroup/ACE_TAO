// @(#) $Id$

#include "tao/IFR_Client_Adapter.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID(tao, IFR_Client_Adapter, "$Id$")

TAO_IFR_Client_Adapter::~TAO_IFR_Client_Adapter (void)
{
}

// Methods to be overridden.

CORBA::Boolean
TAO_IFR_Client_Adapter::interfacedef_cdr_insert (TAO_OutputCDR &,
                                                 CORBA_InterfaceDef_ptr)
{
  return 0;
}

void
TAO_IFR_Client_Adapter::interfacedef_any_insert (CORBA_Any &,
                                                 CORBA_InterfaceDef_ptr)
{
}

void
TAO_IFR_Client_Adapter::dispose (CORBA_InterfaceDef_ptr)
{
}

CORBA_InterfaceDef_ptr
TAO_IFR_Client_Adapter::get_interface (CORBA::ORB_ptr,
                                       const char *
                                       TAO_ENV_ARG_DECL_NOT_USED)
{
  return 0;
}

CORBA_InterfaceDef_ptr
TAO_IFR_Client_Adapter::get_interface_remote (const CORBA::Object_ptr
                                              TAO_ENV_ARG_DECL_NOT_USED)
{
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_IFR_Client_Adapter>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_IFR_Client_Adapter>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


