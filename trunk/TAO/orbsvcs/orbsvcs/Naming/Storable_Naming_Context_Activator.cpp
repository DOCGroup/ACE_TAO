//=============================================================================
/**
 *  @file   Storable_Naming_Context_Activator.cpp
 *
 *  $Id$
 *
 *  @author Byron Harris <harris_b@ociweb.com>
 */
//=============================================================================


#include "orbsvcs/Naming/Storable_Naming_Context_Activator.h"

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT) && !defined (CORBA_E_MICRO)
#include "orbsvcs/Naming/Naming_Context_Interface.h"
#include "orbsvcs/Naming/Storable_Naming_Context.h"
#include "orbsvcs/Naming/Storable_Naming_Context_Factory.h"
#include "orbsvcs/Naming/Storable.h"
#include "tao/Storable_Factory.h"
#include "ace/Auto_Ptr.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Storable_Naming_Context_Activator::TAO_Storable_Naming_Context_Activator (
  CORBA::ORB_ptr orb,
  TAO::Storable_Factory *persistence_factory,
  TAO_Storable_Naming_Context_Factory *context_impl_factory,
  const ACE_TCHAR *persistence_directory)
  : orb_(orb),
    persistence_factory_(persistence_factory),
    context_impl_factory_(context_impl_factory),
    persistence_directory_(persistence_directory)
{
}

TAO_Storable_Naming_Context_Activator::~TAO_Storable_Naming_Context_Activator ()
{
  delete persistence_factory_;
  delete this->context_impl_factory_;
}

PortableServer::Servant
TAO_Storable_Naming_Context_Activator::incarnate (
    const PortableServer::ObjectId &oid,
    PortableServer::POA_ptr poa)
{

  // Make sure complete initialization has been done
  ACE_ASSERT (persistence_factory_ != 0);

  // Make sure complete initialization has been done
  ACE_ASSERT (context_impl_factory_ != 0);

  CORBA::String_var poa_id = PortableServer::ObjectId_to_string (oid);

  // The approached used is to simply verify that there is a
  // persistence element that exists that corresponds to the
  // poa_id. If so, an empty context is created. Later, when the
  // context is accessed it will be determined that the contents of
  // the persistence elment needs to be read in.

  { // Does this already exist on disk?

    ACE_TString file_name = ACE_TEXT_CHAR_TO_TCHAR (poa_id.in ());
    ACE_Auto_Ptr<TAO::Storable_Base> fl (
       persistence_factory_->create_stream (ACE_TEXT_ALWAYS_CHAR (file_name.c_str ()),
                                            ACE_TEXT ("rw")));

    if (!fl->exists()) {
      throw CORBA::OBJECT_NOT_EXIST ();
    }
  }

  // Put together a servant for the new Naming Context.
  // Will throw NO_MEMORY exception if unable to construct one
  TAO_Storable_Naming_Context *context_impl =
    this->context_impl_factory_->create_naming_context_impl (orb_,
                                                             poa,
                                                             poa_id.in (),
                                                             persistence_factory_);

  // Put <context_impl> into the auto pointer temporarily, in case next
  // allocation fails.
  ACE_Auto_Basic_Ptr<TAO_Storable_Naming_Context> temp (context_impl);

  TAO_Naming_Context *context = 0;
  ACE_NEW_THROW_EX (context,
                    TAO_Naming_Context (context_impl),
                    CORBA::NO_MEMORY ());

  // Let <implementation> know about it's <interface>.
  context_impl->interface (context);

  // Release auto pointer, and start using reference counting to
  // control our servant.
  temp.release ();

  return context;
}

void
TAO_Storable_Naming_Context_Activator::etherealize (
    const PortableServer::ObjectId &/*oid*/,
    PortableServer::POA_ptr /*adapter*/,
    PortableServer::Servant servant,
    CORBA::Boolean /*cleanup_in_progress*/,
    CORBA::Boolean remaining_activations)
{
  if (!remaining_activations)
    {
      servant->_remove_ref ();
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MINIMUM_POA */
