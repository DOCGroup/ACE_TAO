// $Id$

#include "orbsvcs/LoadBalancing/LB_IORInterceptor.h"
#include "orbsvcs/LoadBalancing/LB_ObjectReferenceFactory.h"
#include "orbsvcs/LoadBalancing/LB_LoadAlert.h"

#include "tao/debug.h"
#include "tao/ORB_Constants.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
    load_alert_ (load_alert),
    la_ref_ (),
    lock_ ()
{
}

char *
TAO_LB_IORInterceptor::name (void)
{
  return CORBA::string_dup ("TAO_LB_IORInterceptor");
}

void
TAO_LB_IORInterceptor::destroy (void)
{
  // Force the LoadManager reference to be released since the ORB's
  // lifetime is tied to object reference lifetimes.
  (void) this->lm_.out ();
}

void
TAO_LB_IORInterceptor::establish_components (
    PortableInterceptor::IORInfo_ptr)
{
}

void
TAO_LB_IORInterceptor::components_established (
    PortableInterceptor::IORInfo_ptr info)
{
  int argc = 0;
  ACE_TCHAR **argv= 0;
  CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                        argv,
                                        this->orb_id_.in ());

  // Save a copy of the current ObjectReferenceFactory.
  PortableInterceptor::ObjectReferenceFactory_var old_orf =
    info->current_factory ();

  PortableInterceptor::ObjectReferenceFactory * tmp;
  ACE_NEW_THROW_EX (tmp,
                    TAO_LB_ObjectReferenceFactory (old_orf.in (),
                                                   this->object_groups_,
                                                   this->repository_ids_,
                                                   this->location_.in (),
                                                   orb.in (),
                                                   this->lm_.in ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableInterceptor::ObjectReferenceFactory_var orf = tmp;

  info->current_factory (orf.in ());
}

void
TAO_LB_IORInterceptor::adapter_manager_state_changed (
    const char *,
    PortableInterceptor::AdapterState state)
{
  if (state == PortableInterceptor::ACTIVE)
    {
      this->register_load_alert ();
    }
}

void
TAO_LB_IORInterceptor::adapter_state_changed (
    const PortableInterceptor::ObjectReferenceTemplateSeq &,
    PortableInterceptor::AdapterState state)
{
  if (state == PortableInterceptor::ACTIVE)
    {
      this->register_load_alert ();
    }
}

void
TAO_LB_IORInterceptor::register_load_alert (void)
{
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);

    if (!CORBA::is_nil (this->la_ref_.in ()))
      return;
  }

  try
    {
      // By now, the RootPOA has been fully initialized, so it is safe
      // to activate the LoadAlert object.
      CosLoadBalancing::LoadAlert_var la =
        this->load_alert_._this ();

      {
        ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);

        this->la_ref_ = la;
      }

    }
  catch (const CORBA::Exception& ex)
    {
      if (TAO_debug_level > 0)
        ex._tao_print_exception ("LoadAlert::_this()");
    }

  try
    {
      PortableGroup::Location location (1);
      location.length (1);
      location[0].id = CORBA::string_dup (this->location_.in ());

      this->lm_->register_load_alert (location,
                                      this->la_ref_.in ());
    }
  catch (const CosLoadBalancing::LoadAlertAlreadyPresent& ex)
    {
      if (TAO_debug_level > 0)
        ex._tao_print_exception ("LoadManager::register_load_alert");

      throw CORBA::BAD_INV_ORDER ();
    }
  catch (const CosLoadBalancing::LoadAlertNotAdded& ex)
    {
      if (TAO_debug_level > 0)
        ex._tao_print_exception ("LoadManager::register_load_alert");

      throw CORBA::INTERNAL ();
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
