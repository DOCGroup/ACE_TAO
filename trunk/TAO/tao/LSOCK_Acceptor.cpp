// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//      LSOCK_Acceptor.cpp
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

#include "tao/LSOCK_Acceptor.h"
#include "tao/GIOP.h"

CORBA::ULong
TAO_LSOCK_Acceptor::tag (void)
{
  return this->tag_;
}

TAO_LSOCK_Acceptor::TAO_LSOCK_Acceptor (void)
  : base_acceptor_ (),
    tag_ (TAO_IOP_TAG_INTERNET_IOP)
{
}

TAO_Profile *
TAO_LSOCK_Acceptor::create_profile (TAO_ObjectKey &)
{
  return 0;
}

ACE_Event_Handler *
TAO_LSOCK_Acceptor::acceptor (void)
{
  return &base_acceptor_;
}

#endif  /* !ACE_LACKS_UNIX_DOMAIN_SOCKETS */
