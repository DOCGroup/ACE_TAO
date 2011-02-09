// $Id$
// ============================================================================
//
// = FILENAME
//   Load_Balancer_i.cpp
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================

#include "Load_Balancer_i.h"
#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_time.h"

Object_Group_Factory_i::Object_Group_Factory_i (void)
{
}

Object_Group_Factory_i::~Object_Group_Factory_i (void)
{
}

void
Object_Group_Factory_i::remove_group (const ACE_CString &id,
                                      int random)
{
  // Remove the entry from the appropriate map of groups.
  if (random)
    random_groups_.unbind (id);
  else
    rr_groups_.unbind (id);
}

Load_Balancer::Object_Group_ptr
Object_Group_Factory_i::make_round_robin (const char * id)
{
  return make_group (0,
                     id);
}

Load_Balancer::Object_Group_ptr
Object_Group_Factory_i::make_random (const char * id)
{
  return make_group (1,
                     id);
}

Load_Balancer::Object_Group_ptr
Object_Group_Factory_i::make_group (int random,
                                    const char * id)
{
  ACE_CString group_id (id);

  // Check to make sure we don't already have a group with the same
  // <id>.
  if (rr_groups_.find (group_id) == 0
      || random_groups_.find (group_id) == 0)
    throw Load_Balancer::duplicate_group ();
  else
    {
      // Store our result here for return.
      Load_Balancer::Object_Group_var group;

      // Create an appropriate servant.
      Object_Group_i * group_servant;
      if (random)
        ACE_NEW_THROW_EX (group_servant,
                          Random_Object_Group (id, this),
                          CORBA::NO_MEMORY ());
      else
        ACE_NEW_THROW_EX (group_servant,
                          RR_Object_Group (id, this),
                          CORBA::NO_MEMORY ());

      // Temporarily put the servant into the auto_ptr.
      ACE_Auto_Basic_Ptr<Object_Group_i> temp (group_servant);

      // Register with the poa, begin using ref. counting.
      group = group_servant->_this ();

      group_servant->_remove_ref ();
      temp.release ();

      // Make an entry in appropriate map of groups.
      if (random)
        {
          if (random_groups_.bind (group_id, group) == -1)
            throw CORBA::INTERNAL ();

          ACE_DEBUG ((LM_DEBUG,
                      "Load_Balancer: Created new Random Group"
                      " with id <%C>\n", id));
        }
      else
        {
          if (rr_groups_.bind (group_id, group) == -1)
            throw CORBA::INTERNAL ();
          ACE_DEBUG ((LM_DEBUG,
                      "Load_Balancer: Created new Round Robin Group"
                      " with id <%C>\n", id));
        }
      // Return.
      return group._retn ();
    }
}

Load_Balancer::Object_Group_ptr
Object_Group_Factory_i::resolve (const char * id)
{
  ACE_CString group_id (id);
  Load_Balancer::Object_Group_var group;

  if (rr_groups_.find (group_id, group) == -1
      && random_groups_.find (group_id, group) == -1)
    throw Load_Balancer::no_such_group ();
  else
    return group._retn ();
}

Load_Balancer::Group_List *
Object_Group_Factory_i::list_groups (int random)
{
  Load_Balancer::Group_List * list;

  // Figure out the length of the list.
  CORBA::ULong len;
  if (random)
    len = random_groups_.current_size ();
  else
    len = rr_groups_.current_size ();

  // Allocate the list of <len> length.
  ACE_NEW_THROW_EX (list,
                    Load_Balancer::Group_List (len),
                    CORBA::NO_MEMORY ());
  list->length (len);

  // Create an iterator for group structure to populate the list.
  Object_Group_Factory_i::HASH_MAP::ITERATOR *group_iter;
  Object_Group_Factory_i::HASH_MAP::ITERATOR random_iter (random_groups_);
  Object_Group_Factory_i::HASH_MAP::ITERATOR rr_iter (rr_groups_);
  if (random)
    group_iter = &random_iter;
  else
    group_iter = &rr_iter;

  // Iterate over groups and populate the list.
  Object_Group_Factory_i::HASH_MAP::ENTRY *hash_entry = 0;
  for (CORBA::ULong i = 0; i < len; i++)
    {
      group_iter->next (hash_entry);
      group_iter->advance ();

      (*list)[i] = hash_entry->ext_id_.c_str ();
    }

  return list;
}

