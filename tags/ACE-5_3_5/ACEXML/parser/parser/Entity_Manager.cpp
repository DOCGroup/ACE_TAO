// $Id$

#include "ACEXML/parser/parser/Entity_Manager.h"

static const ACEXML_Char empty_string[] = { 0 };

#if !defined (__ACEXML_INLINE__)
# include "ACEXML/parser/parser/Entity_Manager.i"
#endif /* __ACEXML_INLINE__ */


ACEXML_Entity_Manager::ACEXML_Entity_Manager (void)
  : entities_ ()
{

}

ACEXML_Entity_Manager::~ACEXML_Entity_Manager (void)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

  // Instantiations for the Hash Map
template class ACE_Equal_To<ACEXML_String>;
template class ACE_Hash<ACEXML_String>;
template class ACE_Hash_Map_Entry<ACEXML_String, ACEXML_String>;
template class ACE_Hash_Map_Manager_Ex<ACEXML_String, ACEXML_String, ACE_Hash<ACEXML_String>, ACE_Equal_To<ACEXML_String>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACEXML_String, ACEXML_String, ACE_Hash<ACEXML_String>, ACE_Equal_To<ACEXML_String>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACEXML_String, ACEXML_String, ACE_Hash<ACEXML_String>, ACE_Equal_To<ACEXML_String>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACEXML_String, ACEXML_String, ACE_Hash<ACEXML_String>, ACE_Equal_To<ACEXML_String>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Bucket_Iterator<ACEXML_String, ACEXML_String, ACE_Hash<ACEXML_String>, ACE_Equal_To<ACEXML_String>, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Equal_To<ACEXML_String>
#pragma instantiate ACE_Hash<ACEXML_String>
#pragma instantiate ACE_Hash_Map_Entry<ACEXML_String, ACEXML_String>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACEXML_String, ACEXML_String, ACE_Hash<ACEXML_String>, ACE_Equal_To<ACEXML_String>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACEXML_String, ACEXML_String, ACE_Hash<ACEXML_String>, ACE_Equal_To<ACEXML_String>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACEXML_String, ACEXML_String, ACE_Hash<ACEXML_String>, ACE_Equal_To<ACEXML_String>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACEXML_String, ACEXML_String, ACE_Hash<ACEXML_String>, ACE_Equal_To<ACEXML_String>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Bucket_Iterator<ACEXML_String, ACEXML_String, ACE_Hash<ACEXML_String>, ACE_Equal_To<ACEXML_String>, ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
