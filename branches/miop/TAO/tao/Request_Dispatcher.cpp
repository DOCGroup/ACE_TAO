// $Id$

#include "Request_Dispatcher.h"
#include "TAO_Server_Request.h"
#include "ORB_Core.h"

ACE_RCSID(tao, GIOP_Message_Base, "$Id$")

TAO_Request_Dispatcher::~TAO_Request_Dispatcher (void)
{
}

void
TAO_Request_Dispatcher::dispatch (TAO_ORB_Core *orb_core,
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

      // Extract the object key from the TaggedProfile.
      if (request.profile ().extract_object_key (tagged_profile) != 1)
        {
          ACE_TRY_THROW (CORBA::MARSHAL (TAO_DEFAULT_MINOR_CODE,
                                         CORBA::COMPLETED_NO));
        }
    }

  // Dispatch.
  orb_core->adapter_registry ()->dispatch (request.object_key (),
                                           request,
                                           context,
                                           forward_to,
                                           ACE_TRY_ENV);
  ACE_TRY_CHECK;
}

