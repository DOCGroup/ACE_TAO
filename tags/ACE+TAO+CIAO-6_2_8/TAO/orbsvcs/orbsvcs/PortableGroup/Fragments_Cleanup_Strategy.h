// -*- C++ -*-

// ================================================================
/**
 *  @file   Fragments_Cleanup_Strategy.h
 *
 *  $Id$
 *
 *  @author Vladimir Zykov <vz@prismtech.com>
 */
// ================================================================

#ifndef TAO_FRAGMENTS_CLEANUP_STRATEGY_H
#define TAO_FRAGMENTS_CLEANUP_STRATEGY_H

#include /**/ "ace/pre.h"

#include "orbsvcs/PortableGroup/portablegroup_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/PortableGroup/UIPMC_Mcast_Transport.h"

#include /**/ "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// ****************************************************************

namespace TAO_PG
{

  /**
   * @class Fragments_Cleanup_Strategy
   *
   * @brief The base class for all cleanup strategies
   *
   * This class is used by TAO_UIPMC_Mcast_Transport for cleaning
   * received fragments.
   */
  class TAO_PortableGroup_Export Fragments_Cleanup_Strategy
  {
  public:
    /// The constructor
    Fragments_Cleanup_Strategy (int bound);

    /// The destructor
    virtual ~Fragments_Cleanup_Strategy (void);

    /// Sub-classes must implement these methods
    virtual void cleanup (TAO_UIPMC_Mcast_Transport::Packets_Map &) = 0;

  protected:
    /// The bound that triggers a cleanup.
    int bound_;
  };

  /**
   * @class Time_Bound_Fragments_Cleanup_Strategy
   *
   * @brief Cleanup if a message cannot reassemble for a long time.
   */
  class TAO_PortableGroup_Export Time_Bound_Fragments_Cleanup_Strategy
    : public Fragments_Cleanup_Strategy
  {
  public:
    /// The constructor
    Time_Bound_Fragments_Cleanup_Strategy (int bound);

    virtual void cleanup (TAO_UIPMC_Mcast_Transport::Packets_Map &packets);
  };

  /**
   * @class Number_Bound_Fragments_Cleanup_Strategy
   *
   * @brief Cleanup if there are too many messages cannot reassemble.
   */
  class TAO_PortableGroup_Export Number_Bound_Fragments_Cleanup_Strategy
    : public Fragments_Cleanup_Strategy
  {
  public:
    /// The constructor
    Number_Bound_Fragments_Cleanup_Strategy (int bound);

    virtual void cleanup (TAO_UIPMC_Mcast_Transport::Packets_Map &packets);
  };

  /**
   * @class Memory_Bound_Fragments_Cleanup_Strategy
   *
   * @brief Cleanup if non-reassembled messages take too much memory.
   */
  class TAO_PortableGroup_Export Memory_Bound_Fragments_Cleanup_Strategy
    : public Fragments_Cleanup_Strategy
  {
  public:
    /// The constructor
    Memory_Bound_Fragments_Cleanup_Strategy (int bound);

    virtual void cleanup (TAO_UIPMC_Mcast_Transport::Packets_Map &packets);
  };

} // namespace TAO_PG

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "orbsvcs/PortableGroup/Fragments_Cleanup_Strategy.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_FRAGMENTS_CLEANUP_STRATEGY_H */
