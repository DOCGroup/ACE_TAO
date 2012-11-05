// -*- C++ -*-
// $Id$

#include "tao/Transport_Connector.h"
#include "tao/Transport.h"
#include "tao/ORB_Core.h"
#include "tao/MProfile.h"
#include "tao/Profile.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/debug.h"
#include "tao/Connect_Strategy.h"
#include "tao/LF_Multi_Event.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/Connection_Handler.h"
#include "tao/Profile_Transport_Resolver.h"
#include "tao/Wait_Strategy.h"
#include "tao/SystemException.h"
#include "tao/Endpoint.h"
#include "tao/Base_Transport_Property.h"

#include "ace/OS_NS_string.h"

//@@ TAO_CONNECTOR_SPL_INCLUDE_ADD_HOOK

#if !defined (__ACE_INLINE__)
# include "tao/Transport_Connector.inl"
#endif /* __ACE_INLINE__ */

namespace
{
  class TransportCleanupGuard
  {
  public:
    // Constructor.  Initially assume that we're going to clean up the
    // transport upon destruction.
    TransportCleanupGuard (TAO_Transport *tp)
      : tp_ (tp),
        awake_ (true)
    {
    }

    ~TransportCleanupGuard ()
    {
      if (this->awake_ && this->tp_)
        {
          // Purge from the connection cache.  If we are not in the
          // cache, this does nothing.
          this->tp_->purge_entry ();

          // Close the handler and remove the reference.
          this->tp_->close_connection ();
          this->tp_->remove_reference ();
        }
    }

    /// Turn off the guard.
    void clear ()
    {
      this->awake_ = false;
    }

  private:
    TAO_Transport * const tp_;
    bool awake_;
  };
}

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Connector
TAO_Connector::TAO_Connector (CORBA::ULong tag)
  : active_connect_strategy_ (0),
    tag_ (tag),
    orb_core_ (0)
{
}

TAO_Connector::~TAO_Connector (void)
{
  delete this->active_connect_strategy_;
}

TAO_Profile *
TAO_Connector::corbaloc_scan (const char *str, size_t &len)
{
  if (this->check_prefix (str) != 0)
    return 0;
  const char *comma_pos = ACE_OS::strchr (str,',');
  const char *slash_pos = ACE_OS::strchr (str,'/');
  if (comma_pos == 0 && slash_pos == 0)
    {
      len = ACE_OS::strlen (str);
    }
  else if (comma_pos == 0 || comma_pos > slash_pos)
    len = (slash_pos - str);
  else len = comma_pos - str;
  return this->make_profile();
}

int
TAO_Connector::make_mprofile (const char *string, TAO_MProfile &mprofile)
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
      throw ::CORBA::INV_OBJREF (
        CORBA::SystemException::_tao_minor_code (
          0,
          EINVAL),
        CORBA::COMPLETED_NO);
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
                  ACE_TEXT ("<%C>\n"),
                  string));
    }

  ACE_CString ior;

  ior.set (string, ACE_OS::strlen (string), 1);

  // Find out where the protocol ends
  ACE_CString::size_type ior_index = ior.find ("://");

  if (ior_index == ACE_CString::npos)
    {
      throw ::CORBA::INV_OBJREF ();
      // No colon ':' in the IOR!
    }
  else
    {
      ior_index += 3;
      // Add the length of the colon and the two forward slashes `://'
      // to the IOR string index (i.e. 3)
    }

  // Find the object key
  const ACE_CString::size_type objkey_index =
    ior.find (this->object_key_delimiter (), ior_index);

  if (objkey_index == 0 || objkey_index == ACE_CString::npos)
    {
      throw ::CORBA::INV_OBJREF ();
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
  for (ACE_CString::size_type i = ior_index; i < objkey_index; ++i)
    {
      if (ior[i] == endpoint_delimiter)
        ++profile_count;
    }

  // Tell the MProfile object how many Profiles it should hold.
  // MProfile::set(size) returns the number profiles it can hold.
  if (mprofile.set (profile_count) != static_cast<int> (profile_count))
    {
      throw ::CORBA::INV_OBJREF (
        CORBA::SystemException::_tao_minor_code (
          TAO_MPROFILE_CREATION_ERROR,
          0),
        CORBA::COMPLETED_NO);
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

  ACE_CString::size_type begin = 0;
  ACE_CString::size_type end = ior_index - 1;
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

      if (end < ior.length () && end != ior.npos)
        {
          ACE_CString endpoint = ior.substring (begin, end - begin);

          // Add the object key to the string.
          endpoint += ior.substring (objkey_index);

          // The endpoint should now be of the form:
          //    `N.n@endpoint/object_key'
          // or
          //    `endpoint/object_key'

          TAO_Profile *profile =
            this->make_profile ();
          // Failure:  Problem during profile creation

          // Initialize a Profile using the individual endpoint
          // string.
          // @@ Not exception safe!  We need a TAO_Profile_var!
          profile->parse_string (endpoint.c_str ()
                                );

          // Give up ownership of the profile.
          if (mprofile.give_profile (profile) == -1)
            {
              profile->_decr_refcnt ();

              throw ::CORBA::INV_OBJREF (
                CORBA::SystemException::_tao_minor_code (
                  TAO_MPROFILE_CREATION_ERROR,
                  0),
                CORBA::COMPLETED_NO);
              // Failure presumably only occurs when MProfile is full!
              // This should never happen.
            }
        }
      else
        {
          throw ::CORBA::INV_OBJREF ();
          // Unable to seperate endpoints
        }
    }

  return 0;  // Success
}

