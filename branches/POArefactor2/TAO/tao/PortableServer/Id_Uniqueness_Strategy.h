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

#ifndef TAO_ID_UNIQUENESS_STRATEGY_H
#define TAO_ID_UNIQUENESS_STRATEGY_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "PortableServerC.h"
#include "Policy_Strategy.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_MINIMUM_POA == 0)

namespace CORBA
{
  class PolicyError;
  class PolicyList;
}

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export Id_Uniqueness_Strategy :
       public virtual Policy_Strategy
    {
    public:
      virtual ~Id_Uniqueness_Strategy (void);

      void strategy_init(TAO_POA *poa);

      /*
       * Validate if the  servant may be activated
       * @retval true This servant may be activated
       * @retval false This servant may not be activated
       */
      virtual bool validate (PortableServer::Servant servant,
                             int &wait_occurred_restart_call
                             ACE_ENV_ARG_DECL) = 0;
    };

    class TAO_PortableServer_Export Unique_Id_Uniqueness_Strategy :
       public virtual Id_Uniqueness_Strategy
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

    class TAO_PortableServer_Export Multiple_Id_Uniqueness_Strategy :
       public virtual Id_Uniqueness_Strategy
    {
    public:
      virtual ~Multiple_Id_Uniqueness_Strategy (void);

      virtual bool validate (PortableServer::Servant servant,
                             int &wait_occurred_restart_call
                             ACE_ENV_ARG_DECL);
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_ID_UNIQUENESS_STRATEGY_H */
