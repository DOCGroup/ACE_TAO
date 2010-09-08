// $Id$

#ifndef TAO_SIMPLE_DYNAMIC_C
#define TAO_SIMPLE_DYNAMIC_C

#include "Simple_Dynamic.h"



template <class T>
TAO_Simple_Dynamic_Property<T>::TAO_Simple_Dynamic_Property (const T& dp)
  : dp_ (dp)
{
}

template <class T> CORBA::Any*
TAO_Simple_Dynamic_Property<T>::evalDP (const char*,
                                        CORBA::TypeCode_ptr,
                                        const CORBA::Any&)
{
  CORBA::Any* return_value = 0;
  ACE_NEW_RETURN (return_value, CORBA::Any, 0);

  (*return_value) <<= this->dp_;
  return return_value;
}

#endif /* TAO_SIMPLE_DYNAMIC_C */