int
TAO_Connector::supports_parallel_connects(void) const
{
  return 0; // by default, we don't support parallel connection attempts;
}

TAO_Transport*
TAO_Connector::make_parallel_connection (TAO::Profile_Transport_Resolver *,
                                         TAO_Transport_Descriptor_Interface &,
                                         ACE_Time_Value *)
{
  return 0;
}


TAO_Transport*
TAO_Connector::parallel_connect (TAO::Profile_Transport_Resolver *r,
                                 TAO_Transport_Descriptor_Interface *desc,
                                 ACE_Time_Value *timeout)
{
  if (this->supports_parallel_connects() == 0)
    {
      errno = ENOTSUP;
      return 0;
    }

  errno = 0; // need to clear errno to ensure a stale enotsup is not set
  if (desc == 0)
    return 0;
  TAO_Endpoint *root_ep = desc->endpoint();
  TAO_Transport *base_transport = 0;

  TAO::Transport_Cache_Manager &tcm =
    this->orb_core ()->lane_resources ().transport_cache ();

  // Iterate through the endpoints. Since find_transport takes a
  // Transport Descriptor rather than an endpoint, we must create a
  // local TDI for each endpoint. The first one found will be used.
  for (TAO_Endpoint *ep = root_ep->next_filtered (this->orb_core(),0);
       ep != 0;
       ep = ep->next_filtered(this->orb_core(),root_ep))
    {
      TAO_Base_Transport_Property desc2(ep,0);
      size_t busy_count = 0;
      if (tcm.find_transport (&desc2, base_transport, busy_count) ==
          TAO::Transport_Cache_Manager::CACHE_FOUND_AVAILABLE)
        {
          if (TAO_debug_level)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("TAO (%P|%t) - TAO_Connector::parallel_connect: ")
                          ACE_TEXT ("found a transport [%d]\n"),
                          base_transport->id ()));
            }
          return base_transport;
        }
    }

  // Now we have searched the cache on all endpoints and come up
  // empty. We need to initiate connections on each of the
  // endpoints. Presumably only one will have a route and will succeed,
  // and the rest will fail. This requires the use of asynch
  // connection establishment. Maybe a custom wait strategy is needed
  // at this point to register several potential transports so that
  // when one succeeds the rest are cancelled or closed.

  unsigned int endpoint_count = 0;
  for (TAO_Endpoint *ep = root_ep->next_filtered (this->orb_core(),0);
       ep != 0;
       ep = ep->next_filtered(this->orb_core(),root_ep))
    if (this->set_validate_endpoint (ep) == 0)
      ++endpoint_count;
  if (endpoint_count == 0)
    return 0;
  return this->make_parallel_connection (r,*desc,timeout);
}

