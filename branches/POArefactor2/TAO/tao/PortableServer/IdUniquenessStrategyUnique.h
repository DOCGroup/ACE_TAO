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
#include "PS_ForwardC.h"
#include "IdUniquenessStrategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export Unique_Id_Uniqueness_Strategy :
       public virtual IdUniquenessStrategy
    {
    public:
      Unique_Id_Uniqueness_Strategy (void);

      virtual ~Unique_Id_Uniqueness_Strategy (void);

      void strategy_init (TAO_POA *poa);

      virtual bool validate (PortableServer::Servant servant,
                             int &wait_occurred_restart_call
                             ACE_ENV_ARG_DECL);
    private:
      TAO_POA* poa_;
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_ID_UNIQUENESSSTRATEGYUNIQUE_H */
