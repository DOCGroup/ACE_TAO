// -*- C++ -*-
// $Id$

#include "tao/IORInterceptor/IORInfo.h"
#include "tao/PortableServer/Root_POA.h"

#include "tao/PolicyC.h"
#include "tao/IOPC.h"
#include "tao/ORB_Constants.h"

#if !defined (__ACE_INLINE__)
# include "tao/IORInterceptor/IORInfo.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_IORInfo::TAO_IORInfo (TAO_Root_POA *poa)
  : poa_ (poa),
    components_established_ (false)
{
}

TAO_IORInfo::~TAO_IORInfo (void)
{
}

CORBA::Policy_ptr
TAO_IORInfo::get_effective_policy (CORBA::PolicyType type)
{
  this->check_validity ();

  CORBA::Policy_var policy =
    this->poa_->get_policy (type);

  if (!CORBA::is_nil (policy.in ()))
    {
      return policy._retn ();
    }

  // TODO: Now check the global ORB policies.
  // ........

  // No policy matching the given PolicyType was found.
  throw ::CORBA::INV_POLICY (CORBA::OMGVMCID | 3, CORBA::COMPLETED_NO);
}

void
TAO_IORInfo::add_ior_component (const IOP::TaggedComponent &component)
{
  this->check_validity ();

  if (this->components_established_)
    throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);

  // Add the given tagged component to all profiles.
  this->poa_->save_ior_component (component);
}

void
TAO_IORInfo::add_ior_component_to_profile (
    const IOP::TaggedComponent &component,
    IOP::ProfileId profile_id)
{
  this->check_validity ();

  if (this->components_established_)
    throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);

  this->poa_->save_ior_component_and_profile_id (component, profile_id);
}

char *
TAO_IORInfo::manager_id (void)
{
  this->check_validity ();

  PortableServer::POAManager_var poa_manager = this->poa_->the_POAManager ();
  return poa_manager->get_id ();
}

PortableInterceptor::AdapterState
TAO_IORInfo::state (void)
{
  this->check_validity ();

  return this->poa_->get_adapter_state ();
}

PortableInterceptor::ObjectReferenceTemplate *
TAO_IORInfo::adapter_template (void)
{
  this->check_validity ();

  // Return the Object Reference Template whenever an IOR Interceptor
  // is invoked.  Its value is the template created for the adapter
  // policies and the IOR Interceptor calls to add_ior_component and
  // add_ior_component_to_profile.  It's a const value and its value
  // never changes.
  PortableInterceptor::ObjectReferenceTemplate *adapter_template =
    this->poa_->get_adapter_template ();

  if (adapter_template == 0)
    {
      throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);
    }

  return adapter_template;
}

PortableInterceptor::ObjectReferenceFactory *
TAO_IORInfo::current_factory (void)
{
  this->check_validity ();

  // Return the current_factory that is used to create the object
  // references by the adapter.  Though initially, its value is the
  // same as the adapter_template, unlike adapter_template, its value
  // can be changed.  The value of the current_factory can be changed
  // only during the call to components_established method.
  PortableInterceptor::ObjectReferenceFactory *adapter_factory =
    this->poa_->get_obj_ref_factory ();

  if (adapter_factory == 0)
    {
      throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14, CORBA::COMPLETED_NO);
    }

  return adapter_factory;
}

void
TAO_IORInfo::current_factory (
    PortableInterceptor::ObjectReferenceFactory * current_factory)
{
  this->check_validity ();

  this->poa_->set_obj_ref_factory (current_factory);
}

void
TAO_IORInfo::check_validity (void)
{
  if (this->poa_ == 0)
    {
      // Although not defined by the spec, duplicate the behavior used
      // by the ORBInitInfo object once CORBA::ORB_init() has been
      // called.  Specifically, the IORInfo object is no longer valid
      // once the POA has invoked all IORInterceptor interception
      // points.  This also prevents memory access violations from
      // occuring if the POA is destroyed before this IORInfo object.
      throw ::CORBA::OBJECT_NOT_EXIST (TAO::VMCID, CORBA::COMPLETED_NO);
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
