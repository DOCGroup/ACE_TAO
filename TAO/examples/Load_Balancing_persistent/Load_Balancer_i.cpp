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
Object_Group_Factory_i::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}


Load_Balancer::Object_Group_ptr
Object_Group_Factory_i::make_round_robin (const char * id)
{

  void *tmp_rr (0);

  if (this->mem_pool_->find (rr_name_bind,
                             tmp_rr) == 0)
    {
      this->rr_groups_ = reinterpret_cast <HASH_MAP *> (tmp_rr);
    }
  else
    {
      void *hash_map = this->mem_pool_->malloc (sizeof (HASH_MAP));

      if (hash_map == 0)
        throw CORBA::NO_MEMORY ();

      this->rr_groups_ = new (hash_map) HASH_MAP (this->mem_pool_);

      // Bind it in the mem pool with a name
      if (this->mem_pool_->bind (rr_name_bind,
                                 (void *)this->rr_groups_) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to bind\n"),
                            0);
        }
    }

  return this->make_group (0,
                           id);
}

void
Object_Group_Factory_i::unbind_round_robin (const char * id)
{
  if (this->rr_groups_ == 0)
    {
      void *tmp_rr (0);

      if (this->mem_pool_->find (rr_name_bind,
                                 tmp_rr) == -1)
        throw Load_Balancer::no_such_group ();

      this->rr_groups_ = reinterpret_cast <HASH_MAP *> (tmp_rr);
    }

  char *int_id = 0;

  // Throw an exception if not found in the HASH MAP
  if (this->rr_groups_->find (const_cast<char *> (id),
                              this->mem_pool_) < 0)
    throw Load_Balancer::no_such_group ();

  // Unbind the entry
  this->rr_groups_->unbind (const_cast<char *> (id),
                            int_id,
                            this->mem_pool_);

  // Free the memory from the pool
  this->mem_pool_->free (int_id - (ACE_OS::strlen (id) + 1));

  // Change the FLAGS variable
  if (this->flags_ == 0)
    {
      void *tmp_flags (0);

      if (this->mem_pool_->find (flags_name_bind,
                                 tmp_flags) == -1)
        return;

      this->flags_ = reinterpret_cast <CORBA::Short *> (tmp_flags);
    }

  // Bump down the flags value
  --this->flags_;

}

Load_Balancer::Object_Group_ptr
Object_Group_Factory_i::make_random (const char * id)
{
  void *tmp_random (0);

  if (this->mem_pool_->find (random_name_bind, tmp_random) == 0)
    {
      this->random_groups_ = reinterpret_cast <HASH_MAP *> (tmp_random);
    }
  else
    {
      void *hash_map = this->mem_pool_->malloc (sizeof (HASH_MAP));

      if (hash_map == 0)
        throw CORBA::NO_MEMORY ();

      this->random_groups_ = new (hash_map) HASH_MAP (this->mem_pool_);

      // Bind it in the mem pool with a name
      if (this->mem_pool_->bind (random_name_bind,
                                 (void *)this->random_groups_) != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to bind\n"),
                            0);
        }
    }

  return this->make_group (1,
                           id);
}


void
Object_Group_Factory_i::unbind_random (const char * id)
{
  if (this->random_groups_ == 0)
    {
      void *tmp_random (0);

      if (this->mem_pool_->find (random_name_bind,
                                 tmp_random) == -1)
        throw Load_Balancer::no_such_group ();

      this->random_groups_ = reinterpret_cast <HASH_MAP *> (tmp_random);
    }

  char *int_id = 0;

  // Throw an exception if not found in the HASH MAP
  if (this->random_groups_->find (const_cast<char *> (id),
                                  this->mem_pool_) < 0)
    throw Load_Balancer::no_such_group ();

  // Unbind the entry
  this->random_groups_->unbind (const_cast<char *> (id),
                                int_id,
                                this->mem_pool_);

  // Free the memory from the pool
  this->mem_pool_->free (int_id - (ACE_OS::strlen (id) + 1));

  // Change the FLAGS variable
  if (this->flags_ == 0)
    {
      void *tmp_flags (0);

      if (this->mem_pool_->find (flags_name_bind,
                                 tmp_flags) == -1)
        return;

      this->flags_ = reinterpret_cast <CORBA::Short *> (tmp_flags);
    }

  // Bump down the flags value
  this->flags_ -= 2;
}

