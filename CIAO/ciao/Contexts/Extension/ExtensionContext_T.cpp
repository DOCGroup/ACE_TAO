// $Id$

#ifndef CIAO_EXTENSION_CONTEXT_IMPL_T_C
#define CIAO_EXTENSION_CONTEXT_IMPL_T_C

namespace CIAO
{
  template <typename BASE_CTX,
            typename COMP>
  ExtensionContext_Impl<BASE_CTX, COMP>::ExtensionContext_Impl (
      Components::CCMHome_ptr the_home,
      Container_ptr c,
      PortableServer::Servant sv,
      const char* id)
    : Context_Impl_Base (the_home, c, id),
      servant_ (sv)
  {
  }

  template <typename BASE_CTX,
            typename COMP>
  ExtensionContext_Impl<BASE_CTX, COMP>::~ExtensionContext_Impl (void)
  {
  }

  template <typename BASE_CTX,
            typename COMP>
  ::Components::ContainerPortableInterceptor::ClientContainerInterceptorRegistration_ptr
  ExtensionContext_Impl<BASE_CTX, COMP>::get_client_interceptor_registration (void)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

  template <typename BASE_CTX,
            typename COMP>
  ::Components::ContainerPortableInterceptor::ServerContainerInterceptorRegistration_ptr
  ExtensionContext_Impl<BASE_CTX, COMP>::get_server_interceptor_registration (void)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

  template <typename BASE_CTX,
            typename COMP>
  ::Components::ContainerPortableInterceptor::StubContainerInterceptorRegistration_ptr
  ExtensionContext_Impl<BASE_CTX, COMP>::get_stub_interceptor_registration (void)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

  template <typename BASE_CTX,
            typename COMP>
  ::Components::ContainerPortableInterceptor::ServantContainerInterceptorRegistration_ptr
  ExtensionContext_Impl<BASE_CTX, COMP>::get_servant_interceptor_registration (void)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

  template <typename BASE_CTX,
            typename COMP>
  ::Components::Cookie *
  ExtensionContext_Impl<BASE_CTX, COMP>::install_service_reference (
        const char * /*service_id*/,
        ::CORBA::Object_ptr /*objref*/)
  {
    throw CORBA::NO_IMPLEMENT ();
  }

  template <typename BASE_CTX,
            typename COMP>
  ::CORBA::Object_ptr
  ExtensionContext_Impl<BASE_CTX, COMP>::uninstall_service_reference (::Components::Cookie * /*ck*/)
  {
    throw CORBA::NO_IMPLEMENT ();
  }
}

#endif /* CIAO_EXTENSION_CONTEXT_IMPL_T_C */

