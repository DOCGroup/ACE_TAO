#include "Transport_Connector.h"
#include "Transport.h"
#include "ORB_Core.h"
#include "Invocation.h"
#include "MProfile.h"
#include "Profile.h"
#include "Environment.h"
#include "Thread_Lane_Resources.h"
#include "debug.h"
#include "Connect_Strategy.h"
#include "Client_Strategy_Factory.h"


#if !defined (__ACE_INLINE__)
# include "Transport_Connector.inl"
#endif /* __ACE_INLINE__ */


ACE_RCSID (tao,
           Connector,
           "$Id$")


// Connector
TAO_Connector::TAO_Connector (CORBA::ULong tag)
  : active_connect_strategy_ (0),
    tag_(tag),
    orb_core_ (0)
{
}

TAO_Connector::~TAO_Connector (void)
{
  delete this->active_connect_strategy_;
}

int
TAO_Connector::make_mprofile (const char *string,
                              TAO_MProfile &mprofile
                              ACE_ENV_ARG_DECL)
{
  // This method utilizes the "Template Method" design pattern to
  // parse the given URL style IOR for the protocol being used
  // and create an mprofile from it.
  //
  // The methods that must be defined by all Connector sub-classes are:
  //      make_profile
  //      check_prefix

  // Check for a valid string
  if (!string || !*string)
    {
      ACE_THROW_RETURN (CORBA::INV_OBJREF (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            EINVAL),
                          CORBA::COMPLETED_NO),
                        -1);
    }

  // Check for the proper prefix in the IOR.  If the proper prefix isn't
  // in the IOR then it is not an IOR we can use.
  if (this->check_prefix (string) != 0)
    {
      return 1;
      // Failure: not the correct IOR for this protocol.
      // DO NOT throw an exception here since the Connector_Registry
      // should be allowed the opportunity to continue looking for
      // an appropriate connector.
    }

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - TAO_Connector::make_mprofile ")
                  ACE_TEXT ("<%s>\n"),
                  string));
    }

  ACE_CString ior;

  ior.set (string, ACE_OS::strlen (string), 1);

  // Find out where the protocol ends
  int ior_index = ior.find ("://");

  if (ior_index == ACE_CString::npos)
    {
      ACE_THROW_RETURN (CORBA::INV_OBJREF (), -1);
      // No colon ':' in the IOR!
    }
  else
    {
      ior_index += 3;
      // Add the length of the colon and the two forward slashes `://'
      // to the IOR string index (i.e. 3)
    }

  // Find the object key
  const int objkey_index =
    ior.find (this->object_key_delimiter (), ior_index);

  if (objkey_index == 0 || objkey_index == ACE_CString::npos)
    {
      ACE_THROW_RETURN (CORBA::INV_OBJREF (), -1);
      // Failure: No endpoints specified or no object key specified.
    }

  const char endpoint_delimiter = ',';
  // The delimiter used to seperate inidividual addresses.

  // Count the number of endpoints in the IOR.  This will be the number
  // of entries in the MProfile.

  CORBA::ULong profile_count = 1;
  // Number of endpoints in the IOR  (initialized to 1).

  // Only check for endpoints after the protocol specification and
  // before the object key.
  for (int i = ior_index; i < objkey_index; ++i)
    {
      if (ior[i] == endpoint_delimiter)
        profile_count++;
    }

  // Tell the MProfile object how many Profiles it should hold.
  // MProfile::set(size) returns the number profiles it can hold.
  if (mprofile.set (profile_count) != ACE_static_cast (int, profile_count))
    {
      ACE_THROW_RETURN (CORBA::INV_OBJREF (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_MPROFILE_CREATION_ERROR,
                            0),
                          CORBA::COMPLETED_NO),
                        -1);
      // Error while setting the MProfile size!
    }

  // The idea behind the following loop is to split the IOR into several
  // strings that can be parsed by each profile.
  // For example,
  //    `1.3@moo,shu,1.1@chicken/arf'
  // will be parsed into:
  //    `1.3@moo/arf'
  //    `shu/arf'
  //    `1.1@chicken/arf'

  int begin = 0;
  int end = ior_index - 1;
  // Initialize the end of the endpoint index

  for (CORBA::ULong j = 0; j < profile_count; ++j)
    {
      begin = end + 1;

      if (j < profile_count - 1)
        end = ior.find (endpoint_delimiter, begin);
      else
        end = objkey_index;  // Handle last endpoint differently

      if (end < ACE_static_cast (int, ior.length ()) && end != ior.npos)
        {
          ACE_CString endpoint = ior.substring (begin, end - begin);

          // Add the object key to the string.
          endpoint += ior.substring (objkey_index);

          // The endpoint should now be of the form:
          //    `N.n@endpoint/object_key'
          // or
          //    `endpoint/object_key'

          TAO_Profile *profile =
            this->make_profile (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);
          // Failure:  Problem during profile creation

          // Initialize a Profile using the individual endpoint
          // string.
          // @@ Not exception safe!  We need a TAO_Profile_var!
          profile->parse_string (endpoint.c_str ()
                                 ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);

          // Give up ownership of the profile.
          if (mprofile.give_profile (profile) == -1)
            {
              profile->_decr_refcnt ();

              ACE_THROW_RETURN (CORBA::INV_OBJREF (
                                  CORBA::SystemException::_tao_minor_code (
                                     TAO_MPROFILE_CREATION_ERROR,
                                     0),
                                  CORBA::COMPLETED_NO),
                                -1);
              // Failure presumably only occurs when MProfile is full!
              // This should never happen.
            }
        }
      else
        {
          ACE_THROW_RETURN (CORBA::INV_OBJREF (), -1);
          // Unable to seperate endpoints
        }
    }

  return 0;  // Success
}

