// $Id$



#include "tao/IOR_LookupTable.h"
#include "tao/ObjectIDList.h"
#include "tao/Exception.h"
#include "tao/Environment.h"
#include "tao/debug.h"


ACE_RCSID(tao, IOR_TableLookup, "$Id$")


int 
TAO_IOR_LookupTable_Callback::find_ior (const ACE_CString &,
                                        ACE_CString &)
{
  // Nothing
  return -1;
}

// = Initialization and termination methods.
TAO_IOR_LookupTable::TAO_IOR_LookupTable (void)
  : table_ (),
    delete_callback_ (1)
{
  ACE_NEW (this->callback_, TAO_IOR_LookupTable_Callback);
}

TAO_IOR_LookupTable::~TAO_IOR_LookupTable (void)
{
  if (this->delete_callback_)
    delete this->callback_;
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
  switch (this->table_.bind (object_name, ior))
    {
    case 1 : // object name already exists in the table.
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) Object Name already exists in the IOR table\n"));
      return 1;
    case -1 : // Failure.
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) Unable to bind in IOR tao\n"));
      return -1;
    }

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "TAO (%P|%t) IOR Table: <%s> -> <%s>\n",
                object_name.c_str (),
                ior.c_str ()));

  return 0;
}

int
TAO_IOR_LookupTable::find_ior (const ACE_CString &object_name,
                               ACE_CString &ior)
{
  // Find the IOR corresponding to the object name.
  // returns 0 on success.
  //        -1 on failure.

  // @@ This debugging output should *NOT* be used since the
  //    object key string is not null terminated, nor can it
  //    be null terminated without copying.  No copying should
  //    be done since performance is somewhat important here.
  //    So, just remove the debugging output entirely.
  //
  //   if (TAO_debug_level > 0)
  //     ACE_DEBUG ((LM_DEBUG,
  //                 "TAO (%P|%t) IOR Table find <%s>\n",
  //                 object_name.c_str ()));

  int result = this->table_.find (object_name, ior);

  if (result != 0)
    result = this->callback_->find_ior (object_name, ior);

  return result;
}

void 
TAO_IOR_LookupTable::register_callback (TAO_IOR_LookupTable_Callback *callback, 
                                        int delete_callback)
{
  if (this->delete_callback_)
    delete this->callback_;

  this->callback_ = callback;
  this->delete_callback_ = delete_callback;
}

CORBA_ORB_ObjectIdList_ptr
TAO_IOR_LookupTable::list_initial_services (CORBA::Environment &ACE_TRY_ENV)
{
  // Unsupported initial services should NOT be included in the below list!
  const char *initial_services[] = { TAO_LIST_OF_INITIAL_SERVICES };
  // Make sure the "terminating" zero is the last array element so
  // that there is a stop condition when iterating through the list.

  const size_t initial_services_size =
    sizeof (initial_services) / sizeof (initial_services[0]);

  const size_t total_size =
    initial_services_size + this->table_.current_size ();

  CORBA_ORB_ObjectIdList_ptr tmp = 0;

  ACE_NEW_THROW_EX (tmp,
                    CORBA_ORB_ObjectIdList (total_size),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  CORBA_ORB_ObjectIdList_var list = tmp;

  size_t index = 0;
  // Index for ObjectIdList members.

  // Iterate over TAO's "built-in" initial references.
  for (index = 0; index < initial_services_size; ++index)
    list[index] = initial_services[index];

  // Now iterate over the initial references created by the user and
  // add them to the sequence.
  TAO_IOR_Map::iterator first = this->table_.begin ();
  TAO_IOR_Map::iterator last  = this->table_.end ();

  for (TAO_IOR_Map::iterator i = first;
       i != last;
       ++i, ++index)
      list[index] = (*i).int_id_.c_str ();

  list->length (total_size);

  return list._retn ();
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
