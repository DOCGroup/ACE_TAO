// $Id$

//================================================================
/**
 * @file Battle_exec.h
 *
 * @author George Edwards <g.edwards@vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_BATTLE_EXEC_H
#define CIAO_BATTLE_EXEC_H

#include "BattleEIC.h"
#include "tao/LocalObject.h"

// The namespace name for the actual implementation classes doesn't
// really matter.  Since there may be several different
// implementations for a component, they can very well be in different
// namespaces.
namespace MyImpl
{
  /**
   * @class Battle_exec_i
   *
   * The Battle implementation class.
   */
  class BATTLE_EXEC_Export Battle_exec_i :
    public virtual BBN_UAV::Battle_Exec,
    // CIAO container implementation depends on correct reference
    // counting of local interfaces, so we take a short cut to
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default constructor.
    Battle_exec_i (void);

    /// Default destructor.
    ~Battle_exec_i (void);

    // Operations from BBN_UAV::Battle

    virtual void
    push_start_engage (BBN_UAV::StartEngage *ev
                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

    // Operations from Components::SessionComponent

    virtual void
    set_session_context (Components::SessionContext_ptr ctx
                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void
    ccm_activate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void
    ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));

    virtual void
    ccm_remove (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));
  protected:
    CORBA::String_var str_;

    /// Copmponent specific context
    BBN_UAV::CCM_Battle_Context_var context_;
  };

  /**
   * @class BattleHome_exec_i
   *
   * Battle home executor implementation class.
   */
  class BATTLE_EXEC_Export BattleHome_exec_i :
    public virtual BBN_UAV::CCM_BattleHome,
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    /// Default ctor.
    BattleHome_exec_i ();

    /// Default dtor.
    ~BattleHome_exec_i ();

    // Explicit home operations.

    // Implicit home operations.

    virtual ::Components::EnterpriseComponent_ptr
    create (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       Components::CCMException));
  };

}

// Executor DLL entry point.  CIAO's deployment and assembly framework
// invokes this function on the resulting DLL to get the home executor.
extern "C" BATTLE_EXEC_Export ::Components::HomeExecutorBase_ptr
createBattleHome_Impl (void);

#endif /* CIAO_BATTLE_EXEC_H*/