Load_Balancer::Group_List *
Object_Group_Factory_i::round_robin_groups (void)
{
  return list_groups (0);
}

Load_Balancer::Group_List *
Object_Group_Factory_i::random_groups (void)
{
  return list_groups (1);
}

Object_Group_i::Object_Group_i (const char * id,
                                Object_Group_Factory_i *my_factory)
  : id_ (id),
    my_factory_ (my_factory)
{
}

Object_Group_i::~Object_Group_i (void)
{
  // Need to delete all the items from the member_id_list, to avoid
  // memory leaks.
  Object_Group_i::ITERATOR iter (member_id_list_);

  do
    {
      delete (iter.next ());
    } while (iter.advance ());
}

char *
Object_Group_i::id (void)
{
  return CORBA::string_dup (id_.c_str ());
}

void
Object_Group_i::bind (const Load_Balancer::Member & member)
{
  ACE_CString member_id (member.id);
  CORBA::Object_var obj = CORBA::Object::_duplicate (member.obj.in ());

  // Insert new member into <members_> and check for duplicates/failures.
  int result = members_.trybind (member_id, obj);
  if (result == 1)
    throw Load_Balancer::duplicate_member ();
  else if (result == -1)
    throw CORBA::INTERNAL ();

  // Insert new member's id into <member_id_list_>.
  ACE_CString *new_id;
  ACE_NEW_THROW_EX (new_id,
                    ACE_CString (member.id),
                    CORBA::NO_MEMORY ());
  if (member_id_list_.insert_tail (new_id) == 0)
    throw CORBA::NO_MEMORY ();

  // Theoretically, we should deal with memory failures more
  // thoroughly.  But, practically, the whole system is going to be
  // hosed anyways ...

  ACE_DEBUG ((LM_DEBUG,
              "Load_Balancer: Added member <%C> to <%C> Group\n",
              member_id.c_str (),
              id_.c_str ()));
}

void
Object_Group_i::unbind (const char * id)
{
  ACE_CString member_id (id);

  // Code below works if list and hash map states are consistent,
  // which is the case unless the system experienced major problems,
  // e.g., ran out of memory ...

  // Check to make sure we have it.
  if (members_.find (member_id) == -1)
    throw Load_Balancer::no_such_member ();

  // Remove all entries for this member.
  members_.unbind (member_id);

  Object_Group_i::ITERATOR iter (member_id_list_);
  while (member_id != *(iter.next ()))
    iter.advance ();
  delete (iter.next ());
  iter.remove ();

  ACE_DEBUG ((LM_DEBUG,
              "Load_Balancer: Removed member with id <%C>"
              "from <%C> object group\n", id, id_.c_str ()));
}

CORBA::Object_ptr
Object_Group_i::resolve_with_id (const char * id)
{
  CORBA::Object_var obj;
  ACE_CString member_id (id);

  if (members_.find (member_id, obj) == -1)
    throw Load_Balancer::no_such_member ();

  return obj._retn ();
}

Load_Balancer::Member_ID_List *
Object_Group_i::members (void)
{
  Load_Balancer::Member_ID_List * list;

  // Figure out the length of the list.
  CORBA::ULong len = members_.current_size ();

  // Allocate the list of <len> length.
  ACE_NEW_THROW_EX (list,
                    Load_Balancer::Member_ID_List (len),
                    CORBA::NO_MEMORY ());
  list->length (len);

  // Create an iterator for <member_id_list_> to populate the list.
  Object_Group_i::ITERATOR id_iter (member_id_list_);

  // Iterate over groups and populate the list.
  for (CORBA::ULong i = 0; i < len; i++)
    {
      (*list)[i] = id_iter.next ()->c_str ();
      id_iter.advance ();
    }

  return list;
}

