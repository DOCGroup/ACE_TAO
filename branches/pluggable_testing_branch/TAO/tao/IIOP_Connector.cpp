// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//
// = FILENAME
//
// = DESCRIPTION
//
// = AUTHOR
//
// ============================================================================

#include "tao/IIOP_Connector.h"
#include "tao/IIOP_Profile.h"
#include "tao/GIOP.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Environment.h"

TAO_IIOP_Connector::TAO_IIOP_Connector (void)
  : TAO_Connector (TAO_IOP_TAG_INTERNET_IOP),
    base_connector_ ()
{
}

int
TAO_IIOP_Connector::open (TAO_Resource_Factory *trf,
                          ACE_Reactor *reactor)
{
  return this->base_connector_.open (reactor,
                                     trf->get_null_creation_strategy (),
                                     trf->get_cached_connect_strategy (),
                                     trf->get_null_activation_strategy ());
}

int
TAO_IIOP_Connector::close (void)
{
  this->base_connector_.close ();
  return 0;
}

int
TAO_IIOP_Connector::connect (TAO_Profile *profile,
                             TAO_Transport *& transport)
{
  if (profile->tag () != TAO_IOP_TAG_INTERNET_IOP)
    return -1;

  TAO_IIOP_Profile *iiop_profile =
    ACE_dynamic_cast (TAO_IIOP_Profile *, profile);

  if (iiop_profile == 0)
    return -1;

  const ACE_INET_Addr &oa = iiop_profile->object_addr ();

  TAO_Client_Connection_Handler* result;

  // the connect call will set the hint () stored in the Profile
  // object; but we obtain the transport in the <result>
  // variable. Other threads may modify the hint, but we are not
  // affected.
  if (this->base_connector_.connect (iiop_profile->hint (),
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
TAO_IIOP_Connector::preconnect (const char *preconnects)
{
  char *preconnections = ACE_OS::strdup (preconnects);

  // @@ Fred&Ossama: cleanup this code before the merge!
#if 0
  if (preconnections)
    {
      ACE_INET_Addr dest;
      TAO_Client_Connection_Handler *handler;
      ACE_Unbounded_Stack<TAO_Client_Connection_Handler *> handlers;

      char *nextptr = 0;
      char *where = 0;

      for (where = ACE::strsplit_r (preconnections, ",", nextptr);
           where != 0;
           where = ACE::strsplit_r (0, ",", nextptr))
        {
          char *tport = 0;
          char *thost = where;
          char *sep = ACE_OS::strchr (where, ':');

          if (sep)
            {
              *sep = '\0';
              tport = sep + 1;

              dest.set (ACE_OS::atoi (tport),
                        thost);

              // Try to establish the connection
              handler = 0;
              if (this->base_connector_.connect (handler, dest) == 0)
                // Save it for later so we can mark it as idle
                handlers.push (handler);
              else
                ACE_ERROR ((LM_ERROR,
                            "(%P|%t) Unable to preconnect to host '%s', port %d.\n",
                            dest.get_host_name (),
                            dest.get_port_number ()));
            }
          else
            ACE_ERROR ((LM_ERROR,
                        "(%P|%t) Yow!  Couldn't find a ':' separator in '%s' spec.\n",
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
      ACE_INET_Addr dest;
      ACE_Unbounded_Stack<ACE_INET_Addr> dests;

      char *nextptr = 0;
      char *where = 0;
      for (where = ACE::strsplit_r (preconnections, ",", nextptr);
           where != 0;
           where = ACE::strsplit_r (0, ",", nextptr))
        {
          char *tport = 0;
          char *thost = where;
          char *sep = ACE_OS::strchr (where, ':');

          if (sep)
            {
              *sep = '\0';
              tport = sep + 1;

              dest.set (ACE_OS::atoi (tport), thost);
              dests.push (dest);
            }
          else
            ACE_ERROR ((LM_ERROR,
                        "(%P|%t) Yow!  Couldn't find a ':' separator in '%s' spec.\n",
                        where));
        }

      // Create an array of addresses from the stack, as well as an
      // array of eventual handlers.
      size_t num_connections = dests.size ();
      ACE_INET_Addr *remote_addrs = 0;
      TAO_Client_Connection_Handler **handlers = 0;
      char *failures = 0;

      ACE_NEW_RETURN (remote_addrs,
                      ACE_INET_Addr[num_connections],
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
           index++)
        {
          if (!failures[index])
            {
              handlers[index]->idle ();
              successes++;
            }
        }
    }
#endif /* 0 */

  ACE_OS::free (preconnections);

  return successes;
}

TAO_Profile*
TAO_IIOP_Connector::create_profile (TAO_InputCDR& cdr)
{
  TAO_Profile* pfile;
  ACE_NEW_RETURN (pfile, TAO_IIOP_Profile, 0);
  
  int r = pfile->decode (cdr);
  if (r == -1)
    {
      pfile->_decr_refcnt ();
      pfile = 0;
    }

  return pfile;
}

int
TAO_IIOP_Connector::make_profile (const char *endpoint,
                                  TAO_Profile *&profile,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  // The endpoint should be of the form:
  //
  //    N.n//host:port/object_key
  //
  // or:
  //
  //    //host:port/object_key

  ACE_NEW_RETURN (profile,
                  TAO_IIOP_Profile (endpoint, ACE_TRY_ENV),
                  -1);

  return 0;  // Success
}


int
TAO_IIOP_Connector::check_prefix (const char *endpoint)
{
  // Check for a valid string
  if (!endpoint || !*endpoint)
    return -1;  // Failure

  const char *protocol[] = { "iiop", "iioploc" };
  // This is valid for any protocol beginning with `iiop' or `iioploc'.

  // Check for the proper prefix in the IOR.  If the proper prefix isn't
  // in the IOR then it is not an IOR we can use.
  if (ACE_OS::strcasecmp (endpoint, protocol[0]) == 0 ||
      ACE_OS::strcasecmp (endpoint, protocol[1]) == 0)
    {
      return 0;  // Success
    }

  return -1;
  // Failure: not an IIOP IOR
  // DO NOT throw an exception here.
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Node<ACE_INET_Addr>;
template class ACE_Unbounded_Stack<ACE_INET_Addr>;
template class ACE_Unbounded_Stack_Iterator<ACE_INET_Addr>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Node<ACE_INET_Addr>
#pragma instantiate ACE_Unbounded_Stack<ACE_INET_Addr>
#pragma instantiate ACE_Unbounded_Stack_Iterator<ACE_INET_Addr>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
