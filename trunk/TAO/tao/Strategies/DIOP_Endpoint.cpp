// This may look like C, but it's really -*- C++ -*-
// $Id$


#include "DIOP_Endpoint.h"

#if defined (TAO_HAS_DIOP) && (TAO_HAS_DIOP != 0)

#include "DIOP_Connection_Handler.h"
#include "tao/debug.h"

ACE_RCSID(tao, DIOP_Endpoint, "$Id$")


#if !defined (__ACE_INLINE__)
# include "DIOP_Endpoint.i"
#endif /* __ACE_INLINE__ */

TAO_DIOP_Endpoint::TAO_DIOP_Endpoint (const ACE_INET_Addr &addr,
                                      int use_dotted_decimal_addresses)
  : TAO_Endpoint (TAO_TAG_UDP_PROFILE),
    host_ (),
    port_ (0),
    object_addr_ (addr),
    /*    hint_ (0), */
    next_ (0)
{
  this->set (addr, use_dotted_decimal_addresses);
}

TAO_DIOP_Endpoint::TAO_DIOP_Endpoint (const char *host,
                                      CORBA::UShort port,
                                      const ACE_INET_Addr &addr)
  : TAO_Endpoint (TAO_TAG_UDP_PROFILE),
    host_ (),
    port_ (port),
    object_addr_ (addr),
    /*    hint_ (0), */
    next_ (0)
{
  if (host != 0)
    this->host_ = host;
}

TAO_DIOP_Endpoint::TAO_DIOP_Endpoint (void)
  : TAO_Endpoint (TAO_TAG_UDP_PROFILE),
    host_ (),
    port_ (0),
    object_addr_ (),
    /*    hint_ (0), */
    next_ (0)
{
}

TAO_DIOP_Endpoint::TAO_DIOP_Endpoint (const char *host,
                                      CORBA::UShort port,
                                      CORBA::Short priority)
  : TAO_Endpoint (TAO_TAG_UDP_PROFILE),
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

TAO_DIOP_Endpoint::~TAO_DIOP_Endpoint (void)
{

}

int
TAO_DIOP_Endpoint::set (const ACE_INET_Addr &addr,
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
                        ACE_TEXT ("DIOP_Endpoint::set ")
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
TAO_DIOP_Endpoint::addr_to_string (char *buffer, size_t length)
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
TAO_DIOP_Endpoint::host (const char *h)
{
  this->host_ = h;

  return this->host_.in ();
}

void
TAO_DIOP_Endpoint::reset_hint (void)
{
  // Commented out for the time being....
  /*  if (this->hint_)
      this->hint_->cleanup_hint ((void **) &this->hint_); */
}

TAO_Endpoint *
TAO_DIOP_Endpoint::next (void)
{
  return this->next_;
}

TAO_Endpoint *
TAO_DIOP_Endpoint::duplicate (void)
{
  // @@ Bala, we probably need to make sure that the duplicate has the
  // same priority as the original.  Although it does not matter in
  // the context this method is currently used, if somebody ends up
  // using this method for some other purpose later, this will be a
  // seed for bugs.

  TAO_DIOP_Endpoint *endpoint = 0;

  ACE_NEW_RETURN (endpoint,
                  TAO_DIOP_Endpoint (this->host_.in (),
                                     this->port_,
                                     this->object_addr_),
                  0);

  return endpoint;
}

CORBA::Boolean
TAO_DIOP_Endpoint::is_equivalent (const TAO_Endpoint *other_endpoint)
{
  TAO_Endpoint *endpt = ACE_const_cast (TAO_Endpoint *,
                                        other_endpoint);

  TAO_DIOP_Endpoint *endpoint = ACE_dynamic_cast (TAO_DIOP_Endpoint *,
                                                  endpt);
  if (endpoint == 0)
    return 0;

  return (this->port () == endpoint->port ()
          && ACE_OS::strcmp(this->host (), endpoint->host()) == 0);
}

CORBA::ULong
TAO_DIOP_Endpoint::hash (void)
{
  return ACE::hash_pjw (this->host ()) + this->port ();
}

#endif /* TAO_HAS_DIOP && TAO_HAS_DIOP != 0 */
