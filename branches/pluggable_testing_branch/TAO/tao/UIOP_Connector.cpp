// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     UIOP_Connector.cpp
//
// = DESCRIPTION
//
// = AUTHOR
//     Fred Kuhns <fredk@cs.wustl.edu>
//     Ossama Othman <othman@cs.wustl.edu>
// ============================================================================

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

#include "tao/UIOP_Connector.h"
#include "tao/UIOP_Profile.h"
#include "tao/GIOP.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Environment.h"

TAO_UIOP_CACHED_CONNECT_STRATEGY TAO_UIOP_Connector::UIOP_Cached_Connect_Strategy_;
TAO_UIOP_NULL_CREATION_STRATEGY TAO_UIOP_Connector::UIOP_Null_Creation_Strategy_;
TAO_UIOP_NULL_ACTIVATION_STRATEGY TAO_UIOP_Connector::UIOP_Null_Activation_Strategy_;

TAO_UIOP_Connector::TAO_UIOP_Connector (void)
  : TAO_Connector (TAO_IOP_TAG_UNIX_IOP),
    base_connector_ ()
{
}

int
TAO_UIOP_Connector::connect (TAO_Profile *profile,
                             TAO_Transport *& transport)
{
  if (profile->tag () != TAO_IOP_TAG_UNIX_IOP)
    return -1;

  TAO_UIOP_Profile *uiop_profile =
    ACE_dynamic_cast (TAO_UIOP_Profile *, profile);

  if (uiop_profile == 0)
    return -1;

  const ACE_UNIX_Addr &oa = uiop_profile->object_addr ();

  TAO_UIOP_Client_Connection_Handler* result;

  // the connect call will set the hint () stored in the Profile
  // object; but we obtain the transport in the <result>
  // variable. Other threads may modify the hint, but we are not
  // affected.
  if (this->base_connector_.connect (uiop_profile->hint (),
                                     result,
                                     oa) == -1)
    { // Give users a clue to the problem.
      if (TAO_orbdebug)
        ACE_DEBUG ((LM_ERROR, "(%P|%t) %s:%u, connection to "
                    "%s failed (%p)\n",
                    __FILE__,
                    __LINE__,
                    profile->addr_to_string (),
                    "errno"));

      return -1;
    }

  transport = result->transport ();

  return 0;
}

int
TAO_UIOP_Connector::open (TAO_Resource_Factory *trf,
                          ACE_Reactor *reactor)
{
  return this->base_connector_.open (reactor,
                                     &UIOP_Null_Creation_Strategy_,
                                     &UIOP_Cached_Connect_Strategy_,
                                     &UIOP_Null_Activation_Strategy_);
}

int
TAO_UIOP_Connector::close (void)
{
  this->base_connector_.close ();
  return 0;
}

int
TAO_UIOP_Connector::preconnect (const char *preconnects)
{
  char *preconnections = ACE_OS::strdup (preconnects);

#if 0
  if (preconnections)
    {
      ACE_UNIX_Addr dest;
      TAO_UIOP_Client_Connection_Handler *handler;
      ACE_Unbounded_Stack<TAO_UIOP_Client_Connection_Handler *> handlers;

      char *nextptr = 0;
      char *where = 0;

      for (where = ACE::strsplit_r (preconnections, ",", nextptr);
           where != 0;
           where = ACE::strsplit_r (0, ",", nextptr))
        {
          char *trendezvous_point = where;
          char *sep = ACE_OS::strchr (where, ':');

          if (sep)
            {
              *sep = '\0';

              dest.set (trendezvous_point);

              // Try to establish the connection
              handler = 0;
              if (this->base_connector_.connect (handler, dest) == 0)
                // Save it for later so we can mark it as idle
                handlers.push (handler);
              else
                ACE_ERROR ((LM_ERROR,
                            "(%P|%t) Unable to preconnect to rendezvous point"
                            " '%s'.\n",
                            dest.get_path_name ()));
            }
          else
            ACE_ERROR ((LM_ERROR,
                        "(%P|%t) Couldn't find a ':' separator in '%s'"
                        " spec.\n",
                        where));
        }

      // Walk the stack of handlers and mark each one as idle now.
      handler = 0;
      while (handlers.pop (handler) == 0)
        handler->idle ();

    }
#else
  int successes = 0;
  if (preconnections)
    {
      ACE_UNIX_Addr dest;
      ACE_Unbounded_Stack<ACE_UNIX_Addr> dests;

      char *nextptr = 0;
      char *where = 0;
      for (where = ACE::strsplit_r (preconnections, ",", nextptr);
           where != 0;
           where = ACE::strsplit_r (0, ",", nextptr))
        {
          char *rendezvous_point = where;
          char *sep = ACE_OS::strchr (where, ':');

          if (sep)
            {
              *sep = '\0';

              dest.set (rendezvous_point);
              dests.push (dest);
            }
          else
            ACE_ERROR ((LM_ERROR,
                        "(%P|%t) Couldn't find a ':' separator in '%s'"
                        " spec.\n",
                        where));
        }

      // Create an array of addresses from the stack, as well as an
      // array of eventual handlers.
      size_t num_connections = dests.size ();
      ACE_UNIX_Addr *remote_addrs = 0;
      TAO_UIOP_Client_Connection_Handler **handlers = 0;
      char *failures = 0;

      ACE_NEW_RETURN (remote_addrs,
                      ACE_UNIX_Addr[num_connections],
                      -1);
      ACE_NEW_RETURN (handlers,
                      TAO_UIOP_Client_Connection_Handler *[num_connections],
                      -1);
      ACE_NEW_RETURN (failures,
                      char[num_connections],
                      -1);

      size_t index = 0;

      // Fill in the remote address array
      while (dests.pop (remote_addrs[index]) == 0)
        handlers[index++] = 0;

      // Finally, try to connect.
      this->base_connector_.connect_n (num_connections,
                                       handlers,
                                       remote_addrs,
                                       failures);
      // Loop over all the failures and set the handlers that
      // succeeded to idle state.
      for (index = 0;
           index < num_connections;
           ++index)
        {
          if (!failures[index])
            {
              handlers[index]->idle ();
              ++successes;
            }
        }
    }
#endif /* 0 */

  ACE_OS::free (preconnections);

  return successes;
}

