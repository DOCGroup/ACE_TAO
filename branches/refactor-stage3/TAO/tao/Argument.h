// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Basic_Argument_T.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons and Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_ARGUMENT_T_H
#define TAO_ARGUMENT_T_H

#include "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corbafwd.h"
#include "tao/DynamicC.h"

namespace TAO
{
  /**
   * @class Argument
   *
   * @brief Base class for template argument classes.
   *
   */
  class TAO_Export Argument
  {
  public:
    virtual CORBA::Boolean marshal (TAO_OutputCDR &) {return 1;}
    virtual CORBA::Boolean demarshal (TAO_InputCDR &) {return 1;}
    virtual void interceptor_param (Dynamic::Parameter &) {}
    virtual void interceptor_result (CORBA::Any *) {}
    virtual CORBA::Boolean interceptor_replace (CORBA::Any &) {return 1;}
  };
};

#include "ace/post.h"

#endif /* TAO_ARGUMENT_T_H */
