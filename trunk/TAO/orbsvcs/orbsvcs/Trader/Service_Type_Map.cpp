// $Id$
#ifndef TAO_SERVICE_TYPE_MAP_C
#define TAO_SERVICE_TYPE_MAP_C

#include "Service_Type_Map.h"
#include "Offer_Id_Iterator.h"

template <class LOCK_TYPE>
TAO_Service_Type_Map<LOCK_TYPE>::TAO_Service_Type_Map (void)
{
}

template <class LOCK_TYPE>
TAO_Service_Type_Map<LOCK_TYPE>::Local_Offer_Iterator*
TAO_Service_Type_Map<LOCK_TYPE>::get_offers (const char* type)
{
  // Construct a Local_Offer_Iterator
  ACE_READ_GUARD_RETURN (LOCK_TYPE, ace_mon, this->type_map_.lock (), 0);
  
  Local_Offer_Iterator* iterator = 0;
  string service_type (type);
  SERVICE_TYPE_MAP::iterator type_iter =
    this->type_map_.find (service_type);
  
  if (type_iter != this->type_map_.end ())
    {
      iterator = new Local_Offer_Iterator (type,
					   this->type_map_, 
					   (*type_iter).second.first);
    }
  
  return iterator;
}

template <class LOCK_TYPE> int
TAO_Service_Type_Map<LOCK_TYPE>::add_type (const char* type)
{
  ACE_WRITE_GUARD_RETURN (LOCK_TYPE, ace_mon, this->type_map_.lock (), -1);

  int return_value = -1; 
  string service_type (type);
  SERVICE_TYPE_MAP::iterator type_iter =
    this->type_map_.find (service_type);

  if (type_iter == this->type_map_.end ())
    {
      OFFER_MAP offer_map;
      SERVICE_TYPE_MAP::value_type
	new_entry (service_type, make_pair (offer_map, 1));	  

      if (! (this->type_map_.insert (new_entry)).second)
	return_value = 0;
    }

  return return_value;
}

template <class LOCK_TYPE> int
TAO_Service_Type_Map<LOCK_TYPE>::remove_type (const char* type)
{
  ACE_WRITE_GUARD_RETURN (LOCK_TYPE, ace_mon, this->type_map_.lock (), -1);

  int return_value = -1; 
  string service_type (type);
  SERVICE_TYPE_MAP::iterator type_iter =
    this->type_map_.find (service_type);

  if (type_iter != this->type_map_.end ())
    {
      this->type_map_.erase (type_iter);
      return_value = 0;
    }

  return return_value;
}

template <class LOCK_TYPE> char*
TAO_Service_Type_Map<LOCK_TYPE>::
insert_offer (const char* type,
	      const CosTrading::Offer& offer)
{
  ACE_READ_GUARD_RETURN (LOCK_TYPE, ace_mon, this->type_map_.lock (), 0);
  
  char* return_value = 0;
  string service_type (type);
  SERVICE_TYPE_MAP::iterator type_iter =
    this->type_map_.find (service_type);
  
  if (type_iter != this->type_map_.end ())
    {
      // If type is found, insert the offer
      OFFER_MAP_PLUS_COUNTER& mc = (*type_iter).second;
      OFFER_MAP &offer_map = mc.first;
      HUGE_NUMBER& starting_number = mc.second;
      
      ACE_WRITE_GUARD_RETURN (LOCK_TYPE, ace_mon, offer_map.lock (), 0);

      return_value = this->generate_offer_id (type, starting_number);
      offer_map[starting_number++] = offer;
    }

  return return_value;
}

