/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   Cache_Entries.h
 *
 *  $Id$
 *
 *  @author Bala Natarajan  <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_CACHE_ENTRIES_H
#define TAO_CACHE_ENTRIES_H
#include "ace/pre.h"

#include "tao/Connection_Descriptor_Interface.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Connection_Handler;
class Tao_Base_Connection_Property;


/**
 * @class TAO_Cache_IntId
 *
 * @brief Helper class for TAO_Connection_Cache_Manager
 *
 * Helper class that wraps the <value> part of the Map or
 * table holding the Connection state.: unifies data items, so
 * they can be stored together as a <value> for a <key> in a
 * table holding the state of the Connection Cache.
 */
class TAO_Export TAO_Cache_IntId
{

public:

  // = Initialization and termination methods.

  /// Constructor.
  TAO_Cache_IntId (void);

  /// Constructor.
  TAO_Cache_IntId (TAO_Connection_Handler *handler);

  /// Copy constructor.
  TAO_Cache_IntId (const TAO_Cache_IntId & rhs);

  /// Destructor.
  ~TAO_Cache_IntId (void);

  /// Assignment operator (does copy memory).
  void operator= (const TAO_Cache_IntId &rhs);

  /// Equality comparison operator (must match both id_ and kind_).
  int operator== (const TAO_Cache_IntId &rhs) const;

  /// Inequality comparison operator.
  int operator!= (const TAO_Cache_IntId &rhs) const;

  /// Return the underlying handler
  TAO_Connection_Handler *handler (void);

  /// Return the underlying handler
  const TAO_Connection_Handler *handler (void) const;

  void recycle_state (ACE_Recyclable_State new_state);

  /// Get/Set <recycle_state>.
  ACE_Recyclable_State recycle_state (void);

private:

  /// The connection handler that needs to be cached.
  TAO_Connection_Handler *handler_;

  /// The state of the handle
  ACE_Recyclable_State recycle_state_;
};


/**
 * @class TAO_Cache_ExtId
 *
 * @brief Helper class for TAO_Connection_Cache_Manager: unifies
 * several  data items, so they can be stored together as a
 * <value> for a <key> in a hash table holding the state of the
 * Connection Cache.
 *
 */
class TAO_Export TAO_Cache_ExtId
{
public:
  // = Initialization and termination methods.

  /// Constructor.
  TAO_Cache_ExtId (void);

  /// Constructor.
  TAO_Cache_ExtId (TAO_Connection_Descriptor_Interface *prop);

  /// Copy constructor.
  TAO_Cache_ExtId (const TAO_Cache_ExtId & rhs);

  /// Destructor.
  ~TAO_Cache_ExtId (void);

  // = Assignment and comparison operators.
  /// Assignment operator (does copy memory).
  void operator= (const TAO_Cache_ExtId &rhs);

  /// Equality comparison operator (must match both id_ and kind_).
  int operator== (const TAO_Cache_ExtId &rhs) const;

  /// Inequality comparison operator.
  int operator!= (const TAO_Cache_ExtId &rhs) const;

  /// <hash> function is required in order for this class to be usable by
  /// ACE_Hash_Map_Manager.
  u_long hash (void) const;

  /// Make a deep copy of the underlying pointer
  void duplicate (void);

  /// Return the index value
  CORBA::ULong index (void);
  CORBA::ULong index (void) const;

  /// Set the index value. This calls should not be used by any users
  /// but for the TAO_Connection_Cache_Manager class.
  void index (CORBA::ULong index);

  // = Accessors
  /// Get the underlying the property pointer
  TAO_Connection_Descriptor_Interface *property (void) const;

private:
  // = Data members.

  /// A property object that we represent.
  TAO_Connection_Descriptor_Interface *connection_property_;

  /// Do we need to delete connection_propert_?
  CORBA::Boolean is_delete_;

  /**
   * This is a supplementary index. Would be set to zero by
   * default. Would be altered by the Connection_Cache of TAO. Please
   * see the documentation of TAO_Connection_Cache_Manager for
   * details.
   */
  CORBA::ULong index_;
};


#if defined (__ACE_INLINE__)
# include "tao/Cache_Entries.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_CACHE_ENTRIES_H */
