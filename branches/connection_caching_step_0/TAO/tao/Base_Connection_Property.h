/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Connection_Property.h
//
// = AUTHOR
//    Bala Natarajan <bala@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_BASE_CONNECTION_PROPERTY_H
#define TAO_BASE_CONNECTION_PROPERTY_H
#include "ace/pre.h"

class TAO_Export TAO_Base_Connection_Property
{
  // = TITLE
  //     Abstracts the base properties of the connection, which will
  //     be used to look up connections in the cache
  //
  // = DESCRIPTION
  //     This class has the fundamental property of the connection
  //     viz. the peer to which it is connected. This class will be
  //     used to do a look up of the connection from the cache.
  //     Note 1: Additional properties for connection like Qos,
  //     Priority that the RT folks would need, can be added by
  //     inheriting from this class.

public:

  TAO_Base_Connection_Property (void);
  // Default constructor


  TAO_Base_Connection_Property (TAO_Endpoint *endpoint,
                                CORBA::Boolean flag = 0);
  // Constructor

  virtual ~TAO_Base_Connection_Property (void);
  // Dtor

  // = Operations that need to be overloaded in all the inherited
  //   classes. Without these the caching of connections may not work
  //   right way.

  TAO_Base_Connection_Property (
      const TAO_Base_Connection_Property &rhs);
  // The copy constructor.

  virtual TAO_Base_Connection_Property *duplicate (void);
  // This call allocates and copies the contents of this class and
  // returns the pointer

  void operator= (const TAO_Base_Connection_Property &rhs);
  // Assignment operator (does copy memory).

  int operator== (const TAO_Base_Connection_Property &rhs) const;
  // Equality comparison operator

  int operator!= (const TAO_Base_Connection_Property &rhs) const;
  // Inequality comparison operator.

  u_long hash (void) const;
  // Generate hash value for our class

private:

  TAO_Endpoint *endpoint_;
  // The base property of the connection ie. the peer's endpoint
  // Note: This endpoint will have a minimal info about the
  // endpoint. This should not be used for any invocation.

  CORBA::Boolean endpoint_flag_;
  // Is the endpoint allocated on the heap? If so, we will have to
  // delete it when we destruct ourselves.
};

#if defined (__ACE_INLINE__)
# include "tao/Base_Connection_Property.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_BASE_CONNECTION_PROPERTY_H*/
