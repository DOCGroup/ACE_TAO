// -*- C++ -*-

//=============================================================================
/**
 *  @file    Extension_Context_T.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================


#ifndef CIAO_EXTENSIONCONTEXT_H
#define CIAO_EXTENSIONCONTEXT_H

#include /**/ "ace/pre.h"

#include "ciao/Contexts/Context_Impl_Base_T.h"
#include "ciao/Containers/Extension/Extension_ContainerC.h"
#include "tao/LocalObject.h"
#include "tao/PortableServer/PS_ForwardC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace CIAO
{
  template <typename BASE_CTX,
            typename COMP>
  class Extension_Context_Impl
    : public virtual BASE_CTX,
      public virtual ::CIAO::Context_Impl_Base_T < ::CIAO::Extension_Container>,
      public virtual ::CORBA::LocalObject
  {
  public:
    /// Type definition of the context type.
    typedef BASE_CTX context_type;

    /// Type definition of the var type for the context.
    typedef typename context_type::_var_type _var_type;

    /// Type definition of the component type.
    typedef COMP component_type;

    Extension_Context_Impl(Components::CCMHome_ptr home,
                          ::CIAO::Extension_Container_ptr c,
                          PortableServer::Servant sv,
                          const char* id);

    virtual ~Extension_Context_Impl (void);

    virtual ::Components::ContainerPortableInterceptor::ClientContainerInterceptorRegistration_ptr
    get_client_interceptor_registration (void);

    virtual ::Components::ContainerPortableInterceptor::ServantContainerInterceptorRegistration_ptr
    get_servant_interceptor_registration (void);

    virtual ::Components::ContainerPortableInterceptor::ServerContainerInterceptorRegistration_ptr
    get_server_interceptor_registration (void);

    virtual ::Components::ContainerPortableInterceptor::StubContainerInterceptorRegistration_ptr
    get_stub_interceptor_registration (void);

    virtual ::Components::Cookie * install_service_reference (
        const char * service_id,
        ::CORBA::Object_ptr objref);

    virtual ::CORBA::Object_ptr uninstall_service_reference (
        ::Components::Cookie * ck);

  private:
    PortableServer::Servant servant_;
    typename COMP::_var_type component_;
  };
}

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ciao/Contexts/Extension/Extension_Context_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ciao/Contexts/Extension/Extension_Context_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* CIAO_EXTENSIONCONTEXT_H */
