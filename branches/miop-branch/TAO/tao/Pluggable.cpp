// This may look like C, but it's really -*- C++ -*-
// $Id$

#include "tao/Pluggable.h"
#include "tao/MProfile.h"
#include "tao/Environment.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/Wait_Strategy.h"
#include "tao/Transport_Mux_Strategy.h"
#include "tao/debug.h"

#include "tao/target_specification.h"

#if !defined (__ACE_INLINE__)
# include "tao/Pluggable.i"
#endif /* __ACE_INLINE__ */


ACE_RCSID(tao, Pluggable, "$Id$")

// *********************************************************************

// Connector
TAO_Connector::TAO_Connector (CORBA::ULong tag)
  : tag_(tag),
    orb_core_ (0)

{
}

TAO_Connector::~TAO_Connector (void)
{
}

int
TAO_Connector::make_mprofile (const char *string,
                              TAO_MProfile &mprofile
                              TAO_ENV_ARG_DECL)
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
                          CORBA_SystemException::_tao_minor_code (
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
                  ACE_TEXT ("TAO (%P|%t) - TAO_Connector::make_mprofile <%s>\n"),
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

  const int objkey_index =
    ior.find (this->object_key_delimiter (), ior_index) + ior_index;
  // Find the object key

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
                          CORBA_SystemException::_tao_minor_code (
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
      begin += end + 1;

      if (j < profile_count - 1)
        end = ior.find (endpoint_delimiter, begin);
      else
        end = objkey_index - begin;  // Handle last endpoint differently

      if (end < ACE_static_cast (int, ior.length ()) && end != ior.npos)
        {
          ACE_CString endpoint = ior.substring (begin, end);
          endpoint += ior.substring (objkey_index);
          // Add the object key to the string.

          // The endpoint should now be of the form:
          //    `N.n@endpoint/object_key'
          // or
          //    `endpoint/object_key'

          TAO_Profile *profile = this->make_profile (TAO_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);
          // Failure:  Problem during profile creation

          // Initialize a Profile using the individual endpoint
          // string.
          // @@ Not exception safe!  We need a TAO_Profile_var!
          profile->parse_string (endpoint.c_str () TAO_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);

          // Give up ownership of the profile.
          if (mprofile.give_profile (profile) == -1)
            {
              profile->_decr_refcnt ();

              ACE_THROW_RETURN (CORBA::INV_OBJREF (
                                  CORBA_SystemException::_tao_minor_code (
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




// ****************************************************************

// Acceptor
TAO_Acceptor::TAO_Acceptor (CORBA::ULong tag)
  : tag_ (tag)
{
}

TAO_Acceptor::~TAO_Acceptor (void)
{
}
