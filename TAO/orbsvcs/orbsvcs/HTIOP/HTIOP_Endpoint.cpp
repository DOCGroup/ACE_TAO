// $Id$

#include "orbsvcs/HTIOP/HTIOP_Endpoint.h"
#include "orbsvcs/HTIOP/HTIOP_Profile.h"

#include "ace/os_include/os_netdb.h"
#include "orbsvcs/Log_Macros.h"

#include "tao/IOPC.h"
#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
# include "orbsvcs/HTIOP/HTIOP_Endpoint.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::HTIOP::Endpoint::Endpoint (const ACE::HTBP::Addr &addr,
                                        int use_dotted_decimal_addresses)
  : TAO_Endpoint (OCI_TAG_HTIOP_PROFILE),
    host_ (),
    port_ (0),
    htid_ (),
    object_addr_ (addr),
    object_addr_set_ (0),
    next_ (0)
{
  this->set (addr, use_dotted_decimal_addresses);
}

TAO::HTIOP::Endpoint::Endpoint (const char *host,
                                        CORBA::UShort port,
                                        const char *htid,
                                        const ACE::HTBP::Addr &addr)
  : TAO_Endpoint (OCI_TAG_HTIOP_PROFILE),
    host_ (),
    port_ (port),
    htid_ (),
    object_addr_ (addr),
    object_addr_set_ (0),
    next_ (0)
{
  if (host != 0)
    this->host_ = host;
  if (htid != 0)
    this->htid_ = htid;
}

TAO::HTIOP::Endpoint::Endpoint (void)
  : TAO_Endpoint (OCI_TAG_HTIOP_PROFILE),
    host_ (),
    port_ (0),
    htid_ (),
    object_addr_ (),
    object_addr_set_ (0),
    next_ (0)
{
}

TAO::HTIOP::Endpoint::Endpoint (const char *host,
                                CORBA::UShort port,
                                const char *htid)
  : TAO_Endpoint (OCI_TAG_HTIOP_PROFILE),
    host_ (),
    port_ (port),
    htid_ (),
    object_addr_ (),
    object_addr_set_ (0),
    next_ (0)
{
  if (host != 0)
    this->host_ = host;
  if (htid != 0)
    this->htid_ = htid;
}

TAO::HTIOP::Endpoint::~Endpoint (void)
{
}

int
TAO::HTIOP::Endpoint::set (const ACE::HTBP::Addr &addr,
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
            ORBSVCS_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("\n\nTAO (%P|%t) ")
                        ACE_TEXT ("TAO::HTIOP::Endpoint::set ")
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
  this->htid_ = addr.get_htid();
  return 0;
}

int
TAO::HTIOP::Endpoint::addr_to_string (char *buffer, size_t length)
{
  if (this->port_ != 0)
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
    }
  else
    {
      if (length < ACE_OS::strlen (this->htid_.in())+1)
        return -1;
      ACE_OS::strcpy (buffer,this->htid_.in());
    }
  return 0;
}

const char *
TAO::HTIOP::Endpoint::host (const char *h)
{
  this->host_ = h;

  return this->host_.in ();
}

const char *
TAO::HTIOP::Endpoint::htid (const char *h)
{
  this->htid_ = h;

  return this->htid_.in ();
}

void
TAO::HTIOP::Endpoint::reset_hint (void)
{
  // Commented out for the time being....
  /*  if (this->hint_)
      this->hint_->cleanup_hint ((void **) &this->hint_); */
}

TAO_Endpoint *
TAO::HTIOP::Endpoint::next (void)
{
  return this->next_;
}

TAO_Endpoint *
TAO::HTIOP::Endpoint::duplicate (void)
{
  // @@ Bala, we probably need to make sure that the duplicate has the
  // same priority as the original.  Although it does not matter in
  // the context this method is currently used, if somebody ends up
  // using this method for some other purpose later, this will be a
  // seed for bugs.

  TAO::HTIOP::Endpoint *endpoint = 0;

  ACE_NEW_RETURN (endpoint,
                  TAO::HTIOP::Endpoint (this->host_.in (),
                                      this->port_,
                                      this->htid_.in(),
                                      this->object_addr_),
                  0);

  return endpoint;
}

CORBA::Boolean
TAO::HTIOP::Endpoint::is_equivalent (const TAO_Endpoint *other_endpoint)
{
  TAO_Endpoint *endpt =
    const_cast<TAO_Endpoint *> (other_endpoint);

  TAO::HTIOP::Endpoint *endpoint =
    dynamic_cast<TAO::HTIOP::Endpoint *> (endpt);
  if (endpoint == 0)
    return 0;

  if (this->htid_.in() && ACE_OS::strlen (this->htid_.in()))
    return (endpoint->htid() &&
            ACE_OS::strcmp (this->htid_.in(),endpoint->htid()) == 0);

  return (this->port_ == endpoint->port_
          && (ACE_OS::strcmp(this->host(), endpoint->host()) == 0));
}

CORBA::ULong
TAO::HTIOP::Endpoint::hash (void)
{
  if (this->htid_.in() && ACE_OS::strlen (this->htid_.in()))
    return ACE::hash_pjw(this->htid_.in());
  return this->object_addr ().hash();
}

TAO_END_VERSIONED_NAMESPACE_DECL
