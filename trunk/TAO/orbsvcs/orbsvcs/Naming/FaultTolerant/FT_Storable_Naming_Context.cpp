// $Id$

#include "orbsvcs/Log_Macros.h"
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
                               TAO::Storable_Factory *pers_factory)
  : TAO_Storable_Naming_Context (orb,
                                 poa,
                                 poa_id,
                                 cxt_factory,
                                 pers_factory),
    stale_ (false)
{

}


TAO_FT_Storable_Naming_Context::~TAO_FT_Storable_Naming_Context (void)
{
  // Perform appropriate cleanup based on the destruction level specified.
}


CORBA::Boolean
TAO_FT_Storable_Naming_Context::is_object_group (CORBA::Object_ptr obj) const
{
  // Ensure the object is not nil first. If so, it cannot be an ObjectGroup.
  if (CORBA::is_nil (obj))
    return 0;

  // If there is a tagged component with tag = IOP::TAG_FT_GROUP in the
  // object reference then it is an object group
  PortableGroup::TagGroupTaggedComponent tagged_component;
  return TAO::PG_Utils::get_tagged_component (obj, tagged_component);
}

CORBA::Object_ptr
TAO_FT_Storable_Naming_Context::resolve (const CosNaming::Name& n)
{
  // Invoke the base class resolve operation to acquire the object at the
  // specified compound name.  Any exceptions should flow back to client.
  CORBA::Object_var resolved_ref =
    TAO_Storable_Naming_Context::resolve(n);

  ACE_GUARD_THROW_EX (TAO_SYNCH_RECURSIVE_MUTEX, ace_mon, this->lock_,
    CORBA::INTERNAL ());

  try {

    // Make sure object is an object group.
    // We will return the object reference as is all the way back
    // out to the client if not
    if (!this->is_object_group (resolved_ref.in ()))
      return resolved_ref._retn ();

    // If there is no naming manager, we will fail and report an error.
    if ( this->naming_manager_ == 0)
    {
      ORBSVCS_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO_FT_Storable_Naming_Context::resolve ")
                  ACE_TEXT ("- No NamingManager defined.\n")));

      throw CORBA::INTERNAL ();
    }

    // The Naming Manager will apply the appropriate strategy to get the
    // next object reference from the object group.
    resolved_ref = this->naming_manager_->next_member (resolved_ref.in ());
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
       ACE_TEXT ("TAO_FT_Storable_Naming_Context::resolve - ")
       ACE_TEXT ("Some unhandled error occurred\n"));
    return CORBA::Object::_nil ();
  }

  return resolved_ref._retn ();
}

int
TAO_FT_Storable_Naming_Context::propagate_update_notification (
                   FT_Naming::ChangeType change_type)
{
  // Notify the peer of the changed context
  FT_Naming::ReplicationManager_var peer =
    TAO_FT_Naming_Replication_Manager::peer_replica ();

  if (CORBA::is_nil (peer.in ()))
    {
      // Replication is not supported without a peer replica.
      return 1;
    }

  FT_Naming::NamingContextUpdate context_info;
  context_info.context_name = this->context_name_.c_str ();

  // We are are updating the context one element before the specified name
  context_info.change_type = change_type;

  try {
    // Notify the naming_manager of the updated context
    peer->notify_updated_context (context_info);
  }
  catch (CORBA::Exception& ex)
    {
      if (TAO_debug_level > 3)
        ex._tao_print_exception (ACE_TEXT ("Unable to communicate with peer.\n"));
      return -1;
    }

  return 0;
}

void
TAO_FT_Storable_Naming_Context::set_naming_manager (
  TAO_FT_Naming_Manager *mgr_impl)
{
  naming_manager_ = mgr_impl;
}

void
TAO_FT_Storable_Naming_Context::stale (bool is_stale)
{
  this->stale_ = is_stale;
}


bool
TAO_FT_Storable_Naming_Context::stale (void)
{
  return stale_;
}


void
TAO_FT_Storable_Naming_Context::context_written (void)
{
  FT_Naming::ChangeType change_type;

  if (this->destroyed_)
    change_type = FT_Naming::DELETED;
  else
    change_type = FT_Naming::UPDATED;

  propagate_update_notification (change_type);
}

bool
TAO_FT_Storable_Naming_Context::is_obsolete (time_t stored_time)
{
  // If data has never been loaded, then the context_ object will
  // be a null pointer.
  return ((this->context_ == 0) ||              // Has not been loaded
          this->stale () ||                     // Explicitly marked stale by peer
          (stored_time > this->last_changed_)); // File has been updated
}

TAO_END_VERSIONED_NAMESPACE_DECL
