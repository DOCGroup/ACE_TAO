// $Id$

#ifndef CIAO_DYNAMIC_COMPONENT_SERVANT_T_C
#define CIAO_DYNAMIC_COMPONENT_SERVANT_T_C

#include "Dynamic_Component_Servant_T.h"

namespace CIAO
{
  template <typename COMP_SERVANT,
            typename COMP_EXEC,
            typename COMP_EXEC_VAR,
            typename EXEC,
            typename COMP>
  Dynamic_Component_Servant<COMP_SERVANT, COMP_EXEC, COMP_EXEC_VAR, EXEC, COMP>
    ::Dynamic_Component_Servant (EXEC *exe, Components::CCMHome_ptr home)
      :Dynamic_Component_Servant_Base (),
       executor_ (EXEC::_duplicate (exe),
       home_ (Components::CCMHome::_duplicate (home.in ())))
  {
  }

  template <typename COMP_SERVANT,
            typename COMP_EXEC,
            typename COMP_EXEC_VAR,
            typename EXEC,
            typename COMP>
  PortableServer::Servant Dynamic_Component_Servant
    <COMP_SERVANT, COMP_EXEC, COMP_EXEC_VAR, EXEC, COMP>::create (void)
  {
    ::Components::EnterpriseComponent_var ciao_ec =
      this->executor_->create (ACE_ENV_SINGLE_ARG_PARAMETER);

    COMP_EXEC_VAR ciao_comp = COMP_EXEC::_narrow (ciao_ec.in ()
                                                  ACE_ENV_ARG_PARAMETER);

    COMP_SERVANT *svt = 0;
    ACE_NEW_RETURN (svt,
                    COMP_SERVANT (ciao_comp.in (),
                                  this->home_.in ()),
                    COMP::_nil ());
  }
}

#endif /* CIAO_DYNAMIC_COMPONENT_SERVANT_T_C */
