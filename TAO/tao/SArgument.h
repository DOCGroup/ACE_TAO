// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    SArgument_Base.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons and Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_SARGUMENT_H
#define TAO_SARGUMENT_H

#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  /**
   * @class SArgument
   *
   * @brief Base class for all IDL operation skeleton parameter wrappers.
   */
  class SArgument
  {
  public:
    virtual CORBA::Boolean marshal (TAO_OutputCDR &)
    {
      return 0;
    }

    virtual CORBA::Boolean unmarshal (TAO_InputCDR &)
    {
      return 0;
    }
  };
};

#include "ace/post.h"

#endif /* TAO_SARGUMENT_H */

