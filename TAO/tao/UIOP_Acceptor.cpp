// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//      UIOP_Acceptor.cpp
//
// = DESCRIPTION
//
//
// = AUTHOR
//     Fred Kuhns <fredk@cs.wustl.edu>
//     Ossama Othman <othman@cs.wustl.edu>
//
// ============================================================================

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

#include "tao/UIOP_Acceptor.h"
#include "tao/UIOP_Profile.h"
#include "tao/MProfile.h"
#include "tao/ORB_Core.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/GIOP.h"

ACE_RCSID(tao, UIOP_Acceptor, "$Id$")

// ****************************************************************

TAO_UIOP_Acceptor::TAO_UIOP_Acceptor (void)
  : TAO_Acceptor (TAO_IOP_TAG_UNIX_IOP),
    base_acceptor_ ()
{
}

int
TAO_UIOP_Acceptor::create_mprofile (const TAO_ObjectKey &object_key,
                                    TAO_MProfile &mprofile)
{
  ACE_UNIX_Addr addr;

  if (base_acceptor_.acceptor ().get_local_addr (addr) == -1)
    return 0;

  // we only make one
  int count = mprofile.profile_count ();
  if ((mprofile.size () - count) < 1)
    {
      if (mprofile.grow (count + 1) == -1)
        return -1;
    }

  TAO_UIOP_Profile *pfile;
  ACE_NEW_RETURN (pfile,
                  TAO_UIOP_Profile (addr, object_key),
                  -1);

  if (mprofile.give_profile (pfile) == -1)
    return -1;

  return 0;
}

int
TAO_UIOP_Acceptor::is_collocated (const TAO_Profile* pfile)
{
  const TAO_UIOP_Profile *profile =
    ACE_dynamic_cast(const TAO_UIOP_Profile*, pfile);

  // @@ We should probably cache this value, but then again some
  //    acceptors have multiple addresses.
  // @@ Fred: any ideas on how to optimize that?
  ACE_UNIX_Addr address;
  if (this->base_acceptor_.acceptor ().get_local_addr (address) == -1)
    return 0;

  return profile->object_addr () == address;
}

ACE_Event_Handler *
TAO_UIOP_Acceptor::acceptor (void)
{
  return &base_acceptor_;
}

int
TAO_UIOP_Acceptor::open (TAO_ORB_Core *orb_core,
                         ACE_CString &address)
{
  ACE_UNIX_Addr addr (address.c_str ());

  return this->open_i (orb_core, addr);
}

int
TAO_UIOP_Acceptor::open_default (TAO_ORB_Core *orb_core)
{
  ACE_UNIX_Addr addr;

  return this->open_i (orb_core, addr);
}

int
TAO_UIOP_Acceptor::open_i (TAO_ORB_Core* orb_core,
                           const ACE_UNIX_Addr& addr)
{
  if (this->base_acceptor_.open (orb_core, addr) != 0)
    return -1;

  // @@ If Profile creation is slow we may need to cache the
  //    rendezvous point here

  return 0;
}

CORBA::ULong
TAO_UIOP_Acceptor::endpoint_count (void)
{
  // @@ for now just assume one!
  return 1;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Acceptor<TAO_UIOP_Server_Connection_Handler, ACE_LSOCK_Acceptor>;
template class TAO_Acceptor_Impl<TAO_UIOP_Server_Connection_Handler, ACE_LSOCK_Acceptor>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Acceptor<TAO_UIOP_Server_Connection_Handler, ACE_LSOCK_Acceptor>
#pragma instantiate TAO_Acceptor_Impl<TAO_UIOP_Server_Connection_Handler, ACE_LSOCK_Acceptor>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


#endif  /* !ACE_LACKS_UNIX_DOMAIN_SOCKETS */
