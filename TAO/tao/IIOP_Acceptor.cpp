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

ACE_RCSID(tao, IIOP_Acceptor, "$Id$")

// ****************************************************************

TAO_IIOP_Acceptor::TAO_IIOP_Acceptor (void)
  : TAO_Acceptor (TAO_IOP_TAG_INTERNET_IOP),
    base_acceptor_ ()
{
}

// @@ Fred&Ossama: Maybe not for the current round of changes, but
//    shouldn't the acceptor know which version to create?
//    And isn't this the right place to setup the tagged components of
//    a v1.[12] profile?

// @@ Fred&Ossama: We need to check this interface: a single
//    TAO_Acceptor may be bound to multiple addresses (think of a
//    multihomed machine with an acceptor listening on the wildcard
//    address), hence the "Right Thing" seems to be that we pass an
//    MProfile that is filled up by the TAO_Acceptor class.
// @@ Right, I agree but for now we assume single endpoint.  fredk

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
                                    this->address_),
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

  // @@ We should probably cache this value, but then again some
  //    acceptors have multiple addresses.
  // @@ Fred: any ideas on how to optimize that?
  ACE_INET_Addr address;
  if (this->base_acceptor_.acceptor ().get_local_addr (address) == -1)
    return 0;

  // @@ Ossama: can you verify that this operator does the right thing?
  return profile->object_addr () == address;
}

ACE_Event_Handler *
TAO_IIOP_Acceptor::acceptor (void)
{
  return &base_acceptor_;
}

int
TAO_IIOP_Acceptor::open (TAO_ORB_Core *orb_core,
                         ACE_CString &address)
{
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

  return this->open_i (orb_core, addr);
}

int
TAO_IIOP_Acceptor::open_i (TAO_ORB_Core* orb_core,
                           const ACE_INET_Addr& addr)
{
  if (this->base_acceptor_.open (orb_core, addr) == -1)
    return -1;

  // @@ Should this be a catastrophic error???
  if (this->base_acceptor_.acceptor ().get_local_addr (this->address_) != 0)
    return -1;

  if (orb_core->orb_params ()->use_dotted_decimal_addresses ())
    {
      const char *tmp;
      if ((tmp = addr.get_host_addr ()) == 0)
        return -1;
      this->host_ = tmp;
    }
  else
    {
      char tmp_host[MAXHOSTNAMELEN+1];
       if (addr.get_host_name (tmp_host, sizeof(tmp_host)) != 0)
         return -1;
       this->host_ = tmp_host;
    }

  if (TAO_debug_level > 5)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) listening on: <%s:%u>\n",
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

template class ACE_Acceptor<TAO_Server_Connection_Handler, TAO_SOCK_ACCEPTOR>;
template class TAO_Acceptor_Impl<TAO_Server_Connection_Handler, TAO_SOCK_ACCEPTOR>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Acceptor<TAO_Server_Connection_Handler, TAO_SOCK_ACCEPTOR>
#pragma instantiate TAO_Acceptor_Impl<TAO_Server_Connection_Handler, TAO_SOCK_ACCEPTOR>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
