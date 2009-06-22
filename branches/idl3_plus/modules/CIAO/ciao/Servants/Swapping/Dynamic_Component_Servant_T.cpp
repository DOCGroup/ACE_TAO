// $Id$

#ifndef CIAO_DYNAMIC_COMPONENT_SERVANT_T_C
#define CIAO_DYNAMIC_COMPONENT_SERVANT_T_C

#include "Dynamic_Component_Servant_T.h"

namespace CIAO
{
  template <typename COMP_SVNT>
  Dynamic_Component_Servant<COMP_SVNT>::Dynamic_Component_Servant (
    Components::EnterpriseComponent_ptr ec,
    Components::CCMHome_ptr home,
    const char* ins_name,
    Home_Servant_Impl_Base *home_servant,
    Session_Container *c)
    : Dynamic_Component_Servant_Base (c)
    , executor_ (Components::EnterpriseComponent::_duplicate (ec))
    , ins_name_ (ins_name)
    , home_servant_ (home_servant)
    , home_ (Components::CCMHome::_duplicate (home))
    , component_removed_ (false)
  {
  }

  template <typename COMP_SVNT>
  void Dynamic_Component_Servant<COMP_SVNT>::update_destroy_count (void)
  {
    component_removed_ = true;
  }

  template <typename COMP_SVNT>
  Dynamic_Component_Servant<COMP_SVNT>::~Dynamic_Component_Servant (void)
  {
  }

  template <typename COMP_SVNT>
  void Dynamic_Component_Servant<COMP_SVNT>::destroy (
    PortableServer::ObjectId &oid)
  {
    // @@Jai, here is a logical problem. You find a servant from a map
    // having a list of servants and you turn on a bit. What is the
    // granularity of the bit? Looks like its too coarse. Do you see
    // what I am saying?
    COMP_SVNT *servant = 0;

    if (0 == this->servant_map_.find (oid, servant))
      {
        servant->remove ();
        component_removed_ = true;
      }
  }

  template <typename COMP_SVNT>
  PortableServer::Servant Dynamic_Component_Servant<COMP_SVNT>::create (
    PortableServer::ObjectId &oid)
  {
    CIAO::Swap_Exec_var swap_exec =
      CIAO::Swap_Exec::_narrow (this->executor_.in ());

    ::Components::EnterpriseComponent_var ciao_ec =
      swap_exec->incarnate ();

    typedef typename COMP_SVNT::_exec_type exec_type;
    typename COMP_SVNT::_exec_type::_var_type ciao_comp =
      exec_type::_narrow (ciao_ec.in ());

    /// @@ Jai, should you be using ACE_NEW_THROW_EX here?
    COMP_SVNT *svt = new COMP_SVNT (ciao_comp.in (),
                                    this->home_.in (),
                                    this->ins_name_.in (),
                                    this->home_servant_,
                                    this->container_);

    // @@Jai, see the problem with a coarse grained stuff here.
    if (component_removed_ == true)
      {
        component_removed_ = false;
      }

    PortableServer::ServantBase_var safe (svt);
    this->servant_map_.bind (oid, svt);
    return safe._retn ();
  }
}

#endif /* CIAO_DYNAMIC_COMPONENT_SERVANT_T_C */