bool
TAO_Connector::wait_for_transport (TAO::Profile_Transport_Resolver *r,
                                   TAO_Transport *transport,
                                   ACE_Time_Value *timeout,
                                   bool force_wait)
{
  if (transport->connection_handler ()->is_timeout ())
    {
      if (TAO_debug_level > 2)
        {
          ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT ("TAO (%P|%t) - TAO_Connector::wait_for_transport, ")
            ACE_TEXT ("transport [%d], Connection Timed out.\n"),
                      transport->id ()));
        }
      transport->purge_entry ();
      return false;
    }
  else if (transport->connection_handler ()->is_closed ())
    {
      if (TAO_debug_level > 2)
        {
          ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT ("TAO (%P|%t) - TAO_Connector::wait_for_transport, ")
            ACE_TEXT ("transport [%d], Connection failed. (%d)\n"),
                      transport->id (), ACE_ERRNO_GET));
        }

      // purge from the connection cache.  If we are not in the
      // cache, this does nothing.
      transport->purge_entry ();

      // Close the handler.
      transport->close_connection ();

      return false;
    }
  else if (transport->connection_handler ()->is_open ())
    {
      if (TAO_debug_level > 5)
        {
          ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT("TAO (%P|%t) - TAO_Connector::wait_for_transport, ")
            ACE_TEXT("transport [%d], connection is open: no wait.\n"),
            transport->id () ));
        }

      return true;
    }
  else if (force_wait || r->blocked_connect ())
    {
      if (TAO_debug_level > 2)
        {
          ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT("TAO (%P|%t) - TAO_Connector::wait_for_transport, ")
            ACE_TEXT("waiting on transport [%d]\n"),
            transport->id () ));
        }

      // We must ensure that there is a timeout if there was none
      // supplied and the connection isn't a blocking connection.  If
      // another thread has called ORB::run() prior to this attempted
      // connection, the wait() call will block forever (or until the ORB
      // thread leaves the reactor, which may not happen).
      int result = 0;
      if (timeout == 0 && !r->blocked_connect ())
        {
          ACE_Time_Value tv (0, 500);
          result = this->active_connect_strategy_->wait (transport, &tv);
        }
      else
        result = this->active_connect_strategy_->wait (transport, timeout);

      if (result == -1 && errno == ETIME)
        {
          if (TAO_debug_level > 2)
            {
              ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("TAO (%P|%t) - TAO_Connector::wait_for_transport, ")
              ACE_TEXT(" timeout while waiting on transport [%d]\n"),
              transport->id () ));
            }
        }
      else if (result == -1)
        {
          if (TAO_debug_level > 2)
            {
              static int complain10times = 10;
              if (complain10times > 0)
                {
                  --complain10times;
                  ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("TAO (%P|%t) - TAO_Connector::wait_for_transport, ")
                    ACE_TEXT(" unknown error waiting on transport [%d] (%d)\n"),
                    transport->id (),
                    ACE_ERRNO_GET));
                }
            }
          // purge from the connection cache.  If we are not in the
          // cache, this does nothing.
          transport->purge_entry ();

          // Close the handler.
          transport->close_connection ();
        }
      else
        {
          if (TAO_debug_level > 5)
            {
              ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("TAO (%P|%t) - TAO_Connector::wait_for_transport, ")
                ACE_TEXT("transport [%d], wait completed ok.\n"),
                transport->id () ));
            }
          return true;
        }
    }
  else
    {
      if (TAO_debug_level > 2)
        {
          ACE_DEBUG ((LM_DEBUG,
            ACE_TEXT ("TAO (%P|%t) - TAO_Connector::wait_for_transport, ")
            ACE_TEXT ("Connection not complete [%d] reset state to ")
            ACE_TEXT ("LFS_CONNECTION_WAIT\n"), transport->id ()));
        }
      transport->connection_handler ()->reset_state (
        TAO_LF_Event::LFS_CONNECTION_WAIT);

      return true;
    }

  return false;
}