int
TAO_Connector::connect (TAO_GIOP_Invocation *invocation,
                        TAO_Transport_Descriptor_Interface *desc
                        ACE_ENV_ARG_DECL)
{
  return this->connect (invocation,
                        desc,
                        0
                        ACE_ENV_ARG_PARAMETER);
}

int
TAO_Connector::connect (TAO_GIOP_Invocation *invocation,
                        TAO_Transport_Descriptor_Interface *desc,
                        ACE_Time_Value *timeout
                        ACE_ENV_ARG_DECL_NOT_USED)
{
  if (this->set_validate_endpoint (desc->endpoint ()) == -1)
    return -1;


  TAO_Transport *base_transport = 0;

  // Check the Cache first for connections
  // If transport found, reference count is incremented on assignment
  // @@todo: We need to send the timeout value to the cache registry
  // too. That should be the next step!
  if (this->orb_core ()->lane_resources ().transport_cache ().find_transport (
        desc,
        base_transport) == 0)
    {
      if (TAO_debug_level > 2)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - Transport_Connector::connect, "
                    "got an existing Transport[%d]\n",
                    base_transport->id ()));

      TAO_Transport *&transport = invocation->transport ();

      // No need to _duplicate and release since base_transport
      // is going out of scope.  Transport now has control of
      // base_transport.
      transport = base_transport;

      // Successful
      return 0;
    }

  // @@TODO: This is not the right place for this!
  // Purge connections (if necessary)
  this->orb_core_->lane_resources ().transport_cache ().purge ();

  return this->make_connection (invocation,
                                desc,
                                timeout);
}


TAO_Transport*
TAO_Connector::connect (TAO::Profile_Connection_Resolver *r,
                        TAO_Endpoint *ep,
                        ACE_Time_Value *timeout
                        ACE_ENV_ARG_DECL_NOT_USED)
{
  if (this->set_validate_endpoint (ep) == -1)
    return -1;

  TAO_Base_Transport_Property desc (ep);

  TAO_Transport *base_transport = 0;

  // Check the Cache first for connections
  // If transport found, reference count is incremented on assignment
  // @@todo: We need to send the timeout value to the cache registry
  // too. That should be the next step!
  if (this->orb_core ()->lane_resources ().transport_cache ().find_transport (
        desc,
        base_transport) == 0)
    {
      if (TAO_debug_level > 2)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - Transport_Connector::connect, "
                    "got an existing Transport[%d]\n",
                    base_transport->id ()));

      // No need to _duplicate since things are taken care within the
      // cache manager.
      return base_transport;
    }

  // @@TODO: This is not the right place for this!
  // Purge connections (if necessary)
  this->orb_core_->lane_resources ().transport_cache ().purge ();

  return this->make_connection (r,
                                desc,
                                timeout);
}


int
TAO_Connector::create_connect_strategy (void)
{
  if (this->active_connect_strategy_ == 0)
    {
      this->active_connect_strategy_ =
        this->orb_core_->client_factory ()->create_connect_strategy (
          this->orb_core_);
    }

  if (this->active_connect_strategy_ == 0)
    return -1;

  return 0;
}
