// This may look like C, but it's really -*- C++ -*-
// $Id$


#include "tao/IIOP_Endpoint.h"
#include "tao/debug.h"

ACE_RCSID(tao, IIOP_Endpoint, "$Id$")

#if !defined (__ACE_INLINE__)
# include "tao/IIOP_Endpoint.i"
#endif /* __ACE_INLINE__ */

TAO_IIOP_Endpoint::TAO_IIOP_Endpoint (const ACE_INET_Addr &addr,
                                      int use_dotted_decimal_addresses)
  : TAO_Endpoint (TAO_TAG_IIOP_PROFILE),
    host_ (),
    port_ (683),  // default port (IANA assigned)
    object_addr_ (addr),
    /* hint_ (0), */
    next_ (0)
{
  this->set (addr, use_dotted_decimal_addresses);
}

TAO_IIOP_Endpoint::TAO_IIOP_Endpoint (const char *host,
                                      CORBA::UShort port,
                                      const ACE_INET_Addr &addr)
  : TAO_Endpoint (TAO_TAG_IIOP_PROFILE),
    host_ (),
    port_ (port),
    object_addr_ (addr),
    /*    hint_ (0), */
    next_ (0)
{
  if (host != 0)
    this->host_ = host;

  this->object_addr_.set_type (-1);
}

TAO_IIOP_Endpoint::TAO_IIOP_Endpoint (void)
  : TAO_Endpoint (TAO_TAG_IIOP_PROFILE),
    host_ (),
    port_ (683),  // default port (IANA assigned)
    object_addr_ (),
    /*    hint_ (0), */
    next_ (0)
{
  this->object_addr_.set_type (-1);
}

TAO_IIOP_Endpoint::TAO_IIOP_Endpoint (const char *host,
                                      CORBA::UShort port,
                                      CORBA::Short priority)
  : TAO_Endpoint (TAO_TAG_IIOP_PROFILE),
    host_ (),
    port_ (port),
    object_addr_ (),
    /*    hint_ (0),*/
    next_ (0)
{
  if (host != 0)
    this->host_ = host;

  this->object_addr_.set_type (-1);
  this->priority (priority);
}

TAO_IIOP_Endpoint::~TAO_IIOP_Endpoint (void)
{

}

int
TAO_IIOP_Endpoint::set (const ACE_INET_Addr &addr,
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
                        ACE_TEXT ("IIOP_Endpoint::set ")
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
}

int
TAO_IIOP_Endpoint::addr_to_string (char *buffer, size_t length)
{
  size_t actual_len =
    ACE_OS::strlen (this->host_.in ()) // chars in host name
    + sizeof (':')                     // delimiter
    + ACE_OS::strlen ("65536")         // max port
    + sizeof ('\0');

  if (length < actual_len)
    return -1;

  ACE_OS::sprintf (buffer, "%s:%d",
                   this->host_.in (), this->port_);

  return 0;
}

const char *
TAO_IIOP_Endpoint::host (const char *h)
{
  this->host_ = h;

  return this->host_.in ();
}

void
TAO_IIOP_Endpoint::reset_hint (void)
{
  // Commented out for the time being....
  /*  if (this->hint_)
      this->hint_->cleanup_hint ((void **) &this->hint_); */
}

TAO_Endpoint *
TAO_IIOP_Endpoint::next (void)
{
  return this->next_;
}

TAO_Endpoint *
TAO_IIOP_Endpoint::duplicate (void)
{
  // @@ Bala, we probably need to make sure that the duplicate has the
  // same priority as the original.  Although it does not matter in
  // the context this method is currently used, if somebody ends up
  // using this method for some other purpose later, this will be a
  // seed for bugs.

  TAO_IIOP_Endpoint *endpoint = 0;

  ACE_NEW_RETURN (endpoint,
                  TAO_IIOP_Endpoint (this->host_.in (),
                                     this->port_,
                                     this->object_addr_),
                  0);

  return endpoint;
}

CORBA::Boolean
TAO_IIOP_Endpoint::is_equivalent (const TAO_Endpoint *other_endpoint)
{
  TAO_Endpoint *endpt = ACE_const_cast (TAO_Endpoint *,
                                        other_endpoint);

  TAO_IIOP_Endpoint *endpoint = ACE_dynamic_cast (TAO_IIOP_Endpoint *,
                                                  endpt);
  if (endpoint == 0)
    return 0;

  return
    (this->port_ == endpoint->port_)
    && (this->object_addr_.get_ip_address () ==
    endpoint->object_addr_.get_ip_address());
}

CORBA::ULong
TAO_IIOP_Endpoint::hash (void)
{
  // We could call ACE_INET_Addr::hash() since it does much the same
  // thing except that it converts the port from network byte order to
  // host byte order.  As such, this implementation is actually less
  // costly.
  return this->object_addr_.get_ip_address () + this->port_;
}