TAO_Transport*
TAO_Connector::connect (TAO::Profile_Transport_Resolver *r,
                        TAO_Transport_Descriptor_Interface *desc,
                        ACE_Time_Value *timeout)
{
  TAO::Transport_Cache_Manager &tcm =
    this->orb_core ()->lane_resources ().transport_cache ();

  // Stay in this loop until we find:
  // a usable connection, or a timeout happens
  while (true)
    {
      // Find a connection in the cache
      // If transport found, reference count is incremented on assignment
      TAO_Transport *base_transport = 0;
      size_t busy_count = 0;
      TAO::Transport_Cache_Manager::Find_Result found =
          tcm.find_transport (desc,
                              base_transport,
                              busy_count);

      if (found == TAO::Transport_Cache_Manager::CACHE_FOUND_AVAILABLE)
        {
          TAO_Connection_Handler *ch = base_transport->connection_handler ();
          // one last check before using the cached connection
          if (ch->error_detected ())
            {
              if (TAO_debug_level > 0)
                {
                  ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("TAO (%P|%t) Transport_Connector::connect, ")
                    ACE_TEXT("error in transport from cache\n")));
                }
              (void) base_transport->close_connection ();
              (void) base_transport->purge_entry ();
              base_transport->remove_reference ();
            }
          else if (ch->is_closed ())
            {
              if (TAO_debug_level > 0)
                {
                  ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("TAO (%P|%t) Transport_Connector::connect, ")
                    ACE_TEXT("closed transport from cache\n")));
                }
              (void) base_transport->purge_entry ();
              base_transport->remove_reference ();
            }
          else
            {
              if (TAO_debug_level > 4)
                {
                  TAO::Connection_Role cr = base_transport->opened_as ();

                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT("TAO (%P|%t) - Transport_Connector::connect, ")
                              ACE_TEXT("got an existing %C Transport[%d] in role %C\n"),
                              base_transport->is_connected () ? "connected" :
                                                                "unconnected",
                              base_transport->id (),
                              cr == TAO::TAO_SERVER_ROLE ? "TAO_SERVER_ROLE" :
                              cr == TAO::TAO_CLIENT_ROLE ? "TAO_CLIENT_ROLE" :
                              "TAO_UNSPECIFIED_ROLE" ));
                }

              // If connected return.
              if (base_transport->is_connected ())
                return base_transport;

              // Is it possible to get a transport from the cache that
              // is not connected? If not, then the following code is
              // bogus. We cannot wait for a connection to complete on
              // a transport in the cache.
              //
              // (mesnier_p@ociweb.com) It is indeed possible to reach
              // this point.  The AMI_Buffering test does. When using
              // non-blocking connects and the first request(s) are
              // asynch and may be queued, the connection
              // establishment may not be completed by the time the
              // invocation is done with it. In that case it is up to
              // a subsequent invocation to handle the connection
              // completion.
              TransportCleanupGuard tg(base_transport);
              if (!this->wait_for_connection_completion (r, *desc,
                                                         base_transport,
                                                         timeout))
                {
                  if (TAO_debug_level > 2)
                    {
                      ACE_ERROR ((LM_ERROR,
                                  "TAO (%P|%t) - Transport_Connector::connect,"
                                  " wait for completion failed\n"));
                    }
                  return 0;
                }

              if (base_transport->is_connected () &&
                  base_transport->wait_strategy ()->register_handler () == -1)
                {
                  // Registration failures.
                  if (TAO_debug_level > 0)
                    {
                      ACE_ERROR ((LM_ERROR,
                                  "TAO (%P|%t) - Transport_Connector::connect, "
                                  "could not register the transport [%d]"
                                  "in the reactor.\n",
                                  base_transport->id ()));
                    }
                  return 0;
                }

              tg.clear ();
              return base_transport;
            }
        }
      else if (found == TAO::Transport_Cache_Manager::CACHE_FOUND_CONNECTING)
        {
          if (r->blocked_connect ())
            {
              if (TAO_debug_level > 4)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT("TAO (%P|%t) - ")
                              ACE_TEXT("Transport_Connector::waiting ")
                              ACE_TEXT("for connection on transport [%d]\n"),
                              base_transport->id ()));
                }

              // If wait_for_transport returns no errors, the base_transport
              // points to the connection we wait for.
              if (this->wait_for_transport (r, base_transport, timeout, false))
                {
                  // be sure this transport is registered with the reactor
                  // before using it.
                  if (!base_transport->register_if_necessary ())
                    {
                        base_transport->remove_reference ();
                        return 0;
                    }
                }

              // In either success or failure cases of wait_for_transport, the
              // ref counter in corresponding to the ref counter added by
              // find_transport is decremented.
              base_transport->remove_reference ();
            }
          else
            {
              if (TAO_debug_level > 4)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT("TAO (%P|%t) - ")
                              ACE_TEXT("Transport_Connector::non-blocking:")
                              ACE_TEXT("returning unconnected ")
                              ACE_TEXT("transport [%d]\n"),
                    base_transport->id ()));
                }

              // return the transport in it's current, unconnected state
              return base_transport;
            }
        }
      else
        {
          if (desc == 0 ||
              (this->set_validate_endpoint (desc->endpoint ()) == -1))
            return 0;

          // @todo: This is not the right place for this! (bugzilla 3023)
          // Purge connections (if necessary)
          tcm.purge ();
          bool const make_new_connection =
            (found == TAO::Transport_Cache_Manager::CACHE_FOUND_NONE) ||
            (found == TAO::Transport_Cache_Manager::CACHE_FOUND_BUSY
                && this->new_connection_is_ok (busy_count));

          if (make_new_connection)
            {
              // we aren't going to use the transport returned from the cache
              // (if any)
              if (base_transport != 0)
                {
                  base_transport->remove_reference ();
                }

              base_transport = this->make_connection (r, *desc, timeout);
              if (base_transport == 0)
                {
                  if (TAO_debug_level > 4)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - Transport_Connector::")
                        ACE_TEXT ("connect, make_connection failed\n")));
                    }
                  return 0;
                }

              if (TAO_debug_level > 4)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT("TAO (%P|%t) - ")
                              ACE_TEXT("Transport_Connector::connect, ")
                              ACE_TEXT("opening Transport[%d] in ")
                              ACE_TEXT("TAO_CLIENT_ROLE\n"),
                              base_transport->id ()));
                }

              // Call post connect hook. If the post_connect_hook () returns
              // false, just purge the entry.
              if (!base_transport->post_connect_hook ())
                {
                  if (TAO_debug_level > 4)
                    {
                      ACE_DEBUG ((LM_DEBUG,
                                  ACE_TEXT("TAO (%P|%t) - Post_connect_hook ")
                                  ACE_TEXT("failed. ")
                                  ACE_TEXT("Purging transport[%d]\n"),
                                  base_transport->id ()));
                    }
                  (void) base_transport->purge_entry ();
                }
              // The new transport is in the cache.  We'll pick it up from the
              // next time thru this loop (using it from here causes more
              // problems than it fixes due to the changes that allow a new
              // connection to be re-used by a nested upcall before we get back
              // here.)
              base_transport->remove_reference ();
            }
          else // not making new connection
            {
              (void) this->wait_for_transport (r, base_transport,
                                               timeout, true);
              base_transport->remove_reference ();
            }
        }
    }
}

