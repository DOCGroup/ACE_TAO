// $Id$

#include "CIAO_common.h"
#include "Battle_exec.h"

#define DISPLACEMENT 256

/// Default constructor.
MyImpl::Battle_exec_i::Battle_exec_i (void)
  : str_ ("BM DEVICE DATA")
{

}

/// Default destructor.
MyImpl::Battle_exec_i::~Battle_exec_i ()
{
}

void
MyImpl::Battle_exec_i::push_start_engage (BBN_UAV::StartEngage *ev
                                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Notify others
  BBN_UAV::BattleReady_var event = new OBV_BBN_UAV::BattleReady;

  //ACE_DEBUG ((LM_DEBUG,
  //            "Battle, received a StartEngage from Engagement \n"));

  this->context_->push_battle_ready (event
                                       ACE_ENV_ARG_PARAMETER);
}

// Operations from Components::SessionComponent
void
MyImpl::Battle_exec_i::set_session_context (Components::SessionContext_ptr ctx
                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::Battle_exec_i::set_session_context\n"));

  this->context_ =
    BBN_UAV::CCM_Battle_Context::_narrow (ctx
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
    ACE_THROW (CORBA::INTERNAL ());
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::Battle_exec_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::Battle_exec_i::ccm_activate\n"));
  char *argv[1] = { "Battle_exec"};

  int argc = sizeof(argv)/sizeof(argv[0]);
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv ACE_ENV_ARG_PARAMETER);

  CIAO_REGISTER_VALUE_FACTORY (orb.in(), BBN_UAV::StartEngage_init,
                               BBN_UAV::StartEngage);
}

void
MyImpl::Battle_exec_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::Battle_exec_i::ccm_passivate\n"));
}

void
MyImpl::Battle_exec_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::Battle_exec_i::ccm_remove\n"));
}

/// Default ctor.
MyImpl::BattleHome_exec_i::BattleHome_exec_i ()
{
}

/// Default dtor.
MyImpl::BattleHome_exec_i::~BattleHome_exec_i ()
{
}

// Explicit home operations.

// Implicit home operations.

::Components::EnterpriseComponent_ptr
MyImpl::BattleHome_exec_i::create (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  return new MyImpl::Battle_exec_i;
}


extern "C" BATTLE_EXEC_Export ::Components::HomeExecutorBase_ptr
createBattleHome_Impl (void)
{
  return new MyImpl::BattleHome_exec_i;
}
