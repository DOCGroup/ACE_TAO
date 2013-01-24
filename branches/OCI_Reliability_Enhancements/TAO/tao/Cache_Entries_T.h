// -*- C++ -*-

//=============================================================================
/**
 *  @file   Cache_Entries_T.h
 *
 *  $Id$
 *
 *
 *  @author Bala Natarajan  <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_CACHE_ENTRIES_T_H
#define TAO_CACHE_ENTRIES_T_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

#ifdef index
# undef index
#endif  /* index */

namespace TAO
{
  /// States of a recyclable object.
  /// @todo: see discussion in bugzilla 3024
  enum Cache_Entries_State
    {
      /// Idle and can be purged.
      ENTRY_IDLE_AND_PURGABLE,

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
   * @brief Helper class for TAO_Transport_Cache_Manager
   *
   * Helper class that wraps the <value> part of the Map or
   * table holding the Transport state.: unifies data items, so
   * they can be stored together as a <value> for a <key> in a
   * table holding the state of the Transport Cache.
   */
  template <typename TRANSPORT_TYPE>
  class Cache_IntId_T
  {
  public:
    typedef TRANSPORT_TYPE transport_type;

    /// Constructor.
    Cache_IntId_T (void);

    /// Constructor.
    Cache_IntId_T (transport_type *transport);

    /// Copy constructor.
    Cache_IntId_T (const Cache_IntId_T & rhs);

    /// Destructor.
    ~Cache_IntId_T (void);

    /// Assignment operator (does copy memory).
    Cache_IntId_T& operator= (const Cache_IntId_T &rhs);

    /// Equality comparison operator (must match both id_ and kind_).
    bool operator== (const Cache_IntId_T &rhs) const;

    /// Inequality comparison operator.
    bool operator!= (const Cache_IntId_T &rhs) const;

    /// Return the underlying transport
    transport_type *transport (void);

    /// Return the underlying transport
    const transport_type *transport (void) const;

    /// Set recycle_state.
    void recycle_state (Cache_Entries_State new_state);

    /// Get recycle_state.
    Cache_Entries_State recycle_state (void) const;

    /// Relinquish ownership of the TAO_Transport object associated with
    /// this Cache_IntId_T.
    /**
     * @note This method should go away once the
     *       Transport_Cache_Map_Manager is improved so that it returns
     *       TAO_Transport objects when performing a find() operation.
     *       This method really only exists to get around inadequacies
     *       in the Transport_Cache_Map_Manager interface.
     */
    transport_type *relinquish_transport (void);

    /// Get the connected flag
    bool is_connected (void) const;

    /// Set the connected flag
    void is_connected (bool connected);

    static const char *state_name (Cache_Entries_State st);

  private:
    /// The transport that needs to be cached.
    transport_type *transport_;

    /// The state of the handle
    Cache_Entries_State recycle_state_;

    /// This is an analog for the transport::is_connected(), which is
    /// guarded by a mutex.
    bool is_connected_;
  };


  /**
   * @class Cache_ExtId_T
   *
   * @brief Helper class for TAO_Transport_Cache_Manager: unifies
   * several data items, so they can be stored together as a
   * @c value for a @c key in a hash table holding the state of the
   * Transport Cache.
   */
  template <typename TRANSPORT_DESCRIPTOR_TYPE>
  class Cache_ExtId_T
  {
  public:
    typedef TRANSPORT_DESCRIPTOR_TYPE transport_descriptor_type;

    /// Constructor.
    Cache_ExtId_T (void);

    /// Constructor.
    explicit Cache_ExtId_T (transport_descriptor_type *prop);

    /// Copy constructor.
    Cache_ExtId_T (const Cache_ExtId_T & rhs);

    /// Destructor.
    ~Cache_ExtId_T (void);

    // = Assignment and comparison operators.
    /// Assignment operator (does copy memory).
    Cache_ExtId_T& operator= (const Cache_ExtId_T &rhs);

    /// Equality comparison operator (must match both id_ and kind_).
    bool operator== (const Cache_ExtId_T &rhs) const;

    /// Inequality comparison operator.
    bool operator!= (const Cache_ExtId_T &rhs) const;

    /// hash function is required in order for this class to be usable by
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
    transport_descriptor_type *property (void) const;

  private:
    /// A property object that we represent.
    transport_descriptor_type *transport_property_;

    /// Do we need to delete transport_property?
    bool is_delete_;

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
# include "tao/Cache_Entries_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Cache_Entries_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("tao/Cache_Entries_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_CACHE_ENTRIES_T_H */
