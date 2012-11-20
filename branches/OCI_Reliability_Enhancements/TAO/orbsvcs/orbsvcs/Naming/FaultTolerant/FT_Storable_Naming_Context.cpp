// $Id$

#include "orbsvcs/Naming/FaultTolerant/FT_Storable_Naming_Context.h"
#include "orbsvcs/Naming/FaultTolerant/FT_Naming_Manager.h"
#include "orbsvcs/FT_NamingManagerC.h"
#include "orbsvcs/PortableGroup/PG_Utils.h"
#include "orbsvcs/PortableGroup/PG_Property_Utils.h"

#include "orbsvcs/Naming/Persistent_Context_Index.h"
#include "ace/OS_NS_stdio.h"

#include "ace/Auto_Ptr.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Initialize the static naming manager
TAO_FT_Naming_Manager *TAO_FT_Storable_Naming_Context::naming_manager_ = 0;

TAO_FT_Storable_Naming_Context::TAO_FT_Storable_Naming_Context (CORBA::ORB_ptr orb,
                               PortableServer::POA_ptr poa,
                               const char *poa_id,
                               TAO_Naming_Service_Persistence_Factory *factory,
                               const ACE_TCHAR *persistence_directory,
                               size_t hash_table_size)
  : TAO_Storable_Naming_Context (orb, 
                                 poa,
                                 poa_id,
                                 factory,
                                 persistence_directory,
                                 hash_table_size)
{

}


TAO_FT_Storable_Naming_Context::~TAO_FT_Storable_Naming_Context (void)
{
  // Perform appropriate cleanup based on the destruction level specified.
}


CORBA::Boolean 
TAO_FT_Storable_Naming_Context::is_object_group (CORBA::Object_ptr obj) const
{
  // If there is a tagged component with tag = IOP::TAG_FT_GROUP in the object reference
  // then it is an object group
  PortableGroup::TagGroupTaggedComponent tagged_component;
  return TAO::PG_Utils::get_tagged_component (obj, tagged_component);
}

CORBA::Object_ptr
TAO_FT_Storable_Naming_Context::resolve (const CosNaming::Name& n)
{
  // Invoke the base class resolve operation to acquire the object at the specified
  // compound name.  Any exceptions should flow back to client.
  CORBA::Object_var resolved_ref =
    TAO_Storable_Naming_Context::resolve(n);

  ACE_GUARD_THROW_EX (TAO_SYNCH_RECURSIVE_MUTEX, ace_mon, this->lock_,
    CORBA::INTERNAL ());

  // Get the locations of the object group members and we will use them to 
  // do the load balancing
  try {

    // Make sure object is an object group.  
    // We will return the object reference all the way back out to the client if not
    if (!this->is_object_group (resolved_ref.in ()))
      return resolved_ref._retn ();

    // If there is no naming manager, we will fail and report an error.
    if ( this->naming_manager_ == 0)
    {
      ACE_ERROR ((LM_ERROR, 
        "TAO_FT_Persistent_Naming_Context::resolve - No NamingManager defined.\n"));

      throw CORBA::INTERNAL ();
    }
    
    // Get the next location selected by the associated strategy 
    PortableGroup::Location next_location;
    if (this->naming_manager_->next_location (resolved_ref.in(), next_location))
    { // Found the location
      // Access the object from the naming service manager by passing in 
      // the next_location value and assign it to the resolved_ref
      resolved_ref = 
        this->naming_manager_->get_member_ref (resolved_ref.in (), next_location);
    }
    else
    { // No locations defined for the object group, so we will return a null object reference
      return CORBA::Object::_nil ();
    }

  }
  catch (const PortableGroup::ObjectGroupNotFound&)
  {
    // This is apparently not an object group, so we should return the 
    // object reference itself
    // No action required
  }
  catch (CORBA::Exception& ex)
  {
     ex._tao_print_exception (
        "TAO_FT_Storable_Naming_Context::resolve - Some unhandled error occurred\n");
      return CORBA::Object::_nil ();
  }

  return resolved_ref._retn ();
}

void 
TAO_FT_Storable_Naming_Context::set_naming_manager (TAO_FT_Naming_Manager *mgr_impl)
{
  naming_manager_ = mgr_impl;
}


