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

#include "tao/IIOP_Acceptor.h"
#include "tao/IIOP_Profile.h"
#include "tao/MProfile.h"
#include "tao/ORB_Core.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/GIOP.h"
#include "tao/debug.h"

#if !defined(__ACE_INLINE__)
#include "tao/IIOP_Acceptor.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, IIOP_Acceptor, "$Id$")

// ****************************************************************

TAO_IIOP_Acceptor::TAO_IIOP_Acceptor (void)
  : TAO_Acceptor (TAO_IOP_TAG_INTERNET_IOP),
    base_acceptor_ (),
    version_ (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR),
    orb_core_ (0)
{
}

// TODO =
//    2) For V1.[1,2] there are tagged components
//    3) Create multiple profiles for wild carded endpoints (may be multiple
//       interfaces over which we can receive requests.  Thus a profile
//       must be made for each one.

int
TAO_IIOP_Acceptor::create_mprofile (const TAO_ObjectKey &object_key,
                                    TAO_MProfile &mprofile)
{
  // @@ we only make one for now
  int count = mprofile.profile_count ();
  if ((mprofile.size () - count) < 1)
    {
      if (mprofile.grow (count + 1) == -1)
        return -1;
    }

  TAO_IIOP_Profile *pfile;
  ACE_NEW_RETURN (pfile,
                  TAO_IIOP_Profile (this->host_.c_str (),
                                    this->address_.get_port_number (),
                                    object_key,
                                    this->address_,
                                    this->version_,
                                    this->orb_core_),
                  -1);

  if (mprofile.give_profile (pfile) == -1)
    return -1;

  return 0;
}

int
TAO_IIOP_Acceptor::is_collocated (const TAO_Profile* pfile)
{
  const TAO_IIOP_Profile *profile =
    ACE_dynamic_cast(const TAO_IIOP_Profile*, pfile);

  // compare the port and sin_addr (numeric host address)
  return profile->object_addr () == this->address_;
}

ACE_Event_Handler *
TAO_IIOP_Acceptor::acceptor (void)
{
  return &base_acceptor_;
}

int
TAO_IIOP_Acceptor::close (void)
{
  return this->base_acceptor_.close ();
}

int
TAO_IIOP_Acceptor::open (TAO_ORB_Core *orb_core,
                         int major,
                         int minor,
                         ACE_CString &address)
{
  if (major >=0 && minor >= 0)
    this->version_.set_version (ACE_static_cast (CORBA::Octet,major),
                                ACE_static_cast (CORBA::Octet,minor));
  ACE_INET_Addr addr (address.c_str ());

  return this->open_i (orb_core, addr);
}

int
TAO_IIOP_Acceptor::open_default (TAO_ORB_Core *orb_core)
{
  // @@ Until we can support multihomed machines correctly we must
  //    pick the "default interface" and only listen on that IP
  //    address.

  ACE_INET_Addr addr;
  char buffer[MAXHOSTNAMELEN + 1];
  if (addr.get_host_name (buffer, sizeof(buffer)) != 0)
    return -1;

  addr.set (u_short(0), buffer, 1);

  this->host_ = buffer;

  return this->open_i (orb_core, addr);
}

int
TAO_IIOP_Acceptor::open_i (TAO_ORB_Core* orb_core,
                           const ACE_INET_Addr& addr)
{
  this->orb_core_ = orb_core;

  if (this->base_acceptor_.open (orb_core, addr) == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "\n\nTAO (%P|%t) IIOP_Acceptor::open_i - %p\n\n",
                    "cannot open acceptor"));
      return -1;
    }

  // @@ Should this be a catastrophic error???
  if (this->base_acceptor_.acceptor ().get_local_addr (this->address_) != 0)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "\n\nTAO (%P|%t) IIOP_Acceptor::open_i - %p\n\n",
                    "cannot get local addr"));
      return -1;
    }

  if (orb_core->orb_params ()->use_dotted_decimal_addresses ())
    {
      const char *tmp;
      if ((tmp = addr.get_host_addr ()) == 0)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "\n\nTAO (%P|%t) IIOP_Acceptor::open_i - %p\n\n",
                        "cannot cache hostname"));
          return -1;
        }
      this->host_ = tmp;
    }
  else
    {
      char tmp_host[MAXHOSTNAMELEN+1];
      if (addr.get_host_name (tmp_host, sizeof(tmp_host)) != 0)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "\n\nTAO (%P|%t) IIOP_Acceptor::open_i - %p\n\n",
                        "cannot cache hostname"));
          return -1;
        }
      this->host_ = tmp_host;
    }

  if (TAO_debug_level > 5)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\nTAO (%P|%t) IIOP_Acceptor::open_i - "
                  "listening on: <%s:%u>\n",
                  this->host_.c_str (),
                  this->address_.get_port_number ()));
    }

  return 0;
}

CORBA::ULong
TAO_IIOP_Acceptor::endpoint_count (void)
{
  // @@ for now just assume one!
  // we should take a look at the local address, if it is zero then
  // get the list of available IP interfaces and return this number.
  return 1;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Acceptor<TAO_IIOP_Server_Connection_Handler, TAO_SOCK_ACCEPTOR>;
template class TAO_Acceptor_Impl<TAO_IIOP_Server_Connection_Handler, TAO_SOCK_ACCEPTOR>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Acceptor<TAO_IIOP_Server_Connection_Handler, TAO_SOCK_ACCEPTOR>
#pragma instantiate TAO_Acceptor_Impl<TAO_IIOP_Server_Connection_Handler, TAO_SOCK_ACCEPTOR>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
