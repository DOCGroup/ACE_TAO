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

template<class T>
class TAO_Simple_Dynamic_Property : public TAO_Dynamic_Property
{
public:

  TAO_Simple_Dynamic_Property (const T& dp);

  virtual CORBA::Any* evalDP (const char* /* name */,
                              CORBA::TypeCode_ptr /* returned_type */,
                              const CORBA::Any& /* extra_info */,
                              CORBA::Environment &)
    TAO_THROW_SPEC ((CosTradingDynamic::DPEvalFailure));

private:

  T dp_;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Simple_Dynamic.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#endif /* TAO_SIMPLE_DYNAMIC_H */
