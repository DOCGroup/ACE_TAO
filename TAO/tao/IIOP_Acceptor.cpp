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
#include "tao/GIOP.h"

CORBA::ULong
TAO_IIOP_Acceptor::tag (void)
{
  return this->tag_;
}

TAO_IIOP_Acceptor::TAO_IIOP_Acceptor (void)
  : base_acceptor_ (),
    tag_ (TAO_IOP_TAG_INTERNET_IOP)
{
}

TAO_Profile *
TAO_IIOP_Acceptor::create_profile (TAO_ObjectKey &)
{
  return 0;
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
