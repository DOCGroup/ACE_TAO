// -*- C++ -*-

//=============================================================================
/**
 *  @file   FT_Persistent_Naming_Context_Factory.h
 *
 *  @author Kevin Stanley stanleyk@ociweb.com>
 */
//=============================================================================


#ifndef TAO_FT_PERSISTENT_NAMING_CONTEXT_FACTORY_H
#define TAO_FT_PERSISTENT_NAMING_CONTEXT_FACTORY_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Naming/Persistent_Naming_Context_Factory.h"
#include "orbsvcs/Naming/FaultTolerant/ftnaming_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_FT_Persistent_Naming_Context_Factory
 *
 * @brief An implementation of the TAO_Naming_Context_Factory that creates
 * TAO_FT_Persistent_Naming_Context to implement the COS Naming Service
 * NamingContext interface.
 */
class TAO_FtNaming_Export TAO_FT_Persistent_Naming_Context_Factory
  : public TAO_Persistent_Naming_Context_Factory
{
public:
  /// Constructor.
  TAO_FT_Persistent_Naming_Context_Factory () = default;

  /// Destructor.  Does not deallocate the hash map: if an instance of
  /// this class goes out of scope, its hash_map remains in persistent storage.
  ~TAO_FT_Persistent_Naming_Context_Factory () override = default;

  /// Factory method for creating an implementation object for naming contexts
  TAO_Persistent_Naming_Context* create_naming_context_impl (
    PortableServer::POA_ptr poa,
    const char *poa_id,
    TAO_Persistent_Context_Index *context_index,
    HASH_MAP * map = 0,
    ACE_UINT32 *counter = 0) override;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_FT_PERSISTENT_NAMING_CONTEXT_FACTORY_H */
