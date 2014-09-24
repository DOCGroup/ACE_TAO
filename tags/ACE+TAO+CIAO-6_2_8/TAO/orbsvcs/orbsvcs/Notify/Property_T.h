// -*- C++ -*-

/**
 *  @file Property_T.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_PROPERTY_T_H
#define TAO_Notify_PROPERTY_T_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SString.h"

#include "orbsvcs/CosNotificationC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_PropertySeq;

/**
 * @class TAO_Notify_PropertyBase_T
 *
 * @brief
 *
 */
template <class TYPE>
class TAO_Notify_PropertyBase_T
{
public:
  /// Constructor
  TAO_Notify_PropertyBase_T (const char* name, const TYPE& initial);

  /// Constructor
  TAO_Notify_PropertyBase_T (const char* name);

  /// Copy Constructor
  TAO_Notify_PropertyBase_T (const TAO_Notify_PropertyBase_T &rhs);

  /// Destructor
  ~TAO_Notify_PropertyBase_T ();

  /// Equality comparison operator.
  bool operator== (const TYPE &rhs) const;

  /// Inequality comparison operator.
  bool operator!= (const TYPE &rhs) const;

  /// Populate the Property Sequence with this valid value.
  void get (CosNotification::PropertySeq& prop_seq);

  // assign a new value
  void assign (const TYPE& value);

  /// Return the value.
  const TYPE& value (void) const;

  /// Return the name
  const char * name (void) const;

  /// Is the current value valid
  CORBA::Boolean is_valid (void) const;

  /// Invalidate this property's value.
  void invalidate (void);

protected:
  /// The Property name.
  const char* name_;

  /// The value
  TYPE value_;

  /// Is the value valid
  CORBA::Boolean valid_;
};


/*******************************************************************************/
/**
 * @class TAO_Notify_Property_T
 *
 * @brief
 *
 */
template <class TYPE>
class TAO_Notify_Property_T : public TAO_Notify_PropertyBase_T<TYPE>
{
public:
  /// Constructor
  TAO_Notify_Property_T (const char* name, const TYPE& initial);

  /// Constructor
  TAO_Notify_Property_T (const char* name);

  /// Init this Property from the sequence.
  /// Returns 0 on success, -1 on error
  int set (const TAO_Notify_PropertySeq& property_seq);

  /// Init this Property from the CosNotification::PropertyValue
  /// Returns 0 on success, -1 on error
  int set (const CosNotification::PropertyValue &value);
};

/*******************************************************************************/
/**
 * @class TAO_Notify_StructProperty_T
 *
 * @brief
 *
 */
template <class TYPE>
class TAO_Notify_StructProperty_T
{
public:
  /// Constructor
  TAO_Notify_StructProperty_T (const char* name, const TYPE& initial);

  /// Constructor
  TAO_Notify_StructProperty_T (const char* name);

  /// Init this Property from the sequence.
  /// Returns 0 on success, -1 on error
  int set (const TAO_Notify_PropertySeq& property_seq);

  /// Return the name
  const char * name (void) const;

  /// Return the value.
  const TYPE& value (void) const;

  /// Is the current value valid
  CORBA::Boolean is_valid (void) const;

protected:
  /// The Property name.
  const char* name_;

  /// The value
  TYPE value_;

  /// Is the value valid
  CORBA::Boolean valid_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Property_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/Notify/Property_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Property_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_PROPERTY_T_H */
