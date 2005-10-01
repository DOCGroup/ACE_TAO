// -*- C++ -*-

//=============================================================================
/**
 *  @file IdUniquenessStrategyFactory.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_IDUNIQUENESSSTRATEGYFACTORY_H
#define TAO_PORTABLESERVER_IDUNIQUENESSSTRATEGYFACTORY_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "StrategyFactory.h"
#include "IdUniquenessPolicyC.h"

namespace TAO
{
  namespace Portable_Server
  {
    class IdUniquenessStrategy;

    class TAO_PortableServer_Export IdUniquenessStrategyFactory
      : public StrategyFactory
    {
    public:
      /// Create a new servant retention strategy
      virtual IdUniquenessStrategy* create (
        ::PortableServer::IdUniquenessPolicyValue value) = 0;

      /// Cleanup the given strategy instance
      virtual void destroy (
        IdUniquenessStrategy *strategy
        ACE_ENV_ARG_DECL) = 0;
    };
  }
}

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_IDUNIQUENESSSTRATEGYFACTORY_H */
