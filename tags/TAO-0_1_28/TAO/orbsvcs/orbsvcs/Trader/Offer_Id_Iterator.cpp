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

TAO_Offer_Id_Iterator::TAO_Offer_Id_Iterator (void)
{
}

TAO_Offer_Id_Iterator::~TAO_Offer_Id_Iterator (void)
{
  int items_left = this->ids_.size (),
    return_value = 0;

  do
    {
      CosTrading::OfferId offer_id = 0;
      
      return_value = this->ids_.dequeue_head (offer_id);
      if (return_value == 0)
	CORBA::string_free (offer_id);
    }
  while (return_value == 0);
}

CORBA::ULong
TAO_Offer_Id_Iterator::max_left (CORBA::Environment& env)
  TAO_THROW_SPEC ((CORBA::SystemException,
		  CosTrading::UnknownMaxLeft))
{
  return this->ids_.size ();
}

void
TAO_Offer_Id_Iterator::destroy (CORBA::Environment& _env)
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
TAO_Offer_Id_Iterator::next_n (CORBA::ULong n,
			       CosTrading::OfferIdSeq_out _ids,
			       CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  // Calculate the number of Ids to be returned in this.
  int items_left = this->ids_.size(),
    difference = items_left - n,
    returnable_items = (difference >= 0) ? n : items_left;
  CORBA::Boolean return_value = (CORBA::Boolean) (difference > 0);
    
  if (returnable_items == 0)
    {
      ACE_NEW_RETURN (_ids, CosTrading::OfferIdSeq, return_value);
    }
  else
    {
      // Allocate space for the returned OfferIds.
      CosTrading::OfferId* id_buf =
	CosTrading::OfferIdSeq::allocbuf (returnable_items);

      if (id_buf != 0)
	{      
	  // Copy in those ids!
	  for (int i = 0; i < returnable_items; i++)
	    {
	      CosTrading::OfferId offer_id = 0;

	      this->ids_.dequeue_head (offer_id);
	      id_buf[i] = offer_id;
	    }
	  
	  // Place them into an OfferIdSeq.
	  ACE_NEW_RETURN (_ids,
			  CosTrading::OfferIdSeq (returnable_items,
						  returnable_items,
						  id_buf,
						  1),
			  return_value);
	}
      else
	ACE_NEW_RETURN (_ids, CosTrading::OfferIdSeq, return_value);
    }

  // Return true only if there are items left to be returned in
  // subsequent calls.
  return return_value;
}

void
TAO_Offer_Id_Iterator::insert_id (CosTrading::OfferId new_id)
{
  this->ids_.enqueue_tail (new_id);
}
