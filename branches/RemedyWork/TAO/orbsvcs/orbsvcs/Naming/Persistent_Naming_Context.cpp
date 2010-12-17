// $Id$

#include "orbsvcs/Naming/Persistent_Naming_Context.h"
#include "orbsvcs/Naming/Persistent_Context_Index.h"
#include "orbsvcs/Naming/Bindings_Iterator_T.h"
#include "ace/OS_NS_stdio.h"

#include "ace/Auto_Ptr.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_Persistent_Bindings_Map::unbind (const char *id, const char *kind)
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

  if (this->map_->find (name, entry, this->allocator_) != 0)
    return -1;
  else
    {
      obj = orb_->string_to_object (entry.ref_);
      type = entry.type_;

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
  this->map_->ACE_Hash_Map_With_Allocator<TAO_Persistent_ExtId, TAO_Persistent_IntId>::~ACE_Hash_Map_With_Allocator ();
  this->allocator_->free (map_);
}

TAO_Persistent_Bindings_Map::HASH_MAP *
TAO_Persistent_Bindings_Map::map (void)
{
  return this->map_;
}

size_t
TAO_Persistent_Bindings_Map::total_size (void)
{
  return this->map_->total_size ();
}

size_t
TAO_Persistent_Bindings_Map::current_size (void)
{
  return map_->current_size ();
}

int
TAO_Persistent_Bindings_Map::open (size_t hash_table_size, ACE_Allocator *alloc)
{
  allocator_ = alloc;

  // Use allocator to allocate space for the hash map.
  void *hash_map = 0;
  size_t map_size = sizeof (HASH_MAP);
  hash_map = this->allocator_->malloc (map_size);

  // If allocation failed ...
  if (hash_map == 0)
    return -1;

  // Initialize allocated hash map through placement new.
  if (open_helper (hash_table_size, hash_map) == -1)
    this->allocator_->free (hash_map);

  return 0;
}

int
TAO_Persistent_Bindings_Map::open_helper (size_t hash_table_size,
                                          void *buffer)
{
  this->map_ = new (buffer) HASH_MAP (hash_table_size, this->allocator_);
  return 0;
}

void
TAO_Persistent_Bindings_Map::set (HASH_MAP *map, ACE_Allocator *alloc)
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
  // Obtain a stringified ior of <obj> (i.e., the representation we can store).
  CORBA::String_var ref = orb_->object_to_string (obj);

  // Calculate and allocate the memory we need to store this name to
  // object binding.
  size_t id_len = ACE_OS::strlen (id) + 1;
  size_t kind_len = ACE_OS::strlen (kind) + 1;
  size_t ref_len = ACE_OS::strlen (ref.in ()) + 1;
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
        // Rebind.
        {
          TAO_Persistent_ExtId old_name;
          TAO_Persistent_IntId old_entry;

          // Check that the types of old and new entries match.
          if (this->map_->find (new_name,
                                old_entry,
                                this->allocator_) == 0
              && type != old_entry.type_)
            result = -2;

          // If types match, perform rebind.
          else
            result = this->map_->rebind (new_name, new_entry,
                                         old_name, old_entry,
                                         this->allocator_);
          if (result == 1)
            {
              // Free up the old binding's memory, if it was replaced.
              // Note, this assumes that the "ref" pointer comes
              // first, and that the id, kind, and ref are contiguously
              // allocated (see beginning of this method for details).
              this->allocator_->free ((void *) old_entry.ref_);
            }
        }

      // Check for failures, and clean up dynamically allocated memory
      // if necessary.
      if (result < 0)
        this->allocator_->free ((void *) ptr);

      else
        // If bind() or rebind() succeeded, they will automatically sync
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
  // Perform appropriate cleanup based on the destruction level specified.

  if (this->destroyed_ > 1)
    {
      // Remove ourselves from context index.
      index_->unbind (poa_id_.c_str ());
      // Remove the underlying data structure from persistent storage.
      persistent_context_->destroy ();
    }
  else if (this->destroyed_ == 1)
    // Remove the underlying data structure from persistent storage.
    persistent_context_->destroy ();
}

void
TAO_Persistent_Naming_Context::set_cleanup_level (int level)
{
  this->destroyed_ = level;
}

