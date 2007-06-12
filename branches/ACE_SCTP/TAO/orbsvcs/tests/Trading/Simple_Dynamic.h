// $Id$

// ========================================================================
//
// = FILE
//     Simple_Dyanmic.h
//
// = DESCRIPTION
//    Class that implements a really simple dynamic property.
//
// = AUTHOR
//     Seth Widoff <sbw1@cs.wustl.edu>
//
// =======================================================================

#ifndef TAO_SIMPLE_DYNAMIC_H
#define TAO_SIMPLE_DYNAMIC_H

#include "orbsvcs/Trader/Trader_Utils.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

template<class T>
class TAO_Simple_Dynamic_Property : public TAO_Dynamic_Property
{
public:

  TAO_Simple_Dynamic_Property (const T& dp);

  virtual CORBA::Any* evalDP (const char* /* name */,
                              CORBA::TypeCode_ptr /* returned_type */,
                              const CORBA::Any& /* extra_info */
                              );

private:
  T dp_;
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Simple_Dynamic.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#endif /* TAO_SIMPLE_DYNAMIC_H */
