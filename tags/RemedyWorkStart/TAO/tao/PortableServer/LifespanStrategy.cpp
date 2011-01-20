// -*- C++ -*-

//=============================================================================
/**
 *  @file    LifespanStrategy.cpp
 *
 *  $Id$
 *
 */
//=============================================================================

#include "tao/PortableServer/LifespanStrategy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    LifespanStrategy::LifespanStrategy () :
      poa_ (0)
    {
    }

    void
    LifespanStrategy::strategy_init (TAO_Root_POA *poa)
    {
      poa_ = poa;
    }

    void
    LifespanStrategy::strategy_cleanup (void)
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

TAO_END_VERSIONED_NAMESPACE_DECL
