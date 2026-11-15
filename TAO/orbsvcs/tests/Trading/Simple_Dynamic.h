#ifndef TAO_SIMPLE_DYNAMIC_H
#define TAO_SIMPLE_DYNAMIC_H

#include "orbsvcs/Trader/Trader_Utils.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/// Implements a dynamic property whose value is supplied at construction.
template <class T>
class TAO_Simple_Dynamic_Property : public TAO_Dynamic_Property
{
public:
  explicit TAO_Simple_Dynamic_Property (T const &dp);
  explicit TAO_Simple_Dynamic_Property (T &&dp);

  CORBA::Any *evalDP (char const * /* name */,
                      CORBA::TypeCode_ptr /* returned_type */,
                      CORBA::Any const &) override; /* extra_info */

private:
  T dp_;
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "Simple_Dynamic.cpp"

#endif /* TAO_SIMPLE_DYNAMIC_H */
