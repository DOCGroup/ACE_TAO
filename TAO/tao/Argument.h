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

#include "corbafwd.h"
#include "DynamicC.h"

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
    Argument (char const *argname) : argname_ (argname ) {}

    virtual CORBA::Boolean marshal (TAO_OutputCDR &) = 0;
    virtual CORBA::Boolean demarshal (TAO_InputCDR &) = 0;
    virtual void add_to_interceptor (Dynamic::Parameter &) = 0;

  private:
    char const * argname_;
  };

  /**
   * @class Stub_Retval
   *
   * @brief Base class for stub return value classes.
   *
   */
  class TAO_Export Stub_Retval
  {
  public:
    virtual CORBA::Boolean demarshal (TAO_InputCDR &) = 0;
    virtual void add_to_interceptor (CORBA::Any *) = 0;
  };

  /**
   * @class Skel_Retval
   *
   * @brief Base class for skelton return value classes.
   *
   */
  class TAO_Export Skel_Retval
  {
  public:
    virtual CORBA::Boolean marshal (TAO_OutputCDR &) = 0;
    virtual void add_to_interceptor (CORBA::Any *) = 0;
  };
};

#include "ace/post.h"

#endif /* TAO_ARGUMENT_T_H */
