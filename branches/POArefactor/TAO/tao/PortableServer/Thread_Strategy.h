// -*- C++ -*-

//=============================================================================
/**
 *  @file Thread_Strategy.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_THREAD_STRATEGY_H
#define TAO_THREAD_STRATEGY_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "PolicyFactory.h"
#include "PortableServerC.h"
#include "Policy_Strategy.h"
#include "ace/Service_Config.h"

// Locking
#include "ace/Synch_Traits.h"
#include "ace/Thread_Mutex.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "ace/Null_Mutex.h"

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
    class TAO_PortableServer_Export Thread_Strategy :
       public virtual Policy_Strategy
    {
    public:
      virtual ~Thread_Strategy (void);

      virtual int enter () = 0;

      virtual int exit () = 0;

      void strategy_init (CORBA::PolicyList *policy_list);
    };

    class TAO_PortableServer_Export Single_Thread_Strategy :
       public virtual Thread_Strategy
    {
    public:
      virtual ~Single_Thread_Strategy (void);

      virtual int enter ();

      virtual int exit ();

    private:
      TAO_SYNCH_RECURSIVE_MUTEX lock_;
    };

    class TAO_PortableServer_Export ORBControl_Thread_Strategy :
       public virtual Thread_Strategy
    {
    public:
      virtual ~ORBControl_Thread_Strategy (void);

      virtual int enter ();

      virtual int exit ();
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_THREAD_STRATEGY_H */
