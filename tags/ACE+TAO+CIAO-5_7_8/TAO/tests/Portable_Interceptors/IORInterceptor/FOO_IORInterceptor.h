// -*- C++ -*-

//=============================================================================
/**
 * @file FOO_IORInterceptor.h
 *
 * $Id$
 *
 * Implementation header for the "FOO" IOR test interceptor.
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef FOO_IOR_INTERCEPTOR_H
#define FOO_IOR_INTERCEPTOR_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/IORInterceptor/IORInterceptor.h"
#include "tao/CodecFactory/CodecFactory.h"
#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class FOO_IORInterceptor
 *
 * @brief Simple concrete IOR interceptor.
 *
 * This is a test IOR interceptor that embeds a tagged component in an
 * IOR.  That tagged component will then be verified on the client
 * side to ensure that IOR interceptor support is functioning
 * properly.
 */
class FOO_IORInterceptor
  : public virtual PortableInterceptor::IORInterceptor_3_0,
    public virtual ::CORBA::LocalObject
{
public:

  /// Constructor
  FOO_IORInterceptor (IOP::Codec_ptr codec);

  /**
   * @name Methods Required by the IOR Interceptor Interface
   *
   * These are methods that must be implemented since they are pure
   * virtual in the abstract base class.  They are the canonical
   * methods required for all IOR interceptors.
   */
  //@{
  /// Return the name of this IORInterceptor.
  virtual char * name (void);

  /// Cleanup resources acquired by this IORInterceptor.
  virtual void destroy (void);

  /// Add the tagged components to the IOR.
  virtual void establish_components (
      PortableInterceptor::IORInfo_ptr info);

  virtual void components_established (
      PortableInterceptor::IORInfo_ptr info);

  virtual void adapter_manager_state_changed (
      const char * id,
      PortableInterceptor::AdapterState state);

  virtual void adapter_state_changed (
      const PortableInterceptor::ObjectReferenceTemplateSeq & templates,
      PortableInterceptor::AdapterState state);

  //@}

private:

  /// The CDR encapsulation coder/decoder used to encapsulate data
  /// into a CDR encapsulation.
  IOP::Codec_var codec_;

};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* FOO_IOR_INTERCEPTOR_H */
