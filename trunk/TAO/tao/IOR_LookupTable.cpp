// $Id$

#include "IOR_LookupTable.h"

ACE_RCSID(tao, IOR_TableLookup, "$Id$")

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
TAO_IOR_LookupTable::add_ior (ACE_CString object_name,
                              ACE_CString ior)
{
  // Make an entry in the table.
  switch (this->hash_map_.bind (object_name, ior))
    {
    case 1 : // object name already exists in the table.
      ACE_DEBUG ((LM_DEBUG,
                  "Object Name already exists in the table\n"));
      return 1;
    case -1 : // Failure.
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Unable to bind in IOR_LookupTable::add_ior ()"),
                        -1);
    }
  
  ACE_DEBUG ((LM_DEBUG,
              "\n%s:%s Added to the table\n",
	      object_name.c_str (),
	      ior.c_str ()));
 
  return 0;
}

int
TAO_IOR_LookupTable::find_ior (ACE_CString object_name,
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
template class ACE_Hash_Map_Reverse_Iterator<ACE_CString, ACE_CString, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Manager<ACE_CString,ACE_CString,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString,ACE_CString,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, ACE_CString>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_CString, ACE_CString, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
