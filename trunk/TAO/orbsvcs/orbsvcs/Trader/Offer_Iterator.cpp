/* -*- C++ -*- */

// ============================================================================
// $Id$
// 
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//   Offer_Iterator.cpp
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ============================================================================
#define ACE_BUILD_DLL

#include "Offer_Iterator.h"


TAO_Offer_Iterator::TAO_Offer_Iterator (const TAO_Property_Filter& pfilter)
  : pfilter_ (pfilter)
{
}

TAO_Offer_Iterator::~TAO_Offer_Iterator (void)
{
}

void 
TAO_Offer_Iterator::destroy (void)
  TAO_THROW_SPEC (CORBA::SystemException)
{
  CORBA::release (this);
}

                

