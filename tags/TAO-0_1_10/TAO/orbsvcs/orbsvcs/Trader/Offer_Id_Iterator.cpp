// ============================================================================
// $Id$
// 
// = LIBRARY 
//    orbsvcs
//  
// = FILENAME 
//    Offer_Id_terator.cpp
//
// = AUTHOR 
//    Seth Widoff <sbw1@cs.wustl.edu>
//  
// ============================================================================ 

#include "Offer_Id_Iterator.h"

TAO_Offer_Id_Iterator::TAO_Offer_Id_Iterator(void)
{
}

TAO_Offer_Id_Iterator::~TAO_Offer_Id_Iterator (void)
{
  int items_left = this->ids_.size ();

  for (int i = 0; i < items_left; i++)
    {
      CORBA::string_free (this->ids_.front ());
      this->ids_.pop ();
    }
}

CORBA::ULong
TAO_Offer_Id_Iterator::max_left(CORBA::Environment& env)
  TAO_THROW_SPEC ((CORBA::SystemException,
		  CosTrading::UnknownMaxLeft))
{
  return this->ids_.size ();
}

void
TAO_Offer_Id_Iterator::destroy(CORBA::Environment& _env)
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

CORBA::Boolean
TAO_Offer_Id_Iterator::next_n(CORBA::ULong n,
			      CosTrading::OfferIdSeq_out _ids,
			      CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  // Calculate the number of Ids to be returned in this . 
  int items_left = this->ids_.size(),
    difference = items_left - n,
    returnable_items = (difference >= 0) ? n : items_left;

  if (returnable_items == 0)
    _ids = new CosTrading::OfferIdSeq;
  else
    {
      // Allocate space for the returned OfferIds.
      CosTrading::OfferId* id_buf =
	CosTrading::OfferIdSeq::allocbuf (returnable_items);
      
      // Copy in those ids!
      for (int i = 0; i < returnable_items; i++)
	{
	  char* offer_id = this->ids_.front ();
	  id_buf[i] = offer_id;
	  this->ids_.pop ();
	}
      
      // Place them into an OfferIdSeq.
      _ids = new CosTrading::OfferIdSeq (returnable_items,
					 returnable_items,
					 id_buf,
					 1);
    }

  // Return true only if there are items left to be returned in
  // subsequent calls.
  return (CORBA::Boolean)(difference > 0);
}

void
TAO_Offer_Id_Iterator::insert_id(const char* new_id)
{
  this->ids_.push (CORBA::string_dup (new_id));
}
