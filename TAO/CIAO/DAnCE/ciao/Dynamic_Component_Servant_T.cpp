// $Id$

#ifndef CIAO_DYNAMIC_COMPONENT_SERVANT_T_C
#define CIAO_DYNAMIC_COMPONENT_SERVANT_T_C

#include "Dynamic_Component_Servant_T.h"

namespace CIAO
{
  template <typename COMP_SERVANT,
            typename COMP_EXEC,
            typename COMP_EXEC_VAR>
  PortableServer::Servant Dynamic_Component_Servant<COMP_SERVANT>::create (void)
  {
    return new COMP_SERVANT;
  }
}

#endif /* CIAO_DYNAMIC_COMPONENT_SERVANT_T_C */
