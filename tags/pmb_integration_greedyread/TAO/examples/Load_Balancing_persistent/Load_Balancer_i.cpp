// $Id$
#include "Load_Balancer_i.h"
#include "ace/Auto_Ptr.h"
#include "ace/Hash_Map_Manager_T.h"

const char *rr_name_bind = "RR_Group";
// Name binding for the location of the Round Robin info in the mem pool

const char *random_name_bind = "Random_Group";
// Name binding for the location of the Random info in the mem pool

const char *flags_name_bind = "FLAGS";
// Name binding for the location of the flags info in the mem pool

const char *dll_name_bind = "DLL_LIST";
// Name binding for the DLL_LIst in the me_pool;

const char *server_id_name_bind = "server_id";
// Some cookie that is used for appending names

Object_Group_Factory_i::Object_Group_Factory_i (CORBA::ORB_ptr orb,
                                                PortableServer::POA_ptr poa)
  :orb_ (orb),
   poa_ (PortableServer::POA::_duplicate (poa)),
   random_groups_ (0),
   rr_groups_ (0),
   flags_ (0)
{
  ACE_MMAP_Memory_Pool::OPTIONS options (ACE_DEFAULT_BASE_ADDR);
  ACE_NEW (this->mem_pool_,
           ALLOCATOR ("Mem_Pool",
                      "Mem_Pool",
                      &options));
}

Object_Group_Factory_i::~Object_Group_Factory_i (void)
{
  delete this->mem_pool_;
}

PortableServer::POA_ptr
Object_Group_Factory_i::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}


Load_Balancer::Object_Group_ptr
Object_Group_Factory_i::make_round_robin (const char * id
                                          ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Load_Balancer::duplicate_group))
{

  if (this->mem_pool_->find (rr_name_bind,
                             (void *&)this->rr_groups_) == -1)
    {
      void *hash_map = this->mem_pool_->malloc (sizeof (HASH_MAP));
      ACE_NEW_THROW_EX (this->rr_groups_,
                        (hash_map) HASH_MAP (this->mem_pool_),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (Load_Balancer::Object_Group::_nil ());

      // Bind it in the mem pool with a name
      if (this->mem_pool_->bind (rr_name_bind,
                                 (void *)this->rr_groups_) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to bind \n"),
                            0);
        }
    }

  return this->make_group (0,
                           id
                           ACE_ENV_ARG_PARAMETER);
}

void
Object_Group_Factory_i::unbind_round_robin (const char * id
                                            ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Load_Balancer::no_such_group))
{
  if (this->rr_groups_ == 0)
    {
      if (this->mem_pool_->find (rr_name_bind,
                                 (void *&)this->rr_groups_) == -1)
        ACE_THROW (Load_Balancer::no_such_group ());
    }

  char *int_id = 0;

  // Throw an exception if not found in the HASH MAP
  if (this->rr_groups_->find (ACE_const_cast (char *, id),
                              this->mem_pool_) < 0)
    ACE_THROW (Load_Balancer::no_such_group ());

  // Unbind the entry
  this->rr_groups_->unbind (ACE_const_cast (char *, id),
                            int_id,
                            this->mem_pool_);

  // Free the memory from the pool
  this->mem_pool_->free (int_id - (ACE_OS::strlen (id) + 1));

  // Change the FLAGS variable
  if (this->flags_ == 0)
    {
      if (this->mem_pool_->find (flags_name_bind,
                                 (void *&)this->flags_) == -1)
        return;
    }

  // Bump down the flags value
  --this->flags_;

}

Load_Balancer::Object_Group_ptr
Object_Group_Factory_i::make_random (const char * id
                                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Load_Balancer::duplicate_group))
{

  if (this->mem_pool_->find (random_name_bind, (void * &)this->random_groups_) == -1)
    {
      void *hash_map = this->mem_pool_->malloc (sizeof (HASH_MAP));

      ACE_NEW_THROW_EX (this->random_groups_,
                        (hash_map) HASH_MAP (this->mem_pool_),
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (Load_Balancer::Object_Group::_nil ());

      // Bind it in the mem pool with a name
      if (this->mem_pool_->bind (random_name_bind,
                                 (void *)this->random_groups_) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to bind \n"),
                            0);
        }
    }

  return this->make_group (1,
                           id
                           ACE_ENV_ARG_PARAMETER);
}


