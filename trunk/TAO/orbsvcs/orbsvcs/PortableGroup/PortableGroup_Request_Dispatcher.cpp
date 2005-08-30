// This may look like C, but it's really -*- C++ -*-
//
// $Id$

#include "UIPMC_Profile.h"
#include "PortableGroup_Request_Dispatcher.h"
#include "PortableGroup_Loader.h"

#include "tao/TAO_Server_Request.h"
#include "tao/ORB_Core.h"

ACE_RCSID (PortableGroup, 
           PortableGroup_Request_Dispatcher, 
           "$Id$")

PortableGroup_Request_Dispatcher::~PortableGroup_Request_Dispatcher (void)
{
}

void
PortableGroup_Request_Dispatcher::dispatch (TAO_ORB_Core *orb_core,
                                            TAO_ServerRequest &request,
                                            CORBA::Object_out forward_to
                                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Check if the request contains a tagged profile
  if (request.profile ().discriminator () == GIOP::ProfileAddr)
    {
      // Get the IOP::Tagged profile.
      const IOP::TaggedProfile &tagged_profile =
        request.profile ().tagged_profile ();
      PortableGroup::TagGroupTaggedComponent group;

      // Extract a Group ID from the profile if it exists.
      int result = TAO_UIPMC_Profile::extract_group_component (tagged_profile,
                                                               group);
      if (result == 0)
        {
          // Got a group.  Dispatch based on the group ID
          this->group_map_.dispatch (&group,
                                     orb_core,
                                     request,
                                     forward_to
                                     ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          return;
        }

      // Else, fall through and dispatch based on object key like normal.
    }

  // Dispatch based on object key.
  orb_core->adapter_registry ()->dispatch (request.object_key (),
                                           request,
                                           forward_to
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
