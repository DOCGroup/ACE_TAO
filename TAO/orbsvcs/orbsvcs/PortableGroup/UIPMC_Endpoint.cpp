#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/PortableGroup/UIPMC_Endpoint.h"
#include "orbsvcs/PortableGroup/UIPMC_Profile.h"

#include "tao/debug.h"
#include "ace/Guard_T.h"
#include "tao/ORB_Constants.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_Memory.h"
#include <cstring>
#include <cstdio>

#if !defined (__ACE_INLINE__)
# include "orbsvcs/PortableGroup/UIPMC_Endpoint.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_UIPMC_Endpoint::TAO_UIPMC_Endpoint ()
  : TAO_Endpoint (IOP::TAG_UIPMC),
    object_addr_ (),
    preferred_path_ (),
    preferred_if_ ()
{
}

TAO_UIPMC_Endpoint::TAO_UIPMC_Endpoint (ACE_INET_Addr const &addr)
  : TAO_Endpoint (IOP::TAG_UIPMC),
    object_addr_ (addr),
    preferred_path_ ()
{
  this->object_addr (addr);
}

TAO_UIPMC_Endpoint::~TAO_UIPMC_Endpoint () = default;

void
TAO_UIPMC_Endpoint::object_addr (ACE_INET_Addr const &addr)
{
  this->port_ = addr.get_port_number();
  char tmp[INET6_ADDRSTRLEN];
  addr.get_host_addr (tmp, sizeof tmp);
  this->host_ = CORBA::string_dup (tmp);
  this->object_addr_.set (addr);
}

char const *
TAO_UIPMC_Endpoint::host () const
{
  return this->host_.in ();
}

int
TAO_UIPMC_Endpoint::addr_to_string (char *buffer, size_t length)
{
  char tmp[INET6_ADDRSTRLEN];
  this->object_addr_.get_host_addr (tmp, sizeof tmp);

  size_t actual_len =
    std::strlen (tmp) // chars in host name
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
    std::sprintf (buffer, "[%s]:%u",
                     tmp,
                     this->port_);
  else
#endif /* ACE_HAS_IPV6 */
  std::sprintf (buffer, "%s:%u",
                   tmp,
                   this->port_);

  return 0;
}

TAO_Endpoint *
TAO_UIPMC_Endpoint::next ()
{
  return this->next_.get ();
}

TAO_Endpoint *
TAO_UIPMC_Endpoint::duplicate ()
{
  TAO_UIPMC_Endpoint *endpoint {};

  ACE_NEW_RETURN (endpoint,
                  TAO_UIPMC_Endpoint (this->object_addr_),
                  nullptr);

  return endpoint;
}

CORBA::Boolean
TAO_UIPMC_Endpoint::is_equivalent (TAO_Endpoint const *other_endpoint)
{
  TAO_UIPMC_Endpoint const *endpoint =
    dynamic_cast<TAO_UIPMC_Endpoint const *> (other_endpoint);

  if (endpoint == nullptr)
    return false;

  return
    (this->port_ == endpoint->port_
     && std::strcmp (this->host (), endpoint->host ()) == 0);
}

CORBA::ULong
TAO_UIPMC_Endpoint::hash ()
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
  std::vector<ACE_CString> preferred;
  TAO_IIOP_Endpoint::find_preferred_interfaces(this->host_.in(),
                                               oc->orb_params ()->preferred_interfaces (),
                                               preferred);

  TAO_UIPMC_Endpoint *latest = this;
  CORBA::ULong count = static_cast<CORBA::ULong> (preferred.size());
  CORBA::ULong i {};

  while (i < count)
    {
#if defined (ACE_HAS_IPV6)
      if (std::strstr (preferred[i].c_str(), "if=") != nullptr
            && this->object_addr_.get_type () == AF_INET6)
        {
          latest->preferred_if_ = CORBA::string_dup (preferred[i].c_str() + 3);
          latest->preferred_path_.host = static_cast<char const *> (nullptr);
          if (TAO_debug_level > 3)
            ORBSVCS_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - TAO_UIPMC_Endpoint::preferred_interfaces, setting network interface name <%C>"
                      " as preferred path for [%C] \n",
                      latest->preferred_if_.in(), this->host_.in ()));
        }
      else
#endif /* ACE_HAS_IPV6 */
        {
          latest->preferred_path_.host = CORBA::string_dup (preferred[i].c_str());

          if (TAO_debug_level > 3)
            ORBSVCS_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - TAO_UIPMC_Endpoint::preferred_interfaces, adding path [%C]"
                      " as preferred local address for [%C] \n",
                      latest->preferred_path_.host.in(), this->host_.in ()));
        }

      ++i;
      if (i < count)
        {
          std::unique_ptr<TAO_Endpoint> tmp_ep (latest->duplicate ());
          TAO_UIPMC_Endpoint * const next_ep =
            dynamic_cast<TAO_UIPMC_Endpoint *> (tmp_ep.get ());
          if (next_ep == nullptr)
            {
              return i;
            }

          tmp_ep.release ();
          latest->next_.reset (next_ep);
          latest = latest->next_.get ();
        }
    }

  if (count > 0 &&
      !oc->orb_params ()->enforce_pref_interfaces ())
    {
      std::unique_ptr<TAO_Endpoint> tmp_ep (latest->duplicate ());
      TAO_UIPMC_Endpoint * const next_ep =
        dynamic_cast<TAO_UIPMC_Endpoint *> (tmp_ep.get ());
      if (next_ep == nullptr)
        {
          return count;
        }

      tmp_ep.release ();
      latest->next_.reset (next_ep);
      latest->next_->preferred_path_.host = static_cast<char const *> (nullptr);
      ++count;
    }

  return count;
}

bool
TAO_UIPMC_Endpoint::is_preferred_network () const
{
  return (this->preferred_path_.host.in () != nullptr &&
          this->preferred_path_.host.in ()[0] != 0);
}

char const *
TAO_UIPMC_Endpoint::preferred_network () const
{
  return this->preferred_path_.host.in ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
