// $Id$

#ifndef CIAO_SESSION_CONTEXT_IMPL_T_C
#define CIAO_SESSION_CONTEXT_IMPL_T_C

namespace CIAO
{
  template <typename BASE_CTX,
            typename COMP>
  Session_Context_Impl<BASE_CTX, COMP>::Session_Context_Impl (
      Components::CCMHome_ptr the_home,
      ::CIAO::Session_Container_ptr c,
      PortableServer::Servant sv,
      const char* id)
    : Context_Impl_Base_T < ::CIAO::Session_Container> (the_home, c, id),
      servant_ (sv)
  {
  }

  template <typename BASE_CTX,
            typename COMP>
  Session_Context_Impl<BASE_CTX, COMP>::~Session_Context_Impl (void)
  {
  }

  template <typename BASE_CTX,
            typename COMP>
  ::CORBA::Object_ptr
  Session_Context_Impl<BASE_CTX, COMP>::get_CCM_object (void)
  {
    ::CORBA::Object_var obj;

    try
      {
        obj = this->container_->get_objref (this->servant_);
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("Caught Exception\n");
      }

    typename COMP::_var_type component = COMP::_narrow (obj.in ());

    if (::CORBA::is_nil (component.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }

    return component._retn ();
  }
}

#endif /* CIAO_SESSION_CONTEXT_IMPL_T_C */
