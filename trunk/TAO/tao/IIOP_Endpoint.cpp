#include "IIOP_Endpoint.h"
#include "IOP_IORC.h"
#include "debug.h"
#include "ORB_Core.h"

#include "ace/Log_Msg.h"
#include "ace/Guard_T.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_strings.h"

ACE_RCSID (tao,
           IIOP_Endpoint,
           "$Id$")

#if !defined (__ACE_INLINE__)
# include "tao/IIOP_Endpoint.i"
#endif /* __ACE_INLINE__ */

#include "ace/OS_NS_stdio.h"
#include "ace/os_include/os_netdb.h"

TAO_IIOP_Endpoint::TAO_IIOP_Endpoint (const ACE_INET_Addr &addr,
                                      int use_dotted_decimal_addresses)
  : TAO_Endpoint (IOP::TAG_INTERNET_IOP)
  , host_ ()
  , port_ (683) // default port (IANA assigned)
  , object_addr_ (addr)
  , object_addr_set_ (false)
  , preferred_path_ ()
  , next_ (0)
{
  this->set (addr, use_dotted_decimal_addresses);
}

TAO_IIOP_Endpoint::TAO_IIOP_Endpoint (const char *host,
                                      CORBA::UShort port,
                                      const ACE_INET_Addr &addr,
                                      CORBA::Short priority)
  : TAO_Endpoint (IOP::TAG_INTERNET_IOP,
                  priority)
  , host_ (host)
  , port_ (port)
  , object_addr_ (addr)
  , object_addr_set_ (false)
  , preferred_path_ ()
  , next_ (0)
{
}

TAO_IIOP_Endpoint::TAO_IIOP_Endpoint (void)
  : TAO_Endpoint (IOP::TAG_INTERNET_IOP)
  , host_ ()
  , port_ (683)  // default port (IANA assigned)
  , object_addr_ ()
  , object_addr_set_ (false)
  , preferred_path_ ()
  , next_ (0)
{
}

TAO_IIOP_Endpoint::TAO_IIOP_Endpoint (const char *host,
                                      CORBA::UShort port,
                                      CORBA::Short priority)
  : TAO_Endpoint (IOP::TAG_INTERNET_IOP)
  , host_ ()
  , port_ (port)
  , object_addr_ ()
  , object_addr_set_ (false)
  , preferred_path_ ()
  , next_ (0)
{
  if (host != 0)
    this->host_ = host;

  this->priority (priority);
}

TAO_IIOP_Endpoint::~TAO_IIOP_Endpoint (void)
{
}

TAO_IIOP_Endpoint::TAO_IIOP_Endpoint (const TAO_IIOP_Endpoint &rhs)
  : TAO_Endpoint (rhs.tag_,
                  rhs.priority_)
  , host_ (rhs.host_)
  , port_ (rhs.port_)
  , object_addr_ (rhs.object_addr_)
  , object_addr_set_ (rhs.object_addr_set_)
  , preferred_path_  (rhs.preferred_path_)
  , next_ (0)
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
      if (use_dotted_decimal_addresses == 0 && TAO_debug_level > 5)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) ")
                      ACE_TEXT ("IIOP_Endpoint::set ")
                      ACE_TEXT ("- %p "),
                      ACE_TEXT ("cannot determine hostname\n")));
        }

      const char *tmp = addr.get_host_addr ();
      if (tmp == 0)
        {
          if (TAO_debug_level > 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("TAO (%P|%t) ")
                          ACE_TEXT ("IIOP_Endpoint::set ")
                          ACE_TEXT ("- %p "),
                          ACE_TEXT ("cannot determine hostname and hostaddr\n")));
            }
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

TAO_Endpoint *
TAO_IIOP_Endpoint::next (void)
{
  return this->next_;
}

TAO_Endpoint *
TAO_IIOP_Endpoint::duplicate (void)
{
  TAO_IIOP_Endpoint *endpoint = 0;

  // @@ NOTE: Not exception safe..
  ACE_NEW_RETURN (endpoint,
                  TAO_IIOP_Endpoint (*this),
                  0);

  return endpoint;
}

