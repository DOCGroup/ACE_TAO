// $Id$

#include "orbsvcs/LoadBalancing/LB_ORBInitializer.h"
#include "orbsvcs/LoadBalancing/LB_IORInterceptor.h"
#include "orbsvcs/LoadBalancing/LB_ServerRequestInterceptor.h"

#include "orbsvcs/CosLoadBalancingC.h"

#include "tao/ORB_Constants.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_LB_ORBInitializer::TAO_LB_ORBInitializer (
  const CORBA::StringSeq & object_groups,
  const CORBA::StringSeq & repository_ids,
  const char * location)
  : object_groups_ (object_groups),
    repository_ids_ (repository_ids),
    location_ (location),
    load_alert_ ()
{
}

void
TAO_LB_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr)
{
}

void
TAO_LB_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  CORBA::Object_var obj =
    info->resolve_initial_references ("LoadManager");

  CosLoadBalancing::LoadManager_var lm =
    CosLoadBalancing::LoadManager::_narrow (obj.in ());

  CORBA::String_var orbid = info->orb_id ();

  PortableInterceptor::IORInterceptor_ptr tmp;
  ACE_NEW_THROW_EX (tmp,
                    TAO_LB_IORInterceptor (this->object_groups_,
                                           this->repository_ids_,
                                           this->location_.in (),
                                           lm.in (),
                                           orbid.in (),
                                           this->load_alert_),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableInterceptor::IORInterceptor_var ior_interceptor = tmp;

  info->add_ior_interceptor (ior_interceptor.in ());

  // ----------------

  PortableInterceptor::ServerRequestInterceptor_ptr sri;
  ACE_NEW_THROW_EX (sri,
                    TAO_LB_ServerRequestInterceptor (this->load_alert_),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableInterceptor::ServerRequestInterceptor_var sr_interceptor = sri;

  info->add_server_request_interceptor (sr_interceptor.in ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
