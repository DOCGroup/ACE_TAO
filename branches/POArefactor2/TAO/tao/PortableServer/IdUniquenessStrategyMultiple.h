// -*- C++ -*-

//=============================================================================
/**
 *  @file Id_Uniqueness_Strategy.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_ID_UNIQUENESSSTRATEGY_MULITPLE_H
#define TAO_ID_UNIQUENESSSTRATEGY_MULITPLE_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "IdUniquenessStrategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export Multiple_Id_Uniqueness_Strategy
      : public virtual IdUniquenessStrategy
    {
    public:
      virtual ~Multiple_Id_Uniqueness_Strategy (void);

      virtual void strategy_init (ServantRetentionStrategy* srs);

      virtual bool is_servant_activation_allowed (
        PortableServer::Servant servant,
        int &wait_occurred_restart_call);

      virtual bool allow_multiple_activations (void) const;
    };
  }
}

#include /**/ "ace/post.h"
#endif /* TAO_ID_UNIQUENESSSTRATEGY_MULITPLE_H */
