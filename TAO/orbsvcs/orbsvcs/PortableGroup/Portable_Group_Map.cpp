// This may look like C, but it's really -*- C++ -*-
//
// $Id$

#include "Portable_Group_Map.h"
#include "tao/ORB_Core.h"
#include "tao/TAO_Server_Request.h"

//#if !defined (__ACE_INLINE__)
//# include "Portable_Group_Map.i"
//#endif /* __ACE_INLINE__ */


/// Constructor.
TAO_Portable_Group_Map::TAO_Portable_Group_Map ()
{
}

/// Destructor.
TAO_Portable_Group_Map::~TAO_Portable_Group_Map (void)
{
  for (Iterator i = this->map_.begin ();
       i != this->map_.end ();
       ++i)
    {
      // Deallocate the id.
      delete (*i).ext_id_;

      // Delete the chain of Map_Entries.
      Map_Entry *entry = (*i).int_id_;
      while (entry)
        {
          Map_Entry *next = entry->next;
          delete entry;
          entry = next;
        }

    }

  this->map_.close ();
}


void
TAO_Portable_Group_Map::add_groupid_objectkey_pair (PortableGroup::TagGroupTaggedComponent *group_id,
                                                    const TAO_ObjectKey &key
                                                    ACE_ENV_ARG_DECL)
{
  Map_Entry *new_entry;

  // We take ownership of the group_id memory.  Be sure we don't
  // forget about it.
  PortableGroup::TagGroupTaggedComponent_var safe_group = group_id;

  ACE_NEW_THROW_EX (new_entry,
                    Map_Entry (),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  // Fill out the entry.
  new_entry->key = key;

  // First, check if the GroupId is already in the map.
  Map_Entry *entry;
  if (this->map_.find (group_id,
                       entry) == 0)
    {
      // Add the object key to the list of object keys serviced by this GroupId.
      new_entry->next = entry->next;
      entry->next = new_entry;
    }
  else
    {
      new_entry->next = 0;

      // Add the
      int result =
        this->map_.bind (group_id,
                         new_entry);

      if (result != 0)
        {
          delete new_entry;
          ACE_THROW (CORBA::INTERNAL ());
        }

      // Transfer ownership of group_id to the map.
      (void) safe_group._retn ();
    }
}

void
TAO_Portable_Group_Map::remove_groupid_objectkey_pair (const PortableGroup::TagGroupTaggedComponent* /*group_id*/,
                                                       const TAO_ObjectKey &/*key*/
                                                       ACE_ENV_ARG_DECL_NOT_USED)
{

}


void
TAO_Portable_Group_Map::dispatch (PortableGroup::TagGroupTaggedComponent* group_id,
                                  TAO_ORB_Core *orb_core,
                                  TAO_ServerRequest &request,
                                  CORBA::Object_out forward_to
                                  ACE_ENV_ARG_DECL)
{
  // Look up the GroupId.
  Map_Entry *entry;
  if (this->map_.find (group_id,
                       entry) == 0)
    {

      // Save the read pointer in the message block since
      // every time we dispatch the request, we need to
      // reset it so that the request demarshals correctly.
      TAO_InputCDR &tao_in = request.incoming ();
      ACE_Message_Block *msgblk =
          ACE_const_cast (ACE_Message_Block *, tao_in.start ());
      char *read_ptr = msgblk->rd_ptr ();

      // Iterate through the list of ObjectKeys.
      while (entry)
        {
          orb_core->adapter_registry ()->dispatch (entry->key,
                                                   request,
                                                   forward_to
                                                   ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          // Reset the read pointer in the message block.
          msgblk->rd_ptr (read_ptr);
          entry = entry->next;
        }
    }
}

u_long
TAO_GroupId_Hash::operator () (const PortableGroup::TagGroupTaggedComponent *id) const
{
  u_long hash = ACE::hash_pjw ((const char *) id->group_domain_id,
                               ACE_OS::strlen ((const char *) id->group_domain_id));

  // Truncate the object_group_id in half for the has.
  hash += (u_long) id->object_group_id;

  hash += id->object_group_ref_version;

  return hash;
}

int
TAO_GroupId_Equal_To::operator () (const PortableGroup::TagGroupTaggedComponent *lhs,
                                   const PortableGroup::TagGroupTaggedComponent *rhs) const
{
  return (ACE_OS::strcmp (lhs->group_domain_id, rhs->group_domain_id) == 0) &&
         (lhs->object_group_id == rhs->object_group_id) &&
         (lhs->object_group_ref_version == rhs->object_group_ref_version);
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<PortableGroup::TagGroupTaggedComponent *, TAO_Portable_Group_Map::Map_Entry *>;
//template class ACE_Hash_Map_Manager<PortableGroup::TagGroupTaggedComponent *, TAO_Portable_Group_Map::Map_Entry *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<PortableGroup::TagGroupTaggedComponent *, TAO_Portable_Group_Map::Map_Entry *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<PortableGroup::TagGroupTaggedComponent *, TAO_Portable_Group_Map::Map_Entry *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<PortableGroup::TagGroupTaggedComponent *, TAO_Portable_Group_Map::Map_Entry *, TAO_GroupId_Hash, TAO_GroupId_Equal_To, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<PortableGroup::TagGroupTaggedComponent *, TAO_Portable_Group_Map::Map_Entry *, TAO_GroupId_Hash, TAO_GroupId_Equal_To, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<PortableGroup::TagGroupTaggedComponent *, TAO_Portable_Group_Map::Map_Entry *, TAO_GroupId_Hash, TAO_GroupId_Equal_To, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<PortableGroup::TagGroupTaggedComponent *, TAO_Portable_Group_Map::Map_Entry *, TAO_GroupId_Hash, TAO_GroupId_Equal_To, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<PortableGroup::TagGroupTaggedComponent *, TAO_Portable_Group_Map::Map_Entry *>
#pragma instantiate ACE_Hash_Map_Manager<PortableGroup::TagGroupTaggedComponent *, TAO_Portable_Group_Map::Map_Entry *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<PortableGroup::TagGroupTaggedComponent *, TAO_Portable_Group_Map::Map_Entry *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<PortableGroup::TagGroupTaggedComponent *, TAO_Portable_Group_Map::Map_Entry *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<PortableGroup::TagGroupTaggedComponent *, TAO_Portable_Group_Map::Map_Entry *, TAO_GroupId_Hash, TAO_GroupId_Equal_To, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<PortableGroup::TagGroupTaggedComponent *, TAO_Portable_Group_Map::Map_Entry *, TAO_GroupId_Hash, TAO_GroupId_Equal_To, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<PortableGroup::TagGroupTaggedComponent *, TAO_Portable_Group_Map::Map_Entry *, TAO_GroupId_Hash, TAO_GroupId_Equal_To, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<PortableGroup::TagGroupTaggedComponent *, TAO_Portable_Group_Map::Map_Entry *, TAO_GroupId_Hash, TAO_GroupId_Equal_To, ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
