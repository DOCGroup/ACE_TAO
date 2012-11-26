// $Id$

#include "orbsvcs/Naming/Naming_Context_Factory.h"
#include "orbsvcs/Naming/Persistent_Naming_Context.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

  /// Constructor.
TAO_Naming_Context_Factory::TAO_Naming_Context_Factory (size_t context_size)
: context_size_(context_size)
{

}

  /// Destructor.  Does not deallocate the hash map: if an instance of
  /// this class goes out of scope, its hash_map remains in persistent storage.
TAO_Naming_Context_Factory::~TAO_Naming_Context_Factory (void)
{
}


TAO_END_VERSIONED_NAMESPACE_DECL
