// -*- C++ -*-

//=============================================================================
/**
 *  @file ThreadStrategy.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_THREAD_STRATEGY_H
#define TAO_THREAD_STRATEGY_H
#include /**/ "ace/pre.h"

#include "Policy_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ThreadPolicyC.h"
#include "tao/orbconf.h"

namespace TAO
{
  namespace Portable_Server
  {
    class ThreadStrategy :
       public Policy_Strategy
    {
    public:
      virtual int enter () = 0;

      virtual int exit () = 0;

      virtual void strategy_init (TAO_Root_POA *poa ACE_ENV_ARG_DECL);

      virtual void strategy_cleanup(ACE_ENV_SINGLE_ARG_DECL);

      virtual ::PortableServer::ThreadPolicyValue type() const = 0;
    };
  }
}

#include /**/ "ace/post.h"
#endif /* TAO_THREAD_STRATEGY_H */
