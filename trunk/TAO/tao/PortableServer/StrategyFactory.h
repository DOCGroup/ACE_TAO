// -*- C++ -*-
//=============================================================================
/**
 *  @file StrategyFactory.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_STRATEGYFACTORY_H
#define TAO_PORTABLESERVER_STRATEGYFACTORY_H
#include /**/ "ace/pre.h"

#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  namespace Portable_Server
  {
    class StrategyFactory :
      public ACE_Service_Object
    {
    public:
      virtual ~StrategyFactory (void);
    };
  } /* namespace Portable_Server */
} /* namespace TAO */

#include /**/ "ace/post.h"
#endif /* TAO_PORTABLESERVER_STRATEGYFACTORY_H */
