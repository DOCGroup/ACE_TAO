// -*- C++ -*-

//=============================================================================
/**
 *  @file    ZIOP_Stub.h
 *
 *  This file contains the declaration for the ZIOP Stub.
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_ZIOP_STUB_H_
#define TAO_ZIOP_STUB_H_

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#include "tao/ZIOP/ziop_export.h"

#include "tao/Stub.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward references.
namespace TAO
{
  class CompressorIdListPolicy;
  class CompressionEnablingPolicy;
}

/**
 * @class TAO_ZIOP_Stub
 *
 * This class represent a stub with extended functionality, which are needed
 * for ZIOPCORBA.
 */
class TAO_ZIOP_Export TAO_ZIOP_Stub : public TAO_Stub
{
public:
  TAO_ZIOP_Stub (char const *repository_id,
                 TAO_MProfile const &profiles,
                 TAO_ORB_Core *orb_core);

  ~TAO_ZIOP_Stub () override;

  /**
   * Returns the effective policy if @a type is a known client-exposed
   * policy type.  Returns the effective override for all other policy
   * types.
   */
  CORBA::Policy_ptr get_policy (CORBA::PolicyType type) override;

  CORBA::Policy_ptr get_cached_policy (TAO_Cached_Policy_Type type) override;

private:
  /// Helper method used to parse the policies.
  void parse_policies ();

  void exposed_compression_enabling_policy (CORBA::Policy_ptr policy);

  void exposed_compression_id_list_policy (CORBA::Policy_ptr policy);

  CORBA::Policy_ptr exposed_compression_enabling_policy ();

  CORBA::Policy_ptr exposed_compression_id_list_policy ();

  CORBA::Policy *effective_compression_enabling_policy ();
  CORBA::Policy *effective_compression_id_list_policy ();

  // The following attribute are used to cache
  // the different kind of policies and avoid to
  // parse the MProfile's policy list each time we
  // are asked about a given policy.

  CORBA::Policy_var compression_enabling_policy_;

  CORBA::Policy_var compression_id_list_policy_;

  CORBA::Boolean are_policies_parsed_ {};

private:
  TAO_ZIOP_Stub (TAO_ZIOP_Stub const &) = delete;
  TAO_ZIOP_Stub (TAO_ZIOP_Stub &&) = delete;
  TAO_ZIOP_Stub &operator= (TAO_ZIOP_Stub const &) = delete;
  TAO_ZIOP_Stub &operator= (TAO_ZIOP_Stub &&) = delete;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif

#include /**/ "ace/post.h"

#endif /* TAO_ZIOP_STUB_FACTORY_H_ */
