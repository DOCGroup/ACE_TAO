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

CORBA::ULong
TAO_UIOP_Connector::tag (void)
{
  return this->tag_;
}

TAO_UIOP_Connector::TAO_UIOP_Connector (void)
  : tag_(TAO_IOP_TAG_INTERNET_IOP),
    base_connector_ ()
{
}

int
TAO_UIOP_Connector::connect (TAO_Profile *profile,
                             TAO_Transport *& transport)
{
  if (profile->tag () != TAO_IOP_TAG_INTERNET_IOP)
    return -1;

  TAO_UIOP_Profile *uiop_profile =
    ACE_dynamic_cast (TAO_UIOP_Profile *, profile);

  if (uiop_profile == 0)
    return -1;

// Establish the connection and get back a <Client_Connection_Handler>.
// @@ We do not have the ORB core
// #if defined (TAO_ARL_USES_SAME_CONNECTOR_PORT)
//   if (this->orb_core_->arl_same_port_connect ())
//     {
//       ACE_UNIX_Addr local_addr (this->orb_core_->orb_params ()->addr ());
//       local_addr.set_port_number (server_addr_p->get_port_number ());
//
//       // Set the local port number to use.
//       if (con->connect (uiop_profile->hint (),
//                         uiop_profile->object_addr (),
//                         0,
//                         local_addr,
//                         1) == -1);
//       {
//         // Give users a clue to the problem.
//       ACE_DEBUG ((LM_ERROR, "(%P|%t) %s:%u, connection to "
//                    "%s failed (%p)\n",
//                    __FILE__,
//                    __LINE__,
//                    uiop_profile->addr_to_string (),
//                    "errno"));
//
//        TAO_THROW_ENV_RETURN_VOID (CORBA::TRANSIENT (), env);
//        }
//    }
//  else
//#endif /* TAO_ARL_USES_SAME_CONNECTOR_PORT */

  const ACE_UNIX_Addr &oa = uiop_profile->object_addr ();

  TAO_Client_Connection_Handler* result;

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
  // @@ Fred: why not just
  //
  // return this->base_connector_.open (....); ????
  //
  if (this->base_connector_.open (reactor,
                                  trf->get_null_creation_strategy (),
                                  trf->get_cached_connect_strategy (),
                                  trf->get_null_activation_strategy ()) != 0)
    return -1;
  return 0;
}

int
TAO_UIOP_Connector::close (void)
{
  this->base_connector_.close ();
  return 0;
}

int
TAO_UIOP_Connector::preconnect (char *preconnections)
{
#if 0
  if (preconnections)
    {
      ACE_UNIX_Addr dest;
      TAO_Client_Connection_Handler *handler;
      ACE_Unbounded_Stack<TAO_Client_Connection_Handler *> handlers;

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
      TAO_Client_Connection_Handler **handlers = 0;
      char *failures = 0;

      ACE_NEW_RETURN (remote_addrs,
                      ACE_UNIX_Addr[num_connections],
                      -1);
      ACE_NEW_RETURN (handlers,
                      TAO_Client_Connection_Handler *[num_connections],
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
  return successes;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Node<ACE_UNIX_Addr>;
template class ACE_Unbounded_Stack<ACE_UNIX_Addr>;
template class ACE_Unbounded_Stack_Iterator<ACE_UNIX_Addr>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Node<ACE_UNIX_Addr>
#pragma instantiate ACE_Unbounded_Stack<ACE_UNIX_Addr>
#pragma instantiate ACE_Unbounded_Stack_Iterator<ACE_UNIX_Addr>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* !ACE_LACKS_UNIX_DOMAIN_SOCKETS */
