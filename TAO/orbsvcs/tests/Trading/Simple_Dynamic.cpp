#ifndef TAO_SIMPLE_DYNAMIC_C
#define TAO_SIMPLE_DYNAMIC_C

#include "Simple_Dynamic.h"

#include <utility>

template <class T>
TAO_Simple_Dynamic_Property<T>::TAO_Simple_Dynamic_Property (T const &dp)
  : dp_ (dp)
{
}

template <class T>
TAO_Simple_Dynamic_Property<T>::TAO_Simple_Dynamic_Property (T &&dp)
  : dp_ (std::move (dp))
{
}

template <class T>
CORBA::Any *
TAO_Simple_Dynamic_Property<T>::evalDP (char const *,
                                        CORBA::TypeCode_ptr,
                                        CORBA::Any const &)
{
  CORBA::Any *return_value {};
  ACE_NEW_RETURN (return_value, CORBA::Any, nullptr);

  (*return_value) <<= this->dp_;
  return return_value;
}

#endif /* TAO_SIMPLE_DYNAMIC_C */
