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

  ACE_NEW_RETURN (offer, ACE_URL_Offer_Seq (how_many), -1);
  
  for (ACE_Unbounded_Set_Iterator<ACE_URL_Record> iter (this->repository_);
       iter.next (item) != 0;
       iter.advance ())
    {
      switch (how)
	{
	case ACE_URL_Locator::NONE:
	  // Offers must not have any properties in <pseq>
	  errno = ACE_URL_Locator::UNIMPLEMENTED;
	  return -1;
	  break;
	case ACE_URL_Locator::SOME:
	  // Offers must have at least one property in <pseq>
	  errno = ACE_URL_Locator::UNIMPLEMENTED;
	  return -1;
	  break;
	case ACE_URL_Locator::ALL:
	  // Offers must have all the properties in <pseq>
	  errno = ACE_URL_Locator::UNIMPLEMENTED;
	  return -1;
	  break;
	default:
	  delete [] offer ;
	  offer = 0;
	  errno = ACE_URL_Locator::INVALID_ARGUMENT;
	  return -1;
	}
      if (how_many == 0)
	break;
    }

  return 0;
}

int
ACE_URL_Local_Locator::export_offer (ACE_URL_Offer *offer,
				     ACE_WString &offer_id)
{
  ACE_URL_Record *item = 0;

  // First check if we have registered this URL already.
  for (ACE_Unbounded_Set_Iterator<ACE_URL_Record> iter (this->repository_);
       iter.next (item) != 0; 
       iter.advance ())
    if (*item->offer_->url () == *offer->url ())
      {
	errno = ACE_URL_Locator::OFFER_EXIST;
	return -1;
      }

  ACE_URL_Record *new_offer;

  // Offer is not in repository, we can add new one in safely.
  ACE_NEW_RETURN (new_offer, ACE_URL_Record (offer),
		  ACE_URL_Locator::NOMEM);

  this->repository_.push (*new_offer);

  offer_id = *new_offer->id_;
  return 0;
}

int
ACE_URL_Local_Locator::withdraw_offer (const ACE_WString &offer_id)
{
  ACE_URL_Record *item = 0;
  
  // Iterate thru repository and remove offer with <offer_id>.
  for (ACE_Unbounded_Set_Iterator<ACE_URL_Record> iter (this->repository_);
       iter.next (item) != 0;
       iter.advance ())
      if (offer_id == *item->id_)
	{
	  if (this->repository_.remove (*item) == 0)
	    return 0
	  else
	    {
	      errno = ACE_URL_Locator::UNKNOWN;
	      return -1;
	    }
	}

  errno = ACE_URL_Locator::NO_SUCH_OFFER;
  return 0;
}

int
ACE_URL_Local_Locator::describe_offer (const ACE_WString &offer_id,
				       ACE_URL_Offer *offer)
{
  ACE_URL_Record *item = 0;
  
  // Iterate thru the repository and produce a copy of offer's
  // description.
  for (ACE_Unbounded_Set_Iterator<ACE_URL_Record> iter (this->repository_);
       iter.next (item) != 0;
       iter.advance ())
    if (offer_id == *item->id_)
      {
	ACE_NEW_RETURN (offer, ACE_URL_Offer (*item->offer_), -1);
	return 0;
      }

  errno = ACE_URL_Locator::NO_SUCH_OFFER;
  return -1;
}

int
ACE_URL_Local_Locator::modify_offer (const ACE_WString &offer_id,
				     const char *url,
				     const ACE_URL_Property_Seq *del,
				     const ACE_URL_Property_Seq *modify)
{
  errno = ACE_URL_Locator::UNIMPLEMENTED;
  return -1;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Unbounded_Set<ACE_URL_Record>;
template class ACE_Unbounded_Set_Iterator<ACE_URL_Record>;
template class ACE_Node<ACE_URL_Record>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Unbounded_Set<ACE_URL_Record>
#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_URL_Record>
#pragma instantiate ACE_Node<ACE_URL_Record>
#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
#endif /* ACE_LOCAL_LOCATOR_C */
