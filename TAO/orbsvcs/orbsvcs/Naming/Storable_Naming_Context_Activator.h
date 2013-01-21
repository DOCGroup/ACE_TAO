// -*- C++ -*-


//=============================================================================
/**
 *  @file   Storable_Naming_Context_Activator.h
 *
 *  $Id$
 *
 *  @author Byron Harris <harris_b@ociweb.com>
 */
//=============================================================================


#ifndef TAO_STORABLE_NAMING_CONTEXT_ACTIVATOR_H
#define TAO_STORABLE_NAMING_CONTEXT_ACTIVATOR_H
#include /**/ "ace/pre.h"

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/ServantActivatorC.h"
#include "tao/LocalObject.h"

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)

#include "orbsvcs/Naming/naming_serv_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  class Storable_Factory;
}

class TAO_Storable_Naming_Context_Factory;

/**
 * A servant activator to be use with a TAO_Storable_Naming_Context.
 * Permits contexts saved to disk in one naming service process to be
 * "lazily" created on demand in other processes by reading the
 * context file off disk only when a request to the context is made.
 */
class TAO_Naming_Serv_Export TAO_Storable_Naming_Context_Activator :
  public virtual PortableServer::ServantActivator
{
public:

  /**
   * The constructor takes arguments needed to create a
   * TAO_Storable_Naming_Context and TAO_Naming_Context on demand.
   */
  TAO_Storable_Naming_Context_Activator (
    CORBA::ORB_ptr orb,
    TAO::Storable_Factory *factory,
    TAO_Storable_Naming_Context_Factory *context_impl_factory,
    const ACE_TCHAR *persistence_directory);

  virtual ~TAO_Storable_Naming_Context_Activator();

  /**
   * Create a TAO_Storable_Naming_Context and TAO_Naming_Context on
   * demand if a request to a naming context CORBA reference is made
   * and the servant implementing this reference does not yet exist.
   */
  virtual PortableServer::Servant incarnate (const PortableServer::ObjectId &oid,
                                             PortableServer::POA_ptr poa);

  /**
   * Used by the POA to delete the servant created from a call to incarnate.
   */
  virtual void etherealize (const PortableServer::ObjectId &oid,
                            PortableServer::POA_ptr adapter,
                            PortableServer::Servant servant,
                            CORBA::Boolean cleanup_in_progress,
                            CORBA::Boolean remaining_activations);

private:

  CORBA::ORB_ptr orb_;

  /// The factory for constructing the persistence mechanism for the contexts
  TAO::Storable_Factory *persistence_factory_;

  /// The factory for constructing naming contexts within the index
  TAO_Storable_Naming_Context_Factory *context_impl_factory_;

  const ACE_TCHAR *persistence_directory_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MINIMUM_POA */

#include /**/ "ace/post.h"
#endif
