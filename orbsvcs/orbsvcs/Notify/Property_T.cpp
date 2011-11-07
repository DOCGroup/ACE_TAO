// $Id$

#ifndef TAO_Notify_PROPERTY_T_CPP
#define TAO_Notify_PROPERTY_T_CPP

#include "orbsvcs/Notify/Property_T.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Property_T.inl"
#endif /* __ACE_INLINE__ */

#include "orbsvcs/Notify/PropertySeq.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/*****************************************************************************/

template <class TYPE>
TAO_Notify_PropertyBase_T<TYPE>::TAO_Notify_PropertyBase_T (const char* name)
  : name_ (name), valid_(0)
{
}

template <class TYPE>
TAO_Notify_PropertyBase_T<TYPE>::TAO_Notify_PropertyBase_T (const char* name, const TYPE& initial)
  : name_ (name), value_ (initial), valid_ (1)
{
}

template <class TYPE>
TAO_Notify_PropertyBase_T<TYPE>::TAO_Notify_PropertyBase_T (
  const TAO_Notify_PropertyBase_T &rhs)
  : name_ (rhs.name_),
    value_ (rhs.value_),
    valid_ (rhs.valid_)
{

}

template <class TYPE>
TAO_Notify_PropertyBase_T<TYPE>::~TAO_Notify_PropertyBase_T ()
{
}

template <class TYPE> void
TAO_Notify_PropertyBase_T<TYPE>::get (CosNotification::PropertySeq& prop_seq)
{
  /// Make space
  prop_seq.length (prop_seq.length () + 1);

  prop_seq[prop_seq.length () - 1].value <<= this->value_;
}

/*******************************************************************************/

template <class TYPE>
TAO_Notify_Property_T<TYPE>::TAO_Notify_Property_T (const char* name)
  :TAO_Notify_PropertyBase_T <TYPE> (name)
{
}

template <class TYPE>
TAO_Notify_Property_T<TYPE>::TAO_Notify_Property_T (const char* name, const TYPE& initial)
  :TAO_Notify_PropertyBase_T <TYPE> (name, initial)
{
}

template <class TYPE> int
TAO_Notify_Property_T<TYPE>::set (const TAO_Notify_PropertySeq& property_seq)
{
  CosNotification::PropertyValue value;

  if (property_seq.find (this->name_, value) == 0 && (value >>= this->value_))
    {
      this->valid_ = 1;
      return 0;
    }

  this->valid_ = 0;
  return -1;
}

template <class TYPE> int
TAO_Notify_Property_T<TYPE>::set(const CosNotification::PropertyValue &value)
{
  if (value >>= this->value_)
  {
    this->valid_ = 1;
    return 0;
  }

  return -1;
}

/*******************************************************************************/

template <class TYPE>
TAO_Notify_StructProperty_T<TYPE>::TAO_Notify_StructProperty_T (const char* name)
  :name_ (name), valid_(0)
{
}

template <class TYPE>
TAO_Notify_StructProperty_T<TYPE>::TAO_Notify_StructProperty_T (const char* name, const TYPE& initial)
  :name_ (name), value_ (initial), valid_ (1)
{
}

template <class TYPE> int
TAO_Notify_StructProperty_T<TYPE>::set (
  const TAO_Notify_PropertySeq& property_seq)
{
  CosNotification::PropertyValue value;

  if (property_seq.find (this->name_, value) == 0)
    {
      TYPE* extract_type = 0;

      if ((value >>= extract_type)  && extract_type != 0) // make sure we get something valid.
        {
          this->value_ = *extract_type; // copy
          this->valid_ = 1;
          return 0;
        }
    }

  this->valid_ = 0;
  return -1;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_Notify_PROPERTY_T_CPP */
