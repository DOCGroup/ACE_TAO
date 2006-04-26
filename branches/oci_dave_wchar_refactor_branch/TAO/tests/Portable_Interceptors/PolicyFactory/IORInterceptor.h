// -*- C++ -*-

//=============================================================================
/**
 * @file IORInterceptor.h
 *
 * $Id$
 *
 * Implementation header for the IOR test interceptor.
 *
 * @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef IOR_INTERCEPTOR_H
#define IOR_INTERCEPTOR_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/IORInterceptor/IORInterceptor.h"
#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class IORInterceptor
 *
 * @brief Simple concrete IOR interceptor.
 *
 * This is a test IOR interceptor that embeds a tagged component in an
 * IOR.  That tagged component will then be verified on the client
 * side to ensure that IOR interceptor support is functioning
 * properly.
 */
class IORInterceptor
  : public virtual PortableInterceptor::IORInterceptor,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor
  IORInterceptor (void);

  /**
   * @name Methods Required by the IOR Interceptor Interface
   *
   * These are methods that must be implemented since they are pure
   * virtual in the abstract base class.  They are the canonical
   * methods required for all IOR interceptors.
   */
  //@{
  /// Return the name of this IORInterceptor.
  virtual char * name (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Cleanup resources acquired by this IORInterceptor.
  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Add the tagged components to the IOR.
  virtual void establish_components (
      PortableInterceptor::IORInfo_ptr info
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
  //@}

protected:

  /// Destructor.
  /**
   * Protected destructor to enforce proper memory management through
   * the reference counting mechanism.
   */
  ~IORInterceptor (void);

private:

  /// IORInterceptor successfully called and executed.
  bool success_;

};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* IOR_INTERCEPTOR_H */
