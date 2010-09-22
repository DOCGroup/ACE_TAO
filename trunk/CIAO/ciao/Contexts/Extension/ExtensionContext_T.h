// -*- C++ -*-

//=============================================================================
/**
 *  @file    ExtensionContext_T.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================


#ifndef CIAO_EXTENSIONCONTEXT_H
#define CIAO_EXTENSIONCONTEXT_H

#include /**/ "ace/pre.h"

#include "CIAO_Server_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ciao/CCM_ExtensionContextC.h"
#include "ciao/Context_Impl_Base.h"
#include "tao/Objref_VarOut_T.h"

namespace Components
{
  class CCMHome;
  typedef CCMHome *CCMHome_ptr;

  typedef
    TAO_Objref_Var_T<
        CCMHome
      >
    CCMHome_var;
}

namespace CIAO
{
  template <typename BASE_CTX>
            typename COMP>
  class ExtensionContext_Impl
    : public virtual BASE_CTX,
      public virtual ::CIAO::Context_Impl_Base,
      public virtual ::CORBA::LocalObject
  {
  public:
    ExtensionContext_Impl(Components::CCMHome_ptr home,
                          ::CIAO::Extension_Container_ptr c,
                          PortableServer::Servant sv,
                          const char* id);

    virtual ~ExtensionContext_Impl (void);

    virtual ::Components::ContainerPortableInterceptor::ClientContainerInterceptorRegistration_ptr
    get_client_interceptor_registration (void);

    virtual ::Components::ContainerPortableInterceptor::ServerContainerInterceptorRegistration_ptr
    get_server_interceptor_registration (void);

    virtual ::Components::ContainerPortableInterceptor::StubContainerInterceptorRegistration_ptr
    get_stub_interceptor_registration (void);

    virtual ::Components::ContainerPortableInterceptor::ServantContainerInterceptorRegistration_ptr
    get_servant_interceptor_registration (void);

    virtual ::Components::Cookie * install_service_reference (
        const char * service_id,
        ::CORBA::Object_ptr objref);

    virtual ::CORBA::Object_ptr uninstall_service_reference (
        ::Components::Cookie * ck);

  private:
    ExtensionContext_Impl (void);
    PortableServer::Servant servant_;
    typename COMP::_var_type component_;
  };
}

#include /**/ "ace/post.h"

#endif /* CIAO_EXTENSIONCONTEXT_H */
