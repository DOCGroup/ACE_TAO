// -*- C++ -*-

/**
 *  @file Property_Boolean.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_PROPERTY_BOOLEAN_H
#define TAO_Notify_PROPERTY_BOOLEAN_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotificationC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_PropertySeq;

/**
 * @class TAO_Notify_Property_Boolean
 *
 * @brief Boolean Property.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Property_Boolean
{
public:
  /// Constructor
  TAO_Notify_Property_Boolean (const char* name, CORBA::Boolean initial);

  /// Constructor
  TAO_Notify_Property_Boolean (const char* name);

  /// Assignment from TAO_Notify_Property_Boolean
  TAO_Notify_Property_Boolean& operator= (const TAO_Notify_Property_Boolean& rhs);

  /// Assignment from CORBA::Boolean
  TAO_Notify_Property_Boolean& operator= (const CORBA::Boolean& rhs);

  /// Equality comparison operator.
  bool operator== (const CORBA::Boolean &rhs) const;

  /// Inequality comparison operator.
  bool operator!= (const CORBA::Boolean &rhs) const;

  int set (const TAO_Notify_PropertySeq& property_seq);

  void get (CosNotification::PropertySeq& prop_seq);

  /// Return the name
  const char * name (void) const;

  /// Return the value.
  CORBA::Boolean value (void) const;

  /// Is the current value valid
  CORBA::Boolean is_valid (void) const;

protected:
  /// The Property name.
  const char* name_;

  /// The value
  CORBA::Boolean value_;

  /// Is the value valid
  CORBA::Boolean valid_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Property_Boolean.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_PROPERTY_BOOLEAN_H */