template <class LOCK_TYPE> int
TAO_Service_Type_Map<LOCK_TYPE>::
remove_offer (const char* type, HUGE_NUMBER id)
{
  ACE_READ_GUARD_RETURN (LOCK_TYPE, ace_mon, this->type_map_.lock (), -1);
  
  int return_value = -1;
  SERVICE_TYPE_MAP::iterator type_iter;

  type_iter = this->type_map_.find (string (type));
  
  if (type_iter != this->type_map_.end ())
    {
      OFFER_MAP::iterator offer_iter;
      OFFER_MAP_PLUS_COUNTER& mc = (*type_iter).second;
      OFFER_MAP &offer_map = mc.first;
      
      ACE_WRITE_GUARD_RETURN (LOCK_TYPE, ace_mon, offer_map.lock (), -1);

      offer_iter = offer_map.find (id);
      if (offer_iter != offer_map.end ())
	{
	  offer_map.erase (offer_iter);      
	  return_value = 0;
	}
    }

  return return_value;
}
  
template <class LOCK_TYPE> int
TAO_Service_Type_Map<LOCK_TYPE>::
remove_offer (const CosTrading::OfferId offer_id,
	      CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::IllegalOfferId,
		  CosTrading::UnknownOfferId))
{
  char* stype = 0;
  HUGE_NUMBER index = -1;

  this->parse_offer_id (offer_id, stype, index, _env);
  TAO_CHECK_ENV_RETURN (_env, -1);

  if (this->remove_offer (stype, index) == -1)
    TAO_THROW_RETURN (CosTrading::UnknownOfferId (offer_id), -1);

  return 0;
}

template <class LOCK_TYPE> CosTrading::Offer*
TAO_Service_Type_Map<LOCK_TYPE>::
lookup_offer (const CosTrading::OfferId offer_id,
	      char*& type_name,
	      CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::IllegalOfferId,
		  CosTrading::UnknownOfferId))
{
  HUGE_NUMBER index;
  CosTrading::Offer* offer = 0;
  this->parse_offer_id (offer_id, type_name, index, _env);
  TAO_CHECK_ENV_RETURN (_env, offer);

  if ((offer = this->lookup_offer (type_name, index)) == 0)
    TAO_THROW_RETURN (CosTrading::UnknownOfferId (offer_id), offer);

  return offer;
}


template <class LOCK_TYPE> CosTrading::Offer*
TAO_Service_Type_Map<LOCK_TYPE>::
lookup_offer (const CosTrading::OfferId offer_id,
	      CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::IllegalOfferId,
		  CosTrading::UnknownOfferId))
{
  char* type_name;
  HUGE_NUMBER index;
  CosTrading::Offer* offer = 0;

  this->parse_offer_id (offer_id, type_name, index, _env);
  TAO_CHECK_ENV_RETURN (_env, offer);

  if ((offer = this->lookup_offer (type_name, index)) == 0)
    TAO_THROW_RETURN (CosTrading::UnknownOfferId (offer_id), offer);

  return offer;
}
  
template <class LOCK_TYPE> CosTrading::Offer*
TAO_Service_Type_Map<LOCK_TYPE>::
lookup_offer (const char* type, HUGE_NUMBER id)
{
  ACE_READ_GUARD_RETURN (LOCK_TYPE, ace_mon, this->type_map_.lock (), 0);
  
  CosTrading::Offer* return_value = 0;
  SERVICE_TYPE_MAP::iterator type_iter =
    this->type_map_.find (string (type));
  
  if (type_iter != this->type_map_.end ())
    {
      OFFER_MAP::iterator offer_iter;
      OFFER_MAP_PLUS_COUNTER& mc = (*type_iter).second;
      OFFER_MAP &offer_map = mc.first;

      ACE_READ_GUARD_RETURN (LOCK_TYPE, ace_mon, offer_map.lock (), 0);
      
      offer_iter = offer_map.find (id);
      if (offer_iter != offer_map.end ())
	return_value = &((*offer_iter).second);
    }
  
  return return_value;
}

