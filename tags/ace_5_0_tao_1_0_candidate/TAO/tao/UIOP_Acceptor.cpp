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

#include "tao/UIOP_Acceptor.h"

#if !defined (ACE_LACKS_UNIX_DOMAIN_SOCKETS)

#include "tao/UIOP_Profile.h"
#include "tao/MProfile.h"
#include "tao/ORB_Core.h"
#include "tao/GIOP.h"
#include "tao/Server_Strategy_Factory.h"
#include "tao/debug.h"

ACE_RCSID(tao, UIOP_Acceptor, "$Id$")

// ****************************************************************

TAO_UIOP_Acceptor::TAO_UIOP_Acceptor (void)
  : TAO_Acceptor (TAO_IOP_TAG_UNIX_IOP),
    base_acceptor_ (),
    creation_strategy_ (0),
    concurrency_strategy_ (0),
    version_ (TAO_DEF_GIOP_MAJOR, TAO_DEF_GIOP_MINOR),
    orb_core_ (0),
    unlink_on_close_ (1)
{
}

TAO_UIOP_Acceptor::~TAO_UIOP_Acceptor (void)
{
  delete this->creation_strategy_;
  delete this->concurrency_strategy_;
}

int
TAO_UIOP_Acceptor::create_mprofile (const TAO_ObjectKey &object_key,
                                    TAO_MProfile &mprofile)
{
  ACE_UNIX_Addr addr;

  if (this->base_acceptor_.acceptor ().get_local_addr (addr) == -1)
    return 0;

  // we only make one
  int count = mprofile.profile_count ();
  if ((mprofile.size () - count) < 1)
    {
      if (mprofile.grow (count + 1) == -1)
        return -1;
    }

  TAO_UIOP_Profile *pfile = 0;
  ACE_NEW_RETURN (pfile,
                  TAO_UIOP_Profile (addr,
                                    object_key,
                                    this->version_,
                                    this->orb_core_),
                  -1);

  if (mprofile.give_profile (pfile) == -1)
    return -1;

  if (this->orb_core_->orb_params ()->std_profile_components () == 0)
    return 0;

  pfile->tagged_components ().set_orb_type (TAO_ORB_TYPE);

  CONV_FRAME::CodeSetComponentInfo code_set_info;
  code_set_info.ForCharData.native_code_set  = TAO_DEFAULT_CHAR_CODESET_ID;
  code_set_info.ForWcharData.native_code_set = TAO_DEFAULT_WCHAR_CODESET_ID;
  pfile->tagged_components ().set_code_sets (code_set_info);

  return 0;
}

int
TAO_UIOP_Acceptor::is_collocated (const TAO_Profile* pfile)
{
  const TAO_UIOP_Profile *profile =
    ACE_dynamic_cast(const TAO_UIOP_Profile*, pfile);

  // for UNIX Files this is relatively cheap
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
TAO_UIOP_Acceptor::close (void)
{
  ACE_UNIX_Addr addr;

  if (this->base_acceptor_.acceptor ().get_local_addr (addr) == -1)
    return -1;

  if (this->unlink_on_close_)
    (void) ACE_OS::unlink (addr.get_path_name ());

  return this->base_acceptor_.close ();
}

int
TAO_UIOP_Acceptor::open (TAO_ORB_Core *orb_core,
                         int major,
                         int minor,
                         ACE_CString &address)
{
  if (major >= 0 && minor >= 0)
    this->version_.set_version (ACE_static_cast (CORBA::Octet, major),
                                ACE_static_cast (CORBA::Octet, minor));

  return this->open_i (orb_core, address.c_str ());
}

int
TAO_UIOP_Acceptor::open_default (TAO_ORB_Core *orb_core)
{
  ACE_Auto_String_Free tempname (ACE_OS::tempnam (0, "TAO"));

  if (tempname.get () == 0)
    return -1;

  return this->open_i (orb_core, tempname.get ());
}

int
TAO_UIOP_Acceptor::open_i (TAO_ORB_Core* orb_core,
                           const char *rendezvous)
{
  this->orb_core_ = orb_core;

  ACE_NEW_RETURN (this->creation_strategy_,
                  TAO_UIOP_CREATION_STRATEGY (this->orb_core_),
                  -1);

  ACE_NEW_RETURN (this->concurrency_strategy_,
                  TAO_UIOP_CONCURRENCY_STRATEGY (this->orb_core_),
                  -1);

  ACE_UNIX_Addr addr;

  this->rendezvous_point (addr, rendezvous);

  if (this->base_acceptor_.open (addr,
                                 this->orb_core_->reactor (),
                                 this->creation_strategy_,
                                 0,
                                 this->concurrency_strategy_) == -1)
    {
      // Don't unlink an existing rendezvous point since it may be in
      // use by another UIOP server/client.
      if (errno == EADDRINUSE)
        this->unlink_on_close_ = 0;

      return -1;
    }

  // @@ If Profile creation is slow we may need to cache the
  //    rendezvous point here

  if (TAO_debug_level > 5)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\nTAO (%P|%t) UIOP_Acceptor::open_i - "
                  "listening on: <%s>\n",
                  addr.get_path_name ()));
    }

  return 0;
}

