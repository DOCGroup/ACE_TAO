/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//    Persistent_Bindings_Map.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_PERSISTENT_BINDINGS_MAP_H
#define TAO_PERSISTENT_BINDINGS_MAP_H

#include "Persistent_Entries.h"

class TAO_ORBSVCS_Export TAO_Persistent_Bindings_Map
{
  // = TITLE
  //
  //
  // = DESCRIPTION
  //
public:
  typedef ACE_Shared_Hash_Map<TAO_Persistent_ExtId, TAO_Persistent_IntId> HASH_MAP;

  // = Initialization and termination methods.
  TAO_Persistent_Bindings_Map (CORBA::ORB_ptr orb);
  // "Do-nothing" constructor.

  int open (size_t size,
            ACE_Allocator *alloc);
  // Allocate hash map using the <alloc>.

  void set (HASH_MAP *map,
           ACE_Allocator *alloc);
  // The map is already preallocated for us.

  ~TAO_Persistent_Bindings_Map (void);
  // destructor, do some cleanup :TBD: last dtor should "compress"
  // file

  void destroy (void);
  // This method removes frees up the hash map from allocator.
  // It better be empty, since we are not cleaning up the insides. (We
  // could add <close> to clean entries, but not the data inside.

  size_t total_size (void);
  size_t current_size (void);
  // return the size of the underlying hash map.

  virtual int bind (const char *id,
                    const char *kind,
                    CORBA::Object_ptr obj,
                    CosNaming::BindingType type);
  // Bind a new name to a naming context

  virtual int rebind (const char *id,
                      const char *kind,
                      CORBA::Object_ptr obj,
                      CosNaming::BindingType type);
  // Overwrite the value or type of an existing name in a
  // ACE_Local_Name_Space or bind a new name to the context, if it
  // didn't exist yet. (Wide charcter strings interface).

  virtual int unbind (const char * id,
                      const char * kind);

  virtual int find (const char * id,
                    const char * kind,
                    CORBA::Object_ptr & obj,
                    CosNaming::BindingType &type);

  //A method to get the iterator.

  HASH_MAP *map (void);
  // accessor

private:

  int shared_bind (const char *id,
                   const char *kind,
                   CORBA::Object_ptr obj,
                   CosNaming::BindingType type,
                   int rebind);
  // Factor out code from <bind> and <rebind>.

  ACE_Allocator *allocator_;
  // Pointer to the allocator

  HASH_MAP *map_;
  // Pointer to the allocated map manager.

  CORBA::ORB_var orb_;
  // a pointer to the orb so that we can do string/object conversions.
};

#endif /* TAO_PERSISTENT_BINDINGS_MAP */
