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
        (Components::EnterpriseComponent_ptr ec,
         Components::CCMHome_ptr home,
         Home_Servant_Impl_Base *home_servant,
         Session_Container *c)
      : Dynamic_Component_Servant_Base (c)
      , executor_ (Components::EnterpriseComponent::_duplicate (ec))
      , home_servant_ (home_servant)
      , home_ (Components::CCMHome::_duplicate (home))
      , component_removed_ (false)
  {
  }

  template <typename COMP_SVNT,
            typename COMP_EXEC,
            typename COMP_EXEC_VAR,
            typename EXEC,
            typename EXEC_VAR,
            typename COMP>
  void Dynamic_Component_Servant<COMP_SVNT, COMP_EXEC, COMP_EXEC_VAR,
    EXEC, EXEC_VAR, COMP>::update_destroy_count ()
  {
    component_removed_ = true;
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
  void Dynamic_Component_Servant
    <COMP_SVNT, COMP_EXEC, COMP_EXEC_VAR,
     EXEC, EXEC_VAR, COMP>::destroy (PortableServer::ObjectId &oid)
  {
    // @@Jai, here is a logical problem. You find a servant from a map
    // having a list of servants and you turn on a bit. What is the
    // granularity of the bit? Looks like its too coarse. Do you see
    // what I am saying?
    COMP_SVNT *servant;
    if (this->servant_map_.find (oid, servant) == 0)
      {
        servant->remove ();
        component_removed_ = true;
      }
  }

  template <typename COMP_SVNT,
            typename COMP_EXEC,
            typename COMP_EXEC_VAR,
            typename EXEC,
            typename EXEC_VAR,
            typename COMP>
  PortableServer::Servant Dynamic_Component_Servant
    <COMP_SVNT, COMP_EXEC, COMP_EXEC_VAR,
     EXEC, EXEC_VAR, COMP>::create (PortableServer::ObjectId &oid)
  {
    CIAO::Swap_Exec_var swap_exec =
      CIAO::Swap_Exec::_narrow (this->executor_.in ()
                                ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (COMP::_nil ());

    ::Components::EnterpriseComponent_var ciao_ec =
      swap_exec->incarnate (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK_RETURN (COMP::_nil ());

    COMP_EXEC_VAR ciao_comp =
      COMP_EXEC::_narrow (ciao_ec.in ()
                          ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (COMP::_nil ());

    /// @@ Jai, should you be using ACE_NEW_THROW_EX here?
    COMP_SVNT *svt = new COMP_SVNT(ciao_comp.in (), this->home_.in (),
                                   this->home_servant_,
                                   this->container_);

    // @@Jai, see the problem with a coarse grained stuff here.
    if (component_removed_ == true)
      {
        // svt->ciao_preactivate ();
        // svt->ciao_activate ();
        // svt->ciao_postactivate ();
        component_removed_ = false;
      }

    PortableServer::ServantBase_var safe (svt);
    this->servant_map_.bind (oid, svt);
    return safe._retn ();
  }
}

#endif /* CIAO_DYNAMIC_COMPONENT_SERVANT_T_C */
