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

#include "Storable_Naming_Context_Activator.h"

#if (TAO_HAS_MINIMUM_POA == 0)
#include "Naming_Context_Interface.h"
#include "Storable_Naming_Context.h"
#include "Storable.h"
#include "ace/Auto_Ptr.h"

TAO_Storable_Naming_Context_Activator::
TAO_Storable_Naming_Context_Activator (CORBA::ORB_ptr orb,
                                       TAO_Naming_Service_Persistence_Factory *factory,
                                       const ACE_TCHAR *persistence_directory,
                                       size_t context_size)
  : orb_(orb),
    factory_(factory),
    persistence_directory_(persistence_directory),
    context_size_(context_size)
{
}

TAO_Storable_Naming_Context_Activator::~TAO_Storable_Naming_Context_Activator ()
{
}

PortableServer::Servant
TAO_Storable_Naming_Context_Activator::incarnate (const PortableServer::ObjectId &oid,
                                                  PortableServer::POA_ptr poa
                                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::ForwardRequest))
{

  // Make sure complete initialization has been done
  ACE_ASSERT (factory_ != 0);

  CORBA::String_var poa_id = PortableServer::ObjectId_to_string (oid);

  // The approached used is to simply verify that there is a
  // persistence element that exists that corresponds to the
  // poa_id. If so, an empty context is created. Later, when the
  // context is accessed it will be determined that the contents of
  // the persistence elment needs to be read in.

  // Does this already exist on disk?
  ACE_CString file_name(persistence_directory_);
  file_name += "/";
  file_name += poa_id.in();
  TAO_Storable_Base * fl = factory_->create_stream(file_name, "rw");
  if (!fl->exists()) {
    ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                      0);
  }

  // Store the stub we will return here.
  CosNaming::NamingContext_var result (CosNaming::NamingContext::_nil());

  // Put together a servant for the new Naming Context.

  TAO_Storable_Naming_Context *context_impl = 0;
  ACE_NEW_THROW_EX (context_impl,
                    TAO_Storable_Naming_Context (orb_,
                                                 poa,
                                                 poa_id.in (),
                                                 factory_,
                                                 persistence_directory_,
                                                 context_size_),
                                                 CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  // Put <context_impl> into the auto pointer temporarily, in case next
  // allocation fails.
  ACE_Auto_Basic_Ptr<TAO_Storable_Naming_Context> temp (context_impl);

  TAO_Naming_Context *context = 0;
  ACE_NEW_THROW_EX (context,
                    TAO_Naming_Context (context_impl),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  // Let <implementation> know about it's <interface>.
  context_impl->interface (context);

  // Release auto pointer, and start using reference counting to
  // control our servant.
  temp.release ();

  return context;
}

void
TAO_Storable_Naming_Context_Activator::etherealize (const PortableServer::ObjectId &/*oid*/,
                                                    PortableServer::POA_ptr /*adapter*/,
                                                    PortableServer::Servant servant,
                                                    CORBA::Boolean /*cleanup_in_progress*/,
                                                    CORBA::Boolean remaining_activations
                                                    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (!remaining_activations) {
    delete servant;
  }
}

#endif /* TAO_HAS_MINIMUM_POA */
