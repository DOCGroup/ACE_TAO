// $Id$

#include "orbsvcs/Naming/FaultTolerant/FT_Storable_Naming_Context.h"
#include "orbsvcs/Naming/FaultTolerant/FT_Naming_Manager.h"
#include "orbsvcs/Naming/FaultTolerant/FT_Naming_Replication_Manager.h"
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
                               TAO_Storable_Naming_Context_Factory *cxt_factory,
                               TAO_Naming_Service_Persistence_Factory *pers_factory,
                               const ACE_TCHAR *persistence_directory)
  : TAO_Storable_Naming_Context (orb,
                                 poa,
                                 poa_id,
                                 cxt_factory,
                                 pers_factory,
                                 persistence_directory),
    is_dirty_ (0)
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
TAO_FT_Storable_Naming_Context::bind (const CosNaming::Name& n,
                                      CORBA::Object_ptr obj)
{
  // Invoke the parent class bind operation. This will bind
  // the object to the name and store the naming context.
  TAO_Storable_Naming_Context::bind (n, obj);

  try {
    FT_Naming::ReplicationManager_var peer =
      TAO_FT_Naming_Replication_Manager::peer_replica ();

    if (CORBA::is_nil (peer.in ()))
      {
        // Replication is not supported without a peer replica.
        return;
      }

    FT_Naming::NamingContextUpdate context_info;
    context_info.context_name = this->name_.c_str ();
    // We are are updating the context one element before the specified name
    context_info.changed_context = n;
    context_info.changed_context.length (n.length () - 1);
    context_info.change_type = FT_Naming::UPDATED;

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Forwarding notification of bind update for name: %s.\n"),
                this->interface ()->to_string (n)));

    // Notify the naming_manager of the updated context
    peer->notify_updated_context (context_info);
  }
  catch (CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Unable to communicate with peer.\n"));
    }
}

CosNaming::NamingContext_ptr
TAO_FT_Storable_Naming_Context::bind_new_context (const CosNaming::Name& n)
{
  ACE_TRACE("FT>bind_new_context");
  CosNaming::NamingContext_ptr nc =
    TAO_Storable_Naming_Context::bind_new_context (n);

  ACE_DEBUG ((LM_DEBUG,
              "Binding new context (name =%s) [%i].\n", n[0].id.in (), n.length ()));
  try {
    FT_Naming::ReplicationManager_var peer =
      TAO_FT_Naming_Replication_Manager::peer_replica ();

    if (CORBA::is_nil (peer.in ()))
      {
        // Replication is not supported without a peer replica.
        return nc;
      }

    TAO_FT_Storable_Naming_Context *updated_context = this->find_relative_context (n);

    if (updated_context != 0)
    {
      // Try to propagate the update to the impacted context. Return of 0 means success
      // return of 1 means no replica.  Return of -1 means exception in propagation
      int result = updated_context->propagate_update_notification (FT_Naming::UPDATED);
      if (result == -1)
        ACE_ERROR ((LM_ERROR,
                   "TAO_FT_Storable_Naming_Context::bind_new_context - unable to propagate updates.\n"));
    }
  }
  catch (CORBA::Exception& ex)
  {
    ex._tao_print_exception ("TAO_FT_Storable_Naming_Context::bind_new_context - Unexpected exception.\n");
  }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("bind_new_context - Returning nc.\n")));
  return nc;
}

TAO_FT_Storable_Naming_Context*
TAO_FT_Storable_Naming_Context::find_relative_context (
                              const CosNaming::Name &n)
{
  // TODO: We have to resolve to the identified name and then
  // Get the servant for that context and access its impl.
  return 0;
}

void
TAO_FT_Storable_Naming_Context::set_naming_manager (TAO_FT_Naming_Manager *mgr_impl)
{
  naming_manager_ = mgr_impl;
}

void
TAO_FT_Storable_Naming_Context::mark_dirty (CORBA::Boolean is_dirty)
{
  this->is_dirty_ = is_dirty;
}

int
TAO_FT_Storable_Naming_Context::propagate_update_notification (FT_Naming::ChangeType change_type)
{
  try {
    FT_Naming::ReplicationManager_var peer =
      TAO_FT_Naming_Replication_Manager::peer_replica ();

    if (CORBA::is_nil (peer.in ()))
      {// Replication is not supported without a peer replica.
        return 1;
      }

    // Build up the context information about the update
    FT_Naming::NamingContextUpdate context_info;
    context_info.context_name = this->name_.c_str ();
    context_info.change_type = change_type;

    // Notify the naming_manager of the updated context
    peer->notify_updated_context (context_info);
  }
  catch (CORBA::Exception& ex)
  {
    ex._tao_print_exception (
      ACE_TEXT ("TAO_FT_Storable_Naming_Context::report_updated - Unexpected exception.\n"));
    return -1;
  }

  // Success
  return 0;
}

CORBA::Boolean
TAO_FT_Storable_Naming_Context::is_dirty (void)
{
  return is_dirty_;
}


TAO_END_VERSIONED_NAMESPACE_DECL
