// $Id$

#ifndef TAO_SIMPLE_DYNAMIC_C
#define TAO_SIMPLE_DYNAMIC_C

#include "Simple_Dynamic.h"

ACE_RCSID(Trading, Simple_Dynamic, "$Id$")

template <class T>
TAO_Simple_Dynamic_Property<T>::TAO_Simple_Dynamic_Property (const T& dp)
  : dp_ (dp)
{  
}

template <class T> CORBA::Any*
TAO_Simple_Dynamic_Property<T>::evalDP (const char* name,
                                        CORBA::TypeCode_ptr returned_type,
                                        const CORBA::Any& extra_info,
                                        CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CosTradingDynamic::DPEvalFailure))
{
  CORBA::Any* return_value = 0;
  ACE_NEW_RETURN (return_value, CORBA::Any, 0);

  (*return_value) <<= this->dp_;
  return return_value;
}

#endif /* TAO_SIMPLE_DYNAMIC_C */
