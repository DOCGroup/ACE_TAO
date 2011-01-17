// $Id$

#include "orbsvcs/PortableGroup/Portable_Group_Map.h"
#include "tao/ORB_Core.h"
#include "tao/TAO_Server_Request.h"
#include "tao/CDR.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
TAO_Portable_Group_Map::add_groupid_objectkey_pair (
    PortableGroup::TagGroupTaggedComponent *group_id,
    const TAO::ObjectKey &key
  )
{
  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->lock_);

  Map_Entry *new_entry;

  // We take ownership of the group_id memory.  Be sure we don't
  // forget about it.
  PortableGroup::TagGroupTaggedComponent_var safe_group = group_id;

  ACE_NEW_THROW_EX (new_entry,
                    Map_Entry (),
                    CORBA::NO_MEMORY (
                                      CORBA::SystemException::_tao_minor_code (
                                                                               TAO::VMCID,
                                                                               ENOMEM),
                                      CORBA::COMPLETED_NO));

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
          throw CORBA::INTERNAL ();
        }

      // Transfer ownership of group_id to the map.
      (void) safe_group._retn ();
    }
}

void
TAO_Portable_Group_Map::remove_groupid_objectkey_pair (const PortableGroup::TagGroupTaggedComponent* /*group_id*/,
                                                       const TAO::ObjectKey &/*key*/)
{

}


void
TAO_Portable_Group_Map::dispatch (PortableGroup::TagGroupTaggedComponent* group_id,
                                  TAO_ORB_Core *orb_core,
                                  TAO_ServerRequest &request,
                                  CORBA::Object_out forward_to)
{
  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->lock_);

  // Look up the GroupId.
  Map_Entry *entry = 0;
  if (this->map_.find (group_id,
                       entry) == 0)
    {

      // Save the read pointer in the message block since
      // every time we dispatch the request, we need to
      // reset it so that the request demarshals correctly.
      TAO_InputCDR *tao_in = request.incoming ();
      ACE_Message_Block *msgblk =
          const_cast<ACE_Message_Block *> (tao_in->start ());
      char *read_ptr = msgblk->rd_ptr ();

      // Iterate through the list of ObjectKeys.
      while (entry)
        {
          orb_core->adapter_registry ().dispatch (entry->key,
                                                  request,
                                                  forward_to);

          // Reset the read pointer in the message block.
          msgblk->rd_ptr (read_ptr);
          entry = entry->next;
        }
    }
}

u_long
TAO_GroupId_Hash::operator () (const PortableGroup::TagGroupTaggedComponent *id) const
{
  u_long hash =
    ACE::hash_pjw ((const char *) id->group_domain_id,
                   ACE_OS::strlen ((const char *) id->group_domain_id));

  // Truncate the object_group_id in half for the has.
  // Divide by one so that the ACE_U_LongLong representation
  // will automatically cast down to a u_long
  hash += (u_long) (id->object_group_id / 1);

  hash += id->object_group_ref_version;

  return hash;
}

int
TAO_GroupId_Equal_To::operator () (
  const PortableGroup::TagGroupTaggedComponent *lhs,
  const PortableGroup::TagGroupTaggedComponent *rhs) const
{
  return
    ACE_OS::strcmp (lhs->group_domain_id, rhs->group_domain_id) == 0
    && lhs->object_group_id == rhs->object_group_id
    && lhs->object_group_ref_version == rhs->object_group_ref_version;
}

TAO_END_VERSIONED_NAMESPACE_DECL
