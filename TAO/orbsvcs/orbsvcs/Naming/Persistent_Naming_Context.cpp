// $Id$
// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//   Persistent_Naming_Context.cpp
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================
#include "ace/Auto_Ptr.h"
#include "Persistent_Naming_Context.h"
#include "Persistent_Context_Index.h"
#include "Bindings_Iterator_T.h"

ACE_RCSID(Naming, Persistent_Naming_Context, "$Id:")

int
TAO_Persistent_Bindings_Map::unbind (const char *id,
                                     const char *kind)
{
  TAO_Persistent_ExtId name (id, kind);
  TAO_Persistent_IntId entry;
  if (this->map_->unbind (name, entry, this->allocator_) != 0)
    return -1;
  else
    {
      // Free up the memory we allocated in shared_bind().  Note that
      // this assumes that the "ref" pointer comes first and that
      // the ref, id and kind are contiguously allocated (see
      // shared_bind() for details).
      this->allocator_->free ((void *) (entry.ref_));
      return 0;
    }
}

int
TAO_Persistent_Bindings_Map::bind (const char *id,
                                   const char *kind,
                                   CORBA::Object_ptr obj,
                                   CosNaming::BindingType type)
{
  return this->shared_bind (id, kind, obj, type, 0);
}

int
TAO_Persistent_Bindings_Map::rebind (const char *id,
                                     const char *kind,
                                     CORBA::Object_ptr obj,
                                     CosNaming::BindingType type)
{
  return this->shared_bind (id, kind, obj, type, 1);
}

int
TAO_Persistent_Bindings_Map::find (const char *id,
                                   const char *kind,
                                   CORBA::Object_ptr & obj,
                                   CosNaming::BindingType &type)
{
  TAO_Persistent_ExtId name (id, kind);
  TAO_Persistent_IntId entry;

  if (this->map_->find (name,
                        entry,
                        this->allocator_) != 0)
    return -1;
  else
    {
      ACE_DECLARE_NEW_CORBA_ENV;
      obj = orb_->string_to_object (entry.ref_, ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);
      type = entry.type ();

      return 0;
    }
}

TAO_Persistent_Bindings_Map::TAO_Persistent_Bindings_Map (CORBA::ORB_ptr orb)
  : allocator_ (0),
    map_ (0),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

TAO_Persistent_Bindings_Map::~TAO_Persistent_Bindings_Map (void)
{
}

void
TAO_Persistent_Bindings_Map::destroy (void)
{
  allocator_->free (map_);
}

TAO_Persistent_Bindings_Map::HASH_MAP *
TAO_Persistent_Bindings_Map::map (void)
{
  return map_;
}

size_t
TAO_Persistent_Bindings_Map::total_size (void)
{
  return map_->total_size ();
}

size_t
TAO_Persistent_Bindings_Map::current_size (void)
{
  return map_->current_size ();
}

int
TAO_Persistent_Bindings_Map::open (size_t hash_table_size,
                                   ACE_Allocator *alloc)
{
  allocator_ = alloc;

  // Use allocator to allocate space for the hash map.
  void *hash_map = 0;
  size_t map_size = sizeof (HASH_MAP);
  hash_map = this->allocator_->malloc (map_size);
  //@@ check if hash_map == 0 ??

  // Initialize allocated hash map through placement new.
  ACE_NEW_RETURN (this->map_,
                  (hash_map) HASH_MAP (hash_table_size, this->allocator_),
                  -1);

  return 0;
}

void
TAO_Persistent_Bindings_Map::set (HASH_MAP *map,
                                  ACE_Allocator *alloc)
{
  allocator_ = alloc;
  map_ = map;
}

