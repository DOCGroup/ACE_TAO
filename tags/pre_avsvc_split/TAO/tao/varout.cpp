/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    varout.cpp
//
// = DESCRIPTION
//     Templates for _var and _out types
//
// = AUTHOR
//
//   Aniruddha Gokhale
//   Carlos O'Ryan
// ============================================================================

#if !defined (TAO_VAROUT_C)
#define TAO_VAROUT_C

#include "tao/varout.h"
#include "tao/Sequence.h"

#if !defined (__ACE_INLINE__)
#include "tao/varout.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, varout, "$Id$")

template<class T, class T_var>
TAO_Object_Field_T<T,T_var>::
    TAO_Object_Field_T (const TAO_Object_Field_T<T,T_var>& rhs)
  :  TAO_Object_Field (rhs), // keep HP/aCC happy
     ptr_ (T::_duplicate (rhs.ptr_))
{
}

template<class T, class T_var>
TAO_Object_Field_T<T,T_var>::~TAO_Object_Field_T (void)
{
  CORBA::release (this->ptr_);
}

template<class T, class T_var> TAO_Object_Field_T<T,T_var> &
TAO_Object_Field_T<T,T_var>::operator= (T* object)
{
  CORBA::release (this->ptr_);
  this->ptr_ = object;
  return *this;
}

template<class T, class T_var> TAO_Object_Field_T<T,T_var> &
TAO_Object_Field_T<T,T_var>::
    operator= (const TAO_Object_Field_T<T,T_var> &rhs)
{
  if (this != &rhs)
    {
      CORBA::release (this->ptr_);
      this->ptr_ = T::_duplicate (rhs.ptr_);
    }
  return *this;
}

template<class T, class T_var>
TAO_Object_Field_T<T,T_var>::
    TAO_Object_Field_T (const TAO_Object_Manager<T,T_var> & rhs)
  : ptr_ (T::_duplicate (rhs.in ()))
{
}

template<class T, class T_var> TAO_Object_Field_T<T,T_var>&
TAO_Object_Field_T<T,T_var>::
    operator= (const TAO_Object_Manager<T,T_var> & rhs)
{
  T* tmp = T::_duplicate (rhs.in ());
  CORBA::release (this->ptr_);
  this->ptr_ = tmp;

  return *this;
}

template<class T, class T_var>
TAO_Object_Field_T<T,T_var>::
    TAO_Object_Field_T (const T_var & rhs)
  : ptr_ (T::_duplicate (rhs.in ()))
{
}

template<class T, class T_var> TAO_Object_Field_T<T,T_var>&
TAO_Object_Field_T<T,T_var>::
    operator= (const T_var & rhs)
{
  T* tmp = T::_duplicate (rhs.in ());
  CORBA::release (this->ptr_);
  this->ptr_ = tmp;

  return *this;
}

template<class T, class T_var> void
TAO_Object_Field_T<T,T_var>::_downcast (CORBA_Object* base_ptr,
                                        CORBA_Environment &ACE_TRY_ENV)
{
  CORBA::release (this->ptr_);
  this->ptr_ = 0;
  this->ptr_ = T::_narrow (base_ptr, ACE_TRY_ENV);
}

template<class T, class T_var> CORBA_Object*
TAO_Object_Field_T<T,T_var>::_upcast (void)
{
  return this->ptr_;
}

template<class T, class T_var> void
TAO_Object_Field_T<T,T_var>::_release (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = 0;
}

template<class T, class T_var> T* &
TAO_Object_Field_T<T,T_var>::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = 0;
  return this->ptr_;
}

#endif /* TAO_VAROUT_C */
