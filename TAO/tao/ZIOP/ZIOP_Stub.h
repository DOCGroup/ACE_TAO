// -*- C++ -*-

//=============================================================================
/**
 *  @file    ZIOP_Stub.h
 *
 *  $Id$
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

  TAO_ZIOP_Stub (const char *repository_id,
                 const TAO_MProfile &profiles,
                 TAO_ORB_Core *orb_core);

  virtual ~TAO_ZIOP_Stub (void);

  /**
   * Returns the effective policy if @a type is a known client-exposed
   * policy type.  Returns the effective override for all other policy
   * types.
   */
  CORBA::Policy_ptr get_policy (CORBA::PolicyType type);

  CORBA::Policy_ptr get_cached_policy (TAO_Cached_Policy_Type type);
  
private:

  /// Helper method used to parse the policies.
  void parse_policies (void);

  void exposed_compression_enabling_policy (CORBA::Policy_ptr policy);

  void exposed_compression_id_list_policy (CORBA::Policy_ptr policy);

  CORBA::Policy_ptr exposed_compression_enabling_policy (void);

  CORBA::Policy_ptr exposed_compression_id_list_policy (void);

  CORBA::Policy *effective_compression_enabling_policy (void);
  CORBA::Policy *effective_compression_id_list_policy (void);

  // The following attribute are used to cache
  // the different kind of policies and avoid to
  // parse the MProfile's policy list each time we
  // are asked about a given policy.

  CORBA::Policy_var compression_enabling_policy_;

  CORBA::Policy_var compression_id_list_policy_;

  CORBA::Boolean are_policies_parsed_;

private:
  // = Disallow copying and assignment.
  TAO_ZIOP_Stub (const TAO_ZIOP_Stub &);
  TAO_ZIOP_Stub &operator = (const TAO_ZIOP_Stub &);

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_ZIOP_STUB_FACTORY_H_ */
