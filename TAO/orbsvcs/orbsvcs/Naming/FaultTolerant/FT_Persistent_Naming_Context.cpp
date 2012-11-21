// $Id$

#include "orbsvcs/Naming/FaultTolerant/FT_Persistent_Naming_Context.h"
#include "orbsvcs/Naming/FaultTolerant/FT_Naming_Manager.h"
#include "orbsvcs/FT_NamingManagerC.h"
#include "orbsvcs/PortableGroup/PG_Utils.h"
#include "orbsvcs/PortableGroup/PG_Property_Utils.h"

#include "orbsvcs/Naming/Persistent_Context_Index.h"
#include "ace/OS_NS_stdio.h"

#include "ace/Auto_Ptr.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Initialize the static naming manager
TAO_FT_Naming_Manager *TAO_FT_Persistent_Naming_Context::naming_manager_impl_ = 0;

TAO_FT_Persistent_Naming_Context::TAO_FT_Persistent_Naming_Context (PortableServer::POA_ptr poa,
                                                                    const char *poa_id,
                                                                    TAO_Persistent_Context_Index *context_index,
                                                                    HASH_MAP *map,
                                                                    ACE_UINT32 *counter)
  : TAO_Persistent_Naming_Context (poa,
                                   poa_id,
                                   context_index,
                                   map,
                                   counter)
{

}


TAO_FT_Persistent_Naming_Context::~TAO_FT_Persistent_Naming_Context (void)
{
  // Perform appropriate cleanup based on the destruction level specified.
}


CORBA::Boolean
TAO_FT_Persistent_Naming_Context::is_object_group (CORBA::Object_ptr obj) const
{
  // If there is a tagged component with tag = IOP::TAG_FT_GROUP in the object reference
  // then it is an object group
  PortableGroup::TagGroupTaggedComponent tagged_component;
  return TAO::PG_Utils::get_tagged_component (obj, tagged_component);
}

CORBA::Object_ptr
TAO_FT_Persistent_Naming_Context::resolve (const CosNaming::Name& n)
{
  // Invoke the base class resolve operation to acquire the object at the specified
  // compound name.  Any exceptions should flow back to client.
  CORBA::Object_var resolved_ref =
    TAO_Persistent_Naming_Context::resolve(n);

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
    if ( this->naming_manager_impl_ == 0)
    {
      ACE_ERROR ((LM_ERROR,
        "TAO_FT_Persistent_Naming_Context::resolve - No NamingManager defined.\n"));

      throw CORBA::INTERNAL ();
    }

    // Get the next location selected by the associated strategy
    PortableGroup::Location next_location;
    if (this->naming_manager_impl_->next_location (resolved_ref.in(), next_location))
    { // Found the location
      // Access the object from the naming service manager by passing in
      // the next_location value and assign it to the resolved_ref
      resolved_ref =
        this->naming_manager_impl_->get_member_ref (resolved_ref.in (), next_location);
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
        "TAO_FT_Persistent_Naming_Context::resolve - Some unhandled error occurred\n");
      return CORBA::Object::_nil ();
  }

  return resolved_ref._retn ();
}

void
TAO_FT_Persistent_Naming_Context::set_naming_manager_impl (TAO_FT_Naming_Manager *mgr_impl)
{
  naming_manager_impl_ = (mgr_impl);
}


TAO_END_VERSIONED_NAMESPACE_DECL