void
Object_Group_i::destroy (void)
{
  // Deregister with POA.
  PortableServer::POA_var poa =
    this->_default_POA ();

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this);

  poa->deactivate_object (id.in ());

  ACE_DEBUG ((LM_DEBUG,
              "Load_Balancer: Destroyed object group"
              "with id <%C>\n", id_.c_str ()));
}

Random_Object_Group::Random_Object_Group (const char *id,
                                          Object_Group_Factory_i *my_factory)
  : Object_Group_i (id, my_factory)
{
  // Seed the random number generator.
  ACE_OS::srand (static_cast<u_int> (ACE_OS::time ()));
}

Random_Object_Group::~Random_Object_Group (void)
{
}

void
Random_Object_Group::destroy (void)
{
  //Deregisters this <Object_Group> with its
  // <Object_Group_Factory>.
  my_factory_->remove_group (id_, 1);

  Object_Group_i::destroy ();
}

CORBA::Object_ptr
Random_Object_Group::resolve (void)
{
  CORBA::Object_var obj;

  size_t group_size = members_.current_size ();
  if (group_size == 0)
    throw Load_Balancer::no_such_member ();

  // Generate random number in the range [0, group_size - 1]
  size_t member = ACE_OS::rand() % group_size;

  // Get the id of the member to return to the client.
  ACE_CString *id = 0;
  member_id_list_.get (id, member);

  ACE_DEBUG ((LM_DEBUG, "Load_Balancer: In <%C> Group resolved to <%C>\n",
              id_.c_str (),
              id->c_str()));

  // Return the object reference corresponding to the found id to the client.
  members_.find (*id, obj);
  return obj._retn ();
}

RR_Object_Group::RR_Object_Group (const char *id,
                                  Object_Group_Factory_i *my_factory)
  : Object_Group_i (id, my_factory),
    next_ (0)
{
}

RR_Object_Group::~RR_Object_Group (void)
{
}

void
RR_Object_Group::destroy (void)
{
  //Deregisters this <Object_Group> with its
  // <Object_Group_Factory>.
  my_factory_->remove_group (id_, 0);

  Object_Group_i::destroy ();
}

CORBA::Object_ptr
RR_Object_Group::resolve (void)
{
  CORBA::Object_var obj;

  size_t group_size = members_.current_size ();
  if (group_size == 0)
    throw Load_Balancer::no_such_member ();

  // Get the id of the member to return to the client.
  ACE_CString *id = 0;
  member_id_list_.get (id, next_);

  ACE_DEBUG ((LM_DEBUG, "Load_Balancer: In <%C> Group resolved to <%C>\n",
              id_.c_str (),
              id->c_str ()));

  // Adjust <next_> for the next invocation.
  next_ = (next_ + 1) % group_size;

  // Return the object reference corresponding to the found id to the client.
  if (members_.find (*id, obj) == -1)
    throw CORBA::INTERNAL ();

   return obj._retn ();
}

void
RR_Object_Group::unbind (const char *id)
{
  ACE_CString member_id (id);

  // Check to make sure we have it.
  if (members_.find (member_id) == -1)
    throw Load_Balancer::no_such_member ();

  // Remove all entries for this member.
  members_.unbind (member_id);

  // As we remove the id from the <member_id_list>, we note the
  // position of the id in the list.
  size_t position = 0;
  Object_Group_i::ITERATOR iter (member_id_list_);
  while (member_id != *(iter.next ()))
    {
      iter.advance ();
      position++;
    }
  delete (iter.next ());
  iter.remove ();

  // Update <next_> if necessary to reflect the deletion.
  if (position < next_)
    next_--;

  else if (position == next_)
    next_ = next_ % (members_.current_size ());

  ACE_DEBUG ((LM_DEBUG,
              "Load_Balancer: Removed member with id <%C>"
              "from <%C> object group\n", id, id_.c_str ()));
}

