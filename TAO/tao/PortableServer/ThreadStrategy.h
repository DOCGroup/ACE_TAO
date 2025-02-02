// -*- C++ -*-

//=============================================================================
/**
 *  @file ThreadStrategy.h
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_THREAD_STRATEGY_H
#define TAO_THREAD_STRATEGY_H
#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class ThreadStrategy
    {
    public:
      ThreadStrategy () = default;
      virtual ~ThreadStrategy () = default;

      virtual int enter () = 0;

      virtual int exit () = 0;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_THREAD_STRATEGY_H */
