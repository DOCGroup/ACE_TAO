// -*- C++ -*-

//=============================================================================
/**
 *  @file    ExtensionContext.h
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
  class CIAO_SERVER_Export ExtensionContext_Impl
    : public virtual Components::ExtensionContext,
      public virtual CIAO::Context_Impl_Base
  {
  public:
    ExtensionContext_Impl(Components::CCMHome_ptr home, Session_Container * c);

    virtual ~ExtensionContext_Impl (void);

    // Operations from ::Components::CCMContext.

    virtual Components::Principal_ptr get_caller_principal (void);

    virtual Components::CCMHome_ptr get_CCM_home (void);

    virtual CORBA::Boolean get_rollback_only (void);

    virtual Components::Transaction::UserTransaction_ptr get_user_transaction (void);

    virtual CORBA::Boolean is_caller_in_role (const char *role);

    virtual void set_rollback_only (void);

    virtual CORBA::Object_ptr resolve_service_reference(const char *service_id);

    virtual ::Components::ContainerPortableInterceptor::ClientContainerInterceptorRegistration_ptr get_client_interceptor_registration (void);

    virtual ::Components::ContainerPortableInterceptor::ServerContainerInterceptorRegistration_ptr get_server_interceptor_registration (void);

    virtual ::Components::ContainerPortableInterceptor::StubContainerInterceptorRegistration_ptr get_stub_interceptor_registration (void);

    virtual ::Components::ContainerPortableInterceptor::ServantContainerInterceptorRegistration_ptr get_servant_interceptor_registration (void);

    virtual ::Components::Cookie * install_service_reference (
        const char * service_id,
        ::CORBA::Object_ptr objref);

    virtual ::CORBA::Object_ptr uninstall_service_reference (
        ::Components::Cookie * ck);

  private:
    ExtensionContext_Impl (void);
  };
}

#include /**/ "ace/post.h"

#endif /* CIAO_EXTENSIONCONTEXT_H */
