/* -*- C++ -*- */
/**
 *  @file Property_Boolean.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_PROPERTY_BOOLEAN_H
#define TAO_NS_PROPERTY_BOOLEAN_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotificationC.h"

class TAO_NS_PropertySeq;

/**
 * @class TAO_NS_Property_Boolean
 *
 * @brief Boolean Property.
 *
 */
/*******************************************************************************/

class TAO_Notify_Export TAO_NS_Property_Boolean
{
public:
  /// Constuctor
  TAO_NS_Property_Boolean (const ACE_CString& name, CORBA::Boolean initial);

  /// Constuctor
  TAO_NS_Property_Boolean (const ACE_CString& name);

  /// Assignment from TAO_NS_Property_Boolean
  TAO_NS_Property_Boolean& operator= (const TAO_NS_Property_Boolean& rhs);

  /// Assignment from CORBA::Boolean
  TAO_NS_Property_Boolean& operator= (const CORBA::Boolean& rhs);

  /// Equality comparison operator.
  int operator== (const CORBA::Boolean &rhs) const;

  /// Inequality comparison operator.
  int operator!= (const CORBA::Boolean &rhs) const;

  int set (const TAO_NS_PropertySeq& property_seq);

  void get (CosNotification::PropertySeq& prop_seq);

  /// Return the value.
  CORBA::Boolean value (void) const;

  /// Is the current value valid
  CORBA::Boolean is_valid (void) const;

protected:
  /// The Property name.
  ACE_CString name_;

  /// The value
  CORBA::Boolean value_;

  /// Is the value valid
  CORBA::Boolean valid_;
};

#if defined (__ACE_INLINE__)
#include "Property_Boolean.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_PROPERTY_BOOLEAN_H */