CosNaming::NamingContext_ptr
TAO_FT_Storable_Naming_Context::make_new_context (
                              CORBA::ORB_ptr orb,
                              PortableServer::POA_ptr poa,
                              const char *poa_id,
                              size_t context_size,
                              TAO_Naming_Service_Persistence_Factory *factory,
                              const ACE_TCHAR *persistence_directory,
                              TAO_FT_Storable_Naming_Context **new_context)
{
  ACE_TRACE("make_new_context");
  // Store the stub we will return here.
  CosNaming::NamingContext_var result;

  // Put together a servant for the new Naming Context.

  TAO_FT_Storable_Naming_Context *context_impl = 0;
  ACE_NEW_THROW_EX (context_impl,
                    TAO_FT_Storable_Naming_Context (orb,
                                                 poa,
                                                 poa_id,
                                                 factory,
                                                 persistence_directory,
                                                 context_size),
                                                 CORBA::NO_MEMORY ());

  // Put <context_impl> into the auto pointer temporarily, in case next
  // allocation fails.
  ACE_Auto_Basic_Ptr<TAO_FT_Storable_Naming_Context> temp (context_impl);

  TAO_Naming_Context *context = 0;
  ACE_NEW_THROW_EX (context,
                    TAO_Naming_Context (context_impl),
                    CORBA::NO_MEMORY ());

  // Let <implementation> know about it's <interface>.
  context_impl->interface (context);

  // Release auto pointer, and start using reference counting to
  // control our servant.
  temp.release ();
  PortableServer::ServantBase_var s = context;

  // Register the new context with the POA.
  PortableServer::ObjectId_var id =
    PortableServer::string_to_ObjectId (poa_id);

  // If we try to register a naming context that is already registered,
  // the following activation causes a POA::ObjectAlreadyActive exception be
  // thrown which is transmitted as a CORBA::UNKNOWN on the wire. To rectify
  // this problem, we explicitly throw the correct INS exception in
  // this situation.
  try
    {
      poa->activate_object_with_id (id.in (), context);
    }
  catch (const PortableServer::POA::ObjectAlreadyActive&)
    {
      throw CosNaming::NamingContext::AlreadyBound();
    }


  result = context->_this ();

  // return the address of the new context object so that caller can finish
  *new_context = context_impl;

  return result._retn ();
}


CosNaming::NamingContext_ptr 
TAO_FT_Storable_Naming_Context::recreate_all(
                               CORBA::ORB_ptr orb,
                               PortableServer::POA_ptr poa,
                               const char *poa_id,
                               size_t context_size,
                               int reentering,
                               TAO_Naming_Service_Persistence_Factory *factory,
                               const ACE_TCHAR *persistence_directory,
                               int use_redundancy)
{
  ACE_TRACE("recreate_all");

  ACE_UNUSED_ARG (reentering);

  // Whether we are redundant is global
  redundant_ = use_redundancy;

  // Save the root name for later use
  root_name_ = poa_id;

  // Create a new context.
  TAO_FT_Storable_Naming_Context *new_context = 0;
  CosNaming::NamingContext_var result =
    make_new_context (orb,
                      poa,
                      poa_id,
                      context_size,
                      factory,
                      persistence_directory,
                      &new_context);

  // Now does this already exist on disk?
  ACE_TString file_name(persistence_directory);
  file_name += ACE_TEXT("/");
  file_name += ACE_TEXT_CHAR_TO_TCHAR(poa_id);
  ACE_Auto_Ptr<TAO_Storable_Base> fl (factory->create_stream(ACE_TEXT_ALWAYS_CHAR(file_name.c_str()), ACE_TEXT("r")));
  if (fl->exists())
  {
    // Load the map from disk
    File_Open_Lock_and_Check flck (new_context, "r");
  }
  else
  {
    // Since this is a new context, make and empty map in it
    ACE_NEW_THROW_EX (new_context->storable_context_,
                      TAO_Storable_Bindings_Map (context_size,orb),
                      CORBA::NO_MEMORY ());
    new_context->context_ = new_context->storable_context_;
    File_Open_Lock_and_Check flck (new_context, "wc");
    new_context->Write (flck.peer ());
  }

  // build the global file name
  file_name += ACE_TEXT ("_global");

  // Create the stream for the counter used to uniquely creat context names
  gfl_.reset(factory->create_stream (ACE_TEXT_ALWAYS_CHAR(file_name.c_str ()), ACE_TEXT ("crw")));
  if (gfl_->open () != 0)
    {
      delete gfl_.release ();
      throw CORBA::PERSIST_STORE ();
    }

  // get the counter from disk
  TAO_NS_Persistence_Global global;
  *gfl_.get() >> global;
  if (!gfl_.get ()->good () &&
      gfl_.get ()->rdstate () != TAO_Storable_Base::eofbit)
    {
      gfl_.get ()->clear ();
      throw CORBA::INTERNAL ();
    }
  gcounter_ = global.counter ();
  if(redundant_) gfl_->close ();

  return result._retn ();
}


TAO_END_VERSIONED_NAMESPACE_DECL