const ACE_INET_Addr &
TAO_IIOP_Endpoint::object_addr (void) const
{
  // The object_addr_ is initialized here, rather than at IOR decode
  // time for several reasons:
  //   1. A request on the object may never be invoked.
  //   2. The DNS setup may have changed dynamically.
  //   ...etc..

  // Double checked locking optimization.
  if (!this->object_addr_set_)
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                        guard,
                        this->addr_lookup_lock_,
                        this->object_addr_);

      if (!this->object_addr_set_)
        {
          (void) this->object_addr_i ();
        }
    }

  return this->object_addr_;
}

void
TAO_IIOP_Endpoint::object_addr_i (void) const
{
  // We should have already held the lock

  if (this->object_addr_.set (this->port_,
                              this->host_.in ()) == -1)
    {
      // If this call fails, it most likely due a hostname
      // lookup failure caused by a DNS misconfiguration.  If
      // a request is made to the object at the given host and
      // port, then a CORBA::TRANSIENT() exception should be
      // thrown.

      // Invalidate the ACE_INET_Addr.  This is used as a flag
      // to denote that ACE_INET_Addr initialization failed.
      this->object_addr_.set_type (-1);
    }
  else
    {
      this->object_addr_set_ = true;
    }
}

CORBA::ULong
TAO_IIOP_Endpoint::preferred_interfaces (TAO_ORB_Core *oc)
{
  ACE_CString tmp (
    oc->orb_params ()->preferred_interfaces ());

  ssize_t pos = 0;

  pos = tmp.find (this->host_.in ());

  TAO_IIOP_Endpoint *latest = this;

  CORBA::ULong count = 0;

  while (pos != ACE_CString::npos)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Pos value is [%d] \n",
                  pos));

      // Do we have a "," or an '\0'?
      ssize_t new_pos = tmp.find (",",
                                  pos + 1);

      // Length of the preferred path
      int length = 0;

      if (new_pos == ACE_CString::npos)
        length = tmp.length () - pos;
      else
        length = new_pos - pos;

      ACE_CString rem_tmp = tmp.substr (pos, length);

      // Search for the ":"
      ssize_t col_pos = rem_tmp.find (":");

      if (col_pos == ACE_CString::npos) continue;

      ACE_CString path = rem_tmp.substr (col_pos + 1);

      latest->preferred_path_.host =
        CORBA::string_dup (path.c_str ());

      if (TAO_debug_level > 3)
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) Adding path [%s] "
                    " as preferred path for [%s] \n",
                    path.c_str (), this->host_.in ()));

      pos = tmp.find (latest->host_.in (),
                      pos + length);

      if (pos != ACE_CString::npos)
        {
          TAO_Endpoint *tmp_ep =
            latest->duplicate ();

          latest->next_ = dynamic_cast<TAO_IIOP_Endpoint *> (tmp_ep);

          if (latest->next_ == 0) return count;

          latest = latest->next_;
          ++count;
        }
    }

  if (tmp.c_str () != 0 &&
      !oc->orb_params ()->enforce_pref_interfaces ())
    {
      TAO_Endpoint *tmp_ep = latest->duplicate ();

      latest->next_ =
        dynamic_cast<TAO_IIOP_Endpoint *> (tmp_ep);

      if (latest->next_ == 0) return count;

      latest->next_->preferred_path_.host = (const char *) 0;
      ++count;
    }

  return count;
}

CORBA::Boolean
TAO_IIOP_Endpoint::is_equivalent (const TAO_Endpoint *other_endpoint)
{
  const TAO_IIOP_Endpoint *endpoint =
    dynamic_cast<const TAO_IIOP_Endpoint *> (other_endpoint);

  if (endpoint == 0)
    return 0;

  return (this->port_ == endpoint->port_
          && (ACE_OS::strcmp (this->host (), endpoint->host ()) == 0));
}

CORBA::ULong
TAO_IIOP_Endpoint::hash (void)
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

    // A few comments about this optimization. The call below will
    // deadlock if the object_addr_set is false. If you don't belive

    if (!this->object_addr_set_)
      {
        // Set the object_addr first
        (void) this->object_addr_i ();
      }

    this->hash_val_ =
      this->object_addr_.get_ip_address () + this->port ();
  }

  return this->hash_val_;
}

bool
TAO_IIOP_Endpoint::is_preferred_network (void) const
{
  return (this->preferred_path_.host.in () != 0);
}

const char *
TAO_IIOP_Endpoint::preferred_network (void) const
{
  return this->preferred_path_.host.in ();
}
