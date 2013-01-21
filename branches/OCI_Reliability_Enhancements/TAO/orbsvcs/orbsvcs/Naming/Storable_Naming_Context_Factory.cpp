// $Id$

#include "orbsvcs/Naming/Storable_Naming_Context_Factory.h"
#include "orbsvcs/Naming/Storable_Naming_Context.h"

#include "tao/Storable_Factory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Constructor.
TAO_Storable_Naming_Context_Factory::TAO_Storable_Naming_Context_Factory (
  size_t hash_table_size)
: context_size_(hash_table_size)
{

}

/// Destructor.  Does not deallocate the hash map: if an instance of
/// this class goes out of scope, its hash_map remains in persistent storage.
TAO_Storable_Naming_Context_Factory::~TAO_Storable_Naming_Context_Factory (void)
{

}


TAO_Storable_Naming_Context*
TAO_Storable_Naming_Context_Factory::create_naming_context_impl (
  CORBA::ORB_ptr orb,
  PortableServer::POA_ptr poa,
  const char *poa_id,
  TAO::Storable_Factory *persistence_factory)
{
  // Construct the naming context, forwarding the map and counter even if they
  // are defaulted
  TAO_Storable_Naming_Context *context_impl;
  ACE_NEW_THROW_EX (context_impl,
                    TAO_Storable_Naming_Context (orb,
                                                 poa,
                                                 poa_id,
                                                 this,
                                                 persistence_factory,
                                                 this->context_size_),
                    CORBA::NO_MEMORY ());

  return context_impl;
}


TAO_END_VERSIONED_NAMESPACE_DECL
