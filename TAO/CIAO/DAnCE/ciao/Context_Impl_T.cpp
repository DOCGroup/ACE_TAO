// $Id$

#ifndef CIAO_CONTEXT_IMPL_T_C
#define CIAO_CONTEXT_IMPL_T_C

#include "Context_Impl_T.h"

namespace CIAO
{
  template <typename BASE_CTX, 
            typename SVNT, 
            typename COMP, 
            typename COMP_VAR>
  Context_Impl<BASE_CTX, SVNT, COMP, COMP_VAR>::Context_Impl (
      Components::CCMHome_ptr home,
      Session_Container *c,
      SVNT *sv)
    : Context_Impl_Base (home, c),
      servant_ (sv)
  {
  }

  template <typename BASE_CTX, 
            typename SVNT, 
            typename COMP, 
            typename COMP_VAR>
  Context_Impl<BASE_CTX, SVNT, COMP, COMP_VAR>::~Context_Impl (void)
  {
  }

  // Operations from ::Components::SessionContext.

  template <typename BASE_CTX, 
            typename SVNT, 
            typename COMP, 
            typename COMP_VAR>
  CORBA::Object_ptr
  Context_Impl<BASE_CTX, SVNT, COMP, COMP_VAR>::get_CCM_object (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::IllegalState))
  {
    if (CORBA::is_nil (this->component_.in ()))
    {
      ACE_DEBUG ((LM_DEBUG, "i am about to call get_CCM_object ()\n"));

      CORBA::Object_var obj;
      ACE_TRY {
      if (this->servant_ == 0) ACE_DEBUG ((LM_DEBUG, "Cranky \n"));
      obj =
        this->container_->get_objref (this->servant_
                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());
       }
      ACE_CATCHANY { ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                                          "Caught Exception \n"); return 0;}
      ACE_ENDTRY;

      ACE_DEBUG ((LM_DEBUG, "got the object ref\n"));

      this->component_ = COMP::_narrow (obj.in ()
                                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());
      ACE_DEBUG ((LM_DEBUG, "got the component\n"));

      if (CORBA::is_nil (this->component_.in ()))
      {
        ACE_THROW_RETURN (CORBA::INTERNAL (),
                          CORBA::Object::_nil ());
      }
    }

    return COMP::_duplicate (this->component_.in ());
  }
}

#endif /* CIAO_CONTEXT_IMPL_T_C */