bool
TAO_Connector::wait_for_connection_completion (
    TAO::Profile_Transport_Resolver *r,
    TAO_Transport_Descriptor_Interface &desc,
    TAO_Transport *&transport,
    ACE_Time_Value *timeout)
{
  int result = -1;
  if (transport->connection_handler ()->is_open ())
    {
      TAO::Transport_Cache_Manager &tcm =
        this->orb_core ()->lane_resources ().transport_cache ();
      result = tcm.cache_transport (&desc, transport);
      if (result == -1)
      {
        if (TAO_debug_level > 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("TAO (%P|%t) - Transport_Connector::")
                      ACE_TEXT("wait_for_connection_completion, ")
                      ACE_TEXT("transport [%d], Failed to cache transport.\n"),
                      transport->id ()));
        }
      }
    }
  else if (transport->connection_handler ()->is_timeout ())
    {
      if (TAO_debug_level > 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("TAO (%P|%t) - Transport_Connector::")
                      ACE_TEXT("wait_for_connection_completion, ")
                      ACE_TEXT("transport [%d], Connection timed out.\n"),
                      transport->id ()));
        }
      result = -1;
      errno = ETIME;
    }
  else if (transport->connection_handler ()->is_closed ())
    {
      if (TAO_debug_level > 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("TAO (%P|%t) - Transport_Connector::")
                      ACE_TEXT("wait_for_connection_completion, ")
                      ACE_TEXT("transport [%d], Connection failed. (%d) %p\n"),
                      transport->id (), ACE_ERRNO_GET, ACE_TEXT("")));
        }
      result = -1;
    }
  else
    {
      if (TAO_debug_level > 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("TAO (%P|%t) - Transport_Connector::")
                      ACE_TEXT("wait_for_connection_completion, ")
                      ACE_TEXT("transport [%d], Connection not complete.\n"),
                      transport->id ()));
        }

      TAO::Transport_Cache_Manager &tcm =
        this->orb_core ()->lane_resources ().transport_cache ();
      result = tcm.cache_transport (&desc, transport, TAO::ENTRY_CONNECTING);

      if (result != -1)
        {
          if (r->blocked_connect ())
            {
              if (TAO_debug_level > 2)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT("TAO (%P|%t) - Transport_Connector::")
                              ACE_TEXT("wait_for_connection_completion, ")
                              ACE_TEXT("going to wait for connection completion on ")
                              ACE_TEXT("transport[%d]\n"),
                              transport->id ()));
                }

              result = this->active_connect_strategy_->wait (transport, timeout);

              if (TAO_debug_level > 2)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT("TAO (%P|%t) - Transport_Connector::")
                              ACE_TEXT("wait_for_connection_completion, ")
                              ACE_TEXT("transport [%d], wait done result = %d\n"),
                              transport->id (), result));
                }


              // There are three possibilities when wait() returns: (a)
              // connection succeeded; (b) connection failed; (c) wait()
              // failed because of some other error.  It is easy to deal with
              // (a) and (b).  (c) is tricky since the connection is still
              // pending and may get completed by some other thread.  The
              // following code deals with (c).

              if (result == -1)
                {
                  if (errno == ETIME)
                    {
                      if (timeout == 0)
                        {
                          // There was an error during connecting and the errno was
                          // ETIME.  We didn't pass in a timeout, so there's
                          // something wrong with this transport.  So, it must be
                          // purged.
                          transport->purge_entry ();
                        }

                      if (TAO_debug_level > 2)
                        {
                          ACE_DEBUG ((LM_DEBUG,
                                      ACE_TEXT("TAO (%P|%t) - Transport_Connector::")
                                      ACE_TEXT("wait_for_connection_completion, ")
                                      ACE_TEXT("transport [%d], Connection timed out.\n"),
                                      transport->id ()));
                        }
                    }
                  else
                    {
                      // The wait failed for some other reason.
                      // Report that making the connection failed
                      if (TAO_debug_level > 2)
                        {
                          ACE_ERROR ((LM_ERROR,
                                      ACE_TEXT("TAO (%P|%t) - Transport_Connector::")
                                      ACE_TEXT("wait_for_connection_completion, ")
                                      ACE_TEXT("transport [%d], wait for completion failed")
                                      ACE_TEXT(" (%d) %p\n"),
                                      transport->id (), ACE_ERRNO_GET, ACE_TEXT("")));
                        }
                      TAO_Connection_Handler *con =
                        transport->connection_handler ();
                      result = this->check_connection_closure (con);
                      transport->purge_entry ();
                    }
                }
            }
          else //non-blocked connect (based on invocation, not connect strategy)
            {
              transport->connection_handler ()->
                reset_state (TAO_LF_Event::LFS_CONNECTION_WAIT);
              if (TAO_debug_level > 9)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT("TAO (%P|%t) - TAO_Connector[%d]::")
                              ACE_TEXT("wait_for_connection_completion reset_state to ")
                              ACE_TEXT("LFS_CONNECTION_WAIT\n"), transport->id ()));
                }
              result = 0;
            }
        }
    }

  if (result == -1)
    {
      // Set transport to zero, it is not usable, and the reference
      // count we added above was decremented by the base connector
      // handling the connection failure.
      transport = 0;
      return false;
    }
  // Connection not ready yet but we can use this transport, if
  // we need a connected one we will block later to make sure
  // it is connected
  return true;
}

