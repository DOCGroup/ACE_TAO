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

    void
    LifespanStrategy::strategy_init (
      TAO_Root_POA *poa
      ACE_ENV_ARG_DECL_NOT_USED)
    {
      poa_ = poa;
    }

    void
    LifespanStrategy::strategy_cleanup(
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    {
      poa_ = 0;
    }

    CORBA::ULong
    LifespanStrategy::key_type_length (void) const
    {
      return sizeof (char);
    }
  } /* namespace Portable_Server */
} /* namespace TAO */