int
TAO_Persistent_Bindings_Map::shared_bind (const char * id,
                                          const char * kind,
                                          CORBA::Object_ptr obj,
                                          CosNaming::BindingType type,
                                          int rebind)
{
  // Obtain a stringified ior ob <obj> (i.e., the representation we can store).
  ACE_DECLARE_NEW_CORBA_ENV;
  CORBA::String_var ref = orb_->object_to_string (obj, ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  // Calculate and allocate the memory we need to store this name to
  // object binding.
  size_t id_len = ACE_OS::strlen (id) + 1;
  size_t kind_len = ACE_OS::strlen (kind) + 1;
  size_t ref_len = ACE_OS::strlen (ref) + 1;
  size_t total_len = id_len + kind_len + ref_len;
  char *ptr = (char *) this->allocator_->malloc (total_len);

  // Allocation failed - bail out.
  if (ptr == 0)
    return -1;

  // Allocation succeded - place the data into the allocated memory
  // and procceed.
  else
    {
      // Note that the <ref> *must* come first to make sure we can
      // retrieve this pointer later on in unbind().
      char * ref_ptr = ptr;
      char * id_ptr =  ptr + ref_len;
      char * kind_ptr = ptr + ref_len + id_len;
      ACE_OS::strcpy (ref_ptr, ref.in ());
      ACE_OS::strcpy (id_ptr, id);
      ACE_OS::strcpy (kind_ptr, kind);

      TAO_Persistent_ExtId new_name (id_ptr, kind_ptr);
      TAO_Persistent_IntId new_entry (ref_ptr, type);
      int result = -1;

      if (rebind == 0)
        {
          // Do a normal bind.  This will fail if there's already an
          // <new_internal> with the same name.
          result = this->map_->bind (new_name, new_entry, this->allocator_);

          if (result == 1)
            {
              // Entry already existed so bind failed. Free our
              // dynamically allocated  memory.
              this->allocator_->free ((void *) ptr);
              return result;
            }
        }
      else
        {
          // Do a rebind.  If there's already any entry, this will return the existing
          // <new_name> and <new_internal> and overwrite the existing name binding.
          TAO_Persistent_ExtId old_name;
          TAO_Persistent_IntId old_entry;

          result = this->map_->rebind (new_name, new_entry,
                                       old_name, old_entry,
                                       this->allocator_);
          if (result == 1)
            {
              // Free up the memory we allocated in shared_bind().  Note that this
              // assumes that the "ref" pointer comes first and that the id,
              // kind, and ref are contiguously allocated (see above for details)
              this->allocator_->free ((void *) old_entry.ref_);
            }
        }

      if (result == -1)
        // Free our dynamically allocated memory.
        this->allocator_->free ((void *) ptr);
      else
        // If bind() or rebind() succeed, they will automatically sync
        // up the map manager entry.  However, we must sync up our
        // name/value memory.
        this->allocator_->sync (ptr, total_len);

      return result;
    }
}

TAO_Persistent_Naming_Context::TAO_Persistent_Naming_Context (PortableServer::POA_ptr poa,
                                                              const char *poa_id,
                                                              TAO_Persistent_Context_Index *context_index)

  : TAO_Hash_Naming_Context (poa,
                             poa_id),
    counter_ (0),
    persistent_context_ (0),
    index_ (context_index)
{
  ACE_NEW (this->persistent_context_,
           TAO_Persistent_Bindings_Map (context_index->orb ()));

  // Set the superclass pointer.
  context_ = persistent_context_;
}

TAO_Persistent_Naming_Context::TAO_Persistent_Naming_Context (PortableServer::POA_ptr poa,
                                                              const char *poa_id,
                                                              TAO_Persistent_Context_Index *context_index,
                                                              HASH_MAP *map,
                                                              ACE_UINT32 *counter)
  : TAO_Hash_Naming_Context (poa,
                             poa_id),
    counter_ (counter),
    persistent_context_ (0),
    index_ (context_index)
{
  ACE_NEW (this->persistent_context_,
           TAO_Persistent_Bindings_Map (context_index->orb ()));

  // Set the superclass pointer.
  context_ = persistent_context_;

  persistent_context_->set (map, index_->allocator ());
}

int
TAO_Persistent_Naming_Context::init (size_t hash_table_size)
{
  return persistent_context_->open (hash_table_size, index_->allocator ());
}

TAO_Persistent_Naming_Context::~TAO_Persistent_Naming_Context (void)
{
  // This should only get called if we are destroyed, i.e., we don't
  // want persistent state.
  if (this->destroyed_)
    {
      // Remove ourselves from context index.
      index_->unbind (poa_id_.c_str ());
      // Remove the underlying data structure from persistent storage.
      persistent_context_->destroy ();
    }
}

//@@This method may need adjustments to deal with errors correctly.
CosNaming::NamingContext_ptr
TAO_Persistent_Naming_Context::new_context (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                      CosNaming::NamingContext::_nil ());

  // Store the stub we will return from the method here.
  CosNaming::NamingContext_var result;

  // Generate a POA id for the new context.
  char poa_id[BUFSIZ];
  ACE_OS::sprintf (poa_id,
                   "%s_%ld",
                   this->poa_id_.c_str (),
                   (*this->counter_)++);

  // Put together a servant for the new Naming Context.

  TAO_Persistent_Naming_Context *context_impl = 0;
  ACE_NEW_THROW_EX (context_impl,
                    TAO_Persistent_Naming_Context (poa_.in (),
                                                   poa_id,
                                                   index_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (result._retn ());

  // Put <context_impl> into the auto pointer temporarily, in case next
  // allocation fails.
  ACE_Auto_Basic_Ptr<TAO_Persistent_Naming_Context> temp (context_impl);

  if (context_impl->init (persistent_context_->total_size ()) == -1)
    ACE_THROW_RETURN (CORBA::NO_MEMORY (), result._retn ());

  // Register with the index of Naming Contexts.
  if (index_->bind (context_impl->poa_id_.c_str (),
                    context_impl->counter_,
                    context_impl->persistent_context_->map ()) == -1)
    ACE_THROW_RETURN (CORBA::INTERNAL (), result._retn ());
  //@@ At this point destroy_ flag should be enabled to avoid mem. leaks?

  TAO_Naming_Context *context = 0;
  ACE_NEW_THROW_EX (context,
                    TAO_Naming_Context (context_impl),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (result._retn ());

  // Change what we hold in auto pointer.
  temp.release ();
  ACE_Auto_Basic_Ptr<TAO_Naming_Context> temp2 (context);

  // Register the new context with the POA.
  PortableServer::ObjectId_var id =
    PortableServer::string_to_ObjectId (poa_id);

  this->poa_->activate_object_with_id (id.in (),
                                       context,
                                       ACE_TRY_ENV);
  ACE_CHECK_RETURN (result._retn ());

  result = context->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());

  // Give POA the ownership of this servant.
  context->_remove_ref (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());

  // Everything went without errors: release auto pointer and return.
  temp2.release ();
  return result._retn ();
}

void
TAO_Persistent_Naming_Context::list (CORBA::ULong how_many,
                                     CosNaming::BindingList_out &bl,
                                     CosNaming::BindingIterator_out &bi,
                                     CORBA::Environment &ACE_TRY_ENV)
{
  // Allocate nil out parameters in case we won't be able to complete
  // the operation.
  bi = CosNaming::BindingIterator::_nil ();
  ACE_NEW_THROW_EX (bl,
                    CosNaming::BindingList (0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  // Obtain a lock before we proceed with the operation.
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());

  // Dynamically allocate hash map iterator.
  HASH_MAP::ITERATOR *hash_iter = 0;
  ACE_NEW_THROW_EX (hash_iter,
                    HASH_MAP::ITERATOR
                    (*persistent_context_->map ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  // Store <hash_iter temporarily in auto pointer, in case we'll have
  // some failures and throw an exception.
  ACE_Auto_Basic_Ptr<HASH_MAP::ITERATOR> temp (hash_iter);

  // Typedef to the type of BindingIterator servant for ease of use.
  typedef TAO_Bindings_Iterator<HASH_MAP::ITERATOR, HASH_MAP::ENTRY>
    ITER_SERVANT;

  // A pointer to BindingIterator servant.
  ITER_SERVANT *bind_iter = 0;

  // Number of bindings that will go into the BindingList.
  CORBA::ULong n;

  // Calculate number of bindings that will go into bl.
  if (this->context_->current_size () > how_many)
    n = how_many;
  else
    n = this->context_->current_size ();

  // Use hash iterator to populate a BindingList with bindings.
  bl->length (n);

  HASH_MAP::ENTRY *hash_entry;

  for (CORBA::ULong i = 0; i < n; i++)
    {
      hash_iter->next (hash_entry);
      hash_iter->advance ();

      if (ITER_SERVANT::populate_binding (hash_entry, bl[i]) == 0)
          ACE_THROW (CORBA::NO_MEMORY());
    }

  // Now we are done with the BindingsList, and we can follow up on
  // the iterator business.

  // If we do not need to pass back BindingIterator.
  if (this->context_->current_size () <= how_many)
    return;
  else
    {
      // Create a BindingIterator for return.
      ACE_NEW_THROW_EX (bind_iter,
                        ITER_SERVANT (hash_iter, this->poa_.in (), this->lock_),
                        CORBA::NO_MEMORY ());

      // Release <hash_iter> from auto pointer and put <bind_iter> into
      // one.
      temp.release ();
      ACE_Auto_Basic_Ptr<ITER_SERVANT> temp2 (bind_iter);

      // Register with the POA.
      char poa_id[BUFSIZ];
      ACE_OS::sprintf (poa_id,
                       "%s_%ld",
                       this->poa_id_.c_str (),
                       this->counter_++);
      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId (poa_id);

      this->poa_->activate_object_with_id (id.in (),
                                           bind_iter,
                                           ACE_TRY_ENV);
      ACE_CHECK;

      bi = bind_iter->_this (ACE_TRY_ENV);
      ACE_CHECK;

      // Give POA the ownership of this servant.
      bind_iter->_remove_ref (ACE_TRY_ENV);
      ACE_CHECK;

      // Everything went without error, release the auto pointer.
      temp2.release ();
    }
}

//@@ Need instantiation for TAO_Bindings_Iterator.

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Shared_Hash_Map<TAO_Persistent_ExtId, TAO_Persistent_IntId>;
template class ACE_Hash_Map_Manager<TAO_Persistent_ExtId, TAO_Persistent_IntId, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<TAO_Persistent_ExtId, TAO_Persistent_IntId, ACE_Hash<TAO_Persistent_ExtId>, ACE_Equal_To<TAO_Persistent_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<TAO_Persistent_ExtId, TAO_Persistent_IntId>;
template class ACE_Hash<TAO_Persistent_ExtId>;
template class ACE_Equal_To<TAO_Persistent_ExtId>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_Persistent_ExtId, TAO_Persistent_IntId, ACE_Hash<TAO_Persistent_ExtId>, ACE_Equal_To<TAO_Persistent_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<TAO_Persistent_ExtId, TAO_Persistent_IntId, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_Persistent_ExtId, TAO_Persistent_IntId, ACE_Hash<TAO_Persistent_ExtId>, ACE_Equal_To<TAO_Persistent_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<TAO_Persistent_ExtId, TAO_Persistent_IntId, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_Persistent_ExtId, TAO_Persistent_IntId, ACE_Hash<TAO_Persistent_ExtId>, ACE_Equal_To<TAO_Persistent_ExtId>, ACE_Null_Mutex>;
template class ACE_Auto_Basic_Ptr<TAO_Persistent_Naming_Context>;
template class ACE_Auto_Basic_Ptr<ACE_Shared_Hash_Map<TAO_Persistent_Index_ExtId,  TAO_Persistent_Index_IntId>::ITERATOR>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Shared_Hash_Map<TAO_Persistent_ExtId, TAO_Persistent_IntId>
#pragma instantiate ACE_Hash_Map_Manager<TAO_Persistent_ExtId, TAO_Persistent_IntId, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_Persistent_ExtId, TAO_Persistent_IntId, ACE_Hash<TAO_Persistent_ExtId>, ACE_Equal_To<TAO_Persistent_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<TAO_Persistent_ExtId, TAO_Persistent_IntId>
#pragma instantiate ACE_Hash<TAO_Persistent_ExtId>
#pragma instantiate ACE_Equal_To<TAO_Persistent_ExtId>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_Persistent_ExtId, TAO_Persistent_IntId, ACE_Hash<TAO_Persistent_ExtId>, ACE_Equal_To<TAO_Persistent_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_Persistent_ExtId, TAO_Persistent_IntId, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_Persistent_ExtId, TAO_Persistent_IntId, ACE_Hash<TAO_Persistent_ExtId>, ACE_Equal_To<TAO_Persistent_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_Persistent_ExtId, TAO_Persistent_IntId, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_Persistent_ExtId, TAO_Persistent_IntId, ACE_Hash<TAO_Persistent_ExtId>, ACE_Equal_To<TAO_Persistent_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Persistent_Naming_Context>
#pragma instantiate ACE_Auto_Basic_Ptr<ACE_Shared_Hash_Map<TAO_Persistent_Index_ExtId,  TAO_Persistent_Index_IntId>::ITERATOR>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
