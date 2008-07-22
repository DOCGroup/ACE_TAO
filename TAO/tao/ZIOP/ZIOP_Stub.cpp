// $Id$

#include "tao/ZIOP/ZIOP_Stub.h"
#include "tao/ZIOP/ZIOP_Policy_i.h"
#include "tao/ORB_Core.h"
#include "tao/Policy_Set.h"
#include "tao/Policy_Manager.h"
#include "tao/SystemException.h"

ACE_RCSID (ZIOP,
           ZIOP_Stub,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ZIOP_Stub::TAO_ZIOP_Stub (const char *repository_id,
                              const TAO_MProfile &profiles,
                              TAO_ORB_Core *orb_core)
  : TAO_Stub (repository_id,
              profiles,
              orb_core),
    are_policies_parsed_ (false)
{
}

TAO_ZIOP_Stub::~TAO_ZIOP_Stub (void)
{
  if (!CORBA::is_nil (this->compression_enabling_policy_.in ()))
    this->compression_enabling_policy_->destroy ();

  if (!CORBA::is_nil (this->compression_id_list_policy_.in ()))
    this->compression_id_list_policy_->destroy ();
}

void
TAO_ZIOP_Stub::parse_policies (void)
{
  CORBA::PolicyList_var policy_list
    = this->base_profiles_.policy_list ();

  CORBA::ULong const length = policy_list->length ();

  // Cache away the policies that we'll need later.
  for (CORBA::ULong i = 0; i < length; ++i)
    {
      switch (policy_list[i]->policy_type ())
        {
           case ZIOP::COMPRESSION_ENABLING_POLICY_ID:
             {
               this->exposed_compression_enabling_policy (policy_list[i]);
             }
             break;
           case ZIOP::COMPRESSOR_ID_LEVEL_LIST_POLICY_ID:
             {
               this->exposed_compression_id_list_policy (policy_list[i]);
             }
             break;
         }

    }

  this->are_policies_parsed_ = true;
}

CORBA::Policy *
TAO_ZIOP_Stub::exposed_compression_id_list_policy (void)
{
  if (!this->are_policies_parsed_)
    {
      this->parse_policies ();
    }

  return CORBA::Policy::_duplicate (this->compression_id_list_policy_.in ());
}

void
TAO_ZIOP_Stub::exposed_compression_id_list_policy (CORBA::Policy_ptr policy)
{
  this->compression_id_list_policy_ = CORBA::Policy::_duplicate (policy);
}

CORBA::Policy *
TAO_ZIOP_Stub::exposed_compression_enabling_policy (void)
{
  if (!this->are_policies_parsed_)
    {
      this->parse_policies ();
    }

  return CORBA::Policy::_duplicate (this->compression_enabling_policy_.in ());
}

void
TAO_ZIOP_Stub::exposed_compression_enabling_policy (CORBA::Policy_ptr policy)
{
  this->compression_enabling_policy_ = CORBA::Policy::_duplicate (policy);
}

CORBA::Policy_ptr
TAO_ZIOP_Stub::get_policy (CORBA::PolicyType type)
{
  // If we are dealing with a client exposed policy, check if any
  // value came in the IOR/reconcile IOR value and overrides.
  switch (type)
    {
      case ZIOP::COMPRESSION_ENABLING_POLICY_ID :
        {
          return this->effective_compression_enabling_policy ();
        }
      case ZIOP::COMPRESSOR_ID_LEVEL_LIST_POLICY_ID :
        {
          return this->effective_compression_id_list_policy ();
        }
    }

  return this->TAO_Stub::get_policy (type);
}

CORBA::Policy_ptr
TAO_ZIOP_Stub::get_cached_policy (TAO_Cached_Policy_Type type)
{
  // If we are dealing with a client exposed policy, check if any
  // value came in the IOR/reconcile IOR value and overrides.
  switch (type)
    {
      case TAO_CACHED_COMPRESSION_ENABLING_POLICY:
        {
          return this->effective_compression_enabling_policy ();
        }
      case TAO_CACHED_COMPRESSION_ID_LEVEL_LIST_POLICY:
        {
          return this->effective_compression_id_list_policy ();
        }
    }

  return this->TAO_Stub::get_cached_policy (type);
}

CORBA::Policy *
TAO_ZIOP_Stub::effective_compression_enabling_policy (void)
{
  // Get effective override.
  CORBA::Policy_var override =
    this->TAO_Stub::get_cached_policy (
      TAO_CACHED_COMPRESSION_ENABLING_POLICY);

  // Get the value from the ior.
  CORBA::Policy_var exposed = this->exposed_compression_enabling_policy ();

  // Reconcile client-exposed and locally set values.
  if (CORBA::is_nil (exposed.in ()))
    return override._retn ();

  if (CORBA::is_nil (override.in ()))
    return exposed._retn ();

  ZIOP::CompressionEnablingPolicy_var override_policy_var =
    ZIOP::CompressionEnablingPolicy::_narrow (override.in ());

  ZIOP::CompressionEnablingPolicy_var exposed_policy_var =
    ZIOP::CompressionEnablingPolicy::_narrow (exposed.in ());

  // Both override and exposed have been set.
  // See if either of them has empty priority bands.
  if (override_policy_var->compression_enabled () && exposed_policy_var->compression_enabled ())
    return override._retn ();

  if (!override_policy_var->compression_enabled ())
    return override._retn ();

  return exposed._retn ();
}

CORBA::Policy *
TAO_ZIOP_Stub::effective_compression_id_list_policy (void)
{
  // Get effective override.
  CORBA::Policy_var override =
    this->TAO_Stub::get_cached_policy (TAO_CACHED_COMPRESSION_ID_LEVEL_LIST_POLICY);

  // Get the value from the ior.
  CORBA::Policy_var exposed =
    this->exposed_compression_id_list_policy ();

  // Reconcile client-exposed and locally set values.
  if (CORBA::is_nil (exposed.in ()))
    return override._retn ();

  if (CORBA::is_nil (override.in ()))
    return exposed._retn ();

  ZIOP::CompressorIdLevelListPolicy_var override_policy_var =
    ZIOP::CompressorIdLevelListPolicy::_narrow (override.in ());

  ZIOP::CompressorIdLevelListPolicy_var exposed_policy_var =
    ZIOP::CompressorIdLevelListPolicy::_narrow (exposed.in ());

  // Check which compressor id we should use and which level

  return exposed._retn ();
}

TAO_END_VERSIONED_NAMESPACE_DECL

