// -*- C++ -*-

//=============================================================================
/**
 *  @file    LifespanStrategy.cpp
 */
//=============================================================================

#include "tao/PortableServer/LifespanStrategy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    void
    LifespanStrategy::strategy_init (TAO_Root_POA *poa)
    {
      poa_ = poa;
    }

    void
    LifespanStrategy::strategy_cleanup ()
    {
      poa_ = nullptr;
    }

    CORBA::ULong
    LifespanStrategy::key_type_length () const
    {
      return sizeof (char);
    }
  } /* namespace Portable_Server */
} /* namespace TAO */

TAO_END_VERSIONED_NAMESPACE_DECL
