// -*- C++ -*-

// ===================================================================
/**
 *  @file   Connection_Descriptor_Interface.h
 *
 *  $Id$
 *
 *  @author DOC Center - Washington University at St. Louis
 *  @author DOC Laboratory - University of California at Irvine
 */
// ===================================================================

#ifndef TAO_CONNECTION_DESCRIPTOR_INTERFACE_H
#define TAO_CONNECTION_DESCRIPTOR_INTERFACE_H

#include "ace/pre.h"

#include "tao/Endpoint.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_Connection_Descriptor_Interface
 *
 * @brief An abstract base class for Connection Property
 *
 * This class provides an abstract interface and holds minimal info
 * on which the Connection Caching scheme is based on. Concrete
 * connection properties can be got by inheriting from this class and
 * implementing the virtual functions.
 * Note 1: Additional properties for connection like Qos,
 * Priority that the RT folks would need, can be added by
 * inheriting from this class and providing the following
 * methods.
 * 1. duplicate ()
 * 2. is_equivalent ()
 * 3. hash ()
 */


class TAO_Export TAO_Connection_Descriptor_Interface
{
public:

  /// Destructor
  virtual ~TAO_Connection_Descriptor_Interface (void);

  /// This call allocates and copies the contents of this class and
  /// returns the pointer
  virtual  TAO_Connection_Descriptor_Interface *duplicate (void) = 0;

  /// Try to determine if this object is same as the <other_prop>.
  virtual CORBA::Boolean is_equivalent (
      const TAO_Connection_Descriptor_Interface *other_prop) = 0;

  /// Generate hash value for our class
  virtual u_long hash (void) const = 0;

  /// Return the underlying endpoint object
  TAO_Endpoint *endpoint (void);

protected:

  /// Default Constructor
  TAO_Connection_Descriptor_Interface (void);

  /// Constructor
  TAO_Connection_Descriptor_Interface (TAO_Endpoint *endpoint,
                                       CORBA::Boolean flag = 0);

  /// The base property of the connection ie. the peer's endpoint
  TAO_Endpoint *endpoint_;

  /// Is the endpoint allocated on the heap? If so, we will have to
  /// delete it when we destruct ourselves.
  CORBA::Boolean endpoint_from_heap_;

};


#if defined (__ACE_INLINE__)
# include "tao/Connection_Descriptor_Interface.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif /*TAO_CONNECTION_DESCRIPTOR_INTERFACE_H*/
