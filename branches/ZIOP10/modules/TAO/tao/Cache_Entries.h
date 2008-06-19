// -*- C++ -*-

//=============================================================================
/**
 *  @file   Cache_Entries.h
 *
 *  $Id$
 *
 *
 *  @author Bala Natarajan  <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_CACHE_ENTRIES_H
#define TAO_CACHE_ENTRIES_H

#include /**/ "ace/pre.h"

#include "tao/Transport_Descriptor_Interface.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Transport;

#ifdef index
# undef index
#endif  /* index */

namespace TAO
{
  class Transport_Cache_Manager;
  /// States of a recyclable object.
  /// @todo: see discussion in bugzilla 3024
  enum Cache_Entries_State
    {
      /// Idle and can be purged.
      ENTRY_IDLE_AND_PURGABLE,

      /// Idle but cannot be purged.
      ENTRY_IDLE_BUT_NOT_PURGABLE,

      /// Can be purged, but is not idle (mostly for debugging).
      ENTRY_PURGABLE_BUT_NOT_IDLE,

      /// Busy (i.e., cannot be recycled or purged).
      ENTRY_BUSY,

      /// Closed.
      ENTRY_CLOSED,

      /// Connection in process, but not complete
      ENTRY_CONNECTING,

      /// Unknown state.
      ENTRY_UNKNOWN
    };

  /**
   * @class Cache_IntId
   *
   * @brief Helper class for TAO_Transport_Cache_Manager
   *
   * Helper class that wraps the <value> part of the Map or
   * table holding the Transport state.: unifies data items, so
   * they can be stored together as a <value> for a <key> in a
   * table holding the state of the Transport Cache.
   */
  class TAO_Export Cache_IntId
  {
  public:
    friend class TAO::Transport_Cache_Manager;
    /// Constructor.
    Cache_IntId (void);

    /// Constructor.
    Cache_IntId (TAO_Transport *transport);

    /// Copy constructor.
    Cache_IntId (const Cache_IntId & rhs);

    /// Destructor.
    ~Cache_IntId (void);

    /// Assignment operator (does copy memory).
    Cache_IntId& operator= (const Cache_IntId &rhs);

    /// Equality comparison operator (must match both id_ and kind_).
    bool operator== (const Cache_IntId &rhs) const;

    /// Inequality comparison operator.
    bool operator!= (const Cache_IntId &rhs) const;

    /// Return the underlying transport
    TAO_Transport *transport (void);

    /// Return the underlying transport
    const TAO_Transport *transport (void) const;

    /// Set <recycle_state>.
    void recycle_state (Cache_Entries_State new_state);

    /// Get <recycle_state>.
    Cache_Entries_State recycle_state (void) const;

    /// Relinquish ownership of the TAO_Transport object associated with
    /// this Cache_IntId.
    /**
     * @note This method should go away once the
     *       Transport_Cache_Map_Manager is improved so that it returns
     *       TAO_Transport objects when performing a find() operation.
     *       This method really only exists to get around inadequacies
     *       in the Transport_Cache_Map_Manager interface.
     */
    TAO_Transport *relinquish_transport (void);

  private:
    /// The transport that needs to be cached.
    TAO_Transport *transport_;

    /// The state of the handle
    Cache_Entries_State recycle_state_;

    /// This is an analog for the transport::is_connected(), which is
    /// guarded by a mutex.
    bool is_connected_;
  };


  /**
   * @class Cache_ExtId
   *
   * @brief Helper class for TAO_Transport_Cache_Manager: unifies
   * several  data items, so they can be stored together as a
   * <value> for a <key> in a hash table holding the state of the
   * Transport Cache.
   */
  class TAO_Export Cache_ExtId
  {
  public:

    /// Constructor.
    Cache_ExtId (void);

    /// Constructor.
    Cache_ExtId (TAO_Transport_Descriptor_Interface *prop);

    /// Copy constructor.
    Cache_ExtId (const Cache_ExtId & rhs);

    /// Destructor.
    ~Cache_ExtId (void);

    // = Assignment and comparison operators.
    /// Assignment operator (does copy memory).
    Cache_ExtId& operator= (const Cache_ExtId &rhs);

    /// Equality comparison operator (must match both id_ and kind_).
    bool operator== (const Cache_ExtId &rhs) const;

    /// Inequality comparison operator.
    bool operator!= (const Cache_ExtId &rhs) const;

    /// <hash> function is required in order for this class to be usable by
    /// ACE_Hash_Map_Manager_Ex.
    u_long hash (void) const;

    /// Make a deep copy of the underlying pointer
    void duplicate (void);

    /// Return the index value
    CORBA::ULong index (void) const;

    /// Set the index value. This calls should not be used by any users
    /// but for the TAO_Transport_Cache_Manager class.
    void index (CORBA::ULong index);

    /// Increment the index value
    void incr_index (void);

    // = Accessors
    /// Get the underlying the property pointer
    TAO_Transport_Descriptor_Interface *property (void) const;

  private:

    /// A property object that we represent.
    TAO_Transport_Descriptor_Interface *transport_property_;

    /// Do we need to delete transport_property?
    CORBA::Boolean is_delete_;

    /**
     * This is a supplementary index. Would be set to zero by
     * default. Would be altered by the Transport_Cache of TAO. Please
     * see the documentation of TAO_Transport_Cache_Manager for
     * details.
     */
    CORBA::ULong index_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Cache_Entries.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CACHE_ENTRIES_H */
