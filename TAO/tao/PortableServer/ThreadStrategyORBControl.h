// -*- C++ -*-

//=============================================================================
/**
 *  @file ThreadStrategyORBControl.h
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_THREADSTRATEGYORBCONTROL_H
#define TAO_THREADSTRATEGYORBCONTROL_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/ThreadStrategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    class ThreadStrategyORBControl : public ThreadStrategy
    {
    public:
      int enter () override;

      int exit () override;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_THREADSTRATEGYORBCONTROL_H */
