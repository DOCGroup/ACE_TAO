#include "tao/IORInterceptor/IORInfo.h"
#include "tao/PortableServer/Root_POA.h"

#include "tao/PolicyC.h"
#include "tao/IOPC.h"
#include "tao/ORB_Constants.h"


ACE_RCSID (IORInterceptor,
           IORInfo,
           "$Id$")

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
TAO_IORInfo::get_effective_policy (CORBA::PolicyType type
                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  CORBA::Policy_var policy =
    this->poa_->get_policy (type ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  if (!CORBA::is_nil (policy.in ()))
    {
      return policy._retn ();
    }

  // TODO: Now check the global ORB policies.
  // ........

  // No policy matching the given PolicyType was found.
  ACE_THROW_RETURN (CORBA::INV_POLICY (CORBA::OMGVMCID | 3,
                                       CORBA::COMPLETED_NO),
                    CORBA::Policy::_nil ());
}

void
TAO_IORInfo::add_ior_component (const IOP::TaggedComponent &component
                                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->components_established_)
    ACE_THROW (CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14,
                                     CORBA::COMPLETED_NO));

  // Add the given tagged component to all profiles.
  this->poa_->save_ior_component (component
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_IORInfo::add_ior_component_to_profile (
    const IOP::TaggedComponent &component,
    IOP::ProfileId profile_id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->components_established_)
    ACE_THROW (CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14,
                                     CORBA::COMPLETED_NO));

  this->poa_->save_ior_component_and_profile_id (component,
                                                 profile_id
                                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

char *
TAO_IORInfo::manager_id (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  PortableServer::POAManager_var poa_manager = this->poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);
  return poa_manager->get_id (ACE_ENV_SINGLE_ARG_PARAMETER);
}

PortableInterceptor::AdapterState
TAO_IORInfo::state (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableInterceptor::NON_EXISTENT);

  return this->poa_->get_adapter_state (ACE_ENV_SINGLE_ARG_PARAMETER);
}

PortableInterceptor::ObjectReferenceTemplate *
TAO_IORInfo::adapter_template (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Return the Object Reference Template whenever an IOR Interceptor
  // is invoked.  Its value is the template created for the adapter
  // policies and the IOR Interceptor calls to add_ior_component and
  // add_ior_component_to_profile.  It's a const value and its value
  // never changes.
  PortableInterceptor::ObjectReferenceTemplate *adapter_template =
    this->poa_->get_adapter_template (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (adapter_template == 0)
    {
      ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14,
                                              CORBA::COMPLETED_NO),
                        0);
    }

  return adapter_template;
}

PortableInterceptor::ObjectReferenceFactory *
TAO_IORInfo::current_factory (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Return the current_factory that is used to create the object
  // references by the adapter.  Though initially, its value is the
  // same as the adapter_template, unlike adapter_template, its value
  // can be changed.  The value of the current_factory can be changed
  // only during the call to components_established method.
  PortableInterceptor::ObjectReferenceFactory *adapter_factory =
    this->poa_->get_obj_ref_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (adapter_factory == 0)
    {
      ACE_THROW_RETURN (CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 14,
                                              CORBA::COMPLETED_NO),
                        0);
    }

  return adapter_factory;
}

void
TAO_IORInfo::current_factory (
    PortableInterceptor::ObjectReferenceFactory * current_factory
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->check_validity (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->poa_->set_obj_ref_factory (current_factory
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_IORInfo::check_validity (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->poa_ == 0)
    {
      // Although not defined by the spec, duplicate the behavior used
      // by the ORBInitInfo object once CORBA::ORB_init() has been
      // called.  Specifically, the IORInfo object is no longer valid
      // once the POA has invoked all IORInterceptor interception
      // points.  This also prevents memory access violations from
      // occuring if the POA is destroyed before this IORInfo object.
      ACE_THROW (CORBA::OBJECT_NOT_EXIST (TAO::VMCID,
                                          CORBA::COMPLETED_NO));
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
