// $Id$

#include "IOR_LookupTable.h"

ACE_RCSID(orbsvcs, IOR_TableLookup, "$Id$")

  // = Initialization and termination methods.
TAO_IOR_LookupTable::TAO_IOR_LookupTable (void)
  : object_name_ (),
    ior_ ()
{
}

TAO_IOR_LookupTable::~TAO_IOR_LookupTable (void)
{
}

int
TAO_IOR_LookupTable::init (void)
{
  // This creates and initialises the table.
  // Currently works on the hash map manager.

return 0;
}

int
TAO_IOR_LookupTable::add_ior (const ACE_CString &object_name,
                              const ACE_CString &ior)
{

  // Make an entry in the table.
  switch (this->hash_map_.bind (object_name, ior))
    {
    case 1 : // object name already exists in the table.
      return 1;
    case -1 : // Failure.
      return -1;
    }

  return 0;
}

int
TAO_IOR_LookupTable::find_ior (const ACE_CString &object_name,
                               ACE_CString &ior)
{
  // Find the IOR corresponding to the object name.
  // returns 0 on success.
  //        -1 on failure.

  return this->hash_map_.find (object_name, ior);
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Manager<ACE_CString, ACE_CString, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_CString,ACE_CString,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<ACE_CString, ACE_CString>;
template class ACE_Hash<ACE_CString>;
template class ACE_Equal_To<ACE_CString>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_CString, ACE_CString, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString,ACE_CString,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString,ACE_CString,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, ACE_CString>
#pragma instantiate ACE_Hash<ACE_CString>
#pragma instantiate ACE_Equal_To<ACE_CString>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_CString, ACE_CString, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