int
TAO_UIOP_Connector::make_profile (const char *endpoint,
                                  TAO_Profile *&profile,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  // The endpoint should be of the form:
  //
  //    N.n//rendesvouz_point/object_key
  //
  // or:
  //
  //    //rendesvouz_point/object_key 

  ACE_NEW_RETURN (profile,
                  TAO_UIOP_Profile (endpoint, ACE_TRY_ENV),
                  -1);

  return 0;  // Success
}


int
TAO_UIOP_Connector::check_prefix (const char *endpoint)
{
  // Parse the given URL style IOR and create an mprofile from it.

  // Check for a valid string
  if (!endpoint || !*endpoint)
    return 1;  // Failure

  const char protocol[] = "uiop";
  // This is valid for any protocol beginning with `iiop'.


  // Check for the proper prefix in the IOR.  If the proper prefix isn't
  // in the IOR then it is not an IOR we can use.
  if (ACE_OS::strncasecmp (endpoint,
                           protocol,
                           ACE_OS::strlen (protocol)) == 0)
    {
      return 0;  // Success
    }
  else
    {
      return 1;
      // Failure: not an UIOP IOR
      // DO NOT throw an exception here.
    }
}


# if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Node<ACE_UNIX_Addr>;
template class ACE_Unbounded_Stack<ACE_UNIX_Addr>;
template class ACE_Unbounded_Stack_Iterator<ACE_UNIX_Addr>;

template class ACE_Connector<TAO_UIOP_Client_Connection_Handler, ACE_LSOCK_CONNECTOR>;
template class ACE_Connect_Strategy<TAO_UIOP_Client_Connection_Handler, ACE_LSOCK_CONNECTOR>;
template class ACE_Cached_Connect_Strategy<TAO_UIOP_Client_Connection_Handler, ACE_LSOCK_CONNECTOR, TAO_Cached_Connector_Lock>;
template class ACE_Strategy_Connector<TAO_UIOP_Client_Connection_Handler,
                               ACE_LSOCK_CONNECTOR>;

template class ACE_Concurrency_Strategy<TAO_UIOP_Client_Connection_Handler>;
template class ACE_Creation_Strategy<TAO_UIOP_Client_Connection_Handler>;

template class ACE_NOOP_Creation_Strategy<TAO_UIOP_Client_Connection_Handler>;
template class ACE_NOOP_Concurrency_Strategy<TAO_UIOP_Client_Connection_Handler>;
template class ACE_Recycling_Strategy<TAO_UIOP_Client_Connection_Handler>;

# elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Node<ACE_UNIX_Addr>
#pragma instantiate ACE_Unbounded_Stack<ACE_UNIX_Addr>
#pragma instantiate ACE_Unbounded_Stack_Iterator<ACE_UNIX_Addr>

#pragma instantiate ACE_Connector<TAO_UIOP_Client_Connection_Handler, ACE_LSOCK_CONNECTOR>
#pragma instantiate ACE_Connect_Strategy<TAO_UIOP_Client_Connection_Handler, ACE_LSOCK_CONNECTOR>
#pragma instantiate ACE_Cached_Connect_Strategy<TAO_UIOP_Client_Connection_Handler, ACE_LSOCK_CONNECTOR, TAO_Cached_Connector_Lock>
#pragma instantiate ACE_Strategy_Connector<TAO_UIOP_Client_Connection_Handler,
                               ACE_LSOCK_CONNECTOR>

#pragma instantiate ACE_Concurrency_Strategy<TAO_UIOP_Client_Connection_Handler>
#pragma instantiate ACE_Creation_Strategy<TAO_UIOP_Client_Connection_Handler>

#pragma instantiate ACE_NOOP_Creation_Strategy<TAO_UIOP_Client_Connection_Handler>
#pragma instantiate ACE_NOOP_Concurrency_Strategy<TAO_UIOP_Client_Connection_Handler>

#pragma instantiate ACE_Recycling_Strategy<TAO_UIOP_Client_Connection_Handler>

# endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* !ACE_LACKS_UNIX_DOMAIN_SOCKETS */