template <class LOCK_TYPE>
TAO_Service_Type_Map<LOCK_TYPE>::Random_Offer_Locator*
TAO_Service_Type_Map<LOCK_TYPE>::lookup_offers (const char* type)
{
  // Construct a Random_Offer_Locator.
  ACE_READ_GUARD_RETURN (LOCK_TYPE, ace_mon, this->type_map_.lock (), 0);
  
  Random_Offer_Locator* iterator = 0;
  string service_type (type);
  SERVICE_TYPE_MAP::iterator type_iter =
    this->type_map_.find (service_type);
  
  if (type_iter != this->type_map_.end ())
    {
      iterator = new Random_Offer_Locator (this->type_map_, 
					   (*type_iter).second.first);
    }

  return iterator;
}

template <class LOCK_TYPE> TAO_Offer_Id_Iterator*
TAO_Service_Type_Map<LOCK_TYPE>::retrieve_all_offer_ids (void)
{
  // Fill an TAO_Offer_Id_Iterator with the offer_ids of all offers
  // exported to the TAO_Service_Type_Map. Iterates through the entire 
  // map, cramming offer_id strings into a newly constructed
  // TAO_Offer_Id_Iterator. 
  TAO_Offer_Id_Iterator* id_iterator = new TAO_Offer_Id_Iterator ();
  ACE_READ_GUARD_RETURN (LOCK_TYPE, ace_mon, this->type_map_.lock (), 0);
  
  for (SERVICE_TYPE_MAP::iterator type_iter = this->type_map_.begin ();
       type_iter != this->type_map_.end ();
       type_iter++)
    {
      char* service_type = (char *) (*type_iter).first.data ();
      OFFER_MAP& offer_map = (*type_iter).second.first;

      ACE_READ_GUARD_RETURN (LOCK_TYPE, ace_mon, offer_map.lock (), 0);
      
      for (OFFER_MAP::iterator offer_iter = offer_map.begin();
	   offer_iter != offer_map.end();
	   offer_iter++)
	{
	  HUGE_NUMBER offer_index = (*offer_iter).first;
	  CosTrading::OfferId offer_id = 
	    this->generate_offer_id (service_type, offer_index); 

	  id_iterator->insert_id (offer_id);
	}
    }      
  
  return id_iterator;
}

template <class LOCK_TYPE> TAO_Service_Type_Map<LOCK_TYPE>::TYPE_NAME_SEQ*
TAO_Service_Type_Map<LOCK_TYPE>::list_all_types (void)
{
  ACE_READ_GUARD_RETURN (LOCK_TYPE, ace_mon, this->type_map_.lock (), 0);
  
  CORBA::ULong i = 0;
  TYPE_NAME* type_names = TYPE_NAME_SEQ::allocbuf (this->type_map_.size ());
  
  for (SERVICE_TYPE_MAP::iterator type_iter = this->type_map_.begin ();
       type_iter != this->type_map_.end ();
       type_iter++)
    type_names[i++] = CORBA::string_dup ((*type_iter).first.data ());

  return new TYPE_NAME_SEQ (i, i, type_names, 1);
}

template <class LOCK_TYPE> void
TAO_Service_Type_Map<LOCK_TYPE>::parse_offer_id (const char *offer_id,
						 char*&service_type,
						 HUGE_NUMBER& id,
						 CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::IllegalOfferId))
{
  // Get service type: it is everything from 17th character to the end.
  service_type = (char *) offer_id + 16;
  
  // Get id: temporarily put the end of string character where the service 
  // type starts, convert to number, replace the character back.
  char &start_of_type = (char) *(offer_id + 16);
  char temp = start_of_type;
  start_of_type = '\0';
  id = ACE_OS::atoi (offer_id);

  start_of_type = temp;

  if (! TAO_Trader_Base::is_valid_identifier_name (service_type))
    TAO_THROW (CosTrading::IllegalOfferId (offer_id));
}

