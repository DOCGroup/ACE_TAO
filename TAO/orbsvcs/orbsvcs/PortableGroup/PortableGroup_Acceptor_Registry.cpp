#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/PortableGroup/PortableGroup_Acceptor_Registry.h"
#include "tao/ORB_Core.h"
#include "tao/Profile.h"
#include "tao/Protocol_Factory.h"
#include "tao/GIOP_Message_State.h"
#include "tao/debug.h"
#include "tao/Endpoint.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Leader_Follower.h"
#include "tao/SystemException.h"
#include <new>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_PortableGroup_Acceptor_Registry::~TAO_PortableGroup_Acceptor_Registry ()
{
  // Free the memory for the endpoints.
  for (Entry &entry : this->registry_)
    {
      delete entry.endpoint;
      delete entry.acceptor;
    }
}

void
TAO_PortableGroup_Acceptor_Registry::open (TAO_Profile const *profile,
                                           TAO_ORB_Core &orb_core)
{
  Entry *entry = nullptr;

  if (this->find (profile, entry) == 1)
    {
      // Found it.  Increment the reference count.
      ++entry->cnt;
    }
  else
    {
      // Not found.  Open a new acceptor.

      // Now get the list of available protocol factories.
      TAO_ProtocolFactorySetItor end =
        orb_core.protocol_factories ()->end ();

      for (TAO_ProtocolFactorySetItor factory =
             orb_core.protocol_factories ()->begin ();
           factory != end;
           ++factory)
        {
          if ((*factory)->factory ()->tag () == profile->tag ())
            {
              this->open_i (profile,
                            orb_core,
                            factory);

            }
        }
    }
}

constexpr int MAX_ADDR_LENGTH = 32;

void
TAO_PortableGroup_Acceptor_Registry::open_i (TAO_Profile const *profile,
                                             TAO_ORB_Core &orb_core,
                                             TAO_ProtocolFactorySetItor &factory)
{
  TAO_Acceptor * const acceptor = (*factory)->factory ()->make_acceptor ();

  if (acceptor != nullptr)
    {
      // Extract the desired endpoint/protocol version if one
      // exists.
      TAO_GIOP_Message_Version const &version = profile->version ();
      char buffer [MAX_ADDR_LENGTH] {};

      // Removed the constness of profile.  We're not changing
      // anything, but need to call a nonconst function.
      TAO_Profile * const nc_profile = const_cast<TAO_Profile *> (profile);
      nc_profile->endpoint ()->addr_to_string (buffer, MAX_ADDR_LENGTH);

      if (acceptor->open (&orb_core,
                          orb_core.lane_resources ().leader_follower ().reactor(),
                          version.major,
                          version.minor,
                          buffer,
                          0) == -1)
        {
          delete acceptor;

          if (TAO_debug_level > 0)
            ORBSVCS_ERROR ((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) ")
                        ACE_TEXT ("unable to open acceptor ")
                        ACE_TEXT ("for <%s>%p\n"),
                        buffer,
                        ""));

          throw CORBA::BAD_PARAM (
            CORBA::SystemException::_tao_minor_code (
              TAO_ACCEPTOR_REGISTRY_OPEN_LOCATION_CODE,
              EINVAL),
            CORBA::COMPLETED_NO);
        }

      // Add acceptor to list.
      Entry tmp_entry;
      tmp_entry.acceptor = acceptor;
      tmp_entry.endpoint = nc_profile->endpoint ()->duplicate ();
      tmp_entry.cnt = 1;

      try
        {
          this->registry_.push_back (tmp_entry);
        }
      catch (std::bad_alloc const &)
        {
          delete tmp_entry.endpoint;
          delete acceptor;

          if (TAO_debug_level > 0)
            ORBSVCS_ERROR ((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) ")
                        ACE_TEXT ("unable to add acceptor to registry")
                        ACE_TEXT ("for <%s>%p\n"),
                        buffer,
                        ""));

          throw CORBA::NO_MEMORY (
            CORBA::SystemException::_tao_minor_code (
              TAO_ACCEPTOR_REGISTRY_OPEN_LOCATION_CODE,
              ENOMEM),
            CORBA::COMPLETED_NO);
        }
    }
  else
    {
      if (TAO_debug_level > 0)
        ORBSVCS_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) ")
                    ACE_TEXT ("unable to create acceptor ")
                    ));

      throw CORBA::BAD_PARAM (
        CORBA::SystemException::_tao_minor_code (
          TAO_ACCEPTOR_REGISTRY_OPEN_LOCATION_CODE,
          EINVAL),
        CORBA::COMPLETED_NO);
    }
}

int
TAO_PortableGroup_Acceptor_Registry::find (TAO_Profile const *profile,
                                           Entry *&entry)
{
  for (Entry &current : this->registry_)
    {
      // Since the endpoint routine is nonconst, need to
      // cast away the constness even though we're not
      // changing anything.
      TAO_Profile * const nc_profile = const_cast<TAO_Profile *> (profile);
      if (current.endpoint->is_equivalent (nc_profile->endpoint ()))
        {
          entry = &current;
          return 1;
        }
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
