// $Id$

#include "Persistent_Context_Index.h"
#include "Persistent_Naming_Context.h"
#include "ace/Auto_Ptr.h"

int
TAO_Persistent_Context_Index::unbind (const char *poa_id)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1);

  TAO_Persistent_Index_ExtId name (poa_id);
  TAO_Persistent_Index_IntId entry;
  if (this->index_->unbind (name, entry, this->allocator_) != 0)
    return -1;
  else
    {
      // Free up the memory we allocated in bind().
      this->allocator_->free ((void *) (entry.counter_));
      return 0;
    }
}

int
TAO_Persistent_Context_Index::bind (const char *poa_id,
                                    ACE_UINT32 *&counter,
                                    TAO_Persistent_Context_Index::CONTEXT *hash_map)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1);

  size_t poa_id_len = ACE_OS::strlen (poa_id) + 1;
  size_t counter_len = sizeof (ACE_UINT32);
  char *ptr = (char *) this->allocator_->malloc (poa_id_len + counter_len);

  if (ptr == 0)
    return -1;
  else
    {
      counter = ACE_reinterpret_cast (ACE_UINT32 *, ptr);
      *counter = 0;
      char * poa_id_ptr = ptr + counter_len;
      ACE_OS::strcpy (poa_id_ptr, poa_id);

      TAO_Persistent_Index_ExtId name (poa_id_ptr);
      TAO_Persistent_Index_IntId entry (counter, hash_map);
      int result = -1;

      // Do a normal bind.  This will fail if there's already an
      // entry with the same name.
      result = this->index_->bind (name, entry, this->allocator_);

      if (result == 1)
        {
          // Entry already existed so bind failed. Free our dynamically allocated memory.
          this->allocator_->free ((void *) ptr);
          return result;
        }

      if (result == -1)
        // Free our dynamically allocated memory.
        this->allocator_->free ((void *) ptr);
      else
        // If bind() succeed, it will automatically sync
        // up the map manager entry.  However, we must sync up our
        // name/value memory.
        this->allocator_->sync (ptr, poa_id_len + counter_len);

      return result;
    }
}

TAO_Persistent_Context_Index::TAO_Persistent_Context_Index
  (CORBA::ORB_ptr orb,
   PortableServer::POA_ptr poa)
  : allocator_ (0),
    index_ (0),
    index_file_ (0),
    base_address_ (0),
    orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

TAO_Persistent_Context_Index::~TAO_Persistent_Context_Index (void)
{
  delete allocator_;
  ACE_OS::free (ACE_reinterpret_cast (void*, ACE_const_cast (LPTSTR, index_file_)));
}

ACE_Allocator*
TAO_Persistent_Context_Index::allocator (void)
{
  return allocator_;
}

char*
TAO_Persistent_Context_Index::root_ior (void)
{
  return CORBA::string_dup (root_ior_.in ());
}

CORBA::ORB_ptr
TAO_Persistent_Context_Index::orb (void)
{
  return orb_.in ();
}

int
TAO_Persistent_Context_Index::open (LPCTSTR file_name,
                                    void *base_address)
{
  this->base_address_ = base_address;

  index_file_ = ACE_OS::strdup (file_name);
  if (index_file_ == 0)
    return -1;

  return create_index ();
}

int
TAO_Persistent_Context_Index::init (void)
{
  // Return value of this function (necessary to keep compilers quiet).
  int status = 0;

  if (index_->current_size () == 0)
    // CASE 1:there are no Naming Contexts registered.  We need to create
    // one.
    {
      // Store the stub we will return from the method here.
      CosNaming::NamingContext_var result;

      // Put together a servant for the new Naming Context.

      TAO_Persistent_Naming_Context *context_impl = 0;
      ACE_NEW_RETURN (context_impl,
                      TAO_Persistent_Naming_Context (poa_.in (),
                                                     "NameService",
                                                     this),
                      -1);

      // Put <context_impl> into the auto pointer temporarily, in case next
      // allocation fails.
      ACE_Auto_Basic_Ptr<TAO_Persistent_Naming_Context> temp (context_impl);

      if (context_impl->init (ACE_DEFAULT_MAP_SIZE) == -1)
          return -1;

      // register with the index @@need to roll back in case this fails.
      this->bind ("NameService",
                  context_impl->counter_,
                  context_impl->persistent_context_->map ());

      TAO_Naming_Context *context = 0;
      ACE_NEW_RETURN (context,
                      TAO_Naming_Context (context_impl),
                      -1);

      // Change what we hold in auto pointer.
      temp.release ();
      ACE_Auto_Basic_Ptr<TAO_Naming_Context> temp2 (context);

      // Register with the POA.
      ACE_DECLARE_NEW_CORBA_ENV;
      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId ("NameService");

      this->poa_->activate_object_with_id (id.in (),
                                           context,
                                           ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);

      result = context->_this (ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);

      this->root_ior_=
        orb_->object_to_string (result.in (), ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);

      // Give POA the ownership of this servant.
      context->_remove_ref (ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);

      // Everything went successfully.
      status = 0;
    }

  else
    // CASE 2:Recreate all Naming Contexts.
    status = recreate_all ();

  return status;
}

