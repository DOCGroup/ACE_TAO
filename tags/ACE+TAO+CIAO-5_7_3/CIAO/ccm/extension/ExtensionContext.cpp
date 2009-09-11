// $Id$

#include "ExtensionContext.h"

namespace CIAO
{
  ExtensionContext_Impl::ExtensionContext_Impl (Components::CCMHome_ptr home,
                                                Session_Container * c)
    : CIAO::Context_Impl_Base (home, c)
  {
  }

  ExtensionContext_Impl::~ExtensionContext_Impl (void)
  {
  }

  ::Components::ContainerPortableInterceptor::ClientContainerInterceptorRegistration_ptr
  ExtensionContext_Impl::get_client_interceptor_registration (void)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

  ::Components::ContainerPortableInterceptor::ServerContainerInterceptorRegistration_ptr
  ExtensionContext_Impl::get_server_interceptor_registration (void)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

  ::Components::ContainerPortableInterceptor::StubContainerInterceptorRegistration_ptr
  ExtensionContext_Impl::get_stub_interceptor_registration (void)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

  ::Components::ContainerPortableInterceptor::ServantContainerInterceptorRegistration_ptr
  ExtensionContext_Impl::get_servant_interceptor_registration (void)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

  ::Components::Cookie *
  ExtensionContext_Impl::install_service_reference (
        const char * /*service_id*/,
        ::CORBA::Object_ptr /*objref*/)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

  ::CORBA::Object_ptr
  ExtensionContext_Impl::uninstall_service_reference (::Components::Cookie * /*ck*/)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

  // Operations from ::Components::CCMContext.

  Components::Principal_ptr
  ExtensionContext_Impl::get_caller_principal (void)
  {
    return CIAO::Context_Impl_Base::get_caller_principal ();
  }

  Components::CCMHome_ptr
  ExtensionContext_Impl::get_CCM_home (void)
  {
    return CIAO::Context_Impl_Base::get_CCM_home ();
  }

  CORBA::Boolean
  ExtensionContext_Impl::get_rollback_only (void)
  {
    return CIAO::Context_Impl_Base::get_rollback_only ();
  }

  Components::Transaction::UserTransaction_ptr
  ExtensionContext_Impl::get_user_transaction (void)
  {
    return CIAO::Context_Impl_Base::get_user_transaction ();
  }

  CORBA::Boolean
  ExtensionContext_Impl::is_caller_in_role (const char * role)
  {
    return CIAO::Context_Impl_Base::is_caller_in_role (role);
  }

  void
  ExtensionContext_Impl::set_rollback_only (void)
  {
    CIAO::Context_Impl_Base::set_rollback_only ();
  }

  CORBA::Object_ptr
  ExtensionContext_Impl::resolve_service_reference(const char *service_id)
  {
    return CIAO::Context_Impl_Base::resolve_service_reference (service_id);
  }
}
