// -*- C++ -*-

// ===================================================================
/**
 *  @file   Transport_Descriptor_Interface.h
 *
 *  $Id$
 *
 *  @author Bala Natarajan <bala@cs.wustl.edu>
 */
// ===================================================================

#ifndef TAO_CONNECTION_DESCRIPTOR_INTERFACE_H
#define TAO_CONNECTION_DESCRIPTOR_INTERFACE_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Endpoint;

/**
 * @class TAO_Transport_Descriptor_Interface
 *
 * @brief An abstract base class for Transport Property
 *
 * This class provides an abstract interface and holds minimal info
 * on which the Transport Caching scheme is based on. Concrete
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
class TAO_Export TAO_Transport_Descriptor_Interface
{
public:
  /// Destructor
  virtual ~TAO_Transport_Descriptor_Interface (void);

  /// This call allocates and copies the contents of this class and
  /// returns the pointer
  virtual  TAO_Transport_Descriptor_Interface *duplicate (void) = 0;

  /// Try to determine if this object is same as the @a other_prop.
  virtual CORBA::Boolean is_equivalent (
      const TAO_Transport_Descriptor_Interface *other_prop) = 0;

  /// Generate hash value for our class
  virtual u_long hash (void) const = 0;

  /// Return the underlying endpoint object
  TAO_Endpoint *endpoint (void);

  /// Reset the endpoint pointer to point to another, if that one is
  /// part of the chain based by the current endpoint. Although this
  /// method is public it should only be used by the protocol specific
  /// connector, right before caching, and only when a parallel
  /// connect was attempted with more than one possible endpoints.
  CORBA::Boolean reset_endpoint (TAO_Endpoint *ep);

  /// Set the BiDir flag
  void set_bidir_flag (CORBA::Boolean flag);

protected:
  /// Default Constructor
  TAO_Transport_Descriptor_Interface (void);

  /// Constructor
  TAO_Transport_Descriptor_Interface (TAO_Endpoint *endpoint,
                                      CORBA::Boolean take_ownership = false);

  /// The base property of the connection ie. the peer's endpoint
  TAO_Endpoint *endpoint_;

  /// Should the endpoint be used in either direction?
  CORBA::Boolean bidir_flag_;

  /// Is the endpoint allocated on the heap? If so, we will have to
  /// delete it when we destruct ourselves.
  CORBA::Boolean release_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Transport_Descriptor_Interface.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /*TAO_CONNECTION_DESCRIPTOR_INTERFACE_H*/
