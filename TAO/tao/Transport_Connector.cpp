#include "Transport_Connector.h"
#include "Transport.h"
#include "ORB_Core.h"
#include "MProfile.h"
#include "Profile.h"
#include "Environment.h"
#include "Thread_Lane_Resources.h"
#include "debug.h"
#include "Connect_Strategy.h"
#include "Client_Strategy_Factory.h"
#include "Connection_Handler.h"
#include "Profile_Transport_Resolver.h"
#include "Wait_Strategy.h"
#include "SystemException.h"

#include "ace/OS_NS_string.h"

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

TAO_Profile *
TAO_Connector::corbaloc_scan (const char *str,
                              size_t &len
                              ACE_ENV_ARG_DECL)
{
  if (this->check_prefix (str) != 0)
    return 0;
  const char *comma_pos = ACE_OS::strchr (str,',');
  const char *slash_pos = ACE_OS::strchr (str,'/');
  if (comma_pos == 0 && slash_pos == 0)
    {
      if (TAO_debug_level)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("(%P|%t) TAO_CONNECTOR::corbaloc_scan warning: ")
                    ACE_TEXT("supplied string contains no comma or slash: %s\n"),
                    str));
      len = ACE_OS::strlen (str);
    }
  else if (comma_pos == 0 || comma_pos > slash_pos)
    len = (slash_pos - str);
  else len = comma_pos - str;
  return this->make_profile(ACE_ENV_SINGLE_ARG_PARAMETER);
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
                            0,
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
                  ACE_TEXT_CHAR_TO_TCHAR (string)));
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
        ++profile_count;
    }

  // Tell the MProfile object how many Profiles it should hold.
  // MProfile::set(size) returns the number profiles it can hold.
  if (mprofile.set (profile_count) != static_cast<int> (profile_count))
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
        {
          end = ior.find (endpoint_delimiter, begin);
        }
      else
        {
          end = objkey_index;  // Handle last endpoint differently
        }

      if (end < static_cast<int> (ior.length ()) && end != ior.npos)
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


TAO_Transport*
TAO_Connector::connect (TAO::Profile_Transport_Resolver *r,
                        TAO_Transport_Descriptor_Interface *desc,
                        ACE_Time_Value *timeout
                        ACE_ENV_ARG_DECL_NOT_USED)
{
  if (desc == 0 ||
      (this->set_validate_endpoint (desc->endpoint ()) == -1))
    return 0;

  TAO_Transport *base_transport = 0;

  TAO_Transport_Cache_Manager &tcm =
    this->orb_core ()->lane_resources ().transport_cache ();

  // Check the Cache first for connections
  // If transport found, reference count is incremented on assignment
  // @@todo: We need to send the timeout value to the cache registry
  // too. That should be the next step!
  if (tcm.find_transport (desc,
                          base_transport) != 0)
    {
      // @@TODO: This is not the right place for this!
      // Purge connections (if necessary)
      tcm.purge ();

      TAO_Transport* t = this->make_connection (r,
                                                *desc,
                                                timeout);

      if (t == 0)
        return t;

      t->opened_as (TAO::TAO_CLIENT_ROLE);

      if (TAO_debug_level > 4)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - Transport_Connector::connect, "
                    "opening Transport[%d] in TAO_CLIENT_ROLE\n",
                    t->id ()));

      return t;
    }

  if (TAO_debug_level > 4)
    {
      TAO::Connection_Role cr =
        base_transport->opened_as ();

      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport_Connector::connect, "
                  "got an existing %s Transport[%d] in role %s\n",
                  base_transport->is_connected () ? "connected" : "unconnected",
                  base_transport->id (),
                  cr == TAO::TAO_SERVER_ROLE ? "TAO_SERVER_ROLE" :
                  cr == TAO::TAO_CLIENT_ROLE ? "TAO_CLIENT_ROLE" :
                  "TAO_UNSPECIFIED_ROLE" ));
    }

  // If connected return..
  if (base_transport->is_connected ())
    return base_transport;

  if (!this->wait_for_connection_completion (r,
                                             base_transport,
                                             timeout))
    {
      if (TAO_debug_level > 2)
        ACE_ERROR ((LM_ERROR,
                    "TAO (%P|%t) - Transport_Connector::"
                    "connect, "
                    "wait for completion failed\n"));
      return 0;
    }

  if (base_transport->is_connected () &&
      base_transport->wait_strategy ()->register_handler () != 0)
    {
      // Registration failures.

      // Purge from the connection cache, if we are not in the cache, this
      // just does nothing.
      (void) base_transport->purge_entry ();

      // Close the handler.
      (void) base_transport->close_connection ();

      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "TAO (%P|%t) - Transport_Connector [%d]::connect, "
                    "could not register the transport "
                    "in the reactor.\n",
                    base_transport->id ()));

      return 0;
    }

  return base_transport;
}

