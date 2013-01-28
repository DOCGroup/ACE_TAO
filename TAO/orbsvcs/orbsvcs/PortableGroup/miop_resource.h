// -*- C++ -*-

//=============================================================================
/**
 *  @file   miop_resource.h
 *
 *  $Id$
 *
 *  @author Vladimir Zykov <vz@prismtech.com>
 */
//=============================================================================


#ifndef TAO_MIOP_RESOURCE_H
#define TAO_MIOP_RESOURCE_H

#include /**/ "ace/pre.h"

#include "orbsvcs/PortableGroup/portablegroup_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"
#include "tao/Versioned_Namespace.h"

#include "ace/Service_Object.h"
#include "ace/Service_Config.h"
#include "ace/Time_Value.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_PG
{
  class Fragments_Cleanup_Strategy;
}

/**
 * @class TAO_MIOP_Resource_Factory
 *
 * @brief TAO's MIOP resource factory
 *
 * Using a <{resource source specifier}> as a discriminator, the
 * factory can return resource instances which are, e.g., global,
 * stored in thread-specific storage, stored in shared memory,
 * etc.
 */
class TAO_PortableGroup_Export TAO_MIOP_Resource_Factory
  : public ACE_Service_Object
{
public:

  /// Constructor.
  TAO_MIOP_Resource_Factory (void);

  /// Destructor.
  virtual ~TAO_MIOP_Resource_Factory (void);

  /**
   * @name Service Configurator Hooks
   */
  //@{
  /// Dynamic linking hook
  virtual int init (int argc, ACE_TCHAR *argv[]);
  //@}

  /**
   * @name Member Accessors
   */
  //@{
  TAO_PG::Fragments_Cleanup_Strategy *fragments_cleanup_strategy (void) const;

  /// Get Maximum number of fragments allowed.
  u_long max_fragments (void) const;

  /// Get MTU value (roughly).
  u_long max_fragment_size (void) const;

  /// Get time required for transfering one maximum sized fragment.
  u_long max_fragment_rate (void) const;

  /// Get number of bytes that can be sent without delay.
  u_long send_hi_water_mark (void) const;

  /// Get the desired socket transmit buffer's size in bytes (Zero is unspecified).
  u_long send_buffer_size (void) const;

  /// Get the desired socket receive buffer's size in bytes (Zero is unspecified).
  u_long receive_buffer_size (void) const;
  //@}

  /// Get the client-side transmission rate throttling enable flag.
  bool enable_throttling (void) const;

  /// Get the server-side eager complete message dequeuing enable flag.
  bool enable_eager_dequeue (void) const;

private:
  enum Fragments_Cleanup_Strategy_Type
    {
      TAO_MIOP_CLEANUP_TIME_BOUND,
      TAO_MIOP_CLEANUP_NUMBER_BOUND,
      TAO_MIOP_CLEANUP_MEMORY_BOUND
    };

  Fragments_Cleanup_Strategy_Type fragments_cleanup_strategy_type_;

  int fragments_cleanup_bound_;

  mutable TAO_PG::Fragments_Cleanup_Strategy *fragments_cleanup_strategy_;

  /// Maximum number of fragments.
  u_long max_fragments_;

  /// Maximum Size of a single fragment.
  u_long max_fragment_size_;

  /// Time required for transfering one maximum sized fragment.
  u_long max_fragment_rate_;

  /// Number of bytes that can be sent without delay.
  u_long send_hi_water_mark_;

  /// Get the desired socket send buffer's size in bytes.
  u_long send_buffer_size_;

  /// Get the desired socket receive buffer's size in bytes.
  u_long receive_buffer_size_;

  /// Get the client-side transmission rate throttling enable flag.
  bool enable_throttling_;

  /// Get the server-side eager complete message dequeuing enable flag.
  bool enable_eager_dequeue_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE (TAO_MIOP_Resource_Factory)
ACE_FACTORY_DECLARE (TAO_PortableGroup, TAO_MIOP_Resource_Factory)

#include /**/ "ace/post.h"
#endif /* TAO_MIOP_RESOURCE_H */
