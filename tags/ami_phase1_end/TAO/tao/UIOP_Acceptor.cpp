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
#include "tao/GIOP.h"

CORBA::ULong
TAO_UIOP_Acceptor::tag (void)
{
  return this->tag_;
}

TAO_UIOP_Acceptor::TAO_UIOP_Acceptor (void)
  : base_acceptor_ (),
    tag_ (TAO_IOP_TAG_INTERNET_IOP)
{
}

TAO_Profile *
TAO_UIOP_Acceptor::create_profile (TAO_ObjectKey &)
{
  return 0;
}

ACE_Event_Handler *
TAO_UIOP_Acceptor::acceptor (void)
{
  return &base_acceptor_;
}

#endif  /* !ACE_LACKS_UNIX_DOMAIN_SOCKETS */
