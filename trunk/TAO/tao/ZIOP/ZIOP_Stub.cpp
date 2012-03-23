// $Id$

#include "tao/ZIOP/ZIOP_Stub.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/ZIOP/ZIOP_Policy_i.h"
#include "tao/ZIOP/ZIOP.h"
#include "tao/ORB_Core.h"
#include "tao/Policy_Set.h"
#include "tao/Policy_Manager.h"
#include "tao/SystemException.h"

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
      default:
        break;
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

  ZIOP::CompressorIdLevelListPolicy_var override_policy_var =
    ZIOP::CompressorIdLevelListPolicy::_narrow (override.in ());

  ZIOP::CompressorIdLevelListPolicy_var exposed_policy_var =
    ZIOP::CompressorIdLevelListPolicy::_narrow (exposed.in ());

    // Reconcile client-exposed and locally set values.
  if (CORBA::is_nil (exposed_policy_var.in ()))
    return override._retn ();

  if (CORBA::is_nil (override_policy_var.in ()))
    return exposed._retn ();

  // Check which compressor id we should use and which level
  for (CORBA::ULong nr_exposed = 0;
        nr_exposed < exposed_policy_var->compressor_ids ()->length ();
        ++nr_exposed)
    {
      ::Compression::CompressorIdLevel_var exposed_compressor =
        exposed_policy_var->compressor_ids ()->operator [](nr_exposed);

      if (TAO_debug_level > 9)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - ")
                      ACE_TEXT ("TAO_ZIOP_Stub::effective_compression_id_list_policy, ")
                      ACE_TEXT ("exposed_policy = %d, compressor_id = %C, ")
                      ACE_TEXT ("compression_level = %d\n"),
                      nr_exposed,
                      TAO_ZIOP_Loader::ziop_compressorid_name (
                            exposed_compressor.ptr ()->compressor_id),
                      exposed_compressor.ptr ()->compression_level));
        }
      // check if a local policy matches this exposed policy
      for (CORBA::ULong nr_override = 0;
            nr_override < override_policy_var->compressor_ids ()->length ();
            ++nr_override)
        {
          ::Compression::CompressorIdLevel_var override_compressor =
            override_policy_var->compressor_ids ()->operator [] (nr_override);
          if (TAO_debug_level > 9)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO (%P|%t) - ")
                          ACE_TEXT ("TAO_ZIOP_Stub::effective_compression_id_list_policy, ")
                          ACE_TEXT ("checking override_policy = %d, compressor_id = %C, ")
                          ACE_TEXT ("compression_level = %d\n"),
                          nr_override,
                          TAO_ZIOP_Loader::ziop_compressorid_name (
                                override_compressor->compressor_id),
                          override_compressor->compression_level));
            }
          if (override_compressor->compressor_id ==
              exposed_compressor->compressor_id)
            {
              CORBA::Policy_var tmp_policy = override_policy_var->copy ();
              ZIOP::CompressorIdLevelListPolicy_var idlevellist_policy_var =
                    ZIOP::CompressorIdLevelListPolicy::_narrow (tmp_policy.in ());
              if (CORBA::is_nil (idlevellist_policy_var.in ()))
                return override._retn ();

              ::Compression::CompressorIdLevelList &entries =
                *idlevellist_policy_var->compressor_ids ();

              // Since the CompressorIdLevelListPolicy holds a prioritized list
              // of the compressors we are allowed to use, and this is sent
              // across to the server, BUT we are going to always use entry 0
              // here at the client; we must ensure the other compressors are not
              // lost but that the chosen compressor is placed first in the list
              // (using the correctly minimized compression_level).
              for (CORBA::ULong shuffle = nr_override; 0u < shuffle; --shuffle)
                {
                  entries[shuffle].compressor_id=     entries[shuffle-1u].compressor_id;
                  entries[shuffle].compression_level= entries[shuffle-1u].compression_level;
                }
              entries[0].compressor_id=     override_compressor->compressor_id;
              entries[0].compression_level= ACE_MIN (
                override_compressor->compression_level,
                exposed_compressor->compression_level);

              return idlevellist_policy_var._retn ();
            }
        }
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
#endif
