// This may look like C, but it's really -*- C++ -*-
// $Id: NIOP_Endpoint.cpp,v 1.15 2006/05/16 12:35:39 jwillemsen Exp $


#include "tao/Strategies/NIOP_Endpoint.h"

#if defined (TAO_HAS_NIOP) && (TAO_HAS_NIOP != 0)

#include "tao/Strategies/NIOP_Connection_Handler.h"
#include "tao/debug.h"
#include "tao/ORB_Constants.h"

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"


ACE_RCSID (Strategies,
           NIOP_Endpoint,
           "$Id: NIOP_Endpoint.cpp,v 1.15 2006/05/16 12:35:39 jwillemsen Exp $")


#if !defined (__ACE_INLINE__)
# include "tao/Strategies/NIOP_Endpoint.inl"
#endif /* __ACE_INLINE__ */

#include "ace/os_include/os_netdb.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
/*
TAO_NIOP_Endpoint::TAO_NIOP_Endpoint (const ACE_INET_Addr &addr,
                                      int use_dotted_decimal_addresses)

  : TAO_Endpoint (TAO_TAG_NIOP_PROFILE)
    , host_ ()
    , port_ (0)
    , object_addr_ (addr)
    , object_addr_set_ (0)
    , next_ (0)
{
  this->set (addr, use_dotted_decimal_addresses);
}
*/
TAO_NIOP_Endpoint::TAO_NIOP_Endpoint (const char *host,
                                      CORBA::UShort port)
  //                                    const ACE_INET_Addr &addr
    //                                  CORBA::Short priority)
  : TAO_Endpoint (TAO_TAG_NIOP_PROFILE
                  )//priority)
    , host_ ()
    , port_ (port)
//    , object_addr_ (addr)
  //  , object_addr_set_ (0)
    , next_ (0)
{
  if (host != 0)
    this->host_ = host;
}

TAO_NIOP_Endpoint::TAO_NIOP_Endpoint (void)
  : TAO_Endpoint (TAO_TAG_NIOP_PROFILE),
    host_ (),
    port_ (0),
//    object_addr_ (),
  //  object_addr_set_ (0),
    next_ (0)
{
}
              /*
TAO_NIOP_Endpoint::TAO_NIOP_Endpoint (const char *host,
                                      CORBA::UShort port,
                                      CORBA::Short priority)
  : TAO_Endpoint (TAO_TAG_NIOP_PROFILE),
    host_ (),
    port_ (port),
    object_addr_ (),
    object_addr_set_ (0),
    next_ (0)
{
  if (host != 0)
    this->host_ = host;

  this->priority (priority);
}
                */
TAO_NIOP_Endpoint::~TAO_NIOP_Endpoint (void)
{

}

/*int
TAO_NIOP_Endpoint::set (const ACE_INET_Addr &addr,
                        int use_dotted_decimal_addresses)
{
  char tmp_host[MAXHOSTNAMELEN + 1];

  if (use_dotted_decimal_addresses
      || addr.get_host_name (tmp_host, sizeof (tmp_host)) != 0)
    {
      const char *tmp = addr.get_host_addr ();
      if (tmp == 0)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("\n\nTAO (%P|%t) ")
                        ACE_TEXT ("NIOP_Endpoint::set ")
                        ACE_TEXT ("- %p\n\n"),
                        ACE_TEXT ("cannot determine hostname")));
          return -1;
        }
      else
        this->host_ = tmp;
    }
  else
    this->host_ = CORBA::string_dup (tmp_host);

  this->port_ = addr.get_port_number();

  return 0;
} */

int
TAO_NIOP_Endpoint::addr_to_string (char *buffer, size_t length)
{
  size_t const actual_len =
    ACE_OS::strlen (this->host_.in ()) // chars in host name
    + sizeof (':')                     // delimiter
    + ACE_OS::strlen ("65536")         // max port
    + sizeof ('\0');

  if (length < actual_len)
    return -1;

  ACE_OS::sprintf (buffer, "%s:%d",
                   this->host_.in (), 0);//this->port_);

  return 0;
}

const char *
TAO_NIOP_Endpoint::host (const char *h)
{
  this->host_ = h;

  return this->host_.in ();
}

TAO_Endpoint *
TAO_NIOP_Endpoint::next (void)
{
  return 0;
}

TAO_Endpoint *
TAO_NIOP_Endpoint::duplicate (void)
{
  TAO_NIOP_Endpoint *endpoint = 0;

  ACE_NEW_RETURN (endpoint,
                  TAO_NIOP_Endpoint (this->host_.in (),
                                     this->port_),
                  0);

  return endpoint;
}

CORBA::Boolean
TAO_NIOP_Endpoint::is_equivalent (const TAO_Endpoint *other_endpoint)
{
  TAO_Endpoint *endpt = const_cast<TAO_Endpoint *> (other_endpoint);

  TAO_NIOP_Endpoint *endpoint = dynamic_cast<TAO_NIOP_Endpoint *> (endpt);
  if (endpoint == 0)
    return false;

  return (this->port () == endpoint->port ()
          && ACE_OS::strcmp(this->host (), endpoint->host()) == 0);
}

CORBA::ULong
TAO_NIOP_Endpoint::hash (void)
{
  if (this->hash_val_ != 0)
    return this->hash_val_;

  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                      guard,
                      this->addr_lookup_lock_,
                      this->hash_val_);
    // .. DCL
    if (this->hash_val_ != 0)
      return this->hash_val_;

    this->hash_val_ =
      ACE::hash_pjw (this->host ()) + this->port ();
  }

  return this->hash_val_;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_NIOP && TAO_HAS_NIOP != 0 */