int
TAO_Persistent_Context_Index::recreate_all (void)
{
  INDEX::ITERATOR *index_iter = 0;

  ACE_NEW_RETURN (index_iter,
                  (INDEX::ITERATOR) (*index_),
                  -1);

  ACE_Auto_Basic_Ptr<INDEX::ITERATOR> it (index_iter);

  // Because of broken old g++!!!
  typedef ACE_Shared_Hash_Map<TAO_Persistent_Index_ExtId,
    TAO_Persistent_Index_IntId>  IND_DEF;

  IND_DEF::ENTRY *entry = 0;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Starting to recreate Naming Contexts from the file... \n"));

  do
    {
      index_iter->next (entry);

      // Put together a servant for the new Naming Context.

      TAO_Persistent_Naming_Context *context_impl = 0;
      ACE_NEW_RETURN (context_impl,
                      TAO_Persistent_Naming_Context (poa_.in (),
                                                     entry->ext_id_.poa_id_,
                                                     this,
                                                     entry->int_id_.hash_map_,
                                                     entry->int_id_.counter_),
                  -1);


      // Put <context_impl> into the auto pointer temporarily, in case next
      // allocation fails.
      ACE_Auto_Basic_Ptr<TAO_Persistent_Naming_Context> temp (context_impl);

      TAO_Naming_Context *context = 0;
      ACE_NEW_RETURN (context,
                      TAO_Naming_Context (context_impl),
                      -1);

      // Change what we hold in auto pointer.
      temp.release ();
      ACE_Auto_Basic_Ptr<TAO_Naming_Context> temp2 (context);

      // Register with the POA.
      ACE_DECLARE_NEW_CORBA_ENV;
      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId (entry->ext_id_.poa_id_);

      this->poa_->activate_object_with_id (id.in (),
                                           context,
                                           ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);

      CosNaming::NamingContext_var result = context->_this (ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);

      // Give POA the ownership of this servant.
      context->_remove_ref (ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);

      if (context_impl->root ())
        {
          this->root_ior_=
            orb_->object_to_string (result.in (), ACE_TRY_ENV);
          ACE_CHECK_RETURN (-1);
        }

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "Recreating Naming Context with poa_id %s and counter %d\n",
                    entry->ext_id_.poa_id_,
                    (*(context_impl->counter_))));

      temp2.release ();

    } while (index_iter->advance ());

  return 0;
}

int
TAO_Persistent_Context_Index::create_index (void)
{
  if (ACE_OS::strlen (index_file_) >= MAXNAMELEN + MAXPATHLEN)
    {
      errno = ENAMETOOLONG;
      return -1;
    }

#if !defined (CHORUS)
  ACE_MMAP_Memory_Pool::OPTIONS options (base_address_);
#else
  // Use base address == 0, don't use a fixed address.
  ACE_MMAP_Memory_Pool::OPTIONS options (0,
                                         0,
                                         0,
                                         ACE_CHORUS_LOCAL_NAME_SPACE_T_SIZE);
#endif /* CHORUS */

  // Create the allocator with the appropriate options.  The name used
  // for  the lock is the same one as used for the file.
  ACE_NEW_RETURN (this->allocator_,
                  ALLOCATOR (this->index_file_,
                             this->index_file_,
                             &options),
                  -1);

#if !defined (ACE_LACKS_ACCESS)
  // Now check if the backing store has been created successfully
  if (ACE_OS::access (this->index_file_, F_OK) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "create_index\n"),
                      -1);
