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
                                                 IR_InterfaceDef *)
{
  return 0;
}

void 
TAO_IFR_Client_Adapter::interfacedef_any_insert (CORBA_Any &,
                                                 IR_InterfaceDef *)
{
}

IR_InterfaceDef * 
TAO_IFR_Client_Adapter::get_interface (CORBA::ORB_ptr,
                                       const char *,
                                       CORBA::Environment &)
{
  return 0;
}

CORBA_IRObject_ptr
TAO_IFR_Client_Adapter::get_interface_def (CORBA::ORB_ptr,
                                           const char *,
                                           CORBA::Environment &)
{
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_IFR_Client_Adapter>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_IFR_Client_Adapter>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


