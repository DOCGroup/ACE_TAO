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


#ifndef TAO_ARGUMENT_H
#define TAO_ARGUMENT_H

#include "ace/pre.h"
#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Dynamic
{
  struct Parameter;
};

namespace CORBA
{
  class Any;
}

class TAO_OutputCDR;
class TAO_InputCDR;

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
    virtual CORBA::Boolean marshal (TAO_OutputCDR &);
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);
    virtual void interceptor_param (Dynamic::Parameter &);
    virtual void interceptor_result (CORBA::Any *);
    virtual CORBA::Boolean interceptor_replace (CORBA::Any &);
  };
};

#include "ace/post.h"

#endif /* TAO_ARGUMENT_H */
