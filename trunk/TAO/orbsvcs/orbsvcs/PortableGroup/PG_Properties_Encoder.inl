/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    PG_Properties_Encoder.h
 *
 *  $Id$
 *
 *  @author Dale Wilson <wilson_d@ociweb.com>
 */
//=============================================================================
#ifndef TAO_PG_PROPERTIES_ENCODER_H
#define TAO_PG_PROPERTIES_ENCODER_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO_PG
{
  ACE_INLINE
  Properties_Encoder::NamedValue::NamedValue ()
  {
  }

  ACE_INLINE
  Properties_Encoder::NamedValue::NamedValue (const ACE_CString & name, const PortableGroup::Value & value)
    : name_ (name)
    , value_ (value)
  {
  }

  ACE_INLINE
  Properties_Encoder::NamedValue::NamedValue (const Properties_Encoder::NamedValue & rhs)
    : name_ (rhs.name_)
    , value_ (rhs.value_)
  {
  }

  ACE_INLINE
  NamedValue &
  Properties_Encoder::NamedValue::operator = (const Properties_Encoder::NamedValue & rhs)
  {
    if (this != &rhs)
    {
      this->name_ = rhs->name_;
      this->value_ = rhs->value_;
    }
  }

  ACE_INLINE
  bool 
  Properties_Encoder::NamedValue::operator == (const Properties_Encoder::NamedValue &rhs) const
  {
    return this->name_ == rhs.name_;
  }

  ACE_INLINE
  bool 
  Properties_Encoder::NamedValue::operator != (const Properties_Encoder::NamedValue &rhs) const
  {
    return this->name_ != rhs.name_;
  }

} //namespace TAO_PG

#endif // TAO_PG_PROPERTIES_ENCODER_H
