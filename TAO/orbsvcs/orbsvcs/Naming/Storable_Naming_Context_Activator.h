/* -*- C++ -*- */
// $Id$
// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//   Storable_Naming_Context_Activator.h
//
// = AUTHOR
//    Byron Harris <harris_b@ociweb.com>
//
// ============================================================================

#ifndef TAO_STORABLE_NAMING_CONTEXT_ACTIVATOR_H
#define TAO_STORABLE_NAMING_CONTEXT_ACTIVATOR_H
#include "ace/pre.h"

#include "tao/PortableServer/PortableServer.h"

#if (TAO_HAS_MINIMUM_POA == 0)

#include "naming_export.h"

class TAO_Naming_Service_Persistence_Factory;

/**
 * A servant activator to be use with a TAO_Storable_Naming_Context.
 * Permits contexts saved to disk in one naming service process to be
 * "lazily" created on demand in other processes by reading the
 * context file off disk only when a request to the context is made.
 */
class TAO_Naming_Export TAO_Storable_Naming_Context_Activator :
  public PortableServer::ServantActivator
{
public:

  /**
   * The constructor takes arguments needed to create a
   * TAO_Storable_Naming_Context and TAO_Naming_Context on demand.
   */
  TAO_Storable_Naming_Context_Activator(CORBA::ORB_ptr orb,
                                        TAO_Naming_Service_Persistence_Factory *factory,
                                        const ACE_TCHAR *persistence_directory,
                                        size_t context_size);

  virtual ~TAO_Storable_Naming_Context_Activator();

  /**
   * Create a TAO_Storable_Naming_Context and TAO_Naming_Context on
   * demand if a request to a naming context CORBA reference is made
   * and the servant implementing this reference does not yet exist.
   */
  virtual PortableServer::Servant incarnate (const PortableServer::ObjectId &oid,
                                             PortableServer::POA_ptr poa
                                             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::ForwardRequest));

  /**
   * Used by the POA to delete the servant created from a call to incarnate.
   */
  virtual void etherealize (const PortableServer::ObjectId &oid,
                            PortableServer::POA_ptr adapter,
                            PortableServer::Servant servant,
                            CORBA::Boolean cleanup_in_progress,
                            CORBA::Boolean remaining_activations
                            ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  CORBA::ORB_ptr orb_;
  TAO_Naming_Service_Persistence_Factory *factory_;
  const ACE_TCHAR *persistence_directory_;
  size_t context_size_;
};

#endif /* TAO_HAS_MINIMUM_POA */

#include "ace/post.h"
#endif
