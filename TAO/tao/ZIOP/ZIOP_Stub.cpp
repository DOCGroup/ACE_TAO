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
  // Get the value from the IOR (This is the SERVERS available compressor's list).
  CORBA::Policy_var policy (
    this->exposed_compression_id_list_policy ());
  ZIOP::CompressorIdLevelListPolicy_var serverCompressors (
    ZIOP::CompressorIdLevelListPolicy::_narrow (policy.in ()));
  // If SERVER does not have an available compressor's list, compression can't go ahead.
  if (CORBA::is_nil (serverCompressors.in ()))
    {
      if (6 < TAO_debug_level)
        {
          ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT ("TAO (%P|%t) - ")
            ACE_TEXT ("TAO_ZIOP_Stub::effective_compression_id_list_policy, ")
            ACE_TEXT ("no serverCompressorIdLevelListPolicy (did not compress).")));
        }
      return 0;
    }
  ::Compression::CompressorIdLevelList &serverList =
    *serverCompressors->compressor_ids ();

  // Get effective override (This is the CLIENTS compressor's priority ordered list).
  policy =
    this->TAO_Stub::get_cached_policy (TAO_CACHED_COMPRESSION_ID_LEVEL_LIST_POLICY);
  ZIOP::CompressorIdLevelListPolicy_var clientCompressors (
    ZIOP::CompressorIdLevelListPolicy::_narrow (policy.in ()));
  // Likewise if CLIENT does not have a compressor's list, compression can't go ahead.
  if (CORBA::is_nil (clientCompressors.in ()))
    {
      if (6 < TAO_debug_level)
        {
          ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT ("TAO (%P|%t) - ")
            ACE_TEXT ("TAO_ZIOP_Stub::effective_compression_id_list_policy, ")
            ACE_TEXT ("no clientCompressorIdLevelListPolicy (did not compress).")));
        }
      return 0;
    }
  ::Compression::CompressorIdLevelList &clientList =
    *clientCompressors->compressor_ids ();

  // For each CLIENT compressor (in priority order) check...
  for (CORBA::ULong client = 0u; client < clientList.length (); ++client)
    {
      ::Compression::CompressorIdLevel_var clientEntry (clientList[client]);

      // ... which each SERVER compressor id if it is available to use.
      for (CORBA::ULong server = 0u; server < serverList.length (); ++server)
        {
          ::Compression::CompressorIdLevel_var serverEntry (serverList[server]);

          if (clientEntry->compressor_id == serverEntry->compressor_id)
            {
              // OK we found a match, however we need to make the highest priority
              // compressor (the one we are going to employ) use slot 0 in the
              // list we are returning. Since we can't modify the original list
              // we have to make a copy and modify that.
              policy= clientCompressors->copy ();
              ZIOP::CompressorIdLevelListPolicy_var returningCompressors (
                ZIOP::CompressorIdLevelListPolicy::_narrow (policy.in ()));
              if (CORBA::is_nil (returningCompressors.in ()))
                {
                  // This shouldn't happen, it's basically an internal error.
                  if (6 < TAO_debug_level)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - ")
                        ACE_TEXT ("TAO_ZIOP_Stub::effective_compression_id_list_policy, ")
                        ACE_TEXT ("could not copy clientCompressorIdLevelListPolicy (did not compress).")));
                    }
                  return 0;
                }
              ::Compression::CompressorIdLevelList &returningList =
                *returningCompressors->compressor_ids ();

              // We must ensure the other compressors are not lost, as they tell the server
              // which compressors are available for it to use with the reply message.
              for (CORBA::ULong shuffle = client; 0u < shuffle; --shuffle)
                {
                  returningList[shuffle].compressor_id=     returningList[shuffle-1u].compressor_id;
                  returningList[shuffle].compression_level= returningList[shuffle-1u].compression_level;
                }

              // The one we found is the one we are going to use (now the highest priority)
              // but with the correct (minimized) compression level of the client and server.
              returningList[0].compressor_id=     clientEntry->compressor_id;
              returningList[0].compression_level= ACE_MIN (clientEntry->compression_level,
                                                           serverEntry->compression_level);
              if (6 < TAO_debug_level)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("TAO (%P|%t) - ")
                              ACE_TEXT ("TAO_ZIOP_Stub::effective_compression_id_list_policy, ")
                              ACE_TEXT ("found (Client %d: %s@%d == Server %d: %s@%d) using @%d.\n"),
                              client,
                              TAO_ZIOP_Loader::ziop_compressorid_name (clientEntry->compressor_id),
                              clientEntry->compression_level,
                              server,
                              TAO_ZIOP_Loader::ziop_compressorid_name (serverEntry->compressor_id),
                              serverEntry->compression_level,
                              returningList[0].compression_level));
                }

              return returningCompressors._retn ();
            }

          if (7 < TAO_debug_level)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO (%P|%t) - ")
                          ACE_TEXT ("TAO_ZIOP_Stub::effective_compression_id_list_policy, ")
                          ACE_TEXT ("checking (Client %d: %s@%d != Server %d: %s@%d).\n"),
                          client,
                          TAO_ZIOP_Loader::ziop_compressorid_name (clientEntry->compressor_id),
                          clientEntry->compression_level,
                          server,
                          TAO_ZIOP_Loader::ziop_compressorid_name (serverEntry->compressor_id),
                          serverEntry->compression_level));
            }
        } // next serverEntry
    } // next clientEntry

  if (6 < TAO_debug_level)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) - ")
                  ACE_TEXT ("TAO_ZIOP_Stub::effective_compression_id_list_policy, ")
                  ACE_TEXT("no matching CompressorIdLevelListPolicy (did not compress).\n")));
    }
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
#endif
