// $Id$

#ifndef TAO_PERSISTENT_CONTEXT_INDEX_T_CPP
#define TAO_PERSISTENT_CONTEXT_INDEX_T_CPP

#include "Persistent_Context_Index_T.h"
#include "Persistent_Naming_Context.h"
#include "ace/Auto_Ptr.h"

template <ACE_MEM_POOL_1, class ACE_LOCK> int
TAO_Persistent_Context_Index<ACE_MEM_POOL_2, ACE_LOCK>::unbind (const char *poa_id)
{
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, *this->lock_, -1);

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

template <ACE_MEM_POOL_1, class ACE_LOCK> int
TAO_Persistent_Context_Index<ACE_MEM_POOL_2, ACE_LOCK>::bind (const char *poa_id,
                                                              ACE_UINT32 *&counter,
                                                              TAO_Index::CONTEXT *hash_map)
{
  ACE_GUARD_RETURN (ACE_LOCK, ace_mon, *this->lock_, -1);

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

template <ACE_MEM_POOL_1, class ACE_LOCK>
TAO_Persistent_Context_Index<ACE_MEM_POOL_2, ACE_LOCK>::TAO_Persistent_Context_Index
  (CORBA::ORB_ptr orb,
   PortableServer::POA_ptr poa)
  : lock_ (0),
    allocator_ (0),
    index_ (0),
    index_file_ (0),
    base_address_ (0),
    orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

template <ACE_MEM_POOL_1, class ACE_LOCK>
TAO_Persistent_Context_Index<ACE_MEM_POOL_2, ACE_LOCK>::~TAO_Persistent_Context_Index (void)
{
  delete lock_;
  delete allocator_;
  ACE_OS::free (ACE_reinterpret_cast (void*, ACE_const_cast (LPTSTR, index_file_)));
}

template <ACE_MEM_POOL_1, class ACE_LOCK> ACE_Allocator*
TAO_Persistent_Context_Index<ACE_MEM_POOL_2, ACE_LOCK>::allocator (void)
{
  return allocator_;
}

template <ACE_MEM_POOL_1, class ACE_LOCK> char*
TAO_Persistent_Context_Index<ACE_MEM_POOL_2, ACE_LOCK>::root_ior (void)
{
  return CORBA::string_dup (root_ior_.in ());
}

template <ACE_MEM_POOL_1, class ACE_LOCK> CORBA::ORB_ptr
TAO_Persistent_Context_Index<ACE_MEM_POOL_2, ACE_LOCK>::orb (void)
{
  return orb_.in ();
}

template <ACE_MEM_POOL_1, class ACE_LOCK> int
TAO_Persistent_Context_Index<ACE_MEM_POOL_2, ACE_LOCK>::open (LPCTSTR file_name,
                                                              void *base_address)
{
  this->base_address_ = base_address;

  index_file_ = ACE_OS::strdup (file_name);
  if (index_file_ == 0)
    return -1;

  return create_index ();
}

template <ACE_MEM_POOL_1, class ACE_LOCK> int
TAO_Persistent_Context_Index<ACE_MEM_POOL_2, ACE_LOCK>::init (void)
{
  if (index_->current_size () == 0)
    // CASE 1:there are no Naming Contexts registered.  We need to create
    // one.
    {
      TAO_Persistent_Naming_Context *c_impl = 0;
      TAO_Naming_Context *c = 0;
      CosNaming::NamingContext_var result;

      ACE_NEW_RETURN (c,
                      TAO_Naming_Context,
                      -1);

      // Put c into the auto pointer temporarily, in case next
      // allocation fails.
      ACE_Auto_Basic_Ptr<TAO_Naming_Context> temp (c);

      ACE_NEW_RETURN (c_impl,
                      TAO_Persistent_Naming_Context (c,
                                                     this,
                                                     poa_.in (),
                                                     "NameService"),
                      -1);

      if (c_impl->init () == -1)
        {
          delete c_impl;
          return -1;
        }
      // Allocation succeeded, get rid of auto pointer.
      temp.release ();

      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_TRY
        {
          PortableServer::ObjectId_var id =
            PortableServer::string_to_ObjectId ("NameService");

          this->poa_->activate_object_with_id (id.in (),
                                               c,
                                               ACE_TRY_ENV);
          ACE_TRY_CHECK;

          result = c->_this (ACE_TRY_ENV);
          ACE_TRY_CHECK;

          this->root_ior_=
            orb_->object_to_string (result.in (), ACE_TRY_ENV);
          ACE_TRY_CHECK;

          // register with the index @@need to roll back in case this fails.
          this->bind ("NameService",
                      c_impl->counter_,
                      c_impl->context_.map ());
          return 0;
            }
      ACE_CATCHANY
        {
          delete c;
          return -1;
        }
      ACE_ENDTRY;
      ACE_CHECK_RETURN (-1);

      // Below will never be reached, but the compiler is not smart to
      // figure it out.  This will keep the warnings away.
      return -1;
    }

  else
    // CASE 2:Recreate all Naming Contexts.
    return recreate_all ();
}

template <ACE_MEM_POOL_1, class ACE_LOCK> int
TAO_Persistent_Context_Index<ACE_MEM_POOL_2, ACE_LOCK>::recreate_all (void)
{
  // We can't use our INDEX typedef here because of the broken old g++!

  ACE_Shared_Hash_Map<TAO_Persistent_Index_ExtId,
    TAO_Persistent_Index_IntId>::ITERATOR * index_iter = 0;

  ACE_NEW_RETURN (index_iter,
                  (ACE_Shared_Hash_Map<TAO_Persistent_Index_ExtId, TAO_Persistent_Index_IntId>::ITERATOR) (*index_),
                  -1);

  ACE_Auto_Basic_Ptr<ACE_Shared_Hash_Map<TAO_Persistent_Index_ExtId,
    TAO_Persistent_Index_IntId>::ITERATOR> it (index_iter);

  ACE_Shared_Hash_Map<TAO_Persistent_Index_ExtId,
    TAO_Persistent_Index_IntId>::ENTRY *entry = 0;

   if (TAO_debug_level > 0)
     ACE_DEBUG ((LM_DEBUG, "Starting to recreate Naming Contexts from the file... \n"));

  do
    {
      index_iter->next (entry);

      TAO_Persistent_Naming_Context *c_impl = 0;
      TAO_Naming_Context *c = 0;
      CosNaming::NamingContext_var result;

      ACE_NEW_RETURN (c,
                      TAO_Naming_Context,
                      -1);

      // Put c into the auto pointer temporarily, in case next
      // allocation fails.
      ACE_Auto_Basic_Ptr<TAO_Naming_Context> temp (c);

      ACE_NEW_RETURN (c_impl,
                      TAO_Persistent_Naming_Context (c,
                                                     this,
                                                     poa_.in (),
                                                     entry->ext_id_.poa_id_,
                                                     entry->int_id_.hash_map_,
                                                     entry->int_id_.counter_),
                  -1);

      // Allocation succeeded, get rid of auto pointer.
      temp.release ();

      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_TRY
        {
          PortableServer::ObjectId_var id =
            PortableServer::string_to_ObjectId (entry->ext_id_.poa_id_);

          this->poa_->activate_object_with_id (id.in (),
                                               c,
                                               ACE_TRY_ENV);
          ACE_TRY_CHECK;

          result = c->_this (ACE_TRY_ENV);
          ACE_TRY_CHECK;

          if (ACE_OS::strcmp ("NameService", entry->ext_id_.poa_id_) == 0)
            this->root_ior_=
              orb_->object_to_string (result.in (), ACE_TRY_ENV);
          ACE_TRY_CHECK;

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "Recreating Naming Context with poa_id %s and counter %d\n",
                        entry->ext_id_.poa_id_,
                        (*(c_impl->counter_))));
        }
      ACE_CATCHANY
        {
          delete c;
          return -1;
        }
      ACE_ENDTRY;
      ACE_CHECK_RETURN (-1);

    } while (index_iter->advance ());

  return 0;
}

