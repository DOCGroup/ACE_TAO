// $Id$

#ifndef CIAO_DYNAMIC_COMPONENT_SERVANT_T_C
#define CIAO_DYNAMIC_COMPONENT_SERVANT_T_C

#include "Dynamic_Component_Servant_T.h"

namespace CIAO
{
  template <typename COMP_SERVANT,
            typename COMP_EXEC,
            typename COMP_EXEC_VAR,
            typename EXEC>
  Dynamic_Component_Servant<COMP_SERVANT, COMP_EXEC, COMP_EXEC_VAR, EXEC>
    ::Dynamic_Component_Servant (EXEC *exe, Components::CCMHome_ptr home)
      :Dynamic_Component_Servant_Base (),
       executor_ (EXEC::_duplicate (exe))
  {
  }

  template <typename COMP_SERVANT,
            typename COMP_EXEC,
            typename COMP_EXEC_VAR,
            typename EXEC>
  PortableServer::Servant Dynamic_Component_Servant
    <COMP_SERVANT, COMP_EXEC, COMP_EXEC_VAR, EXEC>::create (void)
  {
    return new COMP_SERVANT;
  }
}

#endif /* CIAO_DYNAMIC_COMPONENT_SERVANT_T_C */
