// $Id$

#if !defined (ACE_LOCAL_LOCATOR_C)
#define ACE_LOCAL_LOCATOR_C

#define ACE_BUILD_DLL
#include "Local_Locator.h"

#if !defined (__ACE_INLINE__)
#include "Local_Locator.i"
#endif /* __ACE_INLINE__ */

int
ACE_URL_Local_Locator::url_query (const ACE_URL_Locator::ACE_Selection_Criteria how,
				  const ACE_URL_Property_Seq *pseq,
				  const size_t how_many,
				  size_t &num_query,
				  ACE_URL_Offer_Seq *offer)
{
  ACE_URL_Record *item = 0;

  // Nanbor, do you really want to return NOMEM, or -1 (since errno
  // will already be set to ENOMEM by the ACE_NEW_RETURN macro).
  ACE_NEW_RETURN (offer, ACE_URL_Offer_Seq (how_many), ACE_URL_Locator::NOMEM);
  
  for (ACE_Unbounded_Stack_Iterator<ACE_URL_Record> iter (this->repository_);
       iter.next (item) != 0;
       iter.advance ())
    {
      switch (how)
	{
	case ACE_URL_Locator::NONE:
	  // Offers must not have any properties in <pseq>
	  return ACE_URL_Locator::UNIMPLEMENTED;
	  break;
	case ACE_URL_Locator::SOME:
	  // Offers must have at least one property in <pseq>
	  return ACE_URL_Locator::UNIMPLEMENTED;
	  break;
	case ACE_URL_Locator::ALL:
	  // Offers must have all the properties in <pseq>
	  return ACE_URL_Locator::UNIMPLEMENTED;
	  break;
	default:
	  delete [] offer ;
	  offer = 0;
	  return ACE_URL_Locator::INVALID_ARGUMENT;
	}
      if (how_many == 0)
	break;
    }

  // Nanbor, I recommend just returning 0 for success and -1 for
  // failure.  That is more consistent with the rest of ACE.  In
  // general, it's a better idea to use errno to indicate *which*
  // error occurred, and -1 to indicate to the caller to check errno.
  return ACE_URL_Locator::OK;
}

int
ACE_URL_Local_Locator::export_offer (ACE_URL_Offer *offer,
				     ACE_WString &offer_id)
{
  ACE_URL_Record *item = 0;

  // First check if we have registered this URL already.
  for (ACE_Unbounded_Stack_Iterator<ACE_URL_Record> iter (this->repository_);
       iter.next (item) != 0; 
       iter.advance ())
    if (*item->offer_->url () == *offer->url ())
      // Nanbor, here's a good example of where it might make more
      // sense to set errno to EEXIST and return -1.
      return ACE_URL_Locator::OFFER_EXIST;
  
  ACE_URL_Record *new_offer;

  ACE_NEW_RETURN (new_offer, ACE_URL_Record (offer),
		  ACE_URL_Locator::NOMEM);

  this->repository_.push (*new_offer);

  offer_id = *new_offer->id_;
  return ACE_URL_Locator::OK;
}

int
ACE_URL_Local_Locator::withdraw_offer (const ACE_WString &offer_id)
{
  ACE_URL_Record *item = 0;
  
  // Nanbor, please make sure that you comment code like this.
  for (ACE_Unbounded_Stack_Iterator<ACE_URL_Record> iter (this->repository_);
       iter.next (item) != 0;
       iter.advance ())
      if (offer_id == *item->id_)
	{
	  if (this->repository_.remove (*item) == 0)
	    return ACE_URL_Locator::OK;
	  else
	    return ACE_URL_Locator::UNKNOWN;
	}

  return ACE_URL_Locator::NO_SUCH_OFFER;
}

int
ACE_URL_Local_Locator::describe_offer (const ACE_WString &offer_id,
				       ACE_URL_Offer *offer)
{
  ACE_URL_Record *item = 0;
  
  // Nanbor, please make sure that you comment this code.
  for (ACE_Unbounded_Stack_Iterator<ACE_URL_Record> iter (this->repository_);
       iter.next (item) != 0;
       iter.advance ())
    if (offer_id == *item->id_)
      {
	ACE_NEW_RETURN (offer, ACE_URL_Offer (*item->offer_),
			ACE_URL_Locator::NOMEM);
	return ACE_URL_Locator::OK;
      }

  return ACE_URL_Locator::NO_SUCH_OFFER;
}

int
ACE_URL_Local_Locator::modify_offer (const ACE_WString &offer_id,
				     const char *url,
				     const ACE_URL_Property_Seq *del,
				     const ACE_URL_Property_Seq *modify)
{
  return ACE_URL_Locator::UNIMPLEMENTED;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Unbounded_Stack<ACE_URL_Record>;
template class ACE_Unbounded_Stack_Iterator<ACE_URL_Record>;
template class ACE_Node<ACE_URL_Record>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Unbounded_Stack<ACE_URL_Record>
#pragma instantiate ACE_Unbounded_Stack_Iterator<ACE_URL_Record>
#pragma instantiate ACE_Node<ACE_URL_Record>
#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
#endif /* ACE_LOCAL_LOCATOR_C */
