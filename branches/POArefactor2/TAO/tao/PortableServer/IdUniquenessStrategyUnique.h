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

#ifndef TAO_ID_UNIQUENESSSTRATEGYUNIQUE_H
#define TAO_ID_UNIQUENESSSTRATEGYUNIQUE_H
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
    class TAO_PortableServer_Export Unique_Id_Uniqueness_Strategy
      : public virtual IdUniquenessStrategy
    {
    public:

      Unique_Id_Uniqueness_Strategy (void);

      virtual ~Unique_Id_Uniqueness_Strategy (void);

      virtual void strategy_init(TAO_POA *poa ACE_ENV_ARG_DECL);

      virtual void strategy_cleanup(ACE_ENV_SINGLE_ARG_DECL);

      virtual bool is_servant_activation_allowed (
        PortableServer::Servant servant,
        int &wait_occurred_restart_call);

      virtual bool allow_multiple_activations (void) const;

    private:
      TAO_POA* poa_;
    };
  }
}

#include /**/ "ace/post.h"
#endif /* TAO_ID_UNIQUENESSSTRATEGYUNIQUE_H */
