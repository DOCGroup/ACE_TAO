// -*- C++ -*-

//=============================================================================
/**
 *  @file Policy_Strategy.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_POLICY_STRATEGY_H
#define TAO_POLICY_STRATEGY_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export Policy_Strategy :
       public virtual ACE_Service_Object
    {
    public:
      virtual ~Policy_Strategy (void) {};

      virtual void strategy_init(TAO_POA *poa) = 0;
    };
  }
}

#include /**/ "ace/post.h"
#endif /* TAO_POLICY_STRATEGY_H */
