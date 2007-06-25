// -*- C++ -*-
//
//$Id$

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
    public virtual TAO_Local_RefCounted_Object
{
public:

  Server_IORInterceptor (Gateway::Object_Factory_ptr gateway_object_factory);

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

protected:

  ~Server_IORInterceptor (void);

private:

  Gateway::Object_Factory_ptr gateway_object_factory_;

};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* SERVER_IORINTERCEPTOR_H */
