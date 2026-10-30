// -*- C++ -*-
//
#ifndef SERVER_IORINTERCEPTOR_H
#define SERVER_IORINTERCEPTOR_H

#include "GatewayC.h"
#include "tao/IORInterceptor/IORInterceptor.h"
#include "tao/LocalObject.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class Server_IORInterceptor
  : public virtual PortableInterceptor::IORInterceptor_3_0,
    public virtual ::CORBA::LocalObject
{
public:
  explicit Server_IORInterceptor (Gateway::Object_Factory_ptr gateway_object_factory);
  Server_IORInterceptor (Server_IORInterceptor const &) = delete;
  Server_IORInterceptor (Server_IORInterceptor &&) = delete;
  Server_IORInterceptor &operator= (Server_IORInterceptor const &) = delete;
  Server_IORInterceptor &operator= (Server_IORInterceptor &&) = delete;

  /**
   * @name Methods Required by the IOR Interceptor Interface
   *
   * These are methods that must be implemented since they are pure
   * virtual in the abstract base class.  They are the canonical
   * methods required for all IOR interceptors.
   */
  //@{
  /// Return the name of this IORInterceptor.
  char * name () override;

  /// Cleanup resources acquired by this IORInterceptor.
  void destroy () override;

  /// Add the tagged components to the IOR.
  void establish_components (
      PortableInterceptor::IORInfo_ptr info) override;

  void components_established (
      PortableInterceptor::IORInfo_ptr info) override;

  void adapter_manager_state_changed (
      char const *id,
      PortableInterceptor::AdapterState state) override;

  void adapter_state_changed (
      PortableInterceptor::ObjectReferenceTemplateSeq const &templates,
      PortableInterceptor::AdapterState state) override;

  //@}

protected:
  ~Server_IORInterceptor ();

private:
  Gateway::Object_Factory_ptr gateway_object_factory_;
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* SERVER_IORINTERCEPTOR_H */
