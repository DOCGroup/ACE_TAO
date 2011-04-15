// -*- C++ -*-
// $Id$

#include "tao/CSD_Framework/CSD_Object_Adapter.h"
#include "tao/CSD_Framework/CSD_Strategy_Proxy.h"
#include "tao/CSD_Framework/CSD_POA.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CSD_Object_Adapter::TAO_CSD_Object_Adapter (
  const TAO_Server_Strategy_Factory::Active_Object_Map_Creation_Parameters &creation_parameters,
  TAO_ORB_Core &orb_core)
  :  TAO_Object_Adapter (creation_parameters, orb_core)
{

}

TAO_CSD_Object_Adapter::~TAO_CSD_Object_Adapter ()
{
}

void
TAO_CSD_Object_Adapter::do_dispatch (
  TAO_ServerRequest& req,
  TAO::Portable_Server::Servant_Upcall& upcall)
{
  TAO_Root_POA& poa = upcall.poa ();
  TAO_CSD_POA* csd_poa = dynamic_cast<TAO_CSD_POA*> (&poa);

  if (csd_poa == 0)
    {
      throw ::CORBA::BAD_PARAM ();
    }

  TAO::CSD::Strategy_Proxy& proxy
    = csd_poa->servant_dispatching_strategy_proxy ();
  proxy.dispatch_request (req, upcall);
}

TAO_END_VERSIONED_NAMESPACE_DECL
