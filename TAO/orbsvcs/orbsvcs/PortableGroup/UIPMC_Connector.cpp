// This may look like C, but it's really -*- C++ -*-
//
// $Id$

#include "UIPMC_Connector.h"

#include "ace/Connector.h"

#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "tao/Environment.h"
#include "tao/Base_Transport_Property.h"
#include "tao/Protocols_Hooks.h"
#include "tao/Invocation.h"

#include "UIPMC_Profile.h"

ACE_RCSID(tao, UIPMC_Connector, "$Id$")

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_NonBlocking_Connect_Handler<TAO_UIPMC_Connection_Handler>;

template class ACE_Map_Entry<ACE_INET_Addr, TAO_UIPMC_Connection_Handler *>;
template class ACE_Hash_Map_Iterator_Base_Ex < ACE_INET_Addr, TAO_UIPMC_Connection_Handler *, ACE_Hash < ACE_INET_Addr >, ACE_Equal_To < ACE_INET_Addr >, ACE_Null_Mutex >;
template class ACE_Hash_Map_Iterator_Ex<ACE_INET_Addr, TAO_UIPMC_Connection_Handler *, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_INET_Addr, TAO_UIPMC_Connection_Handler *, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_NonBlocking_Connect_Handler<TAO_UIPMC_Connection_Handler>

#pragma instantiate ACE_Map_Entry<ACE_INET_Addr, TAO_UIPMC_Connection_Handler *>;
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex < ACE_INET_Addr,TAO_UIPMC_Connection_Handler *, ACE_Hash < ACE_INET_Addr >, ACE_Equal_To < ACE_INET_Addr >, ACE_Null_Mutex >
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_INET_Addr, TAO_UIPMC_Connection_Handler *, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_INET_Addr, TAO_UIPMC_Connection_Handler *, ACE_Hash<ACE_INET_Addr>, ACE_Equal_To<ACE_INET_Addr>, ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

TAO_UIPMC_Connector::TAO_UIPMC_Connector (CORBA::Boolean)
  : TAO_Connector (TAO_TAG_UIPMC_PROFILE)
{
}

TAO_UIPMC_Connector::~TAO_UIPMC_Connector (void)
{
}

int
TAO_UIPMC_Connector::open (TAO_ORB_Core *orb_core)
{
  this->orb_core (orb_core);

  // For the sake of uniformity do
  if (this->create_connect_strategy () == -1)
    return -1;

  // @@ Michael: We do not use traditional connection management.

  return 0;
}

int
TAO_UIPMC_Connector::close (void)
{
  SvcHandlerIterator iter (svc_handler_table_);

  while (!iter.done ())
    {
      (*iter).int_id_->remove_reference ();
      iter++;
    }

  return 0;
}

int
TAO_UIPMC_Connector::set_validate_endpoint (TAO_Endpoint *endpoint)
{
  if (endpoint->tag () != TAO_TAG_UIPMC_PROFILE)
    return -1;

  TAO_UIPMC_Endpoint *uipmc_endpoint =
    ACE_dynamic_cast (TAO_UIPMC_Endpoint *,
                      endpoint );

  if (uipmc_endpoint == 0)
    return -1;

  const ACE_INET_Addr &remote_address =
    uipmc_endpoint->object_addr ();

  // Verify that the remote ACE_INET_Addr was initialized properly.
  // Failure can occur if hostname lookup failed when initializing the
  // remote ACE_INET_Addr.
  if (remote_address.get_type () != AF_INET)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) UIPMC connection failed.\n")
                      ACE_TEXT ("TAO (%P|%t) This is most likely ")
                      ACE_TEXT ("due to a hostname lookup ")
                      ACE_TEXT ("failure.\n")));
        }

      return -1;
    }

  return 0;
}

int
TAO_UIPMC_Connector::make_connection (TAO_GIOP_Invocation *invocation,
                                      TAO_Transport_Descriptor_Interface *desc,
                                      ACE_Time_Value * /*max_wait_time*/)