CosNaming::NamingContext_ptr
TAO_Persistent_Naming_Context::make_new_context (PortableServer::POA_ptr poa,
                                                 const char *poa_id,
                                                 size_t context_size,
                                                 TAO_Persistent_Context_Index * ind)
{
  // Store the stub we will return here.
  CosNaming::NamingContext_var result;

  // Put together a servant for the new Naming Context.

  TAO_Persistent_Naming_Context *context_impl = 0;
  ACE_NEW_THROW_EX (context_impl,
                    TAO_Persistent_Naming_Context (poa,
                                                   poa_id,
                                                   ind),
                    CORBA::NO_MEMORY ());

  // Put <context_impl> into the auto pointer temporarily, in case next
  // allocation fails.
  ACE_Auto_Basic_Ptr<TAO_Persistent_Naming_Context> temp (context_impl);

  if (context_impl->init (context_size) == -1)
    throw CORBA::NO_MEMORY ();

  // Insure appropriate cleanup in case of exception conditions ahead.
  context_impl->set_cleanup_level (1);

  // Register with the index of Naming Contexts.
  if (ind->bind (context_impl->poa_id_.c_str (),
                 context_impl->counter_,
                 context_impl->persistent_context_->map ()) == -1)
    throw CORBA::INTERNAL ();

  // Insure appropriate cleanup in case of exception conditions ahead.
  context_impl->set_cleanup_level (2);

  TAO_Naming_Context *context = 0;
  ACE_NEW_THROW_EX (context,
                    TAO_Naming_Context (context_impl),
                    CORBA::NO_MEMORY ());

  // Let <implementation> know about it's <interface>.
  context_impl->interface (context);

  // Release auto pointer, and start using reference counting to
  // control our servant.
  temp.release ();
  PortableServer::ServantBase_var s = context;

  // Register the new context with the POA.
  PortableServer::ObjectId_var id =
    PortableServer::string_to_ObjectId (poa_id);

  poa->activate_object_with_id (id.in (),
                                context);

  result = context->_this ();

  // Everything went smoothly, without errors - we don't need any cleanup.
  context_impl->set_cleanup_level (0);

  return result._retn ();
}

CosNaming::NamingContext_ptr
TAO_Persistent_Naming_Context::new_context (void)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    throw CORBA::OBJECT_NOT_EXIST ();

  // Generate a POA id for the new context.
  char poa_id[BUFSIZ];
  ACE_OS::sprintf (poa_id,
                   "%s_%d",
                   this->poa_id_.c_str (),
                   (*this->counter_)++);

  CosNaming::NamingContext_var result =
    make_new_context (this->poa_.in (),
                      poa_id,
                      this->persistent_context_->total_size (),
                      this->index_);

  return result._retn ();
}

void
TAO_Persistent_Naming_Context::list (CORBA::ULong how_many,
                                     CosNaming::BindingList_out &bl,
                                     CosNaming::BindingIterator_out &bi)
{
  // Allocate nil out parameters in case we won't be able to complete
  // the operation.
  bi = CosNaming::BindingIterator::_nil ();
  ACE_NEW_THROW_EX (bl,
                    CosNaming::BindingList (0),
                    CORBA::NO_MEMORY ());

  // Obtain a lock before we proceed with the operation.
  ACE_GUARD_THROW_EX (TAO_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    throw CORBA::OBJECT_NOT_EXIST ();

  // Dynamically allocate hash map iterator.
  HASH_MAP::ITERATOR *hash_iter = 0;
  ACE_NEW_THROW_EX (hash_iter,
                    HASH_MAP::ITERATOR
                    (*persistent_context_->map ()),
                    CORBA::NO_MEMORY ());

  // Store <hash_iter temporarily in auto pointer, in case we'll have
  // some failures and throw an exception.
  ACE_Auto_Basic_Ptr<HASH_MAP::ITERATOR> temp (hash_iter);

  // Silliness below is required because of broken old g++!!!  E.g.,
  // without it, we could have just said HASH_MAP::ITERATOR everywhere we use ITER_DEF.
  typedef ACE_Hash_Map_With_Allocator<TAO_Persistent_ExtId, TAO_Persistent_IntId>::ITERATOR ITER_DEF;
  typedef ACE_Hash_Map_With_Allocator<TAO_Persistent_ExtId, TAO_Persistent_IntId>::ENTRY ENTRY_DEF;

  // Typedef to the type of BindingIterator servant for ease of use.
  typedef TAO_Bindings_Iterator<ITER_DEF, ENTRY_DEF> ITER_SERVANT;

  // A pointer to BindingIterator servant.
  ITER_SERVANT *bind_iter = 0;

  // Number of bindings that will go into the BindingList.
  CORBA::ULong n;

  // Calculate number of bindings that will go into bl.
  if (this->context_->current_size () > how_many)
    n = how_many;
  else
    n = static_cast<CORBA::ULong> (this->context_->current_size ());

  // Use hash iterator to populate a BindingList with bindings.
  bl->length (n);

  ENTRY_DEF *hash_entry = 0;

  for (CORBA::ULong i = 0; i < n; i++)
    {
      hash_iter->next (hash_entry);
      hash_iter->advance ();

      if (ITER_SERVANT::populate_binding (hash_entry, bl[i]) == 0)
          throw CORBA::NO_MEMORY();
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
                        ITER_SERVANT (this, hash_iter, this->poa_.in (), this->lock_),
                        CORBA::NO_MEMORY ());

      // Release <hash_iter> from auto pointer, and start using the
      // reference counting to control our servant.
      temp.release ();
      PortableServer::ServantBase_var iter = bind_iter;

      // Increment reference count on this Naming Context, so it doesn't get
      // deleted before the BindingIterator servant gets deleted.
      interface_->_add_ref ();

      // Register with the POA.
      char poa_id[BUFSIZ];
      ACE_OS::sprintf (poa_id,
                       "%s_%d",
                       this->poa_id_.c_str (),
                       (*this->counter_)++);
      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId (poa_id);

      this->poa_->activate_object_with_id (id.in (),
                                           bind_iter);

      bi = bind_iter->_this ();
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
