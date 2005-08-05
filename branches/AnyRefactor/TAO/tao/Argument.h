// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Argument.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons and Carlos O'Ryan
 */
//=============================================================================


#ifndef TAO_ARGUMENT_H
#define TAO_ARGUMENT_H

#include /**/ "ace/pre.h"

#include "tao/Basic_Types.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"
#include "tao/TAO_Export.h"
#include "tao/ParameterModeC.h"

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
   * @brief Base class for argument class templates.
   *
   * @c Argument subclasses override the virtual methods
   *    appropriately.  For example, a stub "IN" argument class
   *    template would override the @c marshal() method, but wouldn't
   *    need to override the @c demarshal() method.
   */
  class TAO_Export Argument
  {
  public:

    /// Destructor.
    virtual ~Argument (void);

    /// Marshal the argument into the given CDR output stream.
    /**
     * @note The default implementation simply returns @c true.
     */
    virtual CORBA::Boolean marshal (TAO_OutputCDR &);

    /// Demarshal the argument from the given CDR input stream.
    /**
     * @note The default implementation simply returns @c true.
     */
    virtual CORBA::Boolean demarshal (TAO_InputCDR &);

#if TAO_HAS_INTERCEPTORS == 1

    /**
     * @name Portable Interceptor Related Methods
     *
     * These methods are only used when setting up
     * @c PortableInterceptor::RequestInfo instances with the
     * appropriate operation-specific parameter list and return
     * value, if either or both exist.
     */
    //@{
    /// Populate the given @a CORBA::Any result argument.
    /**
     * @note The default implementation is a no-op.
     */
    virtual void interceptor_value (CORBA::Any *) const;

    /// Get the parameter mode of this argument
    virtual CORBA::ParameterMode mode (void) const = 0;
    //@}

#endif /* TAO_HAS_INTERCEPTORS == 1 */

  };

  class TAO_Export InArgument : public Argument
  {
  public:
#if TAO_HAS_INTERCEPTORS == 1
    virtual CORBA::ParameterMode mode (void) const;
#endif
  };

  class TAO_Export InoutArgument : public Argument
  {
  public:
#if TAO_HAS_INTERCEPTORS == 1
    virtual CORBA::ParameterMode mode (void) const;
#endif
  };

  class TAO_Export OutArgument : public Argument
  {
  public:
#if TAO_HAS_INTERCEPTORS == 1
    virtual CORBA::ParameterMode mode (void) const;
#endif
  };

  class TAO_Export RetArgument : public Argument
  {
  public:
#if TAO_HAS_INTERCEPTORS == 1
    virtual CORBA::ParameterMode mode (void) const;
#endif
  };
}

#include /**/ "ace/post.h"

#endif /* TAO_ARGUMENT_H */