void
Object_Group_Factory_i::unbind_random (const char * id
                                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Load_Balancer::no_such_group))
{
  if (this->random_groups_ == 0)
    {
      if (this->mem_pool_->find (random_name_bind,
                                 (void *&)this->random_groups_) == -1)
        ACE_THROW (Load_Balancer::no_such_group ());
    }

  char *int_id = 0;

  // Throw an exception if not found in the HASH MAP
  if (this->random_groups_->find (ACE_const_cast (char *, id),
                                  this->mem_pool_) < 0)
    ACE_THROW (Load_Balancer::no_such_group ());

  // Unbind the entry
  this->random_groups_->unbind (ACE_const_cast (char *, id),
                                int_id,
                                this->mem_pool_);

  // Free the memory from the pool
  this->mem_pool_->free (int_id - (ACE_OS::strlen (id) + 1));

  // Change the FLAGS variable
  if (this->flags_ == 0)
    {
      if (this->mem_pool_->find (flags_name_bind,
                                 (void *&)this->flags_) == -1)
        return;
    }

  // Bump down the flags value
  this->flags_ -= 2;
}

Load_Balancer::Object_Group_ptr
Object_Group_Factory_i::make_group (int random,
                                    const char * id
                                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Load_Balancer::duplicate_group))
{
  // Store our result here for return.
  Load_Balancer::Object_Group_var group;

  // Create an appropriate servant.
  Object_Group_i *group_servant = 0;

  // Check to make sure we don't already have a group with the same
  // <id>.

  if (random)
    {
      if (this->random_groups_->find (ACE_const_cast (char *,id),
                                      this->mem_pool_) == 0)
        ACE_THROW_RETURN (Load_Balancer::duplicate_group (),
                          Load_Balancer::Object_Group::_nil ());
    }
  else
    {
      if (this->rr_groups_->find (ACE_const_cast (char *,id),
                                  this->mem_pool_) == 0)
        ACE_THROW_RETURN (Load_Balancer::duplicate_group (),
                          Load_Balancer::Object_Group::_nil ());
    }



  // As we are sure that it is not in the list go ahead and insert it
  if (random)
    ACE_NEW_THROW_EX (group_servant,
                      Random_Object_Group (id,
                                           this->poa_.in ()),
                      CORBA::NO_MEMORY ());
  else
    ACE_NEW_THROW_EX (group_servant,
                      RR_Object_Group (id,
                                       this->poa_.in ()),
                      CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (group._retn ());

  // Register with the poa, begin using ref. counting.
  group = group_servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (group._retn ());

  group_servant->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (Load_Balancer::Object_Group::_nil ());

  CORBA::String_var ior =
    this->orb_->object_to_string (group.in ()
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (Load_Balancer::Object_Group::_nil ());


  // Calculate and allocate the memory we need to store this name to
  // object binding.
  size_t id_len = ACE_OS::strlen (id) + 1;
  size_t kind_len = ACE_OS::strlen (ior.in ()) + 1;

  char *ptr = (char *) this->mem_pool_->malloc (id_len + kind_len);

  if (ptr == 0)
    ACE_THROW_RETURN (CORBA::NO_MEMORY (),
                      Load_Balancer::Object_Group::_nil ());

  char * id_ptr =  ptr;
  char * ior_ptr = ptr + id_len;

  ACE_OS::strcpy (id_ptr, id);
  ACE_OS::strcpy (ior_ptr, ior.in ());

  // Store the results here
  CORBA::Long result = 0;

  // Make an entry in appropriate map of groups.
  if (random)
    {
      result = this->random_groups_->bind (id_ptr,
                                           ior_ptr,
                                           this->mem_pool_);
    }
  else
    {
      result = this->rr_groups_->bind (id_ptr,
                                       ior_ptr,
                                       this->mem_pool_);
    }



  // Update the value of flags_
  this->update_flags (random
                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (Load_Balancer::Object_Group::_nil ());

  if (result == -1)
    {
      // For some reason the  bind failed. Free our
      // dynamically allocated  memory.
      this->mem_pool_->free ((void *) ptr);
      ACE_THROW_RETURN (Load_Balancer::duplicate_group (),
                        Load_Balancer::Object_Group::_nil ());

    }

  // Return.
  ACE_DEBUG ((LM_DEBUG, "Successfully created new group: %s\n", id));

  return group._retn ();
}


Load_Balancer::Object_Group_ptr
Object_Group_Factory_i::resolve (const char * id
                                 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Load_Balancer::no_such_group))
{



#if defined (DOORS_MEASURE_STATS)
  // Time the calls
  // Record the entry  time.
  ACE_hrtime_t latency_base = ACE_OS::gethrtime ();

#endif /*DOORS_MEASURE_STATS*/

  // It could be that the Load balancing service could have failed
  // before the Client tries to invoke this call.. In such a case the
  // Service should look in to the MMAP file and read in the info
  // before it can resolve the ID sent by the client.. So we check
  // whether the class holds the pointer.. If not we look in to the
  // MMAP file for the relevant info..
  if (!this->rr_groups_)
    {
      if (this->mem_pool_->find (rr_name_bind,
                                 (void *&)this->rr_groups_) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%N|%l) The factory does not have any references "),
                             ACE_TEXT ("to the group that you have sought \n\n")),
                            0);
        }
    }

  if (!this->random_groups_)
    {
      if (this->mem_pool_->find (random_name_bind,
                                 (void *&)this->random_groups_) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%N|%l) The factory does not have any references "),
                             ACE_TEXT ("to the group that you have sought \n\n")),
                            0);

        }
    }

  if (!this->flags_)
    {
      this->mem_pool_->find (flags_name_bind,
                             (void *&)this->flags_);
      this->update_objects (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (Load_Balancer::Object_Group::_nil ());
    }

  char *ior = 0;

  if (rr_groups_->find (ACE_const_cast (char *, id),
                        ior,
                        this->mem_pool_) == -1
      && random_groups_->find (ACE_const_cast (char *, id),
                               ior,
                               this->mem_pool_) == -1)
    ACE_THROW_RETURN (Load_Balancer::no_such_group (),
                      0);

  CORBA::Object_var objref =
    this->orb_->string_to_object (ior
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (Load_Balancer::Object_Group::_nil ());

  Load_Balancer::Object_Group_ptr
    object_group = Load_Balancer::Object_Group::_narrow (objref.in ()
                                                         ACE_ENV_ARG_PARAMETER);

  ACE_CHECK_RETURN (Load_Balancer::Object_Group::_nil ());


#if defined (DOORS_MEASURE_STATS)
  // Grab timestamp again.
  ACE_hrtime_t now = ACE_OS::gethrtime ();

  this->throughput_.sample (0,
                            now - latency_base);

  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
  ACE_OS::printf ("*=*=*=*=Aggregated result *=*=*=*=*= \n");
  this->throughput_.dump_results ("Aggregated", gsf);

#endif /*DOORS_MEASURE_STATS*/

  return object_group;
}

Load_Balancer::Group_List *
Object_Group_Factory_i::list_groups (int random
                                     ACE_ENV_ARG_DECL)
{
  Load_Balancer::Group_List * list;

  // Figure out the length of the list.
  CORBA::ULong len;
  if (random)
    len = random_groups_->current_size ();
  else
    len = rr_groups_->current_size ();

  // Allocate the list of <len> length.
  ACE_NEW_THROW_EX (list,
                    Load_Balancer::Group_List (len),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (list);
  list->length (len);

  // Create an iterator for group structure to populate the list.
  HASH_MAP::ITERATOR *group_iter;
  HASH_MAP::ITERATOR random_iter (*(this->random_groups_));
  HASH_MAP::ITERATOR rr_iter (*(this->rr_groups_));
  if (random)
    group_iter = &random_iter;
  else
    group_iter = &rr_iter;

  // Iterate over groups and populate the list.
  HASH_MAP::ENTRY *hash_entry;
  for (CORBA::ULong i = 0; i < len; i++)
    {
      group_iter->next (hash_entry);
      group_iter->advance ();

      (*list)[i] = ACE_OS::strdup (hash_entry->ext_id_);
    }

  return list;
}

Load_Balancer::Group_List *
Object_Group_Factory_i::round_robin_groups (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return list_groups (0 ACE_ENV_ARG_PARAMETER);
}

Load_Balancer::Group_List *
Object_Group_Factory_i::random_groups (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return list_groups (1 ACE_ENV_ARG_PARAMETER);
}


void
Object_Group_Factory_i::update_flags (int random
                                      ACE_ENV_ARG_DECL)
{
  //First check whether we have memory for flags_
  if (!this->flags_)
    {
      if (this->mem_pool_->find (flags_name_bind,
                                 (void *&) this->flags_) == -1)
        {
          void *value =
            this->mem_pool_->malloc (sizeof (CORBA::Short));
          ACE_NEW_THROW_EX (this->flags_,
                            (value) CORBA::Short (0),
                            CORBA::NO_MEMORY ());
          ACE_CHECK;

          // Initialize the variable
          this->mem_pool_->bind (flags_name_bind,
                                 (void *)this->flags_);
        }
    }

  CORBA::Short val = *(this->flags_);
  switch (val)
    {
    case 0:
      if (random)
        *(this->flags_) = 2;
      else
        *(this->flags_) = 1;
      break;
    case 1:
      if (random)
        *(this->flags_) = 3;
      break;
    case 2:
      if (!random)
        *(this->flags_) = 3;
      break;

    }
}

void
Object_Group_Factory_i::update_objects (ACE_ENV_SINGLE_ARG_DECL)
{
  // Create an appropriate servant.
  Object_Group_i * group_servant = 0;
  Object_Group_i *group_servant_rep = 0;

  // Check the value of of flags_ & do the instantiation and
  // registration

  switch (*(this->flags_))
    {
    case 1:
      ACE_NEW_THROW_EX (group_servant,
                        RR_Object_Group ("Round Robin group",
                                         this->poa_.in ()),
                        CORBA::NO_MEMORY ());
      group_servant->_this ();
      break;

    case 2:
      ACE_NEW_THROW_EX (group_servant,
                        Random_Object_Group ("Random group",
                                             this->poa_.in ()),
                        CORBA::NO_MEMORY ());
      group_servant->_this ();
      break;
    case 3:
      ACE_NEW_THROW_EX (group_servant_rep,
                        Random_Object_Group ("Random group",
                                             this->poa_.in ()),
                        CORBA::NO_MEMORY ());
      group_servant_rep->_this ();

      ACE_NEW_THROW_EX (group_servant,
                        RR_Object_Group ("Round Robin group",
                                         this->poa_.in ()),
                        CORBA::NO_MEMORY ());
      group_servant->_this ();
      break;
    }

}


Object_Group_i::Object_Group_i (const char * id,
                                PortableServer::POA_ptr poa)
  :poa_ (PortableServer::POA::_duplicate (poa)),
   member_id_list_ (0),
   members_ (0),
   id_ (id),
   allocator_ (0)
{

  if (!this->allocator_)
    {
      ACE_MMAP_Memory_Pool::OPTIONS options (ACE_DEFAULT_BASE_ADDR);
      ACE_NEW (this->allocator_,
               ALLOCATOR ("Mem_Pool",
                          "Mem_Pool",
                          &options));
    }
}


Object_Group_i::~Object_Group_i (void)
{
  // Need to delete all the items from the member_id_list, to avoid
  // memory leaks.
  Object_Group_i::ITERATOR iter (*member_id_list_);

  do
    {
      delete (iter.next ());
    } while (iter.advance ());

  delete this->allocator_;
}


PortableServer::POA_ptr
Object_Group_i::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}


char *
Object_Group_i::id (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (id_.c_str ());
}

void
Object_Group_i::bind (const Load_Balancer::Member & member
                      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Load_Balancer::duplicate_member))
{

  if (this->members_ == 0)
    {
      ACE_CString id = this->id (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      id += server_id_name_bind;

      if (this->allocator_->find (id.c_str (), (void *&)this->members_) == -1)
        {
          void *hash_map = this->allocator_->malloc (sizeof (HASH_MAP));
          ACE_NEW_THROW_EX (this->members_,
                            (hash_map) HASH_MAP (this->allocator_),
                            CORBA::NO_MEMORY ());
          ACE_CHECK;

          // Bind it in the mem pool with a name
          if (this->allocator_->bind (id.c_str (),
                                      (void *)this->members_) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "Unable to bind \n"));

            }
        }
    }

  // Check whether the element already exists..
  if (this->members_->find (ACE_const_cast (char *,
                                            (const char *) member.id),
                            this->allocator_) == 0)
    ACE_THROW (Load_Balancer::duplicate_member ());

  size_t id_len = ACE_OS::strlen (member.id) + 1;
  size_t ref_len = ACE_OS::strlen (member.obj) + 1;

  char *mem_alloc = (char *)this->allocator_->malloc (id_len + ref_len);

  if (mem_alloc == 0)
     ACE_THROW (CORBA::NO_MEMORY ());

  char **id_ptr = (char **)this->allocator_->malloc (sizeof (char *));
  *id_ptr = mem_alloc;
  char *ior_ptr = mem_alloc + id_len;

  ACE_OS::strcpy (*id_ptr, member.id);
  ACE_OS::strcpy (ior_ptr, member.obj);


  // Insert new member into <members_> and check for duplicates/failures.
  int result = this->members_->trybind (*id_ptr,
                                        ior_ptr);

  if (result == 1)
    ACE_THROW (Load_Balancer::duplicate_member ());
  else if (result == -1)
    ACE_THROW (CORBA::INTERNAL ());

  // Search the list first from the mem mapp pool and then Insert new
  // member's id into <member_id_list_>.

  ACE_CString id = dll_name_bind;
  id += this->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;


  if (this->allocator_->find (id.c_str (),
                              (void *&)this->member_id_list_)
      == -1)
    {
      void *dll_list = this->allocator_->malloc (sizeof (LIST));
      ACE_NEW_THROW_EX (this->member_id_list_,
                        (dll_list) LIST (this->allocator_),
                        CORBA::NO_MEMORY ());
      ACE_CHECK;

      // Bind it in the mem pool with a name
      if (this->allocator_->bind (id.c_str (),
                                 (void *)this->member_id_list_) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "Unable to bind \n"));
          return;
        }
    }

  if (member_id_list_->insert_tail (id_ptr) == 0)
    ACE_THROW (CORBA::NO_MEMORY ());

  // Theoretically, we should deal with memory failures more
  // thoroughly.  But, practically, the whole system is going to be
  // hosed anyways ...
}

