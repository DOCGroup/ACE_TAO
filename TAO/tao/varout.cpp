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

#if !defined (__ACE_INLINE__)
#include "tao/varout.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, varout, "$Id$")

template<class T> void
TAO_Object_Field_T<T>::_downcast (CORBA_Object* base_ptr,
				  CORBA_Environment &env)
{
  CORBA::release (this->ptr_);
  this->ptr_ = 0;
  this->ptr_ = T::_narrow (base_ptr, env);
  if (env.exception () != 0) return;
}

template<class T> CORBA_Object*
TAO_Object_Field_T<T>::_upcast (void)
{
  return this->ptr_;
}

template<class T> void
TAO_Object_Field_T<T>::_release (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = 0;
}

#endif /* TAO_VAROUT_C */
