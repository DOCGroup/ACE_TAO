// -*- C++ -*-

//=============================================================================
/**
 * @file   ORT_test_IORInterceptor.h
 *
 * @author Ossama Othman <ossama@uci.edu>
 * @author Priyanka Gontla <gontla_p@ociweb.com>
 */
//=============================================================================


#ifndef ORT_TEST_IORINTERCEPTOR_H
#define ORT_TEST_IORINTERCEPTOR_H

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

class ORT_test_IORInterceptor
  : public virtual PortableInterceptor::IORInterceptor_3_0,
    public virtual ::CORBA::LocalObject
{
public:
  ORT_test_IORInterceptor ();

  /**
   * @name Methods Required by the IOR Interceptor Interface
   *
   * These are methods that must be implemented since they are pure
   * virtual in the abstract base class.  They are the canonical
   * methods required for all IOR interceptors.
   */
  //@{
  /// Return the name of this IORInterceptor.
  ORT_test_IORInterceptor (ORT_test_IORInterceptor const &) = delete;
  ORT_test_IORInterceptor &operator= (ORT_test_IORInterceptor const &) = delete;
  ORT_test_IORInterceptor (ORT_test_IORInterceptor &&) = delete;
  ORT_test_IORInterceptor &operator= (ORT_test_IORInterceptor &&) = delete;

  char *name () override;

  /// Cleanup resources acquired by this IORInterceptor.
  void destroy () override;

  /// Add the tagged components to the IOR.
  void establish_components (PortableInterceptor::IORInfo_ptr info) override;

  void components_established (PortableInterceptor::IORInfo_ptr info) override;

  void adapter_manager_state_changed (
      char const *id,
      PortableInterceptor::AdapterState state) override;

  void adapter_state_changed (
      PortableInterceptor::ObjectReferenceTemplateSeq const &templates,
      PortableInterceptor::AdapterState state) override;

  //@}

private:
  CORBA::Long establish_count_;

  CORBA::Long components_establish_count_;
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* ORT_TEST_IORINTERCEPTOR_H */
