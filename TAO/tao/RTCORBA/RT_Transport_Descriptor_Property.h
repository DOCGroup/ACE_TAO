/* -*- C++ -*- */
/**
 *  @file RT_Transport_Descriptor_Property.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_RT_TRANSPORT_DESCRIPTOR_PROPERTY_H
#define TAO_RT_TRANSPORT_DESCRIPTOR_PROPERTY_H
#include "ace/pre.h"

#include "rtcorba_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"

/**
 * @class TAO_RT_Transport_Descriptor_Property
 *
 * @brief Base RT Transport Descriptor Property that can be <insert> ed into the TAO_RT_Transport_Descriptor class.
 *
 */
class TAO_RTCORBA_Export TAO_RT_Transport_Descriptor_Property
{
  friend class TAO_RT_Transport_Descriptor;

public:
  /// Constuctor
  TAO_RT_Transport_Descriptor_Property (void);

  /// Destructor
  ~TAO_RT_Transport_Descriptor_Property ();

  virtual TAO_RT_Transport_Descriptor_Property *duplicate (void) = 0;

  virtual CORBA::Boolean is_equivalent (const TAO_RT_Transport_Descriptor_Property *other_prop) = 0;

protected:
  /// Properties can be chanined using the <next_> pointer;
  TAO_RT_Transport_Descriptor_Property* next_;

private:
  // = Disallow copy constructor and assignment operator.
  ACE_UNIMPLEMENTED_FUNC (TAO_RT_Transport_Descriptor_Property (const TAO_RT_Transport_Descriptor_Property &))
  ACE_UNIMPLEMENTED_FUNC (TAO_RT_Transport_Descriptor_Property &operator = (const TAO_RT_Transport_Descriptor_Property &))
};

/**
 * @class TAO_RT_Transport_Descriptor_Private_Connection_Property
 *
 * @brief Descriptor Property for Private Connections.
 *
 * Holds info necessary to identify private connections and
 * store/look them up in the Transport Cache.  (For description
 * of private connections see RTCORBA::PrivateTransportPolicy.)
 *
 */
class TAO_RTCORBA_Export TAO_RT_Transport_Descriptor_Private_Connection_Property
  : public TAO_RT_Transport_Descriptor_Property
{
public:
  /// Constuctor
  TAO_RT_Transport_Descriptor_Private_Connection_Property (void);
  TAO_RT_Transport_Descriptor_Private_Connection_Property (long object_id);

  /// Destructor
  ~TAO_RT_Transport_Descriptor_Private_Connection_Property ();

  /// Init
  void init (long object_id);

  virtual TAO_RT_Transport_Descriptor_Property *duplicate (void);

  virtual CORBA::Boolean is_equivalent (const TAO_RT_Transport_Descriptor_Property *other_prop);

private:
  /**
   * Unique identifier of the object to which private connection
   * identified with this descriptor belongs.  The value of
   * @c object_id_ is the @c TAO_Stub* of the object.
   */
  long object_id_;

};

/*************************************************************************************************/

/**
 * @class TAO_RT_Transport_Descriptor_Banded_Connection_Property
 *
 * @brief Descriptor Property for Banded Connections.
 *
 * This property holds the Band information necessary to identify a banded connection.
 *
 */
class TAO_RTCORBA_Export TAO_RT_Transport_Descriptor_Banded_Connection_Property
  : public TAO_RT_Transport_Descriptor_Property
{
public:
  /// Constuctor
  TAO_RT_Transport_Descriptor_Banded_Connection_Property (void);
  TAO_RT_Transport_Descriptor_Banded_Connection_Property (CORBA::Short low_priority,
                                                          CORBA::Short high_priority);

  /// Destructor
  ~TAO_RT_Transport_Descriptor_Banded_Connection_Property ();

  /// Init
  void init (CORBA::Short low_priority, CORBA::Short high_priority);

  virtual TAO_RT_Transport_Descriptor_Property *duplicate (void);

  virtual CORBA::Boolean is_equivalent (const TAO_RT_Transport_Descriptor_Property *other_prop);

protected:
  /// The low priority of the Band.
  CORBA::Short low_priority_;

  /// The high priority of the Band.
  CORBA::Short high_priority_;
};

/*************************************************************************************************/

#if defined (__ACE_INLINE__)
#include "RT_Transport_Descriptor_Property.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_RT_TRANSPORT_DESCRIPTOR_PROPERTY_H */
