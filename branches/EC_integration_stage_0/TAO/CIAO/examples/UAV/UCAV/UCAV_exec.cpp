// $Id$

#include "CIAO_common.h"
#include "UCAV_exec.h"

#define DISPLACEMENT 256

/// Default constructor.
MyImpl::UCAV_exec_i::UCAV_exec_i (void)
  : str_ ("BM DEVICE DATA")
{

}

/// Default destructor.
MyImpl::UCAV_exec_i::~UCAV_exec_i ()
{
}

void
MyImpl::UCAV_exec_i::push_start_move (BBN_UAV::StartMove *ev
                                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Notify others
  BBN_UAV::UCAVReady_var event = new OBV_BBN_UAV::UCAVReady;

  //ACE_DEBUG ((LM_DEBUG,
  //            "UCAV, received a StartMove from UCAV_GS \n"));

  this->context_->push_ucav_ready (event
                                       ACE_ENV_ARG_PARAMETER);
}

// Operations from Components::SessionComponent
void
MyImpl::UCAV_exec_i::set_session_context (Components::SessionContext_ptr ctx
                                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::UCAV_exec_i::set_session_context\n"));

  this->context_ =
    BBN_UAV::CCM_UCAV_Context::_narrow (ctx
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
    ACE_THROW (CORBA::INTERNAL ());
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::UCAV_exec_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::UCAV_exec_i::ccm_activate\n"));
  char *argv[1] = { "UCAV_exec"};

  int argc = sizeof(argv)/sizeof(argv[0]);
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv ACE_ENV_ARG_PARAMETER);

  CIAO_REGISTER_VALUE_FACTORY (orb.in(), BBN_UAV::StartMove_init,
                               BBN_UAV::StartMove);
}

void
MyImpl::UCAV_exec_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::UCAV_exec_i::ccm_passivate\n"));
}

void
MyImpl::UCAV_exec_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::UCAV_exec_i::ccm_remove\n"));
}

/// Default ctor.
MyImpl::UCAVHome_exec_i::UCAVHome_exec_i ()
{
}

/// Default dtor.
MyImpl::UCAVHome_exec_i::~UCAVHome_exec_i ()
{
}

// Explicit home operations.

// Implicit home operations.

::Components::EnterpriseComponent_ptr
MyImpl::UCAVHome_exec_i::create (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  return new MyImpl::UCAV_exec_i;
}


extern "C" UCAV_EXEC_Export ::Components::HomeExecutorBase_ptr
createUCAVHome_Impl (void)
{
  return new MyImpl::UCAVHome_exec_i;
}
