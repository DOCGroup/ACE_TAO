// $Id$

#include "orbsvcs/PortableGroup/UIPMC_Endpoint.h"
#include "orbsvcs/PortableGroup/UIPMC_Profile.h"

#include "tao/debug.h"
#include "ace/Guard_T.h"
#include "tao/ORB_Constants.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_Memory.h"

#if !defined (__ACE_INLINE__)
# include "orbsvcs/PortableGroup/UIPMC_Endpoint.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_UIPMC_Endpoint::TAO_UIPMC_Endpoint (void)
  : TAO_Endpoint (IOP::TAG_UIPMC),
    host_ (),
    port_ (0),
    object_addr_ (),
    preferred_path_ (),
    next_ (0),
    preferred_if_()
{
  this->uint_ip_addr (0u);
}

TAO_UIPMC_Endpoint::TAO_UIPMC_Endpoint (const ACE_INET_Addr &addr)
  : TAO_Endpoint (IOP::TAG_UIPMC),
    host_ (),
    port_ (0),
    object_addr_ (addr),
    preferred_path_ (),
    next_ (0),
    preferred_if_()
{
  this->object_addr (addr);
}

// Use of this ctr must be avoided
TAO_UIPMC_Endpoint::TAO_UIPMC_Endpoint (const CORBA::Octet class_d_address[4],
                                        CORBA::UShort port)
  : TAO_Endpoint (IOP::TAG_UIPMC),
    port_ (port),
    preferred_path_ (),
    next_ (0),
    preferred_if_()
{
  for (int i = 0; i<4; i++)
    this->class_d_address_[i] = class_d_address[i];

  this->update_object_addr ();
}

TAO_UIPMC_Endpoint::~TAO_UIPMC_Endpoint (void)
{
}

void
TAO_UIPMC_Endpoint::object_addr (const ACE_INET_Addr &addr)
{
  this->port_ = addr.get_port_number ();
  char tmp[INET6_ADDRSTRLEN];
  addr.get_host_addr (tmp, sizeof tmp);
  this->host_ = CORBA::string_dup (tmp);
  this->object_addr_.set (addr);
  this->uint_ip_addr (addr.get_ip_address ());
}

const char *
TAO_UIPMC_Endpoint::host (void) const
{
  return this->host_.in ();
}

int
TAO_UIPMC_Endpoint::addr_to_string (char *buffer, size_t length)
{
  char tmp[INET6_ADDRSTRLEN];
  this->object_addr_.get_host_addr (tmp, sizeof tmp);

  size_t actual_len =
    ACE_OS::strlen (tmp) // chars in host name
    + sizeof (':')       // delimiter
    + 5u                 // max port
    + sizeof ('\0');

#if defined (ACE_HAS_IPV6)
  if (this->object_addr_.get_type () == AF_INET6)
    actual_len += 2; // '[' + ']'
#endif /* ACE_HAS_IPV6 */

  if (length < actual_len)
    return -1;

#if defined (ACE_HAS_IPV6)
  if (this->object_addr_.get_type () == AF_INET6)
    ACE_OS::sprintf (buffer, "[%s]:%u",
                     tmp,
                     this->port_);
  else
#endif /* ACE_HAS_IPV6 */
  ACE_OS::sprintf (buffer, "%s:%u",
                   tmp,
                   this->port_);

  return 0;
}

TAO_Endpoint *
TAO_UIPMC_Endpoint::next (void)
{
  return this->next_;
}

TAO_Endpoint *
TAO_UIPMC_Endpoint::duplicate (void)
{
  TAO_UIPMC_Endpoint *endpoint = 0;

  ACE_NEW_RETURN (endpoint,
                  TAO_UIPMC_Endpoint (this->object_addr_),
                  0);

  return endpoint;
}

CORBA::Boolean
TAO_UIPMC_Endpoint::is_equivalent (const TAO_Endpoint *other_endpoint)
{
  const TAO_UIPMC_Endpoint *endpoint =
    dynamic_cast<const TAO_UIPMC_Endpoint *> (other_endpoint);

  if (endpoint == 0)
    return 0;

  return
    (this->port_ == endpoint->port_
     && ACE_OS::strcmp (this->host (), endpoint->host ()) == 0);
}

CORBA::ULong
TAO_UIPMC_Endpoint::hash (void)
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

    this->hash_val_ = this->object_addr_.hash ();
  }

  return this->hash_val_;
}

CORBA::ULong
TAO_UIPMC_Endpoint::preferred_interfaces (TAO_ORB_Core *oc)
{
  ACE_Vector<ACE_CString> preferred;
  TAO_IIOP_Endpoint::find_preferred_interfaces(this->host_.in(),
                                               oc->orb_params ()->preferred_interfaces (),
                                               preferred);

  TAO_UIPMC_Endpoint *latest = this;
  CORBA::ULong count = preferred.size();
  CORBA::ULong i = 0;

  while (i < count)
    {
#if defined (ACE_HAS_IPV6)
      if (ACE_OS::strstr (preferred[i].c_str(), "if=") != 0
            && this->object_addr_.get_type () == AF_INET6)
        {
          latest->preferred_if_ = CORBA::string_dup (preferred[i].c_str() + 3);
          latest->preferred_path_.host = (const char *) 0;
          if (TAO_debug_level > 3)
            ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - TAO_UIPMC_Endpoint::preferred_interfaces, setting network interface name <%s>"
                      " as preferred path for [%s] \n",
                      latest->preferred_if_.in(), this->host_.in ()));
        }
      else
#endif /* ACE_HAS_IPV6 */
        {
          latest->preferred_path_.host =
            CORBA::string_dup (preferred[i].c_str());

          if (TAO_debug_level > 3)
            ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - TAO_UIPMC_Endpoint::preferred_interfaces, adding path [%s]"
                      " as preferred local address for [%s] \n",
                      latest->preferred_path_.host.in(), this->host_.in ()));
        }

      ++i;
      if (i < count)
        {
          TAO_Endpoint *tmp_ep =
            latest->duplicate ();
          latest->next_ = dynamic_cast<TAO_UIPMC_Endpoint *> (tmp_ep);
          if (latest->next_ == 0) return i;
          latest = latest->next_;
        }
    }

  if (count > 0 &&
      !oc->orb_params ()->enforce_pref_interfaces ())
    {
      TAO_Endpoint *tmp_ep = latest->duplicate ();

      latest->next_ =
        dynamic_cast<TAO_UIPMC_Endpoint *> (tmp_ep);

      if (latest->next_ == 0) return count;

      latest->next_->preferred_path_.host = (const char *) 0;
      ++count;
    }

  return count;
}

bool
TAO_UIPMC_Endpoint::is_preferred_network (void) const
{
  return (this->preferred_path_.host.in () != 0 &&
          this->preferred_path_.host.in ()[0] != 0);
}

const char *
TAO_UIPMC_Endpoint::preferred_network (void) const
{
  return this->preferred_path_.host.in ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
