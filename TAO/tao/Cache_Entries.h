/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//   Cache_Entries.h
//
// = AUTHOR
//    Bala Natarajan  <bala@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_CACHE_ENTRIES_H
#define TAO_CACHE_ENTRIES_H
#include "ace/pre.h"

#include "tao/Base_Connection_Property.h"

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

class TAO_Export TAO_Cache_IntId
{
  // = TITLE
  //     Helper class for TAO_Connection_Cache_Manager
  //
  // = DESCRIPTION
  //     Helper class that wraps the <value> part of the Map or
  //     table holding the Connection state.: unifies data items, so
  //     they can be stored together as a <value> for a <key> in a
  //     table holding the state of the Connection Cache.

public:

  // = Initialization and termination methods.

  TAO_Cache_IntId (void);
  // Constructor.

  TAO_Cache_IntId (TAO_Connection_Handler *handler);
  // Constructor.

  TAO_Cache_IntId (const TAO_Cache_IntId & rhs);
  // Copy constructor.

  ~TAO_Cache_IntId (void);
  // Destructor.

  void operator= (const TAO_Cache_IntId &rhs);
  // Assignment operator (does copy memory).

  int operator== (const TAO_Cache_IntId &rhs) const;
  // Equality comparison operator (must match both id_ and kind_).

  int operator!= (const TAO_Cache_IntId &rhs) const;
  // Inequality comparison operator.

  TAO_Connection_Handler *handler (void);
  // Return the underlying handler

  const TAO_Connection_Handler *handler (void) const;
  // Return the underlying handler

private:

  TAO_Connection_Handler *handler_;
  // The connection handler that needs to be cached.
};


class TAO_Export TAO_Cache_ExtId
{
  // = TITLE
  //     Helper class for TAO_Connection_Cache_Manager: unifies
  //     several  data items, so they can be stored together as a
  //     <value> for a <key> in a hash table holding the state of the
  //     Connection Cache.
  //
  // = DESCRIPTION
  //
public:
  // = Initialization and termination methods.

  TAO_Cache_ExtId (void);
  // Constructor.

  TAO_Cache_ExtId (TAO_Base_Connection_Property *prop);
  // Constructor.

  TAO_Cache_ExtId (const TAO_Cache_ExtId & rhs);
  // Copy constructor.

  ~TAO_Cache_ExtId (void);
  // Destructor.

  // = Assignment and comparison operators.
  void operator= (const TAO_Cache_ExtId &rhs);
  // Assignment operator (does copy memory).

  int operator== (const TAO_Cache_ExtId &rhs) const;
  // Equality comparison operator (must match both id_ and kind_).

  int operator!= (const TAO_Cache_ExtId &rhs) const;
  // Inequality comparison operator.

  u_long hash (void) const;
  // <hash> function is required in order for this class to be usable by
  // ACE_Hash_Map_Manager.

  void duplicate (void);
  // Make a deep copy of the underlying pointer

  CORBA::ULong index (void);
  CORBA::ULong index (void) const;
  // Return the index value

  void index (CORBA::ULong index);
  // Set the index value. This calls should not be used by any users
  // but for the TAO_Connection_Cache_Manager class.

  // = Accessors
  TAO_Base_Connection_Property *property (void) const;
  // Get the underlying the property pointer

  void recycle_state (ACE_Recyclable_State new_state);

  ACE_Recyclable_State recycle_state (void);
  // Get/Set <recycle_state>.
private:
  // = Data members.

  TAO_Base_Connection_Property *connection_property_;
  // A property object that we represent.

  CORBA::Boolean is_delete_;
  // Do we need to delete connection_propert_?

  CORBA::ULong index_;
  // This is a supplementary index. Would be set to zero by
  // default. Would be altered by the Connection_Cache of TAO. Please
  // see the documentation of TAO_Connection_Cache_Manager for
  // details.

  ACE_Recyclable_State recycle_state_;
  // The state of the handle
};


#if defined (__ACE_INLINE__)
# include "tao/Cache_Entries.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_CACHE_ENTRIES_H */