bool
TAO_Connector::wait_for_connection_completion (
    TAO::Profile_Transport_Resolver *r,
    TAO_Transport *&transport,
    ACE_Time_Value *timeout)
{
  if (TAO_debug_level > 2)
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport_Connector::wait_for_connection_completion, "
                  "going to wait for connection completion on transport"
                  "[%d]\n",
                  transport->id ()));

  // If we don't need to block for a transport just set the timeout to
  // be zero.
  ACE_Time_Value tmp_zero (ACE_Time_Value::zero);
  if (!r->blocked ())
    {
      timeout = &tmp_zero;
    }

  // Wait until the connection is ready, when non-blocking we just do a wait
  // with zero time
  int result =
    this->active_connect_strategy_->wait (
      transport,
      timeout);

  if (TAO_debug_level > 2)
    ACE_DEBUG ((LM_DEBUG,
                "TAO (%P|%t) - Transport_Connector::wait_for_connection_completion, "
                "transport [%d], wait done result = %d\n",
                transport->id (), result));

  // There are three possibilities when wait() returns: (a)
  // connection succeeded; (b) connection failed; (c) wait()
  // failed because of some other error.  It is easy to deal with
  // (a) and (b).  (c) is tricky since the connection is still
  // pending and may get completed by some other thread.  The
  // following method deals with (c).

  if (result == -1)
    {
      if (!r->blocked () && errno == ETIME)
        {
          // If we did a non blocking connect, just ignore
          // any timeout errors
          result = 0;
        }
      else
        {
          // When we need to get a connected transport
          result =
            this->check_connection_closure (
              transport->connection_handler ());
        }

      // In case of errors.
      if (result == -1)
        {
          // Report that making the connection failed, don't print errno
          // because we touched the reactor and errno could be changed
          if (TAO_debug_level > 2)
            ACE_ERROR ((LM_ERROR,
                        "TAO (%P|%t) - Transport_Connector::"
                        "wait_for_connection_completion, "
                        "transport [%d], wait for completion failed\n",
                        transport->id()));

          // Set transport to zero, it is not usable
          transport = 0;

          return false;
        }
    }

  // Connection not ready yet but we can use this transport, if
  // we need a connected one we will block later to make sure
  // it is connected
  return true;
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
    {
      return -1;
    }

  return 0;
}

int
TAO_Connector::check_connection_closure (
  TAO_Connection_Handler *connection_handler)
{
  int result = -1;

  // Check if the handler has been closed.
  int closed =
    connection_handler->is_closed ();

  // In case of failures and close() has not be called.
  if (!closed)
    {
      // First, cancel from connector.
      if (this->cancel_svc_handler (connection_handler) == -1)
        return -1;

      // Double check to make sure the handler has not been closed
      // yet.  This double check is required to ensure that the
      // connection handler was not closed yet by some other
      // thread since it was still registered with the connector.
      // Once connector.cancel() has been processed, we are
      // assured that the connector will no longer open/close this
      // handler.
      closed = connection_handler->is_closed ();

      // If closed, there is nothing to do here.  If not closed,
      // it was either opened or is still pending.
      if (!closed)
        {
          // Check if the handler has been opened.
          const int open = connection_handler->is_open ();

          // Some other thread was able to open the handler even
          // though wait failed for this thread.
          if (open)
            {
              // Set the result to 0, we have an open connection
              result = 0;
            }
          else
            {
              // Assert that it is still connecting.
              ACE_ASSERT (connection_handler->is_connecting ());

              // Force close the handler now.
              connection_handler->close_handler ();
            }
        }
    }

  return result;
}
