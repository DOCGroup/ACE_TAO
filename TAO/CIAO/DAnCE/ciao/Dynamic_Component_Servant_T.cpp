// $Id$

#ifndef CIAO_DYNAMIC_COMPONENT_SERVANT_T_C
#define CIAO_DYNAMIC_COMPONENT_SERVANT_T_C

#include "Dynamic_Component_Servant_T.h"

namespace CIAO
{
  template <typename COMP_SVNT,
            typename COMP_EXEC,
            typename COMP_EXEC_VAR,
            typename EXEC,
            typename EXEC_VAR,
            typename COMP>
  Dynamic_Component_Servant<COMP_SVNT, COMP_EXEC, COMP_EXEC_VAR, 
                            EXEC, EXEC_VAR, COMP>
    ::Dynamic_Component_Servant 
        (EXEC *exe, Components::CCMHome_ptr home,
         Swapping_Container *c)
      :Dynamic_Component_Servant_Base (c),
       executor_ (EXEC::_duplicate (exe)),
       home_ (Components::CCMHome::_duplicate (home))
  {
  }

  template <typename COMP_SVNT,
            typename COMP_EXEC,
            typename COMP_EXEC_VAR,
            typename EXEC,
            typename EXEC_VAR,
            typename COMP>
  Dynamic_Component_Servant<COMP_SVNT, COMP_EXEC, COMP_EXEC_VAR,
    EXEC, EXEC_VAR, COMP>::~Dynamic_Component_Servant (void)
  {
  } 

  template <typename COMP_SVNT,
            typename COMP_EXEC,
            typename COMP_EXEC_VAR,
            typename EXEC,
            typename EXEC_VAR,
            typename COMP>
  PortableServer::Servant Dynamic_Component_Servant
    <COMP_SVNT, COMP_EXEC, COMP_EXEC_VAR, 
     EXEC, EXEC_VAR, COMP>::create (void)
  {
    ::Components::EnterpriseComponent_var ciao_ec =
      this->executor_->create (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (COMP::_nil ());

    COMP_EXEC_VAR ciao_comp = COMP_EXEC::_narrow (ciao_ec.in ()
                                                  ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (COMP::_nil ());

    
    COMP_SVNT *svt = new COMP_SVNT(ciao_comp.in (), this->home_.in (),
                                   this->container_);


    PortableServer::ServantBase_var safe (svt);
    return safe._retn ();
  }
}

#endif /* CIAO_DYNAMIC_COMPONENT_SERVANT_T_C */