Load_Balancer::Object_Group_ptr
Object_Group_Factory_i::make_group (int random,
                                    const char * id)
{
  // Store our result here for return.
  Load_Balancer::Object_Group_var group;

  // Create an appropriate servant.
  Object_Group_i *group_servant = 0;

  // Check to make sure we don't already have a group with the same
  // <id>.

  if (random)
    {
      if (this->random_groups_->find (const_cast<char *> (id),
                                      this->mem_pool_) == 0)
        throw Load_Balancer::duplicate_group ();
    }
  else
    {
      if (this->rr_groups_->find (const_cast<char *> (id),
                                  this->mem_pool_) == 0)
        throw Load_Balancer::duplicate_group ();
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

  // Register with the poa, begin using ref. counting.
  group = group_servant->_this ();

  group_servant->_remove_ref ();

  CORBA::String_var ior =
    this->orb_->object_to_string (group.in ());


  // Calculate and allocate the memory we need to store this name to
  // object binding.
  size_t id_len = ACE_OS::strlen (id) + 1;
  size_t kind_len = ACE_OS::strlen (ior.in ()) + 1;

  char *ptr = (char *) this->mem_pool_->malloc (id_len + kind_len);

  if (ptr == 0)
    throw CORBA::NO_MEMORY ();

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
  this->update_flags (random);

  if (result == -1)
    {
      // For some reason the  bind failed. Free our
      // dynamically allocated  memory.
      this->mem_pool_->free ((void *) ptr);
      throw Load_Balancer::duplicate_group ();

    }

  // Return.
  ACE_DEBUG ((LM_DEBUG, "Successfully created new group: %s\n", id));

  return group._retn ();
}


Load_Balancer::Object_Group_ptr
Object_Group_Factory_i::resolve (const char * id)
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
      void *tmp_rr (0);

      if (this->mem_pool_->find (rr_name_bind,
                                 tmp_rr) == 0)
        {
          this->rr_groups_ = reinterpret_cast <HASH_MAP *> (tmp_rr);
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%N|%l) The factory does not have any references ")
                             ACE_TEXT ("to the group that you have sought \n\n")),
                            0);
        }
    }

  if (!this->random_groups_)
    {
      void *tmp_random (0);

      if (this->mem_pool_->find (random_name_bind,
                                 tmp_random) == 0)
        {
          this->random_groups_ = reinterpret_cast <HASH_MAP *> (tmp_random);
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%N|%l) The factory does not have any references ")
                             ACE_TEXT ("to the group that you have sought \n\n")),
                            0);

        }
    }

  if (!this->flags_)
    {
      void *tmp_flags (0);

      this->mem_pool_->find (flags_name_bind,
                             tmp_flags);
      this->flags_ = reinterpret_cast <CORBA::Short *> (tmp_flags);

      this->update_objects ();
    }

  char *ior = 0;

  if (rr_groups_->find (const_cast<char *> (id),
                        ior,
                        this->mem_pool_) == -1
      && random_groups_->find (const_cast<char *> (id),
                               ior,
                               this->mem_pool_) == -1)
    throw Load_Balancer::no_such_group ();

  CORBA::Object_var objref =
    this->orb_->string_to_object (ior);

  Load_Balancer::Object_Group_ptr
    object_group = Load_Balancer::Object_Group::_narrow (objref.in ());



#if defined (DOORS_MEASURE_STATS)
  // Grab timestamp again.
  ACE_hrtime_t now = ACE_OS::gethrtime ();

  this->throughput_.sample (0,
                            now - latency_base);

  ACE_High_Res_Timer::global_scale_factor_type gsf =
    ACE_High_Res_Timer::global_scale_factor ();
  ACE_OS::printf ("*=*=*=*=Aggregated result *=*=*=*=*=\n");
  this->throughput_.dump_results (ACE_TEXT("Aggregated"), gsf);

#endif /*DOORS_MEASURE_STATS*/

  return object_group;
}