void
TAO_Connector::cleanup_pending (TAO_Transport *&the_winner,
                                TAO_Transport **transport,
                                unsigned int count)
{
  // It is possible that we have more than one connection that happened
  // to complete, or that none completed. Therefore we need to traverse
  // the list and ensure that all of the losers are closed.
  for (unsigned int i = 0; i < count; i++)
    {
      if (transport[i] != the_winner)
        this->check_connection_closure (transport[i]->connection_handler());
      // since we are doing this on may connections, the result isn't
      // particularly important.
    }
}

bool
TAO_Connector::wait_for_connection_completion (
    TAO::Profile_Transport_Resolver *r,
    TAO_Transport *&the_winner,
    TAO_Transport **transport,
    unsigned int count,
    TAO_LF_Multi_Event *mev,
    ACE_Time_Value *timeout)
{
  if (TAO_debug_level > 2)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("TAO (%P|%t) - Transport_Connector::")
                  ACE_TEXT("wait_for_connection_completion, ")
                  ACE_TEXT("waiting for connection completion on ")
                  ACE_TEXT("%d transports, ["),
                  count));
      for (unsigned int i = 0; i < count; i++)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT("%d%C"),transport[i]->id (),
                    (i < (count -1) ? ", " : "]\n")));
    }

  int result = -1;
  if (r->blocked_connect ())
    {
      result = this->active_connect_strategy_->wait (mev, timeout);
      the_winner = 0;
    }
  else
    {
      errno = ETIME;
    }

  if (result != -1)
    {
      the_winner = mev->winner()->transport();
      if (TAO_debug_level > 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - Transport_Connector::")
                      ACE_TEXT ("wait_for_connection_completion, ")
                      ACE_TEXT ("transport [%d]\n"),
                      the_winner->id ()));
        }
    }
  else if (errno == ETIME)
    {
      // this is the most difficult case. In this situation, there is no
      // nominated by the Multi_Event. The best we can do is pick one of
      // the pending connections.
      // Of course, this shouldn't happen in any case, since the wait
      // strategy is called with a timeout value of 0.
      for (unsigned int i = 0; i < count; i++)
        if (!transport[i]->connection_handler()->is_closed())
          {
            the_winner = transport[i];
            break;
          }
    }

  this->cleanup_pending (the_winner, transport, count);

  // In case of errors.
  if (the_winner == 0)
    {
      // Report that making the connection failed, don't print errno
      // because we touched the reactor and errno could be changed
      if (TAO_debug_level > 2)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) - Transport_Connector::")
                      ACE_TEXT ("wait_for_connection_completion, failed\n")
                      ));
        }

      return false;
    }

  // Fix for a subtle problem. What happens if we are supposed to do
  // blocked connect but the transport is NOT connected? Force close
  // the connections
  if (r->blocked_connect () && !the_winner->is_connected ())
    {
      if (TAO_debug_level > 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT("TAO (%P|%t) - Transport_Connector::")
                      ACE_TEXT("wait_for_connection_completion, ")
                      ACE_TEXT("no connected transport for a blocked connection, ")
                      ACE_TEXT("cancelling connections and reverting things\n")));
        }

      // Forget the return value. We are busted anyway. Try our best
      // here.
      (void) this->cancel_svc_handler (the_winner->connection_handler ());
      the_winner = 0;
      return false;
    }

  // Connection may not ready for SYNC_NONE and SYNC_DELAYED_BUFFERING cases
  // but we can use this transport, if we need a connected one we will poll
  // later to make sure it is connected
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

bool
TAO_Connector::new_connection_is_ok (size_t busy_count)
{
  if (this->orb_core_ == 0)
    return true;

  unsigned int mux_limit = this->orb_core_->resource_factory ()
    ->max_muxed_connections ();

  return mux_limit == 0 || busy_count < mux_limit;
}

int
TAO_Connector::check_connection_closure (
  TAO_Connection_Handler *connection_handler)
{
  int result = -1;

  // Check if the handler has been closed.
  bool closed = connection_handler->is_closed ();

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
          const bool open = connection_handler->is_open ();

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

//@@ TAO_CONNECTOR_SPL_METHODS_ADD_HOOK

TAO_END_VERSIONED_NAMESPACE_DECL
