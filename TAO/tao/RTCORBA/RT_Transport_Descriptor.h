/* -*- C++ -*- */
/**
 *  @file RT_Transport_Descriptor.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_RT_TRANSPORT_DESCRIPTOR_H
#define TAO_RT_TRANSPORT_DESCRIPTOR_H
#include "ace/pre.h"

#include "rtcorba_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Transport_Descriptor_Interface.h"
#include "tao/RTCORBA/RT_Transport_Descriptor_Property.h"

/**
 * @class TAO_RT_Transport_Descriptor
 *
 * @brief Transport Descriptor for RTCORBA.
 *
 * The TAO_RT_Transport_Descriptor contains Descriptor Properties.
 * It uses the "Chain of Command" pattern in the implementation of the <_is_equivalent> method.
 *
 */
class TAO_RTCORBA_Export TAO_RT_Transport_Descriptor
  : public TAO_Transport_Descriptor_Interface
{
public:
  /// Constuctor
  TAO_RT_Transport_Descriptor (TAO_Endpoint *endpoint,
                               CORBA::Boolean flag = 0);

  /// Destructor
  ~TAO_RT_Transport_Descriptor ();

  /// Insert Properties.
  void insert (TAO_RT_Transport_Descriptor_Property *descriptor_property);

  /// = TAO_Transport_Descriptor_Interface methods

  /// This call allocates and copies the contents of this class and
  /// returns the pointer
  virtual TAO_Transport_Descriptor_Interface *duplicate (void);

  /// Try to determine if this object is same as the <other_prop>.
  virtual CORBA::Boolean is_equivalent (
      const TAO_Transport_Descriptor_Interface *other_prop);

  /// Generate hash value for our class
  virtual u_long hash (void) const;

private:
  /// Stack of properties.
  TAO_RT_Transport_Descriptor_Property *property_list_;

  /// Flag to delete properties.
  int delete_properties_;
};

#if defined (__ACE_INLINE__)
#include "RT_Transport_Descriptor.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_RT_TRANSPORT_DESCRIPTOR_H */