void
TAO_UIOP_Acceptor::rendezvous_point (ACE_UNIX_Addr &addr,
                                     const char *rendezvous)
{
  // To guarantee portability, local IPC rendezvous points (including
  // the path and filename) should not be longer than 99 characters
  // long. Some platforms may support longer rendezvous points,
  // usually 108 characters including the null terminator, but
  // Posix.1g only requires that local IPC rendezvous point arrays
  // contain a maximum of at least 100 characters, including the null
  // terminator.  If an endpoint is longer than what the platform
  // supports then it will be truncated so that it fits, and a warning
  // will be issued.

  // Avoid using relative paths in your UIOP endpoints.  If possible,
  // use absolute paths instead.  Imagine that the server is given an
  // endpoint to create using -ORBEndpoint uiop://foobar.  A local IPC
  // rendezvous point called foobar will be created in the current
  // working directory.  If the client is not started in the directory
  // where the foobar rendezvous point exists then the client will not
  // be able to communicate with the server since its point of
  // communication, the rendezvous point, was not found. On the other
  // hand, if an absolute path was used, the client would know exactly
  // where to find the rendezvous point.  It is up to the user to
  // make sure that a given UIOP endpoint is accessible by both the
  // server and the client.

  addr.set (rendezvous);

  size_t length = ACE_OS::strlen (addr.get_path_name ());

  // Check if rendezvous point was truncated by ACE_UNIX_Addr since
  // most UNIX domain socket rendezvous points can only be less than
  // 108 characters long.
  if (length < ACE_OS::strlen (rendezvous))
    {
      ACE_DEBUG ((LM_WARNING,
                  "TAO (%P|%t) UIOP rendezvous point was truncated to <%s>\n"
                  "since it was longer than %d characters long.\n",
                  addr.get_path_name (),
                  length));
    }
}

CORBA::ULong
TAO_UIOP_Acceptor::endpoint_count (void)
{
  return 1;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Acceptor<TAO_UIOP_Server_Connection_Handler, ACE_LSOCK_ACCEPTOR>;
template class ACE_Strategy_Acceptor<TAO_UIOP_Server_Connection_Handler, ACE_LSOCK_ACCEPTOR>;
template class ACE_Accept_Strategy<TAO_UIOP_Server_Connection_Handler, ACE_LSOCK_ACCEPTOR>;
template class ACE_Creation_Strategy<TAO_UIOP_Server_Connection_Handler>;
template class ACE_Concurrency_Strategy<TAO_UIOP_Server_Connection_Handler>;
template class ACE_Scheduling_Strategy<TAO_UIOP_Server_Connection_Handler>;
template class TAO_Creation_Strategy<TAO_UIOP_Server_Connection_Handler>;
template class TAO_Concurrency_Strategy<TAO_UIOP_Server_Connection_Handler>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Acceptor<TAO_UIOP_Server_Connection_Handler, ACE_LSOCK_ACCEPTOR>
#pragma instantiate ACE_Strategy_Acceptor<TAO_UIOP_Server_Connection_Handler, ACE_LSOCK_ACCEPTOR>
#pragma instantiate ACE_Accept_Strategy<TAO_UIOP_Server_Connection_Handler, ACE_LSOCK_ACCEPTOR>
#pragma instantiate ACE_Creation_Strategy<TAO_UIOP_Server_Connection_Handler>
#pragma instantiate ACE_Concurrency_Strategy<TAO_UIOP_Server_Connection_Handler>
#pragma instantiate ACE_Scheduling_Strategy<TAO_UIOP_Server_Connection_Handler>
#pragma instantiate TAO_Creation_Strategy<TAO_UIOP_Server_Connection_Handler>
#pragma instantiate TAO_Concurrency_Strategy<TAO_UIOP_Server_Connection_Handler>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


#endif  /* !ACE_LACKS_UNIX_DOMAIN_SOCKETS */
