/* -*- C++ -*- */

// ========================================================================
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
// ========================================================================

#include "Offer_Iterator.h"


TAO_Offer_Iterator::TAO_Offer_Iterator (const TAO_Property_Filter& pfilter)
  : pfilter_ (pfilter)
{
}

TAO_Offer_Iterator::~TAO_Offer_Iterator (void)
{
}

void 
TAO_Offer_Iterator::destroy (CORBA::Environment& env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  // Remove self from POA
  //
  // Note that there is no real error checking here as we can't do
  // much about errors here anyway
  //
  
  TAO_TRY
    {
      PortableServer::POA_var poa = this->_default_POA (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      PortableServer::ObjectId_var id = poa->servant_to_id (this, TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      poa->deactivate_object (id.in (), TAO_TRY_ENV);
    }
  TAO_CATCHANY
    {      
    }
  TAO_ENDTRY;
  
  delete this;  
}

                

