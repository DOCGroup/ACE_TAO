// -*- C++ -*-
// $Id$

#include "tao/Codeset/Codeset_Service_Context_Handler.h"

#include "tao/CDR.h"
#include "tao/Transport.h"
#include "tao/ORB_Core.h"
#include "tao/operation_details.h"
#include "tao/Codeset_Manager.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_Codeset_Service_Context_Handler::process_service_context (
  TAO_Transport&,
  const IOP::ServiceContext&,
  TAO_ServerRequest*)
{
  return 0;
}

int
TAO_Codeset_Service_Context_Handler::generate_service_context (
  TAO_Stub *,
  TAO_Transport& transport,
  TAO_Operation_Details &opdetails,
  TAO_Target_Specification &,
  TAO_OutputCDR &)
{
  if (transport.first_request ())
    {
      TAO_Codeset_Manager * const csm = transport.orb_core ()->codeset_manager ();
      if (csm)
        {
          csm->generate_service_context (opdetails, transport);
        }
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