void
Object_Group_i::unbind (const char * id
                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Load_Balancer::no_such_member))
{
  // Check whether the this->member_ is NULL
  if (this->members_ == 0)
    {
      ACE_CString id = this->id (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      id += server_id_name_bind;

      if (this->allocator_->find (id.c_str (),
                                  (void *&)this->members_) == -1)
        {
          ACE_THROW (Load_Balancer::no_such_member ());
        }
    }
  // Check to make sure we have it.
  if (this->members_->find (ACE_const_cast (char *, id),
                            this->allocator_) == -1)
    ACE_THROW (Load_Balancer::no_such_member ());

  // Remove all entries for this member.
  this->members_->unbind (ACE_const_cast(char *, id),
                          this->allocator_);

  if (this->member_id_list_ == 0)
    {
      ACE_CString id = dll_name_bind;
      id += this->id (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (this->allocator_->find (id.c_str (),
                                  (void *&)this->member_id_list_)
          == -1)
        ACE_THROW (Load_Balancer::no_such_member ());

    }



  Object_Group_i::ITERATOR iter (*(this->member_id_list_));

  while (ACE_OS::strcmp (id,*(iter.next ())))
    iter.advance ();

  this->allocator_->free ((void *) iter.next ());

  iter.remove ();
}

char *
Object_Group_i::resolve_with_id (const char * id
                                 ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Load_Balancer::no_such_member))
{
  CORBA::String_var ior;


  if (this->members_->find (ACE_const_cast (char *,
                                            id),
                      ior.out (), this->allocator_) == -1)
    ACE_THROW_RETURN (Load_Balancer::no_such_member (),
                      0);

  char *retn_ptr = CORBA::string_dup (ior.in ());

  return retn_ptr;

}

Load_Balancer::Member_ID_List *
Object_Group_i::members (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  Load_Balancer::Member_ID_List * list = 0;

  this->read_from_memory (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Figure out the length of the list.
  CORBA::ULong len = this->members_->current_size ();

  // Allocate the list of <len> length.
  ACE_NEW_THROW_EX (list,
                    Load_Balancer::Member_ID_List (len),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (list);
  list->length (len);

  // Create an iterator for <member_id_list_> to populate the list.
  Object_Group_i::ITERATOR id_iter (*this->member_id_list_);

  char **item = 0;
  // Iterate over groups and populate the list.
  for (CORBA::ULong i = 0; i < len; i++)
    {
      this->member_id_list_->get (item);
      (*list)[i] = *(id_iter.next ());
      id_iter.advance ();
    }

  return list;
}

void
Object_Group_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Deregister with POA.
  PortableServer::POA_var poa =
    this->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  poa->deactivate_object (id.in ()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
Object_Group_i::read_from_memory (ACE_ENV_SINGLE_ARG_DECL)
{
    // Sanity check needs to be done in all the places
  ACE_CString id = this->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (!this->members_)
    {
      id += server_id_name_bind;

      if (this->allocator_->find (id.c_str (),
                                  (void *&)this->members_) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "Unable to find tha HASH MAP  in the MMAP file \n"));
        }
    }


  if (!this->member_id_list_)
    {
      id = dll_name_bind;
      id += this->id (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (this->allocator_->find (id.c_str (),
                                  (void *&)this->member_id_list_) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "Unable to find tha HASH MAP  in the MMAP file \n"));
        }
    }

}


Random_Object_Group::Random_Object_Group (const char *id,
                                          PortableServer::POA_ptr poa)
  : Object_Group_i (id, poa)
{
  // Seed the random number generator.
  ACE_OS::srand (ACE_OS::time ());
}

char *
Random_Object_Group::resolve (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Load_Balancer::no_such_member))
{

  this->read_from_memory (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  size_t group_size = this->members_->current_size ();
  if (group_size == 0)
    ACE_THROW_RETURN (Load_Balancer::no_such_member (),
                      0);

  // Generate random number in the range [0, group_size - 1]
  size_t member = ACE_OS::rand() % group_size;

  // Get the id of the member to return to the client.
  char **id = 0;
  this->member_id_list_->get (id, member);
  ACE_DEBUG ((LM_DEBUG, "In Random Group resolved to: %s\n",
              *id));

  // Return the object reference corresponding to the found id to the
  // client.
  char *objref = 0;
  this->members_->find (*id,
                        objref,
                        this->allocator_);
  char *string_ptr = CORBA::string_dup (objref);
  return string_ptr;
}

RR_Object_Group::RR_Object_Group (const char *id,
                                  PortableServer::POA_ptr poa)
  : Object_Group_i (id, poa),
    next_ (0)
{
}

char *
RR_Object_Group::resolve (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Load_Balancer::no_such_member))
{
  char *objref = 0;

  this->read_from_memory (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  size_t group_size = this->members_->current_size ();
  if (group_size == 0)
    ACE_THROW_RETURN (Load_Balancer::no_such_member (),
                      0);

  // Get the id of the member to return to the client.
  char **id;
  this->member_id_list_->get (id, next_);
  ACE_DEBUG ((LM_DEBUG,
              "In RR Group resolved to: %s\n", *id));

  // Adjust <next_> for the next invocation.
  next_ = (next_ + 1) % group_size;


  // Return the object reference corresponding to the found id to the client.
  if (this->members_->find (*id,
                            objref,
                            this->allocator_) == -1)
    ACE_THROW_RETURN (CORBA::INTERNAL (),
                      0);

  char *retn_ptr = CORBA::string_dup (objref);

  return retn_ptr;
}

void
RR_Object_Group::unbind (const char *id
                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Load_Balancer::no_such_member))
{

  if (this->members_ == 0)
    {
      ACE_CString id = this->id (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      id += server_id_name_bind;

      if (this->allocator_->find (id.c_str (),
                                  (void *&)this->members_) == -1)
        {
          ACE_THROW (Load_Balancer::no_such_member ());
        }
    }

  // Check to make sure we have it.
  if (this->members_->find (ACE_const_cast (char *,id),
                            this->allocator_) == -1)
    ACE_THROW (Load_Balancer::no_such_member ());

  // Remove all entries for this member.
  this->members_->unbind (ACE_const_cast (char *, id),
                          this->allocator_);

  // As we remove the id from the <member_id_list>, we note the
  // position of the id in the list.
  if (this->member_id_list_ == 0)
    {
      ACE_CString id = dll_name_bind;
      id += this->id (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (this->allocator_->find (id.c_str (),
                                  (void *&)this->member_id_list_)
          == -1)
        ACE_THROW (Load_Balancer::no_such_member ());

    }

  size_t position = 0;
  Object_Group_i::ITERATOR iter (*member_id_list_);
  while (ACE_OS::strcmp (id ,*(iter.next ())))
    {
      iter.advance ();
      position++;
    }
  this->allocator_->free (iter.next ());
  iter.remove ();

  int curr_size = this->members_->current_size ();

  // Update <next_> if necessary to reflect the deletion.
  if (position < next_)
    this->next_--;
  else if (curr_size == 0)
    this->next_ = 0;
  else if (position == next_)
    this->next_ = next_ % (this->members_->current_size ());
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Iterator_Base_Ex<char *, char *, ACE_Hash<char *>, ACE_Equal_To<char *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<char *, char *, ACE_Hash<char *>, ACE_Equal_To<char *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<char *, char *>;
template class ACE_Hash_Map_With_Allocator<char *, char *>;
template class ACE_Hash_Map_Manager<char *, char *, ACE_Null_Mutex>;
template class ACE_DLList<char *>;
template class ACE_DLList_Iterator<char*>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<char*,char*,ACE_Hash<char*>,ACE_Equal_To<char*>,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<char*,char*,ACE_Hash<char*>,ACE_Equal_To<char*>,ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<char *, char *, ACE_Hash<char *>, ACE_Equal_To<char *>, ACE_Null_Mutex>
pragma instantiate ACE_Hash_Map_Manager_Ex<char *, char *, ACE_Hash<char *>, ACE_Equal_To<char *>, ACE_Null_Mutex>
pragma instantiate ACE_Hash_Map_Entry<char *, char *>
pragma instantiate ACE_Hash_Map_With_Allocator<char *, char *>
pragma instantiate ACE_Hash_Map_Manager<char *, char *, ACE_Null_Mutex>
pragma instantiate ACE_DLList<char *>
pragma instantiate ACE_DLList_Iterator<char*>
pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<char*,char*,ACE_Hash<char*>,ACE_Equal_To<char*>,ACE_Null_Mutex>
template class ACE_Hash_Map_Iterator_Ex<char*,char*,ACE_Hash<char*>,ACE_Equal_To<char*>,ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