{
  TAO_Transport *&transport = invocation->transport ();

  TAO_UIPMC_Endpoint *uipmc_endpoint =
    ACE_dynamic_cast (TAO_UIPMC_Endpoint *,
                      desc->endpoint ());

  if (uipmc_endpoint == 0)
    return -1;

  const ACE_INET_Addr &remote_address =
    uipmc_endpoint->object_addr ();

  TAO_UIPMC_Connection_Handler *svc_handler = 0;

  if (this->svc_handler_table_.find (remote_address, svc_handler) == -1)
    {
      TAO_UIPMC_Connection_Handler *svc_handler_i = 0;
      ACE_NEW_RETURN (svc_handler_i,
                      TAO_UIPMC_Connection_Handler (this->orb_core (),
                                                   0 /* TAO_UIPMC_Properties */),
                      -1);

      svc_handler_i->local_addr (ACE_sap_any_cast (ACE_INET_Addr &));
      svc_handler_i->addr (remote_address);

      svc_handler_i->open (0);

      svc_handler_table_.bind (remote_address,
                               svc_handler_i);
      svc_handler = svc_handler_i;

      if (TAO_debug_level > 2)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) UIPMC_Connector::connect - ")
                    ACE_TEXT ("new connection on HANDLE %d\n"),
                    svc_handler->get_handle ()));
   }

  // @@ Michael: We do not use traditional connection management.
  svc_handler->add_reference ();
  transport = svc_handler->transport ();

  return 0;
}

TAO_Profile *
TAO_UIPMC_Connector::create_profile (TAO_InputCDR& cdr)
{
  TAO_Profile *pfile;
  ACE_NEW_RETURN (pfile,
                  TAO_UIPMC_Profile (this->orb_core ()),
                  0);

  int r = pfile->decode (cdr);
  if (r == -1)
    {
      pfile->_decr_refcnt ();
      pfile = 0;
    }

  return pfile;
}

TAO_Profile *
TAO_UIPMC_Connector::make_profile (ACE_ENV_SINGLE_ARG_DECL)
{
  // The endpoint should be of the form:
  //    N.n@host:port/object_key
  // or:
  //    host:port/object_key

  TAO_Profile *profile = 0;
  ACE_NEW_THROW_EX (profile,
                    TAO_UIPMC_Profile (this->orb_core ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  return profile;
}

int
TAO_UIPMC_Connector::check_prefix (const char *endpoint)
{
  // Check for a valid string
  if (!endpoint || !*endpoint)
    return -1;  // Failure

  const char *protocol[] = { "miop" };

  size_t slot = ACE_OS::strchr (endpoint, ':') - endpoint;
  size_t len0 = ACE_OS::strlen (protocol[0]);

  // Check for the proper prefix in the IOR.  If the proper prefix
  // isn't in the IOR then it is not an IOR we can use.
  if (slot == len0
      && ACE_OS::strncasecmp (endpoint, protocol[0], len0) == 0)
    return 0;

  return -1;
  // Failure: not a MIOP IOR
  // DO NOT throw an exception here.
}

char
TAO_UIPMC_Connector::object_key_delimiter (void) const
{
  return TAO_UIPMC_Profile::object_key_delimiter_;
}

int
TAO_UIPMC_Connector::init_uipmc_properties (void)
{
  // @@ Michael: We have no TCP, so we have no TCP properties.
  return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash <ACE_INET_Addr>;
template class ACE_Equal_To <ACE_INET_Addr>;
template class ACE_Hash_Map_Manager_Ex<ACE_INET_Addr, TAO_UIPMC_Connection_Handler *, ACE_Hash <ACE_INET_Addr>, ACE_Equal_To <ACE_INET_Addr>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<ACE_INET_Addr, TAO_UIPMC_Connection_Handler *>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash <ACE_INET_Addr>
#pragma instantiate ACE_Equal_To <ACE_INET_Addr>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_INET_Addr, TAO_UIPMC_Connection_Handler *, ACE_Hash <ACE_INET_Addr>, ACE_Equal_To <ACE_INET_Addr>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<ACE_INET_Addr, TAO_UIPMC_Connection_Handler *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