Load_Balancer::Group_List *
Object_Group_Factory_i::list_groups (int random)
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
  HASH_MAP::ENTRY *hash_entry = 0;
  for (CORBA::ULong i = 0; i < len; i++)
    {
      group_iter->next (hash_entry);
      group_iter->advance ();

      (*list)[i] = ACE_OS::strdup (hash_entry->ext_id_);
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


void
Object_Group_Factory_i::update_flags (int random)
{
  //First check whether we have memory for flags_
  if (!this->flags_)
    {
      void *tmp_flags (0);

      if (this->mem_pool_->find (flags_name_bind,
                                 tmp_flags) == 0)
        {
          this->flags_ = reinterpret_cast <CORBA::Short *> (tmp_flags);
        }
      else
        {
          void *value =
            this->mem_pool_->malloc (sizeof (CORBA::Short));

          if (value == 0)
            throw CORBA::NO_MEMORY ();

          this->flags_ = new (value) CORBA::Short (0);

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
Object_Group_Factory_i::update_objects (void)
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
Object_Group_i::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}


char *
Object_Group_i::id (void)
{
  return CORBA::string_dup (id_.c_str ());
}

void
Object_Group_i::bind (const Load_Balancer::Member & member)
{

  if (this->members_ == 0)
    {
      ACE_CString id = this->id ();

      id += server_id_name_bind;

      void *tmp_members (0);

      if (this->allocator_->find (id.c_str (), tmp_members) == 0)
        {
          this->members_ = reinterpret_cast <HASH_MAP *> (tmp_members);
        }
      else
        {
          void *hash_map = this->allocator_->malloc (sizeof (HASH_MAP));

          if (hash_map == 0)
            throw CORBA::NO_MEMORY ();

          this->members_ = new (hash_map) HASH_MAP (this->allocator_);

          // Bind it in the mem pool with a name
          if (this->allocator_->bind (id.c_str (),
                                      (void *)this->members_) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "Unable to bind\n"));

            }
        }
    }

  // Check whether the element already exists..
  if (this->members_->find (const_cast<char *> ((const char *) member.id),
                            this->allocator_) == 0)
    throw Load_Balancer::duplicate_member ();

  size_t id_len = ACE_OS::strlen (member.id) + 1;
  size_t ref_len = ACE_OS::strlen (member.obj) + 1;

  char *mem_alloc = (char *)this->allocator_->malloc (id_len + ref_len);

  if (mem_alloc == 0)
     throw CORBA::NO_MEMORY ();

  char **id_ptr = (char **)this->allocator_->malloc (sizeof (char *));
  *id_ptr = mem_alloc;
  char *ior_ptr = mem_alloc + id_len;

  ACE_OS::strcpy (*id_ptr, member.id);
  ACE_OS::strcpy (ior_ptr, member.obj);


  // Insert new member into <members_> and check for duplicates/failures.
  int result = this->members_->trybind (*id_ptr,
                                        ior_ptr);

  if (result == 1)
    throw Load_Balancer::duplicate_member ();
  else if (result == -1)
    throw CORBA::INTERNAL ();

  // Search the list first from the mem mapp pool and then Insert new
  // member's id into <member_id_list_>.

  ACE_CString id = dll_name_bind;
  id += this->id ();


  void *tmp_id_list (0);

  if (this->allocator_->find (id.c_str (),
                              tmp_id_list)
      == 0)
    {
      this->member_id_list_ = reinterpret_cast <LIST *> (tmp_id_list);
    }
  else
    {
      void *dll_list = this->allocator_->malloc (sizeof (LIST));

      if (dll_list == 0)
        throw CORBA::NO_MEMORY ();

      this->member_id_list_ = new (dll_list) LIST (this->allocator_);

      // Bind it in the mem pool with a name
      if (this->allocator_->bind (id.c_str (),
                                 (void *)this->member_id_list_) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "Unable to bind\n"));
          return;
        }
    }

  if (member_id_list_->insert_tail (id_ptr) == 0)
    throw CORBA::NO_MEMORY ();

  // Theoretically, we should deal with memory failures more
  // thoroughly.  But, practically, the whole system is going to be
  // hosed anyways ...
}

void
Object_Group_i::unbind (const char * id)
{
  // Check whether the this->member_ is NULL
  if (this->members_ == 0)
    {
      ACE_CString id = this->id ();

      id += server_id_name_bind;

      void *tmp_members (0);

      if (this->allocator_->find (id.c_str (),
                                  tmp_members) == -1)
        {
          throw Load_Balancer::no_such_member ();
        }

      this->members_ = reinterpret_cast <HASH_MAP *> (tmp_members);
    }
  // Check to make sure we have it.
  if (this->members_->find (const_cast<char *> (id),
                            this->allocator_) == -1)
    throw Load_Balancer::no_such_member ();

  // Remove all entries for this member.
  this->members_->unbind (const_cast<char *> (id),
                          this->allocator_);

  if (this->member_id_list_ == 0)
    {
      ACE_CString id = dll_name_bind;
      id += this->id ();

      void *tmp_id_list (0);

      if (this->allocator_->find (id.c_str (),
                                  tmp_id_list)
          == -1)
        throw Load_Balancer::no_such_member ();

      this->member_id_list_ = reinterpret_cast <LIST *> (tmp_id_list);
    }



  Object_Group_i::ITERATOR iter (*(this->member_id_list_));

  while (ACE_OS::strcmp (id,*(iter.next ())))
    iter.advance ();

  this->allocator_->free ((void *) iter.next ());

  iter.remove ();
}

