// $Id$

#include "CIAO_common.h"
#include "UCAV_GS_exec.h"

#define DISPLACEMENT 256

/// Default constructor.
MyImpl::UCAV_GS_exec_i::UCAV_GS_exec_i (void)
  : str_ ("BM DEVICE DATA")
{

}

/// Default destructor.
MyImpl::UCAV_GS_exec_i::~UCAV_GS_exec_i ()
{
}

void
MyImpl::UCAV_GS_exec_i::push_prepare_move (BBN_UAV::PrepareMove *ev
                                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Notify others
  BBN_UAV::StartMove_var event = new OBV_BBN_UAV::StartMove;

  ACE_DEBUG ((LM_DEBUG,
              "UCAV_GS, received a PrepareMove from Engagement \n"));

  this->context_->push_start_move (event
                                   ACE_ENV_ARG_PARAMETER);

}

// Operations from Components::SessionComponent
void
MyImpl::UCAV_GS_exec_i::set_session_context (Components::SessionContext_ptr ctx
                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::UCAV_GS_exec_i::set_session_context\n"));

  this->context_ =
    BBN_UAV::CCM_UCAV_GS_Context::_narrow (ctx
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
    ACE_THROW (CORBA::INTERNAL ());
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::UCAV_GS_exec_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::UCAV_GS_exec_i::ccm_activate\n"));
  char *argv[1] = { "UCAV_GS_exec"};

  int argc = sizeof(argv)/sizeof(argv[0]);
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv ACE_ENV_ARG_PARAMETER);

  CIAO_REGISTER_VALUE_FACTORY (orb.in(), BBN_UAV::PrepareMove_init,
                               BBN_UAV::PrepareMove);
}

void
MyImpl::UCAV_GS_exec_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::UCAV_GS_exec_i::ccm_passivate\n"));
}

void
MyImpl::UCAV_GS_exec_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::UCAV_GS_exec_i::ccm_remove\n"));
}

/// Default ctor.
MyImpl::UCAV_GSHome_exec_i::UCAV_GSHome_exec_i ()
{
}

/// Default dtor.
MyImpl::UCAV_GSHome_exec_i::~UCAV_GSHome_exec_i ()
{
}

// Explicit home operations.

// Implicit home operations.

::Components::EnterpriseComponent_ptr
MyImpl::UCAV_GSHome_exec_i::create (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  return new MyImpl::UCAV_GS_exec_i;
}


extern "C" UCAV_GS_EXEC_Export ::Components::HomeExecutorBase_ptr
createUCAV_GSHome_Impl (void)
{
  return new MyImpl::UCAV_GSHome_exec_i;
}
