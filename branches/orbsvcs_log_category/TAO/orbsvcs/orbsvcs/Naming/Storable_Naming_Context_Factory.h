// -*- C++ -*-

//=============================================================================
/**
 *  @file   Storable_Naming_Context_Factory.h
 *
 *  $Id$
 *
 *  @author Kevin Stanley stanleyk@ociweb.com>
 */
//=============================================================================


#ifndef TAO_STORABLE_NAMING_CONTEXT_FACTORY_H
#define TAO_STORABLE_NAMING_CONTEXT_FACTORY_H
#include /**/ "ace/pre.h"

#include "tao/ORB.h"
#include "orbsvcs/Naming/nsconf.h"
#include "orbsvcs/Naming/naming_serv_export.h"
#include "orbsvcs/Naming/Hash_Naming_Context.h"
#include "orbsvcs/Naming/Storable_Naming_Context.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  class Storable_Factory;
}


/**
 * @class TAO_Storable_Naming_Context_Factory
 *
 * @brief
 */
class TAO_Naming_Serv_Export TAO_Storable_Naming_Context_Factory
{
public:

  /// Data structure used by TAO_Persistent_Context_Index
  /// - typedef for ease of use.
  typedef TAO_Storable_Naming_Context::HASH_MAP HASH_MAP;

  // = Initialization and termination methods.

  /// Constructor.
  TAO_Storable_Naming_Context_Factory (
    size_t hash_table_size = ACE_DEFAULT_MAP_SIZE);

  /// Destructor.  Does not deallocate the hash map: if an instance of
  /// this class goes out of scope, its hash_map remains in persistent storage.
  virtual ~TAO_Storable_Naming_Context_Factory (void);

  /// Factory method for creating an implementation object for naming contexts.
  /// If an existing naming context implementation is being rebuilt, the map
  /// and counter parameters should be provided to the underlying HASH_MAP
  /// implementation
  virtual TAO_Storable_Naming_Context* create_naming_context_impl (
    CORBA::ORB_ptr orb,
    PortableServer::POA_ptr poa,
    const char *poa_id,
    TAO::Storable_Factory *factory);

protected:
  /// The size for persisted naming context objects in hash map
  size_t context_size_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_STORABLE_NAMING_CONTEXT_FACTORY_H */
