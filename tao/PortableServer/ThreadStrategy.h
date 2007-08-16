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

#include "tao/PortableServer/Policy_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/ThreadPolicyC.h"
#include "tao/orbconf.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class ThreadStrategy
      : public Policy_Strategy
    {
    public:
      virtual int enter () = 0;

      virtual int exit () = 0;

      virtual void strategy_init (TAO_Root_POA *poa);

      virtual void strategy_cleanup(void);

      virtual ::PortableServer::ThreadPolicyValue type() const = 0;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_THREAD_STRATEGY_H */
