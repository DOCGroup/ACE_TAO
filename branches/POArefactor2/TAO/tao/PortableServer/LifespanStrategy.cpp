// -*- C++ -*-

//=============================================================================
/**
 *  @file    LifespanStrategy.cpp
 *
 *  $Id$
 *
 */
//=============================================================================

#include "LifespanStrategy.h"

ACE_RCSID (PortableServer,
           LifespanStrategy,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    LifespanStrategy::LifespanStrategy () :
      poa_ (0)
    {
    }

    LifespanStrategy::~LifespanStrategy ()
    {
    }

    void
    LifespanStrategy::strategy_init (TAO_POA *poa)
    {
      poa_ = poa;
    }

    CORBA::ULong
    LifespanStrategy::key_type_length (void) const
    {
      return sizeof (char);
    }
  } /* namespace Portable_Server */
} /* namespace TAO */

