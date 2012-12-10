// -*- C++ -*-

//=============================================================================
/**
 *  @file   Persistent_Naming_Context_Factory.h
 *
 *  $Id$
 *
 *  @author Kevin Stanley stanleyk@ociweb.com>
 */
//=============================================================================


#ifndef TAO_PERSISTENT_NAMING_CONTEXT_FACTORY_H
#define TAO_PERSISTENT_NAMING_CONTEXT_FACTORY_H
#include /**/ "ace/pre.h"

#include "tao/ORB.h"
#include "orbsvcs/Naming/nsconf.h"
#include "orbsvcs/Naming/naming_serv_export.h"
#include "orbsvcs/Naming/Hash_Naming_Context.h"
#include "orbsvcs/Naming/Persistent_Entries.h"
#include "orbsvcs/Naming/Persistent_Naming_Context.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Persistent_Naming_Context;

/**
 * @class TAO_Persistent_Naming_Context_Factory
 *
 * @brief A factory that creates TAO_Persistent_Naming_Context
 * objects to implement the COS Naming Service NamingContext interface.
 */
class TAO_Naming_Serv_Export TAO_Persistent_Naming_Context_Factory
{
public:

  // = Initialization and termination methods.
  /// Data structure used by TAO_Persistent_Context_Index - typedef for ease of use.
  typedef TAO_Persistent_Naming_Context::HASH_MAP HASH_MAP;

  /// Constructor.
  TAO_Persistent_Naming_Context_Factory (void);

  /// Destructor.  Does not deallocate the hash map: if an instance of
  /// this class goes out of scope, its hash_map remains in persistent storage.
  virtual ~TAO_Persistent_Naming_Context_Factory (void);

  /// Factory method for creating an implementation object for naming contexts
  virtual TAO_Persistent_Naming_Context*
    create_naming_context_impl (PortableServer::POA_ptr poa,
                                const char *poa_id,
                                TAO_Persistent_Context_Index *context_index,
                                HASH_MAP * map = 0,
                                ACE_UINT32 *counter = 0);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_PERSISTENT_NAMING_CONTEXT_FACTORY_H */
