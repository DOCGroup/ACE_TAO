// -*- C++ -*-

//=============================================================================
/**
 *  @file IdUniquenessStrategy.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_ID_UNIQUENESS_STRATEGY_H
#define TAO_ID_UNIQUENESS_STRATEGY_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "Policy_Strategy.h"
#include "PS_ForwardC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  namespace Portable_Server
  {
    class ServantRetentionStrategy;

    class TAO_PortableServer_Export IdUniquenessStrategy
      : public virtual Policy_Strategy
    {
    public:
      virtual ~IdUniquenessStrategy (void);

      /*
       * Validate if the servant may be activated
       * @retval true This servant may be activated
       * @retval false This servant may not be activated
       */
      virtual bool is_servant_activation_allowed (
        PortableServer::Servant s,
        int &w) = 0;

      virtual bool allow_multiple_activations (void) const = 0;
    };
  }
}

#include /**/ "ace/post.h"
#endif /* TAO_ID_UNIQUENESS_STRATEGY_H */