#endif /* ACE_LACKS_ACCESS */

  void *context_index = 0;

  // This is the easy case since if we find the Context Index Map
  // we know it's already initialized.
  if (this->allocator_->find (TAO_NAMING_CONTEXT_INDEX, context_index) == 0)
    this->index_ = (INDEX *) context_index;

  else
    {
      size_t index_size = sizeof (INDEX);
      context_index = this->allocator_->malloc (index_size);

      // Initialize the map into its memory location (e.g., shared memory).
      ACE_NEW_RETURN (this->index_,
                      (context_index) INDEX (this->allocator_),
                      -1);

      if (this->allocator_->bind (TAO_NAMING_CONTEXT_INDEX, context_index) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "create_index\n"), -1);
    }

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Auto_Basic_Ptr<TAO_Naming_Context>;
template class ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_SYNCH_MUTEX>;
template class ACE_Allocator_Adapter<ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_SYNCH_MUTEX> >;
template class ACE_Shared_Hash_Map<TAO_Persistent_Index_ExtId, TAO_Persistent_Index_IntId>;
template class ACE_Hash_Map_Manager<TAO_Persistent_Index_ExtId, TAO_Persistent_Index_IntId, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<TAO_Persistent_Index_ExtId, TAO_Persistent_Index_IntId, ACE_Hash<TAO_Persistent_Index_ExtId>, ACE_Equal_To<TAO_Persistent_Index_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<TAO_Persistent_Index_ExtId, TAO_Persistent_Index_IntId>;
template class ACE_Hash<TAO_Persistent_Index_ExtId>;
template class ACE_Equal_To<TAO_Persistent_Index_ExtId>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_Persistent_Index_ExtId, TAO_Persistent_Index_IntId, ACE_Hash<TAO_Persistent_Index_ExtId>, ACE_Equal_To<TAO_Persistent_Index_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<TAO_Persistent_Index_ExtId, TAO_Persistent_Index_IntId, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_Persistent_Index_ExtId, TAO_Persistent_Index_IntId, ACE_Hash<TAO_Persistent_Index_ExtId>, ACE_Equal_To<TAO_Persistent_Index_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<TAO_Persistent_Index_ExtId, TAO_Persistent_Index_IntId, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_Persistent_Index_ExtId, TAO_Persistent_Index_IntId, ACE_Hash<TAO_Persistent_Index_ExtId>, ACE_Equal_To<TAO_Persistent_Index_ExtId>, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Naming_Context>
#pragma instantiate ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Allocator_Adapter<ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_SYNCH_MUTEX> >
#pragma instantiate ACE_Shared_Hash_Map<TAO_Persistent_Index_ExtId, TAO_Persistent_Index_IntId>
#pragma instantiate ACE_Hash_Map_Manager<TAO_Persistent_Index_ExtId, TAO_Persistent_Index_IntId, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_Persistent_Index_ExtId, TAO_Persistent_Index_IntId, ACE_Hash<TAO_Persistent_Index_ExtId>, ACE_Equal_To<TAO_Persistent_Index_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<TAO_Persistent_Index_ExtId, TAO_Persistent_Index_IntId>
#pragma instantiate ACE_Hash<TAO_Persistent_Index_ExtId>
#pragma instantiate ACE_Equal_To<TAO_Persistent_Index_ExtId>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_Persistent_Index_ExtId, TAO_Persistent_Index_IntId, ACE_Hash<TAO_Persistent_Index_ExtId>, ACE_Equal_To<TAO_Persistent_Index_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_Persistent_Index_ExtId, TAO_Persistent_Index_IntId, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_Persistent_Index_ExtId, TAO_Persistent_Index_IntId, ACE_Hash<TAO_Persistent_Index_ExtId>, ACE_Equal_To<TAO_Persistent_Index_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_Persistent_Index_ExtId, TAO_Persistent_Index_IntId, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_Persistent_Index_ExtId, TAO_Persistent_Index_IntId, ACE_Hash<TAO_Persistent_Index_ExtId>, ACE_Equal_To<TAO_Persistent_Index_ExtId>, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
