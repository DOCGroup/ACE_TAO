#include "orbsvcs/Naming/Persistent_Naming_Context_Factory.h"
#include "orbsvcs/Naming/Persistent_Naming_Context.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Constructor.
TAO_Persistent_Naming_Context_Factory::TAO_Persistent_Naming_Context_Factory ()
{
}

/// Destructor.  Does not deallocate the hash map: if an instance of
/// this class goes out of scope, its hash_map remains in persistent storage.
TAO_Persistent_Naming_Context_Factory::~TAO_Persistent_Naming_Context_Factory ()
{
}


/// Factory method for creating an implementation object for naming contexts
TAO_Persistent_Naming_Context*
TAO_Persistent_Naming_Context_Factory::create_naming_context_impl (
       PortableServer::POA_ptr poa,
       const char *poa_id,
       TAO_Persistent_Context_Index *context_index,
       HASH_MAP * map,
       ACE_UINT32 *counter)
{
  // Construct the naming context, forwarding the map and counter even if they
  // are defaulted
  return new (std::nothrow) TAO_Persistent_Naming_Context (poa,
                                                          poa_id,
                                                          context_index,
                                                          map,
                                                          counter);
}

TAO_END_VERSIONED_NAMESPACE_DECL
