// $Id$
// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//   Persistent_Bindings_Map.cpp
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================

#include "Persistent_Bindings_Map.h"


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
      // shared_bind() for details)
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
TAO_Persistent_Bindings_Map::open (size_t size,
                                   ACE_Allocator *alloc)
{
  allocator_ = alloc;

  void *ns_map = 0;

  size_t map_size = sizeof (HASH_MAP);
  ns_map = this->allocator_->malloc (map_size);

  // Initialize the map into its memory location (e.g., shared memory).
  ACE_NEW_RETURN (this->map_,
                  (ns_map) HASH_MAP (size, this->allocator_),
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
  ACE_DECLARE_NEW_CORBA_ENV;
  CORBA::String_var ref = orb_->object_to_string (obj, ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  size_t id_len = ACE_OS::strlen (id) + 1;
  size_t kind_len = ACE_OS::strlen (kind) + 1;
  size_t ref_len = ACE_OS::strlen (ref) + 1;
  size_t total_len = id_len + kind_len + ref_len;
  char *ptr = (char *) this->allocator_->malloc (total_len);

  if (ptr == 0)
    return -1;
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
              // Entry already existed so bind failed. Free our dynamically allocated memory.
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
              // assumes that the "value" pointer comes first and that the value,
              // name, and type are contiguously allocated (see above for details)
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Shared_Hash_Map<TAO_Persistent_ExtId, TAO_Persistent_IntId>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Shared_Hash_Map<TAO_Persistent_ExtId, TAO_Persistent_IndId>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
