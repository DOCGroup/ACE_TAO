// -*- C++ -*-

//=============================================================================
/**
 * @file   FT_Storable_Naming_Context_Factory.cpp
 *
 * @author Kevin Stanley <stanleyk@ociweb.com>
 */
//=============================================================================

#include "orbsvcs/Naming/FaultTolerant/FT_Storable_Naming_Context_Factory.h"
#include "orbsvcs/Naming/FaultTolerant/FT_Storable_Naming_Context.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Constructor.
TAO_FT_Storable_Naming_Context_Factory::
TAO_FT_Storable_Naming_Context_Factory (size_t hash_table_size,
                                        TAO_FT_Naming_Replication_Manager *rep)
  : TAO_Storable_Naming_Context_Factory (hash_table_size),
    replicator_ (rep)
{
}

TAO_FT_Naming_Replication_Manager *
TAO_FT_Storable_Naming_Context_Factory::replicator ()
{
  return this->replicator_;
}

TAO_Storable_Naming_Context*
TAO_FT_Storable_Naming_Context_Factory::create_naming_context_impl (
  CORBA::ORB_ptr orb,
  PortableServer::POA_ptr poa,
  const char *poa_id,
  TAO::Storable_Factory *persistence_factory
                                                                    )
{
  // Construct the naming context, forwarding the map and counter
  // even if they are defaulted
  TAO_FT_Storable_Naming_Context *context_impl;
  ACE_NEW_THROW_EX (context_impl,
                    TAO_FT_Storable_Naming_Context (orb,
                                                    poa,
                                                    poa_id,
                                                    this,
                                                    persistence_factory),
                    CORBA::NO_MEMORY ());

  return context_impl;
}


TAO_END_VERSIONED_NAMESPACE_DECL