template <class LOCK_TYPE> char*
TAO_Service_Type_Map<LOCK_TYPE>::
generate_offer_id (const char *service_type_name,
		   HUGE_NUMBER id)
{
    // holds final id string.
  string offer_id;

  // hold portions of id string.
  char id_string[16];
  char padding [16];

  ACE_OS::sprintf(id_string, "%d", id);

  size_t padding_size = 16 - ACE_OS::strlen (id_string);
   // Fill padding with zeros.
  for (int i = 0; i < padding_size; i++)
    padding[i] = '0';
  padding[padding_size] = '\0';
  
  offer_id += padding;
  offer_id += id_string;
  offer_id += service_type_name;

  return CORBA::string_dup (offer_id.data ());
}


template <class LOCK_TYPE>
TAO_Service_Type_Map<LOCK_TYPE>::Local_Offer_Iterator::
Local_Offer_Iterator (const char* type,
		      TAO_Service_Type_Map<LOCK_TYPE>::SERVICE_TYPE_MAP& stm,
		      TAO_Service_Type_Map<LOCK_TYPE>::OFFER_MAP& offer_map)
  : offer_map_ (offer_map),
    stm_ (stm),    
    offer_iter_ (offer_map.begin ()),
    type_ (type)
{
  if (this->stm_.lock ().acquire_read () == -1)
    ;
  
  if (this->offer_map_.lock ().acquire_read () == -1)
    ;  
}

template <class LOCK_TYPE>
TAO_Service_Type_Map<LOCK_TYPE>::Local_Offer_Iterator::
~Local_Offer_Iterator (void)
{  
  if (this->offer_map_.lock ().release () == -1)
    ;

  if (this->stm_.lock ().release () == -1)
    ;
}

template <class LOCK_TYPE> CosTrading::OfferId
TAO_Service_Type_Map<LOCK_TYPE>::Local_Offer_Iterator::get_id (void)
{
  return TAO_Service_Type_Map<LOCK_TYPE>::
    generate_offer_id (this->type_, (*this->offer_iter_).first);
}

template <class LOCK_TYPE> int
TAO_Service_Type_Map<LOCK_TYPE>::Local_Offer_Iterator::
has_more_offers (void)
{
  return this->offer_iter_ != this->offer_map_.end ();
}

  
template <class LOCK_TYPE> CosTrading::Offer*
TAO_Service_Type_Map<LOCK_TYPE>::Local_Offer_Iterator::
get_offer (void)
{
  return &(*this->offer_iter_).second;
}

template <class LOCK_TYPE> void
TAO_Service_Type_Map<LOCK_TYPE>::Local_Offer_Iterator::
next_offer (void)
{
  this->offer_iter_++;
}


template <class LOCK_TYPE>
TAO_Service_Type_Map<LOCK_TYPE>::Random_Offer_Locator::
Random_Offer_Locator (TAO_Service_Type_Map<LOCK_TYPE>::SERVICE_TYPE_MAP& stm,
		      TAO_Service_Type_Map<LOCK_TYPE>::OFFER_MAP& offer_map)
  : offer_map_ (offer_map),
    offer_iter_ (offer_map_.begin ()),
    stm_ (stm)
{
  if (this->stm_.lock ().acquire_read () == -1)
    ;
  
  if (this->offer_map_.lock ().acquire_read () == -1)
    ;  
}

template <class LOCK_TYPE>
TAO_Service_Type_Map<LOCK_TYPE>::Random_Offer_Locator::
~Random_Offer_Locator (void)
{  
  if (this->offer_map_.lock ().release () == -1)
    ;

  if (this->stm_.lock ().release () == -1)
    ;
}

template <class LOCK_TYPE> CosTrading::Offer*
TAO_Service_Type_Map<LOCK_TYPE>::Random_Offer_Locator::
get_offer (TAO_Service_Type_Map<LOCK_TYPE>::HUGE_NUMBER id)
{
  CosTrading::Offer* return_value = 0;
  
  this->offer_iter_ = this->offer_map_.find (id);
  if (this->offer_iter_ != this->offer_map_.end ())
    return_value = &(*this->offer_iter_).second;

  return return_value;
}

#endif /* TAO_SERVICE_TYPE_MAP_C */

