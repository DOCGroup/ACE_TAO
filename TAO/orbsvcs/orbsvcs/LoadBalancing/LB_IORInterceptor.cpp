#include "LB_IORInterceptor.h"
#include "LB_ObjectReferenceFactory.h"
#include "LB_LoadAlert.h"

ACE_RCSID (LoadBalancing,
           LB_IORInterceptor,
           "$Id$")


TAO_LB_IORInterceptor::TAO_LB_IORInterceptor (
  const CORBA::StringSeq & object_groups,
  const CORBA::StringSeq & repository_ids,
  const char * location,
  CosLoadBalancing::LoadManager_ptr lm,
  const char * orb_id,
  TAO_LB_LoadAlert & load_alert)
  : object_groups_ (object_groups),
    repository_ids_ (repository_ids),
    location_ (location),
    lm_ (CosLoadBalancing::LoadManager::_duplicate (lm)),
    orb_id_ (CORBA::string_dup (orb_id)),
    load_alert_ (load_alert)
{
}

char *
TAO_LB_IORInterceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("TAO_LB_IORInterceptor");
}

void
TAO_LB_IORInterceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Force the LoadManager reference to be released since the ORB's
  // lifetime is tied to object reference lifetimes.
  (void) this->lm_.out ();
}

void
TAO_LB_IORInterceptor::establish_components (
    PortableInterceptor::IORInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC (())
{
}

void
TAO_LB_IORInterceptor::components_established (
    PortableInterceptor::IORInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  int argc = 0;
  CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                        0,
                                        this->orb_id_.in ()
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Save a copy of the current ObjectReferenceFactory.
  PortableInterceptor::ObjectReferenceFactory_var old_orf =
    info->current_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // This is slightly evil.  We're creating an object reference
  // through a POA that hasn't completed construction!  This can't be
  // portable.  :(
  //
  // Note that this components_established() method is only called
  // once.  This means that there is no chance of the below _this()
  // call causing the load balancer's swapped-in
  // ObjectReferenceFactory from being used to create the object
  // reference.
  CosLoadBalancing::LoadAlert_var la =
    this->load_alert_._this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  PortableInterceptor::ObjectReferenceFactory * tmp;
  ACE_NEW_THROW_EX (tmp,
                    TAO_LB_ObjectReferenceFactory (old_orf.in (),
                                                   this->object_groups_,
                                                   this->repository_ids_,
                                                   this->location_.in (),
                                                   orb.in (),
                                                   this->lm_.in (),
                                                   la.in ()),
                    CORBA::NO_MEMORY (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  PortableInterceptor::ObjectReferenceFactory_var orf = tmp;

  info->current_factory (orf.in ()
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_LB_IORInterceptor::adapter_manager_state_changed (
    PortableInterceptor::AdapterManagerId,
    PortableInterceptor::AdapterState
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO_LB_IORInterceptor:: adapter_state_changed (
    const PortableInterceptor::ObjectReferenceTemplateSeq &,
    PortableInterceptor::AdapterState
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