template <ACE_MEM_POOL_1, class ACE_LOCK> int
TAO_Persistent_Context_Index<ACE_MEM_POOL_2, ACE_LOCK>::create_index (void)
{
  if (ACE_OS::strlen (index_file_) >= MAXNAMELEN + MAXPATHLEN)
    {
      errno = ENAMETOOLONG;
      return -1;
    }

#if !defined (CHORUS)
  ACE_MEM_POOL_OPTIONS options (base_address_);
#else
  // Use base address == 0, don't use a fixed address.
  ACE_MEM_POOL_OPTIONS options (0,
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

  ACE_NEW_RETURN (this->lock_, ACE_LOCK, -1);

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
  if (this->allocator_->find (TAO_NAME_CONTEXTS_INDEX, context_index) == 0)
    this->index_ = (INDEX *) context_index;

  else
    {
      size_t index_size = sizeof (INDEX);
      context_index = this->allocator_->malloc (index_size);

      // Initialize the map into its memory location (e.g., shared memory).
      ACE_NEW_RETURN (this->index_,
                      (context_index) INDEX (this->allocator_),
                      -1);

      if (this->allocator_->bind (TAO_NAME_CONTEXTS_INDEX, context_index) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "create_index\n"), -1);
    }

  return 0;
}

#endif /* TAO_PERSISTENT_CONTEXT_INDEX_T_CPP */
