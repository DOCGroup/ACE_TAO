// $Id$

#ifndef TAO_NS_PROPERTY_T_CPP
#define TAO_NS_PROPERTY_T_CPP

#include "Property_T.h"

#if ! defined (__ACE_INLINE__)
#include "Property_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_Property_T, "$id$")

#include "PropertySeq.h"

/*******************************************************************************/

template <class TYPE>
TAO_NS_PropertyBase_T<TYPE>::TAO_NS_PropertyBase_T (const ACE_CString& name)
  :name_ (name), valid_(0)
{
}

template <class TYPE>
TAO_NS_PropertyBase_T<TYPE>::TAO_NS_PropertyBase_T (const ACE_CString& name, const TYPE& initial)
  :name_ (name), value_ (initial), valid_ (1)
{
}

template <class TYPE>
TAO_NS_PropertyBase_T<TYPE>::TAO_NS_PropertyBase_T (const TAO_NS_PropertyBase_T &rhs)
{
  this->name_ = rhs.name_;
  this->value_ = rhs.value_;
  this->valid_ = rhs.valid_;
}

template <class TYPE>
TAO_NS_PropertyBase_T<TYPE>::~TAO_NS_PropertyBase_T ()
{
}

template <class TYPE> void
TAO_NS_PropertyBase_T<TYPE>::get (CosNotification::PropertySeq& prop_seq)
{
  /// Make space
  prop_seq.length (prop_seq.length () + 1);

  prop_seq[prop_seq.length () - 1].value <<= this->value_;
}

/*******************************************************************************/

template <class TYPE>
TAO_NS_Property_T<TYPE>::TAO_NS_Property_T (const ACE_CString& name)
  :TAO_NS_PropertyBase_T <TYPE> (name)
{
}

template <class TYPE>
TAO_NS_Property_T<TYPE>::TAO_NS_Property_T (const ACE_CString& name, const TYPE& initial)
  :TAO_NS_PropertyBase_T <TYPE> (name, initial)
{
}

template <class TYPE> int
TAO_NS_Property_T<TYPE>::set (const TAO_NS_PropertySeq& property_seq)
{
  CosNotification::PropertyValue value;

  if (property_seq.find (this->name_, value) == -1)
    {
      this->valid_ = 0;
      return -1;
    }

  value >>= this->value_;

  this->valid_ = 1;

  return 0;
}

/*******************************************************************************/

template <class TYPE>
TAO_NS_StructProperty_T<TYPE>::TAO_NS_StructProperty_T (const ACE_CString& name)
  :name_ (name), valid_(0)
{
}

template <class TYPE>
TAO_NS_StructProperty_T<TYPE>::TAO_NS_StructProperty_T (const ACE_CString& name, const TYPE& initial)
  :name_ (name), value_ (initial), valid_ (1)
{
}

template <class TYPE> int
TAO_NS_StructProperty_T<TYPE>::set (const TAO_NS_PropertySeq& property_seq)
{
  CosNotification::PropertyValue value;

  if (property_seq.find (this->name_, value) == -1)
    {
      this->valid_ = 0;
      return -1;
    }

  TYPE* extract_type;
  value >>= extract_type;

  this->value_ = *extract_type; // copy

  this->valid_ = 1;

  return 0;
}

#endif /* TAO_NS_PROPERTY_T_CPP */
