/* -*- C++ -*- */
// $Id$

// ===================================================================
/**
 *  @file  Base_Connection_Property.h
 *
 *  $Id$
 *
 *  @author Bala Natarajan <bala@cs.wustl.edu>
 */
// ===================================================================

#ifndef TAO_BASE_CONNECTION_PROPERTY_H
#define TAO_BASE_CONNECTION_PROPERTY_H
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


/**
 * @class TAO_Base_Connection_Property
 *
 * @brief A concrete implementation for connnection property
 *
 * This class is a concrete implementation of a simple connection
 * property class. This provides an implementation for the virtual
 * functions declared in TAO_Connection_Descriptor_Interface.
 */


class TAO_Export TAO_Base_Connection_Property:
  public TAO_Connection_Descriptor_Interface
{
public:

  /// Default constructor
  TAO_Base_Connection_Property (void);


  /// Constructor
  TAO_Base_Connection_Property (TAO_Endpoint *endpoint,
                                CORBA::Boolean flag = 0);

  /// Dtor
  virtual ~TAO_Base_Connection_Property (void);


  /// The copy constructor.
  TAO_Base_Connection_Property (
      const TAO_Base_Connection_Property &rhs);


  /// This call allocates and copies the contents of this class and
  /// returns the pointer
  virtual  TAO_Connection_Descriptor_Interface *duplicate (void);

  /// Try to determine if this object is same as the <other_prop>.
  virtual CORBA::Boolean is_equivalent (
      const TAO_Connection_Descriptor_Interface *other_prop);

  /// Generate hash value for our class
  virtual u_long hash (void) const;
};

#if defined (__ACE_INLINE__)
# include "tao/Base_Connection_Property.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_BASE_CONNECTION_PROPERTY_H*/
