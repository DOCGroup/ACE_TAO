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

#include "ace/Recyclable.h"

class TAO_Transport;

#ifdef index
# undef index
#endif  /* index */

/**
 * @class TAO_Cache_IntId
 *
 * @brief Helper class for TAO_Transport_Cache_Manager
 *
 * Helper class that wraps the <value> part of the Map or
 * table holding the Transport state.: unifies data items, so
 * they can be stored together as a <value> for a <key> in a
 * table holding the state of the Transport Cache.
 */
class TAO_Export TAO_Cache_IntId
{
public:

  /// Constructor.
  TAO_Cache_IntId (void);

  /// Constructor.
  TAO_Cache_IntId (TAO_Transport *transport);

  /// Copy constructor.
  TAO_Cache_IntId (const TAO_Cache_IntId & rhs);

  /// Destructor.
  ~TAO_Cache_IntId (void);

  /// Assignment operator (does copy memory).
  TAO_Cache_IntId& operator= (const TAO_Cache_IntId &rhs);

  /// Equality comparison operator (must match both id_ and kind_).
  int operator== (const TAO_Cache_IntId &rhs) const;

  /// Inequality comparison operator.
  int operator!= (const TAO_Cache_IntId &rhs) const;

  /// Return the underlying transport
  TAO_Transport *transport (void);

  /// Return the underlying transport
  const TAO_Transport *transport (void) const;

  /// Set <recycle_state>.
  void recycle_state (ACE_Recyclable_State new_state);

  /// Get <recycle_state>.
  ACE_Recyclable_State recycle_state (void);

  /// Relinquish ownership of the TAO_Transport object associated with
  /// this TAO_Cache_IntId.
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
  ACE_Recyclable_State recycle_state_;

};


/**
 * @class TAO_Cache_ExtId
 *
 * @brief Helper class for TAO_Transport_Cache_Manager: unifies
 * several  data items, so they can be stored together as a
 * <value> for a <key> in a hash table holding the state of the
 * Transport Cache.
 */
class TAO_Export TAO_Cache_ExtId
{
public:

  /// Constructor.
  TAO_Cache_ExtId (void);

  /// Constructor.
  TAO_Cache_ExtId (TAO_Transport_Descriptor_Interface *prop);

  /// Copy constructor.
  TAO_Cache_ExtId (const TAO_Cache_ExtId & rhs);

  /// Destructor.
  ~TAO_Cache_ExtId (void);

  // = Assignment and comparison operators.
  /// Assignment operator (does copy memory).
  TAO_Cache_ExtId& operator= (const TAO_Cache_ExtId &rhs);

  /// Equality comparison operator (must match both id_ and kind_).
  int operator== (const TAO_Cache_ExtId &rhs) const;

  /// Inequality comparison operator.
  int operator!= (const TAO_Cache_ExtId &rhs) const;

  /// <hash> function is required in order for this class to be usable by
  /// ACE_Hash_Map_Manager_Ex.
  u_long hash (void) const;

  /// Make a deep copy of the underlying pointer
  void duplicate (void);

  //@{
  /// Return the index value
  CORBA::ULong index (void);
  CORBA::ULong index (void) const;
  //@}

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


#if defined (__ACE_INLINE__)
# include "tao/Cache_Entries.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CACHE_ENTRIES_H */
