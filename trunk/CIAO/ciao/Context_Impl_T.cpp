// $Id$

#ifndef CIAO_CONTEXT_IMPL_T_C
#define CIAO_CONTEXT_IMPL_T_C

#include "Context_Impl_T.h"

namespace CIAO
{
  template <typename BASE_CTX,
            typename SVNT,
            typename COMP>
  Context_Impl<BASE_CTX, SVNT, COMP>::Context_Impl (
      Components::CCMHome_ptr the_home,
      Session_Container *c,
      SVNT *sv)
    : Context_Impl_Base (the_home, c),
      servant_ (sv)
  {
  }

  template <typename BASE_CTX,
            typename SVNT,
            typename COMP>
  Context_Impl<BASE_CTX, SVNT, COMP>::~Context_Impl (void)
  {
  }

  // Operations from ::Components::SessionContext.

  template <typename BASE_CTX,
            typename SVNT,
            typename COMP>
  CORBA::Object_ptr
  Context_Impl<BASE_CTX, SVNT, COMP>::get_CCM_object (
    )
  {
    if (CORBA::is_nil (this->component_.in ()))
      {
        CORBA::Object_var obj;

        try
          {
            obj =
              this->container_->get_objref (this->servant_);
          }
        catch (const CORBA::Exception& ex)
          {
            ex._tao_print_exception ("Caught Exception \n");
            return CORBA::Object::_nil ();
          }


        this->component_ = COMP::_narrow (obj.in ());

        if (CORBA::is_nil (this->component_.in ()))
          {
            throw CORBA::INTERNAL ();
          }
      }

    return COMP::_duplicate (this->component_.in ());
  }
}

#endif /* CIAO_CONTEXT_IMPL_T_C */
