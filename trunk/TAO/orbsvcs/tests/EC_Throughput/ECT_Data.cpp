// $Id$

#include "ECT_Data.h"

TAO_OutputCDR&
operator<< (TAO_OutputCDR& cdr, ECT_Data& x)
{
  // We are a little careless about error checking in this routine,
  // because one the CDR gets the error bit on it is never disabled.
  CORBA::ULong count = x.inventory.current_size ();
  cdr << x.description.in ()
      << count;
  for (ECT_Data::Inventory::ITERATOR i = x.inventory.begin ();
       i != x.inventory.end ();
       ++i)
    {
      const ECT_Data::Inventory::ENTRY& v = *i;
      cdr << v.ext_id_
	  << v.int_id_;
    }
  return cdr;
}

TAO_InputCDR&
operator>> (TAO_InputCDR& cdr, ECT_Data& x)
{
  // We are a little careless about error checking in this routine,
  // because one the CDR gets the error bit on it is never disabled.
  CORBA::ULong count;
  cdr >> x.description.out ()
      >> count;
  for (CORBA::ULong i = 0; i < count && cdr.good_bit (); ++i)
    {
      CORBA::ULong ext_id;
      CORBA::Double int_id;
      cdr >> ext_id
	  >> int_id;
      if (cdr.good_bit ())
	x.inventory.bind (ext_id, int_id);
    }
  return cdr;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Manager<CORBA::ULong,CORBA::Double,ACE_NULL_MUTEX>;
template class ACE_Hash_Map_Entry<CORBA::ULong,CORBA::Double,ACE_NULL_MUTEX>;
template class ACE_Hash_Map_Iterator_Base<CORBA::ULong,CORBA::Double,ACE_NULL_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<CORBA::ULong,CORBA::Double,ACE_NULL_MUTEX>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Manager<CORBA::ULong,CORBA::Double,ACE_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Entry<CORBA::ULong,CORBA::Double,ACE_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base<CORBA::ULong,CORBA::Double,ACE_NULL_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<CORBA::ULong,CORBA::Double,ACE_NULL_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
