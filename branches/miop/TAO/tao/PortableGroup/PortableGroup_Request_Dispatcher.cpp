// $Id$

#include "PortableGroup_Request_Dispatcher.h"
#include "PortableGroup_Loader.h"
#include "UIPMC_Profile.h"
#include "tao/TAO_Server_Request.h"
#include "tao/ORB_Core.h"

ACE_RCSID(PortableGroup, PortableGroup_Request_Dispatcher, "$Id$")

PortableGroup_Request_Dispatcher::PortableGroup_Request_Dispatcher (
                                                                    TAO_PortableGroup_Loader *loader) :
  portable_group_loader_ (loader)
{
}


PortableGroup_Request_Dispatcher::~PortableGroup_Request_Dispatcher (void)
{
}


void
PortableGroup_Request_Dispatcher::dispatch (TAO_ORB_Core *orb_core,
                                            TAO_ServerRequest &request,
                                            void *context,
                                            CORBA::Object_out forward_to,
                                            CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Dispatch based on object key

  if (request.profile_has_dispatch_info ())
    {
      // Get the IOP::Tagged profile.
      IOP::TaggedProfile &tagged_profile =
        request.profile ().tagged_profile ();
      PortableGroup::TagGroupTaggedComponent group;

      // Extract a Group ID from the profile if it exists.
      int result = TAO_UIPMC_Profile::extract_group_component (tagged_profile,
                                                               group);

      if (result == 0)
        {
          // Got a group.  Dispatch based on the group ID
          portable_group_loader_->group_map ().dispatch (&group,
                                  orb_core,
                                  request,
                                  context,
                                  forward_to,
                                  ACE_TRY_ENV);
          ACE_CHECK;

          return;
        }

      // Else, extract the object key from the TaggedProfile.
      if (request.profile ().extract_object_key (tagged_profile) != 1)
        {
          ACE_TRY_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                         CORBA::COMPLETED_NO));
        }
    }

  // Dispatch based on object key like normal.
  orb_core->adapter_registry ()->dispatch (request.object_key (),
                                           request,
                                           context,
                                           forward_to,
                                           ACE_TRY_ENV);
  ACE_CHECK;
}