char *
Object_Group_i::resolve_with_id (const char * id)
{
  CORBA::String_var ior;


  if (this->members_->find (const_cast<char *> (id),
                      ior.out (), this->allocator_) == -1)
    throw Load_Balancer::no_such_member ();

  char *retn_ptr = CORBA::string_dup (ior.in ());

  return retn_ptr;

}

Load_Balancer::Member_ID_List *
Object_Group_i::members (void)
{
  Load_Balancer::Member_ID_List * list = 0;

  this->read_from_memory ();

  // Figure out the length of the list.
  CORBA::ULong len = this->members_->current_size ();

  // Allocate the list of <len> length.
  ACE_NEW_THROW_EX (list,
                    Load_Balancer::Member_ID_List (len),
                    CORBA::NO_MEMORY ());
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
Object_Group_i::destroy (void)
{
  // Deregister with POA.
  PortableServer::POA_var poa =
    this->_default_POA ();

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this);

  poa->deactivate_object (id.in ());
}

void
Object_Group_i::read_from_memory (void)
{
    // Sanity check needs to be done in all the places
  ACE_CString id = this->id ();

  if (!this->members_)
    {
      id += server_id_name_bind;

      void *tmp_members (0);

      if (this->allocator_->find (id.c_str (),
                                  tmp_members) == 0)
        {
          this->members_ = reinterpret_cast <HASH_MAP *> (tmp_members);
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      "Unable to find tha HASH MAP  in the MMAP file\n"));
        }
    }


  if (!this->member_id_list_)
    {
      id = dll_name_bind;
      id += this->id ();

      void *tmp_id_list (0);

      if (this->allocator_->find (id.c_str (),
                                  tmp_id_list) == 0)
        {
          this->member_id_list_ = reinterpret_cast <LIST *> (tmp_id_list);
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      "Unable to find tha HASH MAP  in the MMAP file\n"));
        }
    }

}


Random_Object_Group::Random_Object_Group (const char *id,
                                          PortableServer::POA_ptr poa)
  : Object_Group_i (id, poa)
{
  // Seed the random number generator.
  ACE_OS::srand (static_cast<u_int> (ACE_OS::time ()));
}

char *
Random_Object_Group::resolve (void)
{

  this->read_from_memory ();

  size_t group_size = this->members_->current_size ();
  if (group_size == 0)
    throw Load_Balancer::no_such_member ();

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
RR_Object_Group::resolve (void)
{
  char *objref = 0;

  this->read_from_memory ();

  size_t group_size = this->members_->current_size ();
  if (group_size == 0)
    throw Load_Balancer::no_such_member ();

  // Get the id of the member to return to the client.
  char **id = 0;
  this->member_id_list_->get (id, next_);
  ACE_DEBUG ((LM_DEBUG,
              "In RR Group resolved to: %s\n", *id));

  // Adjust <next_> for the next invocation.
  next_ = (next_ + 1) % group_size;


  // Return the object reference corresponding to the found id to the client.
  if (this->members_->find (*id,
                            objref,
                            this->allocator_) == -1)
    throw CORBA::INTERNAL ();

  char *retn_ptr = CORBA::string_dup (objref);

  return retn_ptr;
}

void
RR_Object_Group::unbind (const char *id)
{

  if (this->members_ == 0)
    {
      ACE_CString id = this->id ();

      id += server_id_name_bind;

      void *tmp_members (0);

      if (this->allocator_->find (id.c_str (),
                                  tmp_members) == -1)
        {
          throw Load_Balancer::no_such_member ();
        }

      this->members_ = reinterpret_cast <HASH_MAP *> (tmp_members);
    }

  // Check to make sure we have it.
  if (this->members_->find (const_cast<char *> (id),
                            this->allocator_) == -1)
    throw Load_Balancer::no_such_member ();

  // Remove all entries for this member.
  this->members_->unbind (const_cast<char *> (id),
                          this->allocator_);

  // As we remove the id from the <member_id_list>, we note the
  // position of the id in the list.
  if (this->member_id_list_ == 0)
    {
      ACE_CString id = dll_name_bind;
      id += this->id ();

      void *tmp_id_list (0);

      if (this->allocator_->find (id.c_str (),
                                  tmp_id_list)
          == -1)
        throw Load_Balancer::no_such_member ();

      this->member_id_list_ = reinterpret_cast <LIST *> (tmp_id_list);
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

  size_t curr_size = this->members_->current_size ();

  // Update <next_> if necessary to reflect the deletion.
  if (position < next_)
    this->next_--;
  else if (curr_size == 0)
    this->next_ = 0;
  else if (position == next_)
    this->next_ = next_ % (this->members_->current_size ());
}

