// $Id$

#include "tao/Managed_Types.h"

#if !defined (__ACE_INLINE__)
#include "tao/Managed_Types.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao,
           Managed_Types,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// assignment from CORBA::String_var makes a copy
TAO_String_Manager&
TAO_String_Manager::operator= (const CORBA::String_var &var)
{
  // Strongly exception safe by means of copy and non-throwing swap
  // technique.
  TAO_String_Manager tmp (var.in ());

  char * old_ptr = this->ptr_;
  this->ptr_ = tmp.ptr_;
  tmp.ptr_  = old_ptr;

  return *this;
}

// ****************************************************************

// assignment from CORBA::WString_var makes a copy
TAO_WString_Manager&
TAO_WString_Manager::operator= (const CORBA::WString_var &var)
{
  // Strongly exception safe by means of copy and non-throwing swap
  // technique.
  TAO_WString_Manager tmp (var.in ());

  CORBA::WChar * old_ptr = this->ptr_;
  this->ptr_ = tmp.ptr_;
  tmp.ptr_  = old_ptr;

  return *this;
}

TAO_END_VERSIONED_NAMESPACE_DECL
