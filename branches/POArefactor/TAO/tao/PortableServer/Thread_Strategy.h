// -*- C++ -*-

//=============================================================================
/**
 *  @file Loadable_Thread_Strategy.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_LOADABLE_THREAD_STRATEGY_H
#define TAO_LOADABLE_THREAD_STRATEGY_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "PolicyFactory.h"
#include "PortableServerC.h"
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
    class TAO_PortableServer_Export Policy_Strategy :
       public virtual ACE_Service_Object
    {
    public:
      virtual ~Policy_Strategy (void);

      void init(CORBA::PolicyList *policy_list) = 0;
    }

    class TAO_PortableServer_Export Thread_Strategy :
       public virtual Policy_Strategy
    {
    public:
      virtual ~Thread_Strategy (void);

      virtual void enter () = 0;

      virtual void exit () = 0;

      void init(CORBA::PolicyList *policy_list)
      {
        // dependent on type create the correct strategy.
      }
    };

    class TAO_PortableServer_Export Single_Thread_Strategy :
       public virtual Thread_Strategy
    {
    public:
      virtual ~Single_Thread_Strategy (void);

      virtual void enter ()
      {
        lock.acquire();
      }

      virtual void exit ()
      {
        lock.release();
      }

    private:
      TAO_SYNCH_RECURSIVE_MUTEX lock_;
    };

    class TAO_PortableServer_Export ORBControl_Thread_Strategy :
       public virtual Thread_Strategy
    {
    public:
      virtual ~ORBControl_Thread_Strategy (void);

      virtual void enter ()
      {
        // Noop
      }

      virtual void exit ()
      {
        // Noop
      }
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_LOADABLE_THREAD_STRATEGY_H */
