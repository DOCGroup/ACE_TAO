/**
 *  @file Property_T.h
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
 */
template <class TYPE>
class TAO_Notify_PropertyBase_T
{
public:
  TAO_Notify_PropertyBase_T (char const *name, TYPE const& initial);

  explicit TAO_Notify_PropertyBase_T (char const *name);

  TAO_Notify_PropertyBase_T (TAO_Notify_PropertyBase_T const &rhs);

  ~TAO_Notify_PropertyBase_T () = default;

  /// Equality comparison operator.
  bool operator== (TYPE const &rhs) const;

  /// Inequality comparison operator.
  bool operator!= (TYPE const &rhs) const;

  /// Populate the Property Sequence with this valid value.
  void get (CosNotification::PropertySeq& prop_seq);

  // assign a new value
  void assign (TYPE const& value);

  /// Return the value.
  TYPE const& value () const;

  /// Return the name
  char const *name () const;

  /// Is the current value valid
  CORBA::Boolean is_valid () const;

  /// Invalidate this property's value.
  void invalidate ();

protected:
  /// The Property name.
  char const *name_;

  /// The value
  TYPE value_ {};

  /// Is the value valid
  CORBA::Boolean valid_ {};
};

/*******************************************************************************/
/**
 * @class TAO_Notify_Property_T
 *
 * @brief
 */
template <class TYPE>
class TAO_Notify_Property_T : public TAO_Notify_PropertyBase_T<TYPE>
{
public:
  TAO_Notify_Property_T (char const *name, TYPE const& initial);

  explicit TAO_Notify_Property_T (char const *name);

  /// Init this Property from the sequence.
  /// Returns 0 on success, -1 on error
  int set (TAO_Notify_PropertySeq const& property_seq);

  /// Init this Property from the CosNotification::PropertyValue
  /// Returns 0 on success, -1 on error
  int set (CosNotification::PropertyValue const &value);
};

/*******************************************************************************/
/**
 * @class TAO_Notify_StructProperty_T
 *
 * @brief
 */
template <class TYPE>
class TAO_Notify_StructProperty_T
{
public:
  TAO_Notify_StructProperty_T (char const *name, TYPE const& initial);

  explicit TAO_Notify_StructProperty_T (char const *name);

  /// Init this Property from the sequence.
  /// Returns 0 on success, -1 on error
  int set (TAO_Notify_PropertySeq const& property_seq);

  /// Return the name
  char const *name () const;

  /// Return the value.
  TYPE const& value () const;

  /// Is the current value valid
  CORBA::Boolean is_valid () const;

protected:
  /// The Property name.
  char const *name_;

  /// The value
  TYPE value_ {};

  /// Is the value valid
  CORBA::Boolean valid_ {};
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Property_T.inl"
#endif /* __ACE_INLINE__ */

#include "orbsvcs/Notify/Property_T.cpp"

#include /**/ "ace/post.h"
#endif /* TAO_Notify_PROPERTY_T_H */
